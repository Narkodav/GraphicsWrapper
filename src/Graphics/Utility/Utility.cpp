#include "Graphics/Utility/Utility.h"

// Utility functions that abstract some common operations
namespace Graphics::Utility
{
    // return std::numeric_limits<uint32_t>::max() if no memory type index found
    uint32_t findMemoryTypeFirstFit(
        const PhysicalDeviceMemoryProperties& memProperties,
        uint32_t typeFilter,
        Flags::MemoryProperty requiredProperties,
        Flags::MemoryProperty forbiddenProperties //= Flags::MemoryProperty::Bits::None
    ) {
        if (typeFilter == 0) {
            return std::numeric_limits<uint32_t>::max();
        }

        const auto& memTypes = memProperties.getMemoryTypes();
        const uint32_t memoryTypeCount = memProperties.getMemoryTypeCount();

        std::vector<uint32_t> compatibleTypes;

        for (uint32_t i = 0; i < memoryTypeCount; ++i) {
            if ((typeFilter & (1u << i)) == 0) continue;
            auto props = memTypes[i].getPropertyFlags();
            if (props.hasFlags(requiredProperties) && props.doesntHaveFlags(forbiddenProperties))
                return i;
        }

        return std::numeric_limits<uint32_t>::max();
    }

    //returns a memory type index based on type filter and properties requirements
	//returns an empty vector if no suitable memory type is found
    std::vector<uint32_t> findCompatibleMemoryTypes(
        const PhysicalDeviceMemoryProperties& memProperties,
        uint32_t typeFilter,
        Flags::MemoryProperty requiredProperties,
        Flags::MemoryProperty forbiddenProperties //= Flags::MemoryProperty::Bits::None
    )
    {
        if (typeFilter == 0) {
            return {};
        }

        const auto& memTypes = memProperties.getMemoryTypes();
        const uint32_t memoryTypeCount = memProperties.getMemoryTypeCount();

		std::vector<uint32_t> compatibleTypes;

        for (uint32_t i = 0; i < memoryTypeCount; ++i) {
            if ((typeFilter & (1u << i)) == 0) continue;
            auto props = memTypes[i].getPropertyFlags();
            if (props.hasFlags(requiredProperties) && props.doesntHaveFlags(forbiddenProperties))
				compatibleTypes.push_back(i);
        }

        return compatibleTypes;
    }

    void sortMemoryTypesByScore(
        const PhysicalDeviceMemoryProperties& memProperties,
        std::vector<uint32_t>& memoryTypeIndices,
        const std::array<int64_t, Flags::MemoryProperty::bitCount()>& weights)
    {
        struct Entry {
            uint32_t index;
            int64_t score;
        };

        std::vector<Entry> entries(memoryTypeIndices.size());

        for (size_t i = 0; i < entries.size(); ++i) {
            const auto flags = memProperties.getMemoryTypes()[memoryTypeIndices[i]].getPropertyFlags();
            entries[i].index = memoryTypeIndices[i];
            for (size_t bit = 0; bit < weights.size(); ++bit) {
                // NOTE: relies on contiguous Vulkan memory property flag bits
                if (flags.hasFlag(1u << bit)) entries[i].score += weights[bit];
            }
        }

        std::sort(entries.begin(), entries.end(),
            [](const Entry& a, const Entry& b) {
                return a.score > b.score;
            });

        for (size_t i = 0; i < entries.size(); ++i) {
            memoryTypeIndices[i] = entries[i].index;
        }
    }

    void sortMemoryTypesByScore(
        const PhysicalDeviceMemoryProperties& memProperties,
        std::vector<uint32_t>& memoryTypeIndices,
        std::function<int(Flags::MemoryProperty flags)> sorter)
    {
        std::sort(memoryTypeIndices.begin(), memoryTypeIndices.end(),
            [&](uint32_t a, uint32_t b) {
                const auto flagsA = memProperties.getMemoryTypes()[a].getPropertyFlags();
                const auto flagsB = memProperties.getMemoryTypes()[b].getPropertyFlags();
                return sorter(flagsA) > sorter(flagsB);
			});
    }

    //returns a supported format based on tiling and features requirements
    PixelFormat findSupportedFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device, const std::vector<PixelFormat>& candidates,
        ImageTiling tiling, Flags::FormatFeature features)
    {
        if (tiling == ImageTiling::Linear)
        {
            for (PixelFormat format : candidates) {
                FormatProperties props = device.getFormatProperties(functions, format);
                if (static_cast<uint32_t>(props.getLinearTilingFeatures() & features) == features) {
                    return format;
                }
            }
        }

        if (tiling == ImageTiling::Optimal)
        {
            for (PixelFormat format : candidates) {
                FormatProperties props = device.getFormatProperties(functions, format);
                if (static_cast<uint32_t>(props.getOptimalTilingFeatures() & features) == features) {
                    return format;
                }
            }
        }
        return PixelFormat::Undefined;
    }

    PixelFormat findDepthFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device) {
        return findSupportedFormat(functions, device,
            { PixelFormat::D32Sfloat, PixelFormat::D32SfloatS8Uint, PixelFormat::D24UnormS8Uint },
            ImageTiling::Optimal,
            Flags::FormatFeature::Bits::DepthStencilAttachment);
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

    Extent2D chooseExtent(const SurfaceCapabilities& capabilities, const Extent2D& frameBufferExtent)
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

    RenderPassData createColorDepthRenderPass(const DeviceFunctionTable& deviceFunctions, const DeviceRef& device,
        std::span<SurfaceFormat> formats, std::span<PresentMode> presentModes, PixelFormat depthFormat)
    {
        RenderPassData data;

        for (size_t i = 0; i < formats.size(); i++) {
            if (formats[i].getFormat() == PixelFormat::B8G8R8A8Srgb &&
                formats[i].getColorSpace() == ColorSpace::SrgbNonlinear) {
                data.surfaceFormat = formats[i];
            }
        }

        for (size_t i = 0; i < presentModes.size(); i++) {
            if (presentModes[i] == PresentMode::Mailbox) {
                data.presentMode = presentModes[i];
            }
        }



        data.attachments.resize(2);

        data.attachments[0].setFormat(data.surfaceFormat.getFormat())
            .setSamples(Flags::SampleCount::Bits::SC1)
            .setLoadOp(AttachmentLoadOp::Clear)
            .setStoreOp(AttachmentStoreOp::Store)
            .setStencilLoadOp(AttachmentLoadOp::DontCare)
            .setStencilStoreOp(AttachmentStoreOp::DontCare)
            .setInitialLayout(ImageLayout::Undefined)
            .setFinalLayout(ImageLayout::PresentSrcKHR);

        data.colorAttachmentRefs.resize(1);
        data.colorAttachmentRefs[0].setAttachment(0)
            .setLayout(ImageLayout::ColorAttachmentOptimal);

        data.subpasses.resize(1);
        data.subpasses[0].setBindPoint(PipelineBindPoint::Graphics)
            .setColorAttachments(data.colorAttachmentRefs);

        data.subpassDependencies[0].setSrcSubpass(Subpass::s_externalSubpass)
            .setDstSubpass(0)
            .setSrcStageMask(Flags::PipelineStage::Bits::ColorAttachmentOutput)
            .setSrcAccessMask(Flags::Access::Bits::None)
            .setDstStageMask(Flags::PipelineStage::Bits::ColorAttachmentOutput)
            .setDstAccessMask(Flags::Access::Bits::ColorAttachmentWrite);

        data.attachments[1].setFormat(depthFormat)
            .setSamples(Flags::SampleCount::Bits::SC1)
            .setLoadOp(AttachmentLoadOp::Clear)
            .setStoreOp(AttachmentStoreOp::DontCare)
            .setStencilLoadOp(AttachmentLoadOp::DontCare)
            .setStencilStoreOp(AttachmentStoreOp::DontCare)
            .setInitialLayout(ImageLayout::Undefined)
            .setFinalLayout(ImageLayout::DepthStencilAttachmentOptimal);
        data.depthAttachmentRef.setAttachment(1)
            .setLayout(ImageLayout::DepthStencilAttachmentOptimal);

        data.subpasses[0].setDepthStencilAttachment(data.depthAttachmentRef);

        data.subpassDependencies[0].setSrcStageMask(Flags::PipelineStage::Bits::EarlyFragmentTests)
            .setSrcAccessMask(Flags::Access::Bits::None)
            .setDstStageMask(Flags::PipelineStage::Bits::EarlyFragmentTests)
            .setDstAccessMask(Flags::Access::Bits::DepthStencilAttachmentWrite);

        data.renderPassInfo.setAttachments(data.attachments)
            .setSubpasses(data.subpasses)
            .setDependencies(data.subpassDependencies);

        data.renderPass.create(deviceFunctions, device, data.renderPassInfo);
        return data;
    }

    SwapChainData createBasicSwapChain(
        const InstanceFunctionTable& functions,
        const DeviceFunctionTable& deviceFunctions,
        const PhysicalDevice& physicalDevice,        
        const DeviceRef& device,
        const SurfaceRef& surface,
        const RenderPassRef& renderPass,
        const Extent2D& preferredExtent,
        PixelFormat preferredFormat /*= Format::B8G8R8A8Srgb*/,
        PixelFormat preferredDepthFormat /*= Format::D32Sfloat*/,
        ColorSpace preferredColorSpace /*= ColorSpace::SrgbNonlinear*/,
        Flags::ImageUsage preferredImageUsage /*= Flags::ImageUsage::Bits::ColorAttachment*/,
		PresentMode preferredPresentMode /*= PresentMode::Mailbox*/,
        uint32_t desiredImageCount /*= 2*/,
        uint32_t desiredImageArrayLayerCount /*= 1*/)
    {
		SwapChainData swapChainData;
        swapChainData.swapChainInfo.setClipped(true)
            .setCompositeAlpha(Flags::CompositeAlpha::Bits::Opaque)
            .setImageExtent(preferredExtent)
            .setImageFormat(preferredFormat)
			.setImageColorSpace(preferredColorSpace)
            .setImageUsage(preferredImageUsage)
            .setImageArrayLayers(desiredImageArrayLayerCount)
            .setMinImageCount(desiredImageCount)
            .setPresentMode(preferredPresentMode)
            .setPreTransform(Flags::SurfaceTransform::Bits::Identity)
            .setSurface(surface);

        swapChainData.swapChain.create(deviceFunctions, device, swapChainData.swapChainInfo);
        swapChainData.swapChainImages = swapChainData.swapChain.getImages(deviceFunctions, device);

        swapChainData.depthImageCreateInfo.setArrayLayers(desiredImageArrayLayerCount)
            .setExtent(Extent3D(preferredExtent, 1))
            .setFormat(preferredDepthFormat)
            .setImageType(ImageType::Image2D)
            .setInitialLayout(ImageLayout::Undefined)
            .setMipLevels(1)
            .setSamples(Flags::SampleCount::Bits::SC1)
            .setTiling(ImageTiling::Optimal)
            .setUsage(Flags::ImageUsage::Bits::DepthStencilAttachment);

        swapChainData.depthImage.create(deviceFunctions, device, swapChainData.depthImageCreateInfo);
        auto memRequirements = swapChainData.depthImage.getMemoryRequirements(device, deviceFunctions);
        swapChainData.depthImageMemoryCreateInfo.setAllocationSize(memRequirements.getSize())
            .setMemoryTypeIndex(findMemoryTypeFirstFit(physicalDevice.getMemoryProperties(functions),
                memRequirements.getMemoryTypeBits(), Flags::MemoryProperty::Bits::DeviceLocal));
        swapChainData.depthImageMemory.create(deviceFunctions, device, swapChainData.depthImageMemoryCreateInfo);
        swapChainData.depthImageMemory.bindImage(deviceFunctions, device, swapChainData.depthImage);

        swapChainData.depthImageViewCreateInfo.setImage(swapChainData.depthImage)
            .setViewType(desiredImageArrayLayerCount > 1 ? ImageViewType::T2DArray : ImageViewType::T2D)
            .setFormat(preferredDepthFormat)
            .setSubresourceRange(ImageSubresourceRange(
                Flags::ImageAspect::Bits::Depth, 0, 1, 0, desiredImageArrayLayerCount))
            .setComponents(ComponentMapping());
        swapChainData.depthImageView.create(device, deviceFunctions, swapChainData.depthImageViewCreateInfo);

        swapChainData.swapChainImageViews.reserve(swapChainData.swapChainImages.size());
        swapChainData.swapChainFrameBuffers.reserve(swapChainData.swapChainImages.size());
        swapChainData.swapChainImageViewCreateInfos.reserve(swapChainData.swapChainImages.size());
        swapChainData.swapChainFrameBufferCreateInfos.reserve(swapChainData.swapChainImages.size());

        swapChainData.attachmentRefs.resize(swapChainData.swapChainImages.size());

        for (size_t i = 0; i < swapChainData.swapChainImages.size(); ++i)
        {
            swapChainData.swapChainImageViewCreateInfos.push_back(ImageViewCreateInfo{});
            swapChainData.swapChainImageViewCreateInfos.back().setComponents(ComponentMapping())
                .setSubresourceRange(ImageSubresourceRange(
                    Flags::ImageAspect::Bits::Color, 0, 1, 0, desiredImageArrayLayerCount))
                .setFormat(preferredFormat)
                .setViewType(desiredImageArrayLayerCount > 1 ? ImageViewType::T2DArray : ImageViewType::T2D)
                .setImage(swapChainData.swapChainImages[i]);

            swapChainData.swapChainImageViews.push_back(ImageView{});
            swapChainData.swapChainImageViews.back().create(device, deviceFunctions,
                swapChainData.swapChainImageViewCreateInfos.back());

            swapChainData.attachmentRefs[i] = { swapChainData.swapChainImageViews.back(), swapChainData.depthImageView };
            swapChainData.swapChainFrameBufferCreateInfos.push_back(FrameBufferCreateInfo{});
            swapChainData.swapChainFrameBufferCreateInfos.back().setRenderPass(renderPass)
                .setAttachments(swapChainData.attachmentRefs[i])
                .setExtent(preferredExtent)
                .setLayers(desiredImageArrayLayerCount);

            swapChainData.swapChainFrameBuffers.push_back(FrameBuffer{});
            swapChainData.swapChainFrameBuffers.back().create(device, deviceFunctions, swapChainData.swapChainFrameBufferCreateInfos.back());
        }

		return swapChainData;
    }

    void destroySwapChainDaTa(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, SwapChainData& data)
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
        data.depthImage.destroy(deviceFunctions, device);
        data.depthImageMemory.destroy(deviceFunctions, device);
		data.swapChain.destroy(deviceFunctions, device);
        data.swapChainImages.clear();
    }

    void recreateBasicSwapChain(SwapChainData& data,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const RenderPassRef& renderPass,
        const Extent2D& preferredExtent)
    {
        data.swapChainInfo.setImageExtent(preferredExtent);

        data.swapChain.recreate(deviceFunctions, device, data.swapChainInfo);
        data.swapChainImages = data.swapChain.getImages(deviceFunctions, device);

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
        data.depthImage.destroy(deviceFunctions, device);

        data.depthImageCreateInfo.setExtent(Extent3D(preferredExtent, 1));
        data.depthImage.create(deviceFunctions, device, data.depthImageCreateInfo);

        data.depthImageViewCreateInfo.setImage(data.depthImage);
        data.depthImageView.create(device, deviceFunctions, data.depthImageViewCreateInfo);

        data.swapChainImageViews.reserve(data.swapChainImages.size());
        data.swapChainFrameBuffers.reserve(data.swapChainImages.size());
        data.swapChainImageViewCreateInfos.reserve(data.swapChainImages.size());
        data.swapChainFrameBufferCreateInfos.reserve(data.swapChainImages.size());
        data.attachmentRefs.resize(data.swapChainImages.size());

        for (size_t i = 0; i < data.swapChainImages.size(); ++i)
        {
            data.swapChainImageViewCreateInfos.push_back(ImageViewCreateInfo{});
            data.swapChainImageViewCreateInfos.back().setComponents(ComponentMapping())
                .setSubresourceRange(ImageSubresourceRange(
                    Flags::ImageAspect::Bits::Color, 0, 1, 0, 1))
                .setFormat(data.swapChainInfo.getImageFormat())
                .setViewType(ImageViewType::T2D)
                .setImage(data.swapChainImages[i]);
            data.swapChainImageViews.push_back(ImageView{});
            data.swapChainImageViews.back().create(device, deviceFunctions,
                data.swapChainImageViewCreateInfos.back());
            data.attachmentRefs[i] = { data.swapChainImageViews.back(), data.depthImageView };
            data.swapChainFrameBufferCreateInfos.push_back(FrameBufferCreateInfo{});
            data.swapChainFrameBufferCreateInfos.back().setRenderPass(renderPass)
                .setAttachments(data.attachmentRefs[i])
                .setExtent(preferredExtent)
                .setLayers(1);
            data.swapChainFrameBuffers.push_back(FrameBuffer{});
            data.swapChainFrameBuffers.back().create(device, deviceFunctions, data.swapChainFrameBufferCreateInfos.back());
		}
    }

    void createBasicGraphicsPipeline(GraphicsPipelineData& data,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const RenderPassRef& renderPass,
        uint32_t subpassIndex,
        PrimitiveTopology desiredTopology,
        PolygonMode desiredPolygonMode, Flags::CullMode desiredCullMode,
        FrontFace desiredFrontFace, CompareOp depthCompareOp,
        bool depthWriteEnable)
    {
        data.colorBlendAttachmentStates.push_back(PipelineColorBlendAttachmentState(false,
            BlendFactor::One, BlendFactor::Zero,
            BlendOp::Add, BlendFactor::One,
            BlendFactor::Zero, BlendOp::Add,
            Flags::ColorComponent::Bits::R | Flags::ColorComponent::Bits::G |
            Flags::ColorComponent::Bits::B | Flags::ColorComponent::Bits::A));

        data.graphicsPipelineInfo.setLayout(data.pipelineLayout)
            .setRenderPass(renderPass)
            .setSubpass(subpassIndex)
            .setStages(data.shaderStages)
            .setVertexInputState(PipelineVertexInputStateCreateInfo(data.vertexBindings, data.vertexAttributes))
            .setInputAssemblyState(PipelineInputAssemblyStateCreateInfo(desiredTopology, false))
            .setDynamicState(PipelineDynamicStateCreateInfo(data.dynamicStates))
            .setViewportState(PipelineViewportStateCreateInfo(data.viewports, data.scissors))
            .setRasterizationState({ false, false, desiredPolygonMode,
                desiredCullMode, desiredFrontFace, false, 0.0f, 0.0f, 0.0f, 1.0f })
            .setMultisampleState(PipelineMultisampleStateCreateInfo(
                false, Flags::SampleCount::Bits::SC1, 1.0f, {}, false, false))
            .setColorBlendState(PipelineColorBlendStateCreateInfo(data.colorBlendAttachmentStates,
                false, LogicOp::Copy, Color::Empty()))
            .setDepthStencilState(PipelineDepthStencilStateCreateInfo(StencilOpState(), StencilOpState(),
                depthCompareOp, false, false, depthWriteEnable, 0, 1));
        data.graphicsPipeline.create(deviceFunctions, device, data.graphicsPipelineInfo);
    }

    ShaderModuleData createShaderModules(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, std::span<const std::string> shaderFilePaths)
    {
        ShaderModuleData data;
        data.shaderCodes.resize(shaderFilePaths.size());
        data.shaderModules.resize(shaderFilePaths.size());
        data.shaderModuleCreateInfos.resize(shaderFilePaths.size());
        data.shaderStages.resize(shaderFilePaths.size());

        for (size_t i = 0; i < shaderFilePaths.size(); ++i)
        {
            data.shaderCodes[i] = ShaderModule::parseShaderCodeSPIRV(shaderFilePaths[i]);
            data.shaderModuleCreateInfos[i].setShaderCode(data.shaderCodes[i]);
            data.shaderModules[i].create(deviceFunctions, device, data.shaderModuleCreateInfos[i]);
            data.shaderStages[i] = ShaderModule::inferShaderStage(data.shaderCodes[i]);
        }

        return data;
    }

    ShaderModuleData createShaderModules(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, std::initializer_list<const std::string> shaderFilePaths)
    {
        ShaderModuleData data;
        data.shaderCodes.resize(shaderFilePaths.size());
        data.shaderModules.resize(shaderFilePaths.size());
        data.shaderModuleCreateInfos.resize(shaderFilePaths.size());
        data.shaderStages.resize(shaderFilePaths.size());

        auto file = shaderFilePaths.begin();

        for (size_t i = 0; i < shaderFilePaths.size(); ++i, ++file)
        {
            data.shaderCodes[i] = ShaderModule::parseShaderCodeSPIRV(*file);
            data.shaderModuleCreateInfos[i].setShaderCode(data.shaderCodes[i]);
            data.shaderModules[i].create(deviceFunctions, device, data.shaderModuleCreateInfos[i]);
            data.shaderStages[i] = ShaderModule::inferShaderStage(data.shaderCodes[i]);
        }

        return data;
    }

    std::vector<PipelineShaderStageCreateInfo> createShaderStageInfos(
        const ShaderModuleData& shaderData)
    {
        std::vector<PipelineShaderStageCreateInfo> shaderStageInfos(
            shaderData.shaderModules.size());

        for (size_t i = 0; i < shaderData.shaderModules.size(); ++i)
            shaderStageInfos[i].setModule(shaderData.shaderModules[i])
            .setName("main").setStage(shaderData.shaderStages[i]);
        return shaderStageInfos;
    }

    //void blitImage(const DeviceFunctionTable& deviceFunctions, const CommandBuffer& commandBuffer,
    //    const ImageRef& srcImage, const ImageRef& dstImage, const Extent2D& srcExtent, const Extent2D& dstExtent)
    //{
    //    ImageBlit blit;
    //    blit.setSrcOffsets({ 0, 0, 0 }, { srcExtent.width, srcExtent.height, 1 });
    //    blit.getSrcSubresource()
    //        .setAspectMask(Flags::ImageAspect::Bits::Color)
    //        .setBaseArrayLayer(0)
    //        .setLayerCount(1)
    //        .setMipLevel(0);
    //    blit.setDstOffsets({ 0, 0, 0 }, { dstExtent.width, dstExtent.height, 1 });
    //    blit.getDstSubresource()
    //        .setAspectMask(Flags::ImageAspect::Bits::Color)
    //        .setBaseArrayLayer(0)
    //        .setLayerCount(1)
    //        .setMipLevel(0);

    //    commandBuffer.blitImage(srcImage, dstImage, { blit }, Flags::ImageLayout::Transfer, Flags::ImageLayout::Transfer);

    //}

    void copyBufferToImage(const DeviceFunctionTable& deviceFunctions, CommandBuffer& commandBuffer,
        const BufferRef& srcBuffer, const ImageRef& dstImage, const Extent3D& dstExtent,
        ImageLayout srcLayout, ImageLayout dstLayout)
    {
        // Transition: Undefined -> TransferDstOptimal
        ImageMemoryBarrier barrier1;
        barrier1.setOldLayout(srcLayout)
            .setNewLayout(ImageLayout::TransferDstOptimal)
            .setSrcAccessMask(Flags::Access::Bits::None)
            .setDstAccessMask(Flags::Access::Bits::TransferWrite)
            .setImage(dstImage)
            .setSubresourceRange(ImageSubresourceRange()
                .setAspectMask(Flags::ImageAspect::Bits::Color)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1));

        commandBuffer.pipelineBarrier(deviceFunctions,
            Flags::PipelineStage::Bits::TopOfPipe,
            Flags::PipelineStage::Bits::Transfer,
            Flags::Dependency::Bits::None,
            {}, {}, std::span<ImageMemoryBarrier>(&barrier1, 1));

        // Copy buffer to image
        BufferImageCopy copyRegion;
        copyRegion.setBufferOffset(0)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource(ImageSubresourceLayers()
                .setAspectMask(Flags::ImageAspect::Bits::Color)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1))
            .setImageOffset({ 0, 0, 0 })
            .setImageExtent(dstExtent);

        commandBuffer.copyBufferToImage(deviceFunctions,
            srcBuffer, dstImage,
            ImageLayout::TransferDstOptimal,
            std::span<BufferImageCopy>(&copyRegion, 1));

        // Transition: TransferDstOptimal -> ShaderReadOnlyOptimal
        ImageMemoryBarrier barrier2;
        barrier2.setOldLayout(ImageLayout::TransferDstOptimal)
            .setNewLayout(dstLayout)
            .setSrcAccessMask(Flags::Access::Bits::TransferWrite)
            .setDstAccessMask(Flags::Access::Bits::ShaderRead)
            .setImage(dstImage)
            .setSubresourceRange(ImageSubresourceRange()
                .setAspectMask(Flags::ImageAspect::Bits::Color)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1));

        commandBuffer.pipelineBarrier(deviceFunctions,
            Flags::PipelineStage::Bits::Transfer,
            Flags::PipelineStage::Bits::FragmentShader,
            Flags::Dependency::Bits::None,
            {}, {}, std::span<ImageMemoryBarrier>(&barrier2, 1));
    }

    void transferData(const DeviceFunctionTable& deviceFunctions, CommandBuffer& commandBuffer,
        const BufferRef& srcBuffer, const BufferRef& dstBuffer, size_t size, size_t srcOffset /*= 0*/, size_t dstOffset /*= 0*/)
    {
        BufferCopy copyRegion;
        copyRegion.setSize(size)
            .setSrcOffset(srcOffset)
            .setDstOffset(dstOffset);
        commandBuffer.copyBuffer(deviceFunctions, srcBuffer, dstBuffer, copyRegion);
    }

    MemoryRequirements initBufferMemoryPairFirstFit(const DeviceFunctionTable& deviceFunctions, DeviceRef device, 
        const PhysicalDeviceMemoryProperties& deviceMemoryProps, Buffer& buffer, Memory& memory, 
        size_t size, Flags::BufferUsage bufferUsage, Flags::MemoryProperty requiredProperties,
        Flags::MemoryProperty forbiddenProperties, //= Flags::MemoryProperty::Bits::None 
        SharingMode sharingMode //= SharingMode::Exclusive
    ) {

        buffer.create(deviceFunctions, device, { size, bufferUsage, sharingMode });
        auto memoryRequirements = buffer.getMemoryRequirements(deviceFunctions, device);
        memory.create(deviceFunctions, device, {
            memoryRequirements.getSize(), Utility::findMemoryTypeFirstFit(deviceMemoryProps,
                memoryRequirements.getMemoryTypeBits(), requiredProperties, forbiddenProperties)
            });
        memory.bindBuffer(deviceFunctions, device, buffer);
        return memoryRequirements;
    }
}