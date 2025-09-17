#pragma once
#include "../PhysicalDevice.h"
#include "../../InstanceFunctionTable.h"
#include "../../Enums.h"
#include "../SwapChain.h"
#include "../FrameBuffer.h"

// Utility functions that abstract some common operations
namespace Graphics::Utility
{
    struct BasicSwapChainData {
        SwapChain swapChain;
        SwapChain::CreateInfo swapChainInfo;

        SwapChain::Images swapChainImages;

        std::vector<Image::View::CreateInfo> swapChainImageViewCreateInfos;
        std::vector<FrameBuffer::CreateInfo> swapChainFrameBufferCreateInfos;
        std::vector<Image::View> swapChainImageViews;
        std::vector<FrameBuffer> swapChainFrameBuffers;

        std::vector<std::vector<Image::ViewRef>> attachmentRefs;

        Image depthImage;
        Image::View depthImageView;

        Image::CreateInfo depthImageCreateInfo;
        Image::View::CreateInfo depthImageViewCreateInfo;
    };

	//returns a memory type index based on type filter and properties requirements
    uint32_t findMemoryType(const InstanceFunctionTable& functions,
        const PhysicalDevice& device, uint32_t typeFilter,
        MemoryProperty::Flags properties);

    //returns a supported format based on tiling and features requirements
    Format findSupportedFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device, const std::vector<Format>& candidates,
        ImageTiling tiling, FormatFeature::Flags features);

	//finds a commonly used depth format
    Format findDepthFormat(const InstanceFunctionTable& functions,
        const PhysicalDevice& device);

	//calculates the number of mip levels for a given extent
    uint32_t calculateMipLevels(const Extent2D& extent);

	//chooses a swap chain extent based on surface capabilities and desired frame buffer extent
	Extent2D chooseExtent(const Surface::Capabilities& capabilities, const Extent2D& frameBufferExtent);

	//creates a basic swap chain with image views and frame buffers, optionally with depth buffer
    BasicSwapChainData createBasicSwapChain(const InstanceFunctionTable& instanceFunctions,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const SurfaceRef& surface,
        const Extent2D& preferredExtent,
		Format preferredFormat = Format::B8G8R8A8Srgb, 
        Format preferredDepthFormat = Format::D32Sfloat,
        ColorSpace preferredColorSpace = ColorSpace::SrgbNonlinear,
        ImageUsage::Flags preferredImageUsage = ImageUsage::Bits::ColorAttachment,
        PresentMode preferredPresentMode = PresentMode::Mailbox,
        uint32_t desiredImageCount = 2);

    void recreateBasicSwapChain(BasicSwapChainData& data,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const SurfaceRef& surface,
        const Extent2D& preferredExtent);

    void destroySwapChainDaTa(BasicSwapChainData& data,
        const DeviceRef& device,
        const DeviceFunctionTable& deviceFunctions);
}