#pragma once
#include "../PhysicalDevice.h"
#include "../../InstanceFunctionTable.h"
#include "../../Enums.h"
#include "../SwapChain.h"
#include "../FrameBuffer.h"
#include "../Pipeline.h"
#include "../DescriptorSet.h"

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
        Memory depthImageMemory;
        Image::View depthImageView;

        Image::CreateInfo depthImageCreateInfo;
        Memory::AllocateInfo depthImageMemoryCreateInfo;
        Image::View::CreateInfo depthImageViewCreateInfo;
    };

    struct BasicGraphicsPipelineData {
        Pipeline graphicsPipeline;
        Pipeline::CreateInfo graphicsPipelineInfo;
        std::vector<Pipeline::ColorBlendAttachmentState> colorBlendAttachmentStates;

        //these are populated beforehand
        Pipeline::LayoutRef pipelineLayout;
        std::vector<Pipeline::ShaderStageCreateInfo> shaderStages;
        std::vector<VertexInputBindingDescription> vertexBindings;
        std::vector<VertexInputAttributeDescription> vertexAttributes;        
        std::vector<DynamicState> dynamicStates;
        std::vector<Viewport> viewports;
        std::vector<Scissor> scissors;
    };

    struct ShaderModuleData {
        std::vector<std::vector<char>> shaderCodes;
        std::vector<ShaderModule> shaderModules;
        std::vector<ShaderModule::CreateInfo> shaderModuleCreateInfos;
        std::vector<ShaderStage::Bits> shaderStages;
    };
    
    //class ShaderReflection {
    //public:
    //    std::vector<VertexInputBindingDescription> vertexBindings;
    //    std::vector<VertexInputAttributeDescription> vertexAttributes;
    //    std::vector<DescriptorSet::Layout::Binding> descriptorBindings;
    //    std::vector<PushConstantRange> pushConstants;
    //};
}