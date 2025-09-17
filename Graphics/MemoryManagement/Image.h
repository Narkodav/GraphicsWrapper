#pragma once
#include "../Common.h"
#include "../Rendering/Instance.h"
#include "../Rendering/Device.h"
#include "../Enums.h"

namespace Graphics {

    class CommandBuffer;

    class Image
    {
    private:

        VkImage m_image = nullptr;
        VkImageView m_view = nullptr;
        vk::MemoryRequirements m_imageMemoryRequirements = {};

        uint32_t m_width = 0;
        uint32_t m_height = 0;
        uint32_t m_mipLevelCount = 1;
        Format m_format = Format::Undefined;
    public:
        Image() = default;

        Image(const Instance& instance, const Device& device,
            size_t width, size_t height, uint32_t mipLevels = 1,
            Format format = Format::R8G8B8A8Srgb,
            ImageUsage::Flags usage = ImageUsage::Bits::TransferDst |
            ImageUsage::Bits::Sampled |
            ImageUsage::Bits::TransferSrc,
            ImageLayout layout = ImageLayout::Undefined);

        Image(Image&& other) noexcept {
            m_image = std::exchange(other.m_image, vk::Image());
            m_view = std::exchange(other.m_view, vk::ImageView());
            m_imageMemoryRequirements = std::exchange(other.m_imageMemoryRequirements,
                vk::MemoryRequirements());
            m_width = std::exchange(other.m_width, 0);
            m_height = std::exchange(other.m_height, 0);
            m_mipLevelCount = std::exchange(other.m_mipLevelCount, 0);
            m_format = std::exchange(other.m_format, Format::Undefined);
        };

        Image& operator=(Image&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(m_image == nullptr && "Image& Image::operator=(Image&& other) noexcept - Image already initialized");

            m_image = std::exchange(other.m_image, vk::Image());
            m_view = std::exchange(other.m_view, vk::ImageView());
            m_imageMemoryRequirements = std::exchange(other.m_imageMemoryRequirements,
                vk::MemoryRequirements());
            m_width = std::exchange(other.m_width, 0);
            m_height = std::exchange(other.m_height, 0);
            m_mipLevelCount = std::exchange(other.m_mipLevelCount, 0);
            m_format = std::exchange(other.m_format, Format::Undefined);

            return *this;
        };

        Image(const Image&) noexcept = delete;
        Image& operator=(const Image&) noexcept = delete;

        ~Image() { assert(m_image == nullptr && "Image::~Image() - Image was not destroyed!"); };
                
        void createView(const Instance& instance, const Device& device,
            ImageAspect::Flags aspectFlags = ImageAspect::Bits::Color,
            uint32_t baseMipLevel = 0, uint32_t levelCount = 0);

        static uint32_t calculateMipLevels(uint32_t width, uint32_t height);

        VkImage getHandle() const { return m_image; };
        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        uint32_t getMipLevelCount() const { return m_mipLevelCount; }
        Format getFormat() const { return m_format; }
        auto getView() const { return m_view; };

        void destroy(const Device& device) {
            if (m_image == nullptr)
                return;

            device.getFunctionTable().execute<DeviceFunction::DestroyImageView>(
                device.getHandle(), m_view, nullptr);
            device.getFunctionTable().execute<DeviceFunction::DestroyImage>(
                device.getHandle(), m_image, nullptr);

            m_image = nullptr;
#ifdef _DEBUG
            std::cout << "void Device::destroy(const Device& device) - Destroyed TextureImage with " << m_mipLevelCount << " mip levels" << std::endl;
#endif
        };

        const auto& getMemoryRequirements() const {
            assert(m_image == nullptr && "const auto& Image::getMemoryRequirements() const - Image is not initialized");
            return m_imageMemoryRequirements;
        }

        friend class CommandBuffer;
    };

}