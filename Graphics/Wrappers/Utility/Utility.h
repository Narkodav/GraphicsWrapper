#pragma once
#include "../PhysicalDevice.h"
#include "../../InstanceFunctionTable.h"
#include "../../Enums.h"
#include "../SwapChain.h"
#include "../FrameBuffer.h"
#include "../ShaderModule.h"
#include "Structs.h"

// Utility functions that abstract some common operations
namespace Graphics::Utility
{    
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
    BasicSwapChainData createBasicSwapChain(
        const InstanceFunctionTable& functions,
        const PhysicalDevice& physicalDevice,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const SurfaceRef& surface,
        const RenderPassRef& renderPass,
        const Extent2D& preferredExtent,
		Format preferredFormat = Format::B8G8R8A8Srgb, 
        Format preferredDepthFormat = Format::D32Sfloat,
        ColorSpace preferredColorSpace = ColorSpace::SrgbNonlinear,
        ImageUsage::Flags preferredImageUsage = ImageUsage::Bits::ColorAttachment,
        PresentMode preferredPresentMode = PresentMode::Mailbox,
        uint32_t desiredImageCount = 2,
        uint32_t desiredImageArrayLayerCount = 1);

    void recreateBasicSwapChain(BasicSwapChainData& data,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const RenderPassRef& renderPass,
        const Extent2D& preferredExtent);

    void destroySwapChainDaTa(BasicSwapChainData& data,
        const DeviceRef& device,
        const DeviceFunctionTable& deviceFunctions);

    void createBasicGraphicsPipeline(BasicGraphicsPipelineData& data,
        const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device,
        const RenderPassRef& renderPass,
        uint32_t subpassIndex,
        PrimitiveTopology desiredTopology,
        PolygonMode desiredPolygonMode, CullMode::Flags desiredCullMode,
        FrontFace desiredFrontFace, CompareOp depthCompareOp,
        bool depthWriteEnable, bool depthTestEnable);


    ShaderModuleData createShaderModules(const DeviceFunctionTable& deviceFunctions,
        const DeviceRef& device, const std::vector<std::string>& shaderFilePaths);

    std::vector<Pipeline::ShaderStageCreateInfo> createShaderStageInfos(const ShaderModuleData& shaderData);
}