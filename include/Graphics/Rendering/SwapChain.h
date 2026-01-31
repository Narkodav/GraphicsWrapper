#pragma once
#include "../Common.h"
#include "Device.h"
#include "SwapChainFormat.h"
#include "RenderPass.h"
#include "Semaphore.h"
#include "../MemoryManagement/Memory.h"

namespace Graphics {

    class SwapChain {
    private:
        VkSwapchainKHR m_swapChain = nullptr;
        SwapChainFormat m_activeSwapChainFormat;

        VkImage* m_swapChainImages;
        VkImageView* m_swapChainImageViews;
        VkFramebuffer* m_swapChainFrameBuffers;

        uint32_t m_imageCount = 0;

        VkImage m_depthImage = nullptr;
        VkImageView m_depthImageView = nullptr;
        Memory m_depthImageMemory;
    public:
        SwapChain() = default;

        SwapChain(const Instance& instance, const Device& device, const Surface& surface,
            const RenderPass& renderPass, const SwapChainFormat& format,
            uint32_t presentQueueIndex, uint32_t workerQueueIndex);

        SwapChain(SwapChain&& other) noexcept {
            m_swapChain = std::exchange(other.m_swapChain, nullptr);
            m_activeSwapChainFormat = std::exchange(other.m_activeSwapChainFormat, {});
            m_imageCount = std::exchange(other.m_imageCount, 0);
            m_swapChainImages = std::exchange(other.m_swapChainImages, std::vector<vk::Image>());
            m_swapChainImageViews = std::exchange(other.m_swapChainImageViews, std::vector<vk::ImageView>());
            m_swapChainFrameBuffers = std::exchange(other.m_swapChainFrameBuffers, std::vector<vk::Framebuffer>());

            m_depthImage = std::exchange(other.m_depthImage, nullptr);
            m_depthImageView = std::exchange(other.m_depthImageView, nullptr);
            m_depthImageMemory = std::exchange(other.m_depthImageMemory, Memory());
        };

        //moving to an initialized swap chain is undefined behavior, destroy before moving
        SwapChain& operator=(SwapChain&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(m_swapChain == nullptr && "SwapChain& SwapChain::operator=(SwapChain&& other)"
                " noexcept - SwapChain already initialized");

            m_swapChain = std::exchange(other.m_swapChain, nullptr);
            m_activeSwapChainFormat = std::exchange(other.m_activeSwapChainFormat, {});
            m_imageCount = std::exchange(other.m_imageCount, 0);
            m_swapChainImages = std::exchange(other.m_swapChainImages, std::vector<vk::Image>());
            m_swapChainImageViews = std::exchange(other.m_swapChainImageViews, std::vector<vk::ImageView>());
            m_swapChainFrameBuffers = std::exchange(other.m_swapChainFrameBuffers, std::vector<vk::Framebuffer>());

            m_depthImage = std::exchange(other.m_depthImage, vk::Image());
            m_depthImageView = std::exchange(other.m_depthImageView, vk::ImageView());
            m_depthImageMemory = std::exchange(other.m_depthImageMemory, Memory());

            return *this;
        };

        SwapChain(const SwapChain& other) noexcept = delete;
        SwapChain& operator=(const SwapChain& other) noexcept = delete;

        ~SwapChain() { assert(m_swapChain == nullptr && "SwapChain::~SwapChain() - SwapChain was not destroyed"); };

        void init(const Instance& instance, const Device& device, const Surface& surface,
            const RenderPass& renderPass, const SwapChainFormat& format, uint32_t presentQueueIndex, uint32_t workerQueueIndex);

        void initDepthImage(const Instance& instance, const Device& device);

        void recreate(const Instance& instance, const Device& device, const Surface& surface,
            const RenderPass& renderPass, const SwapChainFormat& format, uint32_t presentQueueIndex, uint32_t workerQueueIndex);

        void destroy(const Device& device) {
            if (m_swapChain == nullptr)
                return;

            for (size_t i = 0; i < m_imageCount; ++i) {
                device.getFunctionTable().execute<DeviceFunction::DestroyFramebuffer>(
                    device.getHandle(), m_swapChainFrameBuffers[i], nullptr);
                device.getFunctionTable().execute<DeviceFunction::DestroyImageView>(
                    device.getHandle(), m_swapChainImageViews[i], nullptr);
            }

            delete[] m_swapChainFrameBuffers;
            delete[] m_swapChainImageViews;

            if (m_depthImage != nullptr)
            {
                device.getFunctionTable().execute<DeviceFunction::DestroyImageView>(
                    device.getHandle(), &m_depthImageView, nullptr);
                device.getFunctionTable().execute<DeviceFunction::DestroyImage>(
                    device.getHandle(), &m_depthImage, nullptr);
                m_depthImageMemory.destroy(device);
            }

            device.getFunctionTable().execute<DeviceFunction::DestroySwapchainKHR>(
				device.getHandle(), m_swapChain, nullptr);

            m_swapChain = nullptr;
            m_depthImage = nullptr;
#ifdef _DEBUG
            std::cout << "void SwapChain::destroy(const Device& device) - Destroyed SwapChain" << std::endl;
#endif
        };

        static VkSwapchainKHR createSwapChain(const Device& device, const SwapChainSupportDetails& swapChainSupportDetails,
            const Surface& surface, const SwapChainFormat& activeSwapChainFormat, uint32_t imageCount,
            uint32_t presentQueueIndex, uint32_t workerQueueIndex);

        static VkImage* getSwapChainImages(const Device& device, const VkSwapchainKHR& swapChain, uint32_t& imageCount);

        static VkImageView* createImageViews(const Device& device, const SwapChainFormat& activeSwapChainFormat,
            const std::span<VkImage>& swapChainImages);

        static VkFramebuffer* createFrameBuffers(const Device& device, const RenderPass& renderPass,
            const std::span<VkImageView>& imageViews, const SwapChainFormat& format);

        static VkFramebuffer* createFrameBuffers(const Device& device, const RenderPass& renderPass,
            const std::span<VkImageView>& imageViews, VkImageView depthImageView,
            const SwapChainFormat& format);

        const SwapChainFormat& getActiveSwapChainFormat() const { return m_activeSwapChainFormat; };

        std::span<const VkFramebuffer> getFrameBuffers() const {
            return std::span<const VkFramebuffer>(m_swapChainFrameBuffers, m_imageCount);
        };

        bool acquireNextImage(const Device& device, const Semaphore& semaphore, uint32_t& imageIndex) const {
            VkResult result = device.getFunctionTable().execute<DeviceFunction::AcquireNextImageKHR>(
                device.getHandle(), m_swapChain, UINT64_MAX, semaphore.getSemaphore(), nullptr, &imageIndex);
        
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
                return false;
            else if (result != vk::Result::eSuccess)
                if (result != VK_SUCCESS) throw std::runtime_error("Failed to acquire next image: " +
                    s_resultMessages.at(result));
            return true;
        };

        const vk::SwapchainKHR& getSwapChain() const { return m_swapChain; };
        const size_t& getImageCount() const { return m_imageCount; };
    };

}