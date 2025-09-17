#pragma once
#include "Utility.h"

// Utility functions that abstract some common operations
namespace Graphics::Utility
{
    //returns a memory type index based on type filter and properties requirements
    uint32_t findMemoryType(const InstanceFunctionTable& functions,
        const PhysicalDevice& device, uint32_t typeFilter,
        MemoryProperty::Flags properties)
    {
        Memory::Properties memProperties = device.getMemoryProperties(functions);
        auto memTypes = memProperties.getMemoryTypes();

        for (uint32_t i = 0; i < memTypes.size(); i++) {
            if ((typeFilter & (1 << i)) &&
                static_cast<uint32_t>(memTypes[i].getPropertyFlags() & properties) == properties) {
                return i;
            }
        }

        return std::numeric_limits<uint32_t>::max();
    }

    //returns a supported format based on tiling and features requirements
    Format findSupportedFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device, const std::vector<Format>& candidates,
        ImageTiling tiling, FormatFeature::Flags features)
    {
        if (tiling == ImageTiling::Linear)
        {
            for (Format format : candidates) {
                FormatProperties props = device.getFormatProperties(functions, format);
                if (static_cast<uint32_t>(props.getLinearTilingFeatures() & features) == features) {
                    return format;
                }
            }
        }

        if (tiling == ImageTiling::Optimal)
        {
            for (Format format : candidates) {
                FormatProperties props = device.getFormatProperties(functions, format);
                if (static_cast<uint32_t>(props.getOptimalTilingFeatures() & features) == features) {
                    return format;
                }
            }
        }
        return Format::Undefined;
    }

    Format findDepthFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device) {
        return findSupportedFormat(functions, device,
            { Format::D32Sfloat, Format::D32SfloatS8Uint, Format::D24UnormS8Uint },
            ImageTiling::Optimal,
            FormatFeature::Bits::DepthStencilAttachment);
    }

    uint32_t calculateMipLevels(const Extent2D& extent) {
        if (extent.getWidth() == 0 || extent.getHeight() == 0) {
            return 1; // Minimum 1 level
        }

        uint32_t maxDimension = std::max(extent.getWidth(), extent.getHeight());

        // Handle very small textures (1x1, 2x2, etc.)
        if (maxDimension == 1) {
            return 1;
        }

        // Calculate mip levels using log2
        // We use std::log2 with double precision and add a small epsilon to avoid floating point issues
        double log2Result = std::log2(static_cast<double>(maxDimension));
        uint32_t levels = static_cast<uint32_t>(std::floor(
            log2Result + std::numeric_limits<double>::epsilon()) + 1);

        // Ensure at least 1 level
        return std::max(levels, 1u);
    }

    Extent2D chooseExtent(const Surface::Capabilities& capabilities, const Extent2D& frameBufferExtent)
    {
        if (capabilities.getCurrentExtent().getWidth() != std::numeric_limits<uint32_t>::max()) {
            return capabilities.getCurrentExtent();
        }

        Extent2D newExtent;
        newExtent.setWidth(std::clamp(frameBufferExtent.getWidth(),
            capabilities.getMinImageExtent().getWidth(),
            capabilities.getMaxImageExtent().getWidth()));
        newExtent.setHeight(std::clamp(frameBufferExtent.getHeight(),
            capabilities.getMinImageExtent().getHeight(),
            capabilities.getMaxImageExtent().getHeight()));
        return newExtent;
    }

    BasicSwapChainData createBasicSwapChain(const InstanceFunctionTable& instanceFunctions,
        const DeviceFunctionTable& deviceFunctions,
        const PhysicalDevice& physicalDevice,
        const DeviceRef& device,
        const SurfaceRef& surface,
        const RenderPassRef& renderPass,
        const Extent2D& preferredExtent,
        Format preferredFormat /*= Format::B8G8R8A8Srgb*/,
        Format preferredDepthFormat /*= Format::D32Sfloat*/,
        ColorSpace preferredColorSpace /*= ColorSpace::SrgbNonlinear*/,
        ImageUsage::Flags preferredImageUsage /*= ImageUsage::Bits::ColorAttachment*/,
		PresentMode preferredPresentMode /*= PresentMode::Mailbox*/,
        uint32_t desiredImageCount /*= 2*/)
    {
		BasicSwapChainData swapChainData;
        swapChainData.swapChainInfo.setClipped(true)
            .setCompositeAlpha(CompositeAlpha::Bits::Opaque)
            .setImageExtent(preferredExtent)
            .setImageFormat(preferredFormat)
			.setImageColorSpace(preferredColorSpace)
            .setImageUsage(preferredImageUsage)
            .setMinImageCount(desiredImageCount)
            .setPresentMode(preferredPresentMode)
            .setPreTransform(SurfaceTransform::Bits::Identity)
            .setSurface(surface);

        swapChainData.swapChain;
        swapChainData.swapChainImages = swapChainData.swapChain.create(device, deviceFunctions, swapChainData.swapChainInfo);

        swapChainData.depthImageCreateInfo.setArrayLayers(1)
            .setExtent(Extent3D(preferredExtent))
            .setFormat(preferredDepthFormat)
            .setImageType(ImageType::T2D)
            .setInitialLayout(ImageLayout::Undefined)
            .setMipLevels(1)
            .setSamples(SampleCount::Bits::SC1)
            .setTiling(ImageTiling::Optimal)
            .setUsage(ImageUsage::Bits::DepthStencilAttachment);

        swapChainData.depthImage.create(device, deviceFunctions, swapChainData.depthImageCreateInfo);

        swapChainData.depthImageViewCreateInfo.setImage(swapChainData.depthImage)
            .setViewType(ImageViewType::T2D)
            .setFormat(preferredDepthFormat)
            .setSubresourceRange(Image::SubresourceRange(
                ImageAspect::Bits::Depth, 0, 1, 0, 1))
            .setComponents(ComponentMapping());
        swapChainData.depthImageView.create(device, deviceFunctions, swapChainData.depthImage, swapChainData.depthImageViewCreateInfo);

        swapChainData.swapChainImageViews.reserve(swapChainData.swapChainImages.size());
        swapChainData.swapChainFrameBuffers.reserve(swapChainData.swapChainImages.size());
        swapChainData.swapChainImageViewCreateInfos.reserve(swapChainData.swapChainImages.size());
        swapChainData.swapChainFrameBufferCreateInfos.reserve(swapChainData.swapChainImages.size());

        swapChainData.attachmentRefs.resize(swapChainData.swapChainImages.size());

        for (int i = 0; i < swapChainData.swapChainImages.size(); i++)
        {
            swapChainData.swapChainImageViewCreateInfos.push_back(Image::View::CreateInfo{});
            swapChainData.swapChainImageViewCreateInfos.back().setComponents(ComponentMapping())
                .setSubresourceRange(Image::SubresourceRange(
                    ImageAspect::Bits::Color, 0, 1, 0, 1))
                .setFormat(preferredFormat)
                .setViewType(ImageViewType::T2D)
                .setImage(swapChainData.swapChainImages[i]);

            swapChainData.swapChainImageViews.push_back(Image::View{});
            swapChainData.swapChainImageViews.back().create(device, deviceFunctions,
                swapChainData.swapChainImages[i], swapChainData.swapChainImageViewCreateInfos.back());

            swapChainData.attachmentRefs[i] = { swapChainData.swapChainImageViews.back(), swapChainData.depthImageView };

            swapChainData.swapChainFrameBufferCreateInfos.push_back(FrameBuffer::CreateInfo{});
            swapChainData.swapChainFrameBufferCreateInfos.back().setRenderPass(renderPass)
                .setAttachments(swapChainData.attachmentRefs[i])
                .setExtent(preferredExtent)
                .setLayers(1);

            swapChainData.swapChainFrameBuffers.push_back(FrameBuffer{});
            swapChainData.swapChainFrameBuffers.back().create(device, deviceFunctions, swapChainData.swapChainFrameBufferCreateInfos.back());
        }

		return swapChainData;
    }

    void destroySwapChainDaTa(BasicSwapChainData& data,
        const DeviceRef& device,
        const DeviceFunctionTable& deviceFunctions)
    {
        for (auto& framebuffer : data.swapChainFrameBuffers) {
            framebuffer.destroy(device, deviceFunctions);
        }
        data.swapChainFrameBuffers.clear();
        data.swapChainFrameBufferCreateInfos.clear();
        for (auto& imageView : data.swapChainImageViews) {
            imageView.destroy(device, deviceFunctions);
        }
        data.swapChainImageViews.clear();
        data.swapChainImageViewCreateInfos.clear();
        data.attachmentRefs.clear();
        data.depthImageView.destroy(device, deviceFunctions);
        data.depthImage.destroy(device, deviceFunctions);
		data.swapChain.destroy(device, deviceFunctions, data.swapChainImages);
    }

    void recreateBasicSwapChain(BasicSwapChainData& data,
        const InstanceFunctionTable& instanceFunctions,
        const DeviceFunctionTable& deviceFunctions,
        const PhysicalDevice& physicalDevice,
        const DeviceRef& device,
        const RenderPassRef& renderPass,
        const SurfaceRef& surface,
        const Extent2D& preferredExtent)
    {
        data.swapChainInfo.setImageExtent(preferredExtent);

        data.swapChain.recreate(device, deviceFunctions, data.swapChainInfo, data.swapChainImages);
        for (auto& framebuffer : data.swapChainFrameBuffers) {
            framebuffer.destroy(device, deviceFunctions);
        }
        data.swapChainFrameBuffers.clear();
        data.swapChainFrameBufferCreateInfos.clear();
        for (auto& imageView : data.swapChainImageViews) {
            imageView.destroy(device, deviceFunctions);
        }
        data.swapChainImageViews.clear();
        data.swapChainImageViewCreateInfos.clear();
        data.attachmentRefs.clear();
        data.depthImageView.destroy(device, deviceFunctions);
        data.depthImage.destroy(device, deviceFunctions);

        data.depthImageCreateInfo.setExtent(Extent3D(preferredExtent));
        data.depthImage.create(device, deviceFunctions, data.depthImageCreateInfo);

        data.depthImageViewCreateInfo.setImage(data.depthImage);
        data.depthImageView.create(device, deviceFunctions, data.depthImage, data.depthImageViewCreateInfo);

        data.swapChainImageViews.reserve(data.swapChainImages.size());
        data.swapChainFrameBuffers.reserve(data.swapChainImages.size());
        data.swapChainImageViewCreateInfos.reserve(data.swapChainImages.size());
        data.swapChainFrameBufferCreateInfos.reserve(data.swapChainImages.size());
        data.attachmentRefs.resize(data.swapChainImages.size());

        for (int i = 0; i < data.swapChainImages.size(); i++)
        {
            data.swapChainImageViewCreateInfos.push_back(Image::View::CreateInfo{});
            data.swapChainImageViewCreateInfos.back().setComponents(ComponentMapping())
                .setSubresourceRange(Image::SubresourceRange(
                    ImageAspect::Bits::Color, 0, 1, 0, 1))
                .setFormat(data.swapChainInfo.getImageFormat())
                .setViewType(ImageViewType::T2D)
                .setImage(data.swapChainImages[i]);
            data.swapChainImageViews.push_back(Image::View{});
            data.swapChainImageViews.back().create(device, deviceFunctions,
                data.swapChainImages[i], data.swapChainImageViewCreateInfos.back());
            data.attachmentRefs[i] = { data.swapChainImageViews.back(), data.depthImageView };
            data.swapChainFrameBufferCreateInfos.push_back(FrameBuffer::CreateInfo{});
            data.swapChainFrameBufferCreateInfos.back().setRenderPass(renderPass)
                .setAttachments(data.attachmentRefs[i])
                .setExtent(preferredExtent)
                .setLayers(1);
            data.swapChainFrameBuffers.push_back(FrameBuffer{});
            data.swapChainFrameBuffers.back().create(device, deviceFunctions, data.swapChainFrameBufferCreateInfos.back());
		}
    }
}