#include "Image.h"

namespace Graphics {

    Image::Image(const Instance& instance, const Device& device,
        size_t width, size_t height, uint32_t mipLevels /*= 1*/,
        Format format /*= Format::R8G8B8A8Srgb*/,
        ImageUsage::Flags usage /*= ImageUsage::Bits::TransferDst |
        ImageUsage::Bits::Sampled |
        ImageUsage::Bits::TransferSrc*/,
        ImageLayout layout /*= ImageLayout::Undefined*/)
        : m_width(width), m_height(height), m_mipLevelCount(mipLevels), m_format(format)
    {
        // If mipLevels is 0, calculate automatically
        if (m_mipLevelCount == 0) {
            m_mipLevelCount = calculateMipLevels(width, height);
        }

        vk::ImageCreateInfo imageInfo{};
        imageInfo.sType = vk::StructureType::eImageCreateInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = m_mipLevelCount;
        imageInfo.arrayLayers = 1;
        imageInfo.format = convertEnum(format);
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = convertEnum(layout);
        imageInfo.usage = usage;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.flags = vk::ImageCreateFlags();

		auto result = device.getFunctionTable().execute<DeviceFunction::CreateImage>(
			device.getHandle(), &imageInfo, nullptr, &m_image);
        if (result != VK_SUCCESS) throw std::runtime_error(
            "Image::Image(const Instance& instance, const Device& device, "
            "size_t width, size_t height, uint32_t mipLevels /*= 1*/, "
            "Format format /*= Format::R8G8B8A8Srgb*/, "
            "ImageUsage::Flags usage /*= ImageUsage::Bits::TransferDst | "
            "ImageUsage::Bits::Sampled | "
            "ImageUsage::Bits::TransferSrc*/, "
            "ImageLayout layout /*= ImageLayout::Undefined*/) - failed to create image: " +
            s_resultMessages.at(result));

        device.getFunctionTable().execute<DeviceFunction::GetImageMemoryRequirements>(
            device.getHandle(), m_image, &m_imageMemoryRequirements);
    }

    void Image::createView(const Instance& instance, const Device& device,
        ImageAspect::Flags aspectFlags /*= ImageAspect::Bits::Color*/,
        uint32_t baseMipLevel /*= 0*/, uint32_t levelCount /*= 0*/)
    {
        // If levelCount is 0, use all mip levels
        if (levelCount == 0) {
            levelCount = m_mipLevelCount;
        }

        vk::ImageViewCreateInfo createInfo{};
        createInfo.sType = vk::StructureType::eImageViewCreateInfo;
        createInfo.image = m_image;
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = convertEnum(m_format);
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        createInfo.subresourceRange.aspectMask = aspectFlags;
        createInfo.subresourceRange.baseMipLevel = baseMipLevel;
        createInfo.subresourceRange.levelCount = levelCount;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        auto result = device.getFunctionTable().execute<DeviceFunction::CreateImageView>(
            device.getHandle(), &createInfo, nullptr, &m_view);
        if (result != VK_SUCCESS) throw std::runtime_error(
            "void Image::createView(const Instance& instance, const Device& device,"
            "ImageAspect::Flags aspectFlags /*= ImageAspect::Bits::Color*/,"
            "uint32_t baseMipLevel /*= 0*/, uint32_t levelCount /*= 0*/) - failed to create image view: " +
            s_resultMessages.at(result));
    }

    uint32_t Image::calculateMipLevels(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0) {
            return 1; // Minimum 1 level
        }

        uint32_t maxDimension = std::max(width, height);

        // Handle very small textures (1x1, 2x2, etc.)
        if (maxDimension == 1) {
            return 1;
        }

        // Calculate mip levels using log2
        // We use std::log2 with double precision and add a small epsilon to avoid floating point issues
        double log2Result = std::log2(static_cast<double>(maxDimension));
        uint32_t levels = static_cast<uint32_t>(std::floor(log2Result + 1e-6)) + 1;

        // Ensure at least 1 level
        return std::max(levels, 1u);
    }
}