#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <functional>
#include <memory>

namespace Graphics::Rendering {

    class IVulkanAllocator {
    public:
        virtual ~IVulkanAllocator() = default;
        
        virtual vk::Buffer createBuffer(const vk::BufferCreateInfo& createInfo) = 0;
        virtual vk::DeviceMemory allocateMemory(const vk::MemoryAllocateInfo& allocInfo) = 0;
        virtual vk::Image createImage(const vk::ImageCreateInfo& createInfo) = 0;
        virtual vk::ImageView createImageView(const vk::ImageViewCreateInfo& createInfo) = 0;
        virtual vk::CommandPool createCommandPool(const vk::CommandPoolCreateInfo& createInfo) = 0;
        virtual vk::Pipeline createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo) = 0;
        virtual vk::RenderPass createRenderPass(const vk::RenderPassCreateInfo& createInfo) = 0;
        virtual vk::DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo) = 0;
        virtual vk::PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo) = 0;
        virtual vk::Semaphore createSemaphore(const vk::SemaphoreCreateInfo& createInfo) = 0;
        virtual vk::Fence createFence(const vk::FenceCreateInfo& createInfo) = 0;
        
        virtual void clear() = 0;
    };

    class CentralizedVulkanAllocator : public IVulkanAllocator {
    private:
        const vk::Device& m_device;
        const vk::DispatchLoaderDynamic& m_dispatcher;
        
        // Destruction order: reverse of creation dependencies
        std::vector<std::function<void()>> m_destructors;
        
        template<typename T>
        T createAndTrack(std::function<T()> creator, std::function<void(T)> destructor) {
            T object = creator();
            m_destructors.emplace_back([destructor, object]() { destructor(object); });
            return object;
        }
        
    public:
        CentralizedVulkanAllocator(const vk::Device& device, const vk::DispatchLoaderDynamic& dispatcher)
            : m_device(device), m_dispatcher(dispatcher) {}
        
        ~CentralizedVulkanAllocator() { clear(); }
        
        vk::Buffer createBuffer(const vk::BufferCreateInfo& createInfo) override {
            return createAndTrack<vk::Buffer>(
                [&]() { return m_device.createBuffer(createInfo, nullptr, m_dispatcher); },
                [&](vk::Buffer buf) { m_device.destroyBuffer(buf, nullptr, m_dispatcher); }
            );
        }
        
        vk::DeviceMemory allocateMemory(const vk::MemoryAllocateInfo& allocInfo) override {
            return createAndTrack<vk::DeviceMemory>(
                [&]() { return m_device.allocateMemory(allocInfo, nullptr, m_dispatcher); },
                [&](vk::DeviceMemory mem) { m_device.freeMemory(mem, nullptr, m_dispatcher); }
            );
        }
        
        vk::Image createImage(const vk::ImageCreateInfo& createInfo) override {
            return createAndTrack<vk::Image>(
                [&]() { return m_device.createImage(createInfo, nullptr, m_dispatcher); },
                [&](vk::Image img) { m_device.destroyImage(img, nullptr, m_dispatcher); }
            );
        }
        
        vk::ImageView createImageView(const vk::ImageViewCreateInfo& createInfo) override {
            return createAndTrack<vk::ImageView>(
                [&]() { return m_device.createImageView(createInfo, nullptr, m_dispatcher); },
                [&](vk::ImageView view) { m_device.destroyImageView(view, nullptr, m_dispatcher); }
            );
        }
        
        vk::CommandPool createCommandPool(const vk::CommandPoolCreateInfo& createInfo) override {
            return createAndTrack<vk::CommandPool>(
                [&]() { return m_device.createCommandPool(createInfo, nullptr, m_dispatcher); },
                [&](vk::CommandPool pool) { m_device.destroyCommandPool(pool, nullptr, m_dispatcher); }
            );
        }
        
        vk::Pipeline createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo) override {
            auto result = m_device.createGraphicsPipeline(nullptr, createInfo, nullptr, m_dispatcher);
            return createAndTrack<vk::Pipeline>(
                [result]() { return result.value; },
                [&](vk::Pipeline pipeline) { m_device.destroyPipeline(pipeline, nullptr, m_dispatcher); }
            );
        }
        
        vk::RenderPass createRenderPass(const vk::RenderPassCreateInfo& createInfo) override {
            return createAndTrack<vk::RenderPass>(
                [&]() { return m_device.createRenderPass(createInfo, nullptr, m_dispatcher); },
                [&](vk::RenderPass pass) { m_device.destroyRenderPass(pass, nullptr, m_dispatcher); }
            );
        }
        
        vk::DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo) override {
            return createAndTrack<vk::DescriptorSetLayout>(
                [&]() { return m_device.createDescriptorSetLayout(createInfo, nullptr, m_dispatcher); },
                [&](vk::DescriptorSetLayout layout) { m_device.destroyDescriptorSetLayout(layout, nullptr, m_dispatcher); }
            );
        }
        
        vk::PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo) override {
            return createAndTrack<vk::PipelineLayout>(
                [&]() { return m_device.createPipelineLayout(createInfo, nullptr, m_dispatcher); },
                [&](vk::PipelineLayout layout) { m_device.destroyPipelineLayout(layout, nullptr, m_dispatcher); }
            );
        }
        
        vk::Semaphore createSemaphore(const vk::SemaphoreCreateInfo& createInfo) override {
            return createAndTrack<vk::Semaphore>(
                [&]() { return m_device.createSemaphore(createInfo, nullptr, m_dispatcher); },
                [&](vk::Semaphore sem) { m_device.destroySemaphore(sem, nullptr, m_dispatcher); }
            );
        }
        
        vk::Fence createFence(const vk::FenceCreateInfo& createInfo) override {
            return createAndTrack<vk::Fence>(
                [&]() { return m_device.createFence(createInfo, nullptr, m_dispatcher); },
                [&](vk::Fence fence) { m_device.destroyFence(fence, nullptr, m_dispatcher); }
            );
        }
        
        void clear() override {
            // Destroy in reverse order (LIFO)
            for (auto it = m_destructors.rbegin(); it != m_destructors.rend(); ++it) {
                (*it)();
            }
            m_destructors.clear();
        }
    };

    // Null allocator for direct Vulkan usage
    class NullVulkanAllocator : public IVulkanAllocator {
    private:
        const vk::Device& m_device;
        const vk::DispatchLoaderDynamic& m_dispatcher;
        
    public:
        NullVulkanAllocator(const vk::Device& device, const vk::DispatchLoaderDynamic& dispatcher)
            : m_device(device), m_dispatcher(dispatcher) {}
        
        vk::Buffer createBuffer(const vk::BufferCreateInfo& createInfo) override {
            return m_device.createBuffer(createInfo, nullptr, m_dispatcher);
        }
        
        vk::DeviceMemory allocateMemory(const vk::MemoryAllocateInfo& allocInfo) override {
            return m_device.allocateMemory(allocInfo, nullptr, m_dispatcher);
        }
        
        vk::Image createImage(const vk::ImageCreateInfo& createInfo) override {
            return m_device.createImage(createInfo, nullptr, m_dispatcher);
        }
        
        vk::ImageView createImageView(const vk::ImageViewCreateInfo& createInfo) override {
            return m_device.createImageView(createInfo, nullptr, m_dispatcher);
        }
        
        vk::CommandPool createCommandPool(const vk::CommandPoolCreateInfo& createInfo) override {
            return m_device.createCommandPool(createInfo, nullptr, m_dispatcher);
        }
        
        vk::Pipeline createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& createInfo) override {
            return m_device.createGraphicsPipeline(nullptr, createInfo, nullptr, m_dispatcher).value;
        }
        
        vk::RenderPass createRenderPass(const vk::RenderPassCreateInfo& createInfo) override {
            return m_device.createRenderPass(createInfo, nullptr, m_dispatcher);
        }
        
        vk::DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo) override {
            return m_device.createDescriptorSetLayout(createInfo, nullptr, m_dispatcher);
        }
        
        vk::PipelineLayout createPipelineLayout(const vk::PipelineLayoutCreateInfo& createInfo) override {
            return m_device.createPipelineLayout(createInfo, nullptr, m_dispatcher);
        }
        
        vk::Semaphore createSemaphore(const vk::SemaphoreCreateInfo& createInfo) override {
            return m_device.createSemaphore(createInfo, nullptr, m_dispatcher);
        }
        
        vk::Fence createFence(const vk::FenceCreateInfo& createInfo) override {
            return m_device.createFence(createInfo, nullptr, m_dispatcher);
        }
        
        void clear() override {} // No-op for manual management
    };

}