#pragma once
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace Graphics {

    template<typename T>
    struct EnumVulkanConnect;

    template<typename T>
    struct EnumCVulkanConnect;

    enum class DescriptorType
    {
        UniformBuffer = vk::DescriptorType::eUniformBuffer,
        Sampler = vk::DescriptorType::eSampler,
        CombinedImageSampler = vk::DescriptorType::eCombinedImageSampler,
        StorageBuffer = vk::DescriptorType::eStorageBuffer,
        StorageImage = vk::DescriptorType::eStorageImage,
        InputAttachment = vk::DescriptorType::eInputAttachment,
        UniformTexelBuffer = vk::DescriptorType::eUniformTexelBuffer,
        StorageTexelBuffer = vk::DescriptorType::eStorageTexelBuffer,
    };

    template<>
    struct EnumVulkanConnect<DescriptorType> {
        using CorrespondingType = vk::DescriptorType;
    };

    template<>
    struct EnumVulkanConnect<vk::DescriptorType> {
        using CorrespondingType = DescriptorType;
    };

    template<>
    struct EnumCVulkanConnect<DescriptorType> {
        using CorrespondingType = VkDescriptorType;
    };

    template<>
    struct EnumCVulkanConnect<VkDescriptorType> {
        using CorrespondingType = DescriptorType;
    };

    enum class ImageLayout
    {
        Undefined = vk::ImageLayout::eUndefined,
        General = vk::ImageLayout::eGeneral,
        ColorAttachmentOptimal = vk::ImageLayout::eColorAttachmentOptimal,
        DepthStencilAttachmentOptimal = vk::ImageLayout::eDepthStencilAttachmentOptimal,
        DepthStencilReadOnlyOptimal = vk::ImageLayout::eDepthStencilReadOnlyOptimal,
        ShaderReadOnlyOptimal = vk::ImageLayout::eShaderReadOnlyOptimal,
        TransferSrcOptimal = vk::ImageLayout::eTransferSrcOptimal,
        TransferDstOptimal = vk::ImageLayout::eTransferDstOptimal,
        Preinitialized = vk::ImageLayout::ePreinitialized,
        DepthReadOnlyStencilAttachmentOptimal = vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal,
        DepthAttachmentStencilReadOnlyOptimal = vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal,
        DepthAttachmentOptimal = vk::ImageLayout::eDepthAttachmentOptimal,
        DepthReadOnlyOptimal = vk::ImageLayout::eDepthReadOnlyOptimal,
        StencilAttachmentOptimal = vk::ImageLayout::eStencilAttachmentOptimal,
        StencilReadOnlyOptimal = vk::ImageLayout::eStencilReadOnlyOptimal,
        ReadOnlyOptimal = vk::ImageLayout::eReadOnlyOptimal,
        AttachmentOptimal = vk::ImageLayout::eAttachmentOptimal,
        PresentSrcKHR = vk::ImageLayout::ePresentSrcKHR,
        VideoDecodeDstKHR = vk::ImageLayout::eVideoDecodeDstKHR,
        VideoDecodeSrcKHR = vk::ImageLayout::eVideoDecodeSrcKHR,
        VideoDecodeDpbKHR = vk::ImageLayout::eVideoDecodeDpbKHR,
        SharedPresentKHR = vk::ImageLayout::eSharedPresentKHR,
        FragmentDensityMapOptimalEXT = vk::ImageLayout::eFragmentDensityMapOptimalEXT,
        FragmentShadingRateAttachmentOptimalKHR = vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR,
        AttachmentFeedbackLoopOptimalEXT = vk::ImageLayout::eAttachmentFeedbackLoopOptimalEXT,
    };

    template<>
    struct EnumVulkanConnect<ImageLayout> {
        using CorrespondingType = vk::ImageLayout;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageLayout> {
        using CorrespondingType = ImageLayout;
    };

    template<>
    struct EnumCVulkanConnect<ImageLayout> {
        using CorrespondingType = VkImageLayout;
    };

    template<>
    struct EnumCVulkanConnect<VkImageLayout> {
        using CorrespondingType = ImageLayout;
    };

    enum class Format
    {
        Undefined = vk::Format::eUndefined,
        R4G4UnormPack8 = vk::Format::eR4G4UnormPack8,
        R4G4B4A4UnormPack16 = vk::Format::eR4G4B4A4UnormPack16,
        B4G4R4A4UnormPack16 = vk::Format::eB4G4R4A4UnormPack16,
        R5G6B5UnormPack16 = vk::Format::eR5G6B5UnormPack16,
        B5G6R5UnormPack16 = vk::Format::eB5G6R5UnormPack16,
        R5G5B5A1UnormPack16 = vk::Format::eR5G5B5A1UnormPack16,
        B5G5R5A1UnormPack16 = vk::Format::eB5G5R5A1UnormPack16,
        A1R5G5B5UnormPack16 = vk::Format::eA1R5G5B5UnormPack16,
        R8Unorm = vk::Format::eR8Unorm,
        R8Snorm = vk::Format::eR8Snorm,
        R8Uscaled = vk::Format::eR8Uscaled,
        R8Sscaled = vk::Format::eR8Sscaled,
        R8Uint = vk::Format::eR8Uint,
        R8Sint = vk::Format::eR8Sint,
        R8Srgb = vk::Format::eR8Srgb,
        R8G8Unorm = vk::Format::eR8G8Unorm,
        R8G8Snorm = vk::Format::eR8G8Snorm,
        R8G8Uscaled = vk::Format::eR8G8Uscaled,
        R8G8Sscaled = vk::Format::eR8G8Sscaled,
        R8G8Uint = vk::Format::eR8G8Uint,
        R8G8Sint = vk::Format::eR8G8Sint,
        R8G8Srgb = vk::Format::eR8G8Srgb,
        R8G8B8Unorm = vk::Format::eR8G8B8Unorm,
        R8G8B8Snorm = vk::Format::eR8G8B8Snorm,
        R8G8B8Uscaled = vk::Format::eR8G8B8Uscaled,
        R8G8B8Sscaled = vk::Format::eR8G8B8Sscaled,
        R8G8B8Uint = vk::Format::eR8G8B8Uint,
        R8G8B8Sint = vk::Format::eR8G8B8Sint,
        R8G8B8Srgb = vk::Format::eR8G8B8Srgb,
        B8G8R8Unorm = vk::Format::eB8G8R8Unorm,
        B8G8R8Snorm = vk::Format::eB8G8R8Snorm,
        B8G8R8Uscaled = vk::Format::eB8G8R8Uscaled,
        B8G8R8Sscaled = vk::Format::eB8G8R8Sscaled,
        B8G8R8Uint = vk::Format::eB8G8R8Uint,
        B8G8R8Sint = vk::Format::eB8G8R8Sint,
        B8G8R8Srgb = vk::Format::eB8G8R8Srgb,
        R8G8B8A8Unorm = vk::Format::eR8G8B8A8Unorm,
        R8G8B8A8Snorm = vk::Format::eR8G8B8A8Snorm,
        R8G8B8A8Uscaled = vk::Format::eR8G8B8A8Uscaled,
        R8G8B8A8Sscaled = vk::Format::eR8G8B8A8Sscaled,
        R8G8B8A8Uint = vk::Format::eR8G8B8A8Uint,
        R8G8B8A8Sint = vk::Format::eR8G8B8A8Sint,
        R8G8B8A8Srgb = vk::Format::eR8G8B8A8Srgb,
        B8G8R8A8Unorm = vk::Format::eB8G8R8A8Unorm,
        B8G8R8A8Snorm = vk::Format::eB8G8R8A8Snorm,
        B8G8R8A8Uscaled = vk::Format::eB8G8R8A8Uscaled,
        B8G8R8A8Sscaled = vk::Format::eB8G8R8A8Sscaled,
        B8G8R8A8Uint = vk::Format::eB8G8R8A8Uint,
        B8G8R8A8Sint = vk::Format::eB8G8R8A8Sint,
        B8G8R8A8Srgb = vk::Format::eB8G8R8A8Srgb,
        A8B8G8R8UnormPack32 = vk::Format::eA8B8G8R8UnormPack32,
        A8B8G8R8SnormPack32 = vk::Format::eA8B8G8R8SnormPack32,
        A8B8G8R8UscaledPack32 = vk::Format::eA8B8G8R8UscaledPack32,
        A8B8G8R8SscaledPack32 = vk::Format::eA8B8G8R8SscaledPack32,
        A8B8G8R8UintPack32 = vk::Format::eA8B8G8R8UintPack32,
        A8B8G8R8SintPack32 = vk::Format::eA8B8G8R8SintPack32,
        A8B8G8R8SrgbPack32 = vk::Format::eA8B8G8R8SrgbPack32,
        A2R10G10B10UnormPack32 = vk::Format::eA2R10G10B10UnormPack32,
        A2R10G10B10SnormPack32 = vk::Format::eA2R10G10B10SnormPack32,
        A2R10G10B10UscaledPack32 = vk::Format::eA2R10G10B10UscaledPack32,
        A2R10G10B10SscaledPack32 = vk::Format::eA2R10G10B10SscaledPack32,
        A2R10G10B10UintPack32 = vk::Format::eA2R10G10B10UintPack32,
        A2R10G10B10SintPack32 = vk::Format::eA2R10G10B10SintPack32,
        A2B10G10R10UnormPack32 = vk::Format::eA2B10G10R10UnormPack32,
        A2B10G10R10SnormPack32 = vk::Format::eA2B10G10R10SnormPack32,
        A2B10G10R10UscaledPack32 = vk::Format::eA2B10G10R10UscaledPack32,
        A2B10G10R10SscaledPack32 = vk::Format::eA2B10G10R10SscaledPack32,
        A2B10G10R10UintPack32 = vk::Format::eA2B10G10R10UintPack32,
        A2B10G10R10SintPack32 = vk::Format::eA2B10G10R10SintPack32,
        R16Unorm = vk::Format::eR16Unorm,
        R16Snorm = vk::Format::eR16Snorm,
        R16Uscaled = vk::Format::eR16Uscaled,
        R16Sscaled = vk::Format::eR16Sscaled,
        R16Uint = vk::Format::eR16Uint,
        R16Sint = vk::Format::eR16Sint,
        R16Sfloat = vk::Format::eR16Sfloat,
        R16G16Unorm = vk::Format::eR16G16Unorm,
        R16G16Snorm = vk::Format::eR16G16Snorm,
        R16G16Uscaled = vk::Format::eR16G16Uscaled,
        R16G16Sscaled = vk::Format::eR16G16Sscaled,
        R16G16Uint = vk::Format::eR16G16Uint,
        R16G16Sint = vk::Format::eR16G16Sint,
        R16G16Sfloat = vk::Format::eR16G16Sfloat,
        R16G16B16Unorm = vk::Format::eR16G16B16Unorm,
        R16G16B16Snorm = vk::Format::eR16G16B16Snorm,
        R16G16B16Uscaled = vk::Format::eR16G16B16Uscaled,
        R16G16B16Sscaled = vk::Format::eR16G16B16Sscaled,
        R16G16B16Uint = vk::Format::eR16G16B16Uint,
        R16G16B16Sint = vk::Format::eR16G16B16Sint,
        R16G16B16Sfloat = vk::Format::eR16G16B16Sfloat,
        R16G16B16A16Unorm = vk::Format::eR16G16B16A16Unorm,
        R16G16B16A16Snorm = vk::Format::eR16G16B16A16Snorm,
        R16G16B16A16Uscaled = vk::Format::eR16G16B16A16Uscaled,
        R16G16B16A16Sscaled = vk::Format::eR16G16B16A16Sscaled,
        R16G16B16A16Uint = vk::Format::eR16G16B16A16Uint,
        R16G16B16A16Sint = vk::Format::eR16G16B16A16Sint,
        R16G16B16A16Sfloat = vk::Format::eR16G16B16A16Sfloat,
        R32Uint = vk::Format::eR32Uint,
        R32Sint = vk::Format::eR32Sint,
        R32Sfloat = vk::Format::eR32Sfloat,
        R32G32Uint = vk::Format::eR32G32Uint,
        R32G32Sint = vk::Format::eR32G32Sint,
        R32G32Sfloat = vk::Format::eR32G32Sfloat,
        R32G32B32Uint = vk::Format::eR32G32B32Uint,
        R32G32B32Sint = vk::Format::eR32G32B32Sint,
        R32G32B32Sfloat = vk::Format::eR32G32B32Sfloat,
        R32G32B32A32Uint = vk::Format::eR32G32B32A32Uint,
        R32G32B32A32Sint = vk::Format::eR32G32B32A32Sint,
        R32G32B32A32Sfloat = vk::Format::eR32G32B32A32Sfloat,
        R64Uint = vk::Format::eR64Uint,
        R64Sint = vk::Format::eR64Sint,
        R64Sfloat = vk::Format::eR64Sfloat,
        R64G64Uint = vk::Format::eR64G64Uint,
        R64G64Sint = vk::Format::eR64G64Sint,
        R64G64Sfloat = vk::Format::eR64G64Sfloat,
        R64G64B64Uint = vk::Format::eR64G64B64Uint,
        R64G64B64Sint = vk::Format::eR64G64B64Sint,
        R64G64B64Sfloat = vk::Format::eR64G64B64Sfloat,
        R64G64B64A64Uint = vk::Format::eR64G64B64A64Uint,
        R64G64B64A64Sint = vk::Format::eR64G64B64A64Sint,
        R64G64B64A64Sfloat = vk::Format::eR64G64B64A64Sfloat,
        B10G11R11UfloatPack32 = vk::Format::eB10G11R11UfloatPack32,
        E5B9G9R9UfloatPack32 = vk::Format::eE5B9G9R9UfloatPack32,
        D16Unorm = vk::Format::eD16Unorm,
        X8D24UnormPack32 = vk::Format::eX8D24UnormPack32,
        D32Sfloat = vk::Format::eD32Sfloat,
        S8Uint = vk::Format::eS8Uint,
        D16UnormS8Uint = vk::Format::eD16UnormS8Uint,
        D24UnormS8Uint = vk::Format::eD24UnormS8Uint,
        D32SfloatS8Uint = vk::Format::eD32SfloatS8Uint,
        Bc1RgbUnormBlock = vk::Format::eBc1RgbUnormBlock,
        Bc1RgbSrgbBlock = vk::Format::eBc1RgbSrgbBlock,
        Bc1RgbaUnormBlock = vk::Format::eBc1RgbaUnormBlock,
        Bc1RgbaSrgbBlock = vk::Format::eBc1RgbaSrgbBlock,
        Bc2UnormBlock = vk::Format::eBc2UnormBlock,
        Bc2SrgbBlock = vk::Format::eBc2SrgbBlock,
        Bc3UnormBlock = vk::Format::eBc3UnormBlock,
        Bc3SrgbBlock = vk::Format::eBc3SrgbBlock,
        Bc4UnormBlock = vk::Format::eBc4UnormBlock,
        Bc4SnormBlock = vk::Format::eBc4SnormBlock,
        Bc5UnormBlock = vk::Format::eBc5UnormBlock,
        Bc5SnormBlock = vk::Format::eBc5SnormBlock,
        Bc6HUfloatBlock = vk::Format::eBc6HUfloatBlock,
        Bc6HSfloatBlock = vk::Format::eBc6HSfloatBlock,
        Bc7UnormBlock = vk::Format::eBc7UnormBlock,
        Bc7SrgbBlock = vk::Format::eBc7SrgbBlock,
        Etc2R8G8B8UnormBlock = vk::Format::eEtc2R8G8B8UnormBlock,
        Etc2R8G8B8SrgbBlock = vk::Format::eEtc2R8G8B8SrgbBlock,
        Etc2R8G8B8A1UnormBlock = vk::Format::eEtc2R8G8B8A1UnormBlock,
        Etc2R8G8B8A1SrgbBlock = vk::Format::eEtc2R8G8B8A1SrgbBlock,
        Etc2R8G8B8A8UnormBlock = vk::Format::eEtc2R8G8B8A8UnormBlock,
        Etc2R8G8B8A8SrgbBlock = vk::Format::eEtc2R8G8B8A8SrgbBlock,
        EacR11UnormBlock = vk::Format::eEacR11UnormBlock,
        EacR11SnormBlock = vk::Format::eEacR11SnormBlock,
        EacR11G11UnormBlock = vk::Format::eEacR11G11UnormBlock,
        EacR11G11SnormBlock = vk::Format::eEacR11G11SnormBlock,
        Astc4x4UnormBlock = vk::Format::eAstc4x4UnormBlock,
        Astc4x4SrgbBlock = vk::Format::eAstc4x4SrgbBlock,
        Astc5x4UnormBlock = vk::Format::eAstc5x4UnormBlock,
        Astc5x4SrgbBlock = vk::Format::eAstc5x4SrgbBlock,
        Astc5x5UnormBlock = vk::Format::eAstc5x5UnormBlock,
        Astc5x5SrgbBlock = vk::Format::eAstc5x5SrgbBlock,
        Astc6x5UnormBlock = vk::Format::eAstc6x5UnormBlock,
        Astc6x5SrgbBlock = vk::Format::eAstc6x5SrgbBlock,
        Astc6x6UnormBlock = vk::Format::eAstc6x6UnormBlock,
        Astc6x6SrgbBlock = vk::Format::eAstc6x6SrgbBlock,
        Astc8x5UnormBlock = vk::Format::eAstc8x5UnormBlock,
        Astc8x5SrgbBlock = vk::Format::eAstc8x5SrgbBlock,
        Astc8x6UnormBlock = vk::Format::eAstc8x6UnormBlock,
        Astc8x6SrgbBlock = vk::Format::eAstc8x6SrgbBlock,
        Astc8x8UnormBlock = vk::Format::eAstc8x8UnormBlock,
        Astc8x8SrgbBlock = vk::Format::eAstc8x8SrgbBlock,
        Astc10x5UnormBlock = vk::Format::eAstc10x5UnormBlock,
        Astc10x5SrgbBlock = vk::Format::eAstc10x5SrgbBlock,
        Astc10x6UnormBlock = vk::Format::eAstc10x6UnormBlock,
        Astc10x6SrgbBlock = vk::Format::eAstc10x6SrgbBlock,
        Astc10x8UnormBlock = vk::Format::eAstc10x8UnormBlock,
        Astc10x8SrgbBlock = vk::Format::eAstc10x8SrgbBlock,
        Astc10x10UnormBlock = vk::Format::eAstc10x10UnormBlock,
        Astc10x10SrgbBlock = vk::Format::eAstc10x10SrgbBlock,
        Astc12x10UnormBlock = vk::Format::eAstc12x10UnormBlock,
        Astc12x10SrgbBlock = vk::Format::eAstc12x10SrgbBlock,
        Astc12x12UnormBlock = vk::Format::eAstc12x12UnormBlock,
        Astc12x12SrgbBlock = vk::Format::eAstc12x12SrgbBlock,
        G8B8G8R8422Unorm = vk::Format::eG8B8G8R8422Unorm,
        B8G8R8G8422Unorm = vk::Format::eB8G8R8G8422Unorm,
        G8B8R83Plane420Unorm = vk::Format::eG8B8R83Plane420Unorm,
        G8B8R82Plane420Unorm = vk::Format::eG8B8R82Plane420Unorm,
        G8B8R83Plane422Unorm = vk::Format::eG8B8R83Plane422Unorm,
        G8B8R82Plane422Unorm = vk::Format::eG8B8R82Plane422Unorm,
        G8B8R83Plane444Unorm = vk::Format::eG8B8R83Plane444Unorm,
        R10X6UnormPack16 = vk::Format::eR10X6UnormPack16,
        R10X6G10X6Unorm2Pack16 = vk::Format::eR10X6G10X6Unorm2Pack16,
        R10X6G10X6B10X6A10X6Unorm4Pack16 = vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16,
        G10X6B10X6G10X6R10X6422Unorm4Pack16 = vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16,
        B10X6G10X6R10X6G10X6422Unorm4Pack16 = vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16,
        G10X6B10X6R10X63Plane420Unorm3Pack16 = vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16,
        G10X6B10X6R10X62Plane420Unorm3Pack16 = vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16,
        G10X6B10X6R10X63Plane422Unorm3Pack16 = vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16,
        G10X6B10X6R10X62Plane422Unorm3Pack16 = vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16,
        G10X6B10X6R10X63Plane444Unorm3Pack16 = vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16,
        R12X4UnormPack16 = vk::Format::eR12X4UnormPack16,
        R12X4G12X4Unorm2Pack16 = vk::Format::eR12X4G12X4Unorm2Pack16,
        R12X4G12X4B12X4A12X4Unorm4Pack16 = vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16,
        G12X4B12X4G12X4R12X4422Unorm4Pack16 = vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16,
        B12X4G12X4R12X4G12X4422Unorm4Pack16 = vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16,
        //G12X4B12X4R12X63Plane420Unorm3Pack16 = vk::Format::eG12X4B12X4R12X63Plane420Unorm3Pack16,
        //G12X4B12X4R12X62Plane420Unorm3Pack16 = vk::Format::eG12X4B12X4R12X62Plane420Unorm3Pack16,
        //G12X4B12X4R12X63Plane422Unorm3Pack16 = vk::Format::eG12X4B12X4R12X63Plane422Unorm3Pack16,
        //G12X4B12X4R12X62Plane422Unorm3Pack16 = vk::Format::eG12X4B12X4R12X62Plane422Unorm3Pack16,
        //G12X4B12X4R12X63Plane444Unorm3Pack16 = vk::Format::eG12X4B12X4R12X63Plane444Unorm3Pack16,
        G16B16G16R16422Unorm = vk::Format::eG16B16G16R16422Unorm,
        B16G16R16G16422Unorm = vk::Format::eB16G16R16G16422Unorm,
        G16B16R163Plane420Unorm = vk::Format::eG16B16R163Plane420Unorm,
        G16B16R162Plane420Unorm = vk::Format::eG16B16R162Plane420Unorm,
        G16B16R163Plane422Unorm = vk::Format::eG16B16R163Plane422Unorm,
        G16B16R162Plane422Unorm = vk::Format::eG16B16R162Plane422Unorm,
        G16B16R163Plane444Unorm = vk::Format::eG16B16R163Plane444Unorm,
        Pvrtc12BppUnormBlockIMG = vk::Format::ePvrtc12BppUnormBlockIMG,
        Pvrtc14BppUnormBlockIMG = vk::Format::ePvrtc14BppUnormBlockIMG,
        Pvrtc22BppUnormBlockIMG = vk::Format::ePvrtc22BppUnormBlockIMG,
        Pvrtc24BppUnormBlockIMG = vk::Format::ePvrtc24BppUnormBlockIMG,
        Pvrtc12BppSrgbBlockIMG = vk::Format::ePvrtc12BppSrgbBlockIMG,
        Pvrtc14BppSrgbBlockIMG = vk::Format::ePvrtc14BppSrgbBlockIMG,
        Pvrtc22BppSrgbBlockIMG = vk::Format::ePvrtc22BppSrgbBlockIMG,
        Pvrtc24BppSrgbBlockIMG = vk::Format::ePvrtc24BppSrgbBlockIMG,
        Astc4x4SfloatBlockEXT = vk::Format::eAstc4x4SfloatBlockEXT,
        Astc5x4SfloatBlockEXT = vk::Format::eAstc5x4SfloatBlockEXT,
        Astc5x5SfloatBlockEXT = vk::Format::eAstc5x5SfloatBlockEXT,
        Astc6x5SfloatBlockEXT = vk::Format::eAstc6x5SfloatBlockEXT,
        Astc6x6SfloatBlockEXT = vk::Format::eAstc6x6SfloatBlockEXT,
        Astc8x5SfloatBlockEXT = vk::Format::eAstc8x5SfloatBlockEXT,
        Astc8x6SfloatBlockEXT = vk::Format::eAstc8x6SfloatBlockEXT,
        Astc8x8SfloatBlockEXT = vk::Format::eAstc8x8SfloatBlockEXT,
        Astc10x5SfloatBlockEXT = vk::Format::eAstc10x5SfloatBlockEXT,
        Astc10x6SfloatBlockEXT = vk::Format::eAstc10x6SfloatBlockEXT,
        Astc10x8SfloatBlockEXT = vk::Format::eAstc10x8SfloatBlockEXT,
        Astc10x10SfloatBlockEXT = vk::Format::eAstc10x10SfloatBlockEXT,
        Astc12x10SfloatBlockEXT = vk::Format::eAstc12x10SfloatBlockEXT,
        Astc12x12SfloatBlockEXT = vk::Format::eAstc12x12SfloatBlockEXT,
        A4R4G4B4UnormPack16EXT = vk::Format::eA4R4G4B4UnormPack16EXT,
        A4B4G4R4UnormPack16EXT = vk::Format::eA4B4G4R4UnormPack16EXT,
    };

    template<>
    struct EnumVulkanConnect<Format> {
        using CorrespondingType = vk::Format;
    };

    template<>
    struct EnumVulkanConnect<vk::Format> {
        using CorrespondingType = Format;
    };

    template<>
    struct EnumCVulkanConnect<Format> {
        using CorrespondingType = VkFormat;
    };

    template<>
    struct EnumCVulkanConnect<VkFormat> {
        using CorrespondingType = Format;
    };

    enum class Filter
    {
        Linear = vk::Filter::eLinear,
        Nearest = vk::Filter::eNearest,
        CubicEXT = vk::Filter::eCubicEXT,
        CubicIMG = vk::Filter::eCubicIMG,
    };

    template<>
    struct EnumVulkanConnect<Filter> {
        using CorrespondingType = vk::Filter;
    };

    template<>
    struct EnumVulkanConnect<vk::Filter> {
        using CorrespondingType = Filter;
    };

    template<>
    struct EnumCVulkanConnect<Filter> {
        using CorrespondingType = VkFilter;
    };

    template<>
    struct EnumCVulkanConnect<VkFilter> {
        using CorrespondingType = Filter;
    };

    enum class SamplerAddressMode
    {
        Repeat = vk::SamplerAddressMode::eRepeat,
        ClampToBorder = vk::SamplerAddressMode::eClampToBorder,
        ClampToEdge = vk::SamplerAddressMode::eClampToEdge,
        MirrorClampToEdge = vk::SamplerAddressMode::eMirrorClampToEdge,
        MirroredRepeat = vk::SamplerAddressMode::eMirroredRepeat,
    };

    template<>
    struct EnumVulkanConnect<SamplerAddressMode> {
        using CorrespondingType = vk::SamplerAddressMode;
    };

    template<>
    struct EnumVulkanConnect<vk::SamplerAddressMode> {
        using CorrespondingType = SamplerAddressMode;
    };

    template<>
    struct EnumCVulkanConnect<SamplerAddressMode> {
        using CorrespondingType = VkSamplerAddressMode;
    };

    template<>
    struct EnumCVulkanConnect<VkSamplerAddressMode> {
        using CorrespondingType = SamplerAddressMode;
    };

    enum class SamplerMipmapMode
    {
        Linear = vk::SamplerMipmapMode::eLinear,
        Nearest = vk::SamplerMipmapMode::eNearest,
    };

    template<>
    struct EnumVulkanConnect<SamplerMipmapMode> {
        using CorrespondingType = vk::SamplerMipmapMode;
    };

    template<>
    struct EnumVulkanConnect<vk::SamplerMipmapMode> {
        using CorrespondingType = SamplerMipmapMode;
    };

    template<>
    struct EnumCVulkanConnect<SamplerMipmapMode> {
        using CorrespondingType = VkSamplerMipmapMode;
    };

    template<>
    struct EnumCVulkanConnect<VkSamplerMipmapMode> {
        using CorrespondingType = SamplerMipmapMode;
    };

    enum class CommandBufferLevel
    {
        Primary = vk::CommandBufferLevel::ePrimary,
        Secondary = vk::CommandBufferLevel::eSecondary,
    };

    template<>
    struct EnumVulkanConnect<CommandBufferLevel> {
        using CorrespondingType = vk::CommandBufferLevel;
    };

    template<>
    struct EnumVulkanConnect<vk::CommandBufferLevel> {
        using CorrespondingType = CommandBufferLevel;
    };

    template<>
    struct EnumCVulkanConnect<CommandBufferLevel> {
        using CorrespondingType = VkCommandBufferLevel;
    };

    template<>
    struct EnumCVulkanConnect<VkCommandBufferLevel> {
        using CorrespondingType = CommandBufferLevel;
    };

    enum class ImageTiling
    {
        DrmFormatModifierEXT = vk::ImageTiling::eDrmFormatModifierEXT,
        Linear = vk::ImageTiling::eLinear,
        Optimal = vk::ImageTiling::eOptimal,
    };

    template<>
    struct EnumVulkanConnect<ImageTiling> {
        using CorrespondingType = vk::ImageTiling;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageTiling> {
        using CorrespondingType = ImageTiling;
    };

    template<>
    struct EnumCVulkanConnect<ImageTiling> {
        using CorrespondingType = VkImageTiling;
    };

    template<>
    struct EnumCVulkanConnect<VkImageTiling> {
        using CorrespondingType = ImageTiling;
    };
        
    enum class PresentMode
    {
        Immediate = vk::PresentModeKHR::eImmediate,
        Fifo = vk::PresentModeKHR::eFifo,
		Mailbox = vk::PresentModeKHR::eMailbox,
        FifoRelaxed = vk::PresentModeKHR::eFifoRelaxed,
        FifoLatestReadyEXT = vk::PresentModeKHR::eFifoLatestReadyEXT,
        SharedContinuousRefresh = vk::PresentModeKHR::eSharedContinuousRefresh,
        SharedDemandRefresh = vk::PresentModeKHR::eSharedDemandRefresh,
    };

    template<>
    struct EnumVulkanConnect<PresentMode> {
        using CorrespondingType = vk::PresentModeKHR;
    };

    template<>
    struct EnumVulkanConnect<vk::PresentModeKHR> {
        using CorrespondingType = PresentMode;
    };

    template<>
    struct EnumCVulkanConnect<PresentMode> {
        using CorrespondingType = VkPresentModeKHR;
    };

    template<>
    struct EnumCVulkanConnect<VkPresentModeKHR> {
        using CorrespondingType = PresentMode;
    };

    enum class SharingMode
    {
        Concurrent = vk::SharingMode::eConcurrent,
        Exclusive = vk::SharingMode::eExclusive,
    };

    template<>
    struct EnumVulkanConnect<SharingMode> {
        using CorrespondingType = vk::SharingMode;
    };

    template<>
    struct EnumVulkanConnect<vk::SharingMode> {
        using CorrespondingType = SharingMode;
    };

    template<>
    struct EnumCVulkanConnect<SharingMode> {
        using CorrespondingType = VkSharingMode;
    };

    template<>
    struct EnumCVulkanConnect<VkSharingMode> {
        using CorrespondingType = SharingMode;
    };

    enum class ObjectType
    {
        Unknown = vk::ObjectType::eUnknown,
        Instance = vk::ObjectType::eInstance,
        PhysicalDevice = vk::ObjectType::ePhysicalDevice,
        Device = vk::ObjectType::eDevice,
        Queue = vk::ObjectType::eQueue,
        Semaphore = vk::ObjectType::eSemaphore,
        CommandBuffer = vk::ObjectType::eCommandBuffer,
        Fence = vk::ObjectType::eFence,
        DeviceMemory = vk::ObjectType::eDeviceMemory,
        Buffer = vk::ObjectType::eBuffer,
        Image = vk::ObjectType::eImage,
        Event = vk::ObjectType::eEvent,
        QueryPool = vk::ObjectType::eQueryPool,
        BufferView = vk::ObjectType::eBufferView,
        ImageView = vk::ObjectType::eImageView,
        ShaderModule = vk::ObjectType::eShaderModule,
        PipelineCache = vk::ObjectType::ePipelineCache,
        PipelineLayout = vk::ObjectType::ePipelineLayout,
        RenderPass = vk::ObjectType::eRenderPass,
        Pipeline = vk::ObjectType::ePipeline,
        DescriptorSetLayout = vk::ObjectType::eDescriptorSetLayout,
        Sampler = vk::ObjectType::eSampler,
        DescriptorPool = vk::ObjectType::eDescriptorPool,
        DescriptorSet = vk::ObjectType::eDescriptorSet,
        Framebuffer = vk::ObjectType::eFramebuffer,
        CommandPool = vk::ObjectType::eCommandPool,
        SurfaceKHR = vk::ObjectType::eSurfaceKHR,
        SwapchainKHR = vk::ObjectType::eSwapchainKHR,
        DisplayKHR = vk::ObjectType::eDisplayKHR,
        DisplayModeKHR = vk::ObjectType::eDisplayModeKHR,
        DebugReportCallbackEXT = vk::ObjectType::eDebugReportCallbackEXT,
        DebugUtilsMessengerEXT = vk::ObjectType::eDebugUtilsMessengerEXT,
        ValidationCacheEXT = vk::ObjectType::eValidationCacheEXT,
        AccelerationStructureKHR = vk::ObjectType::eAccelerationStructureKHR,
        AccelerationStructureNV = vk::ObjectType::eAccelerationStructureNV,
        PerformanceConfigurationINTEL = vk::ObjectType::ePerformanceConfigurationINTEL,
        DeferredOperationKHR = vk::ObjectType::eDeferredOperationKHR,
        IndirectCommandsLayoutNV = vk::ObjectType::eIndirectCommandsLayoutNV,
        PrivateDataSlot = vk::ObjectType::ePrivateDataSlot,
        VideoSessionKHR = vk::ObjectType::eVideoSessionKHR,
        VideoSessionParametersKHR = vk::ObjectType::eVideoSessionParametersKHR,
        CuModuleNVX = vk::ObjectType::eCuModuleNVX,
        CuFunctionNVX = vk::ObjectType::eCuFunctionNVX,
        OpticalFlowSessionNV = vk::ObjectType::eOpticalFlowSessionNV,
        MicromapEXT = vk::ObjectType::eMicromapEXT,
        ShaderEXT = vk::ObjectType::eShaderEXT
    };

    template<>
    struct EnumVulkanConnect<ObjectType> {
        using CorrespondingType = vk::ObjectType;
    };

    template<>
    struct EnumVulkanConnect<vk::ObjectType> {
        using CorrespondingType = ObjectType;
    };

    template<>
    struct EnumCVulkanConnect<ObjectType> {
        using CorrespondingType = VkObjectType;
    };

    template<>
    struct EnumCVulkanConnect<VkObjectType> {
        using CorrespondingType = ObjectType;
    };

    enum class StructureType
    {
        ApplicationInfo = vk::StructureType::eApplicationInfo,
        InstanceCreateInfo = vk::StructureType::eInstanceCreateInfo,
        DeviceQueueCreateInfo = vk::StructureType::eDeviceQueueCreateInfo,
        DeviceCreateInfo = vk::StructureType::eDeviceCreateInfo,
        SubmitInfo = vk::StructureType::eSubmitInfo,
        MemoryAllocateInfo = vk::StructureType::eMemoryAllocateInfo,
        MappedMemoryRange = vk::StructureType::eMappedMemoryRange,
        BindSparseInfo = vk::StructureType::eBindSparseInfo,
        FenceCreateInfo = vk::StructureType::eFenceCreateInfo,
        SemaphoreCreateInfo = vk::StructureType::eSemaphoreCreateInfo,
        EventCreateInfo = vk::StructureType::eEventCreateInfo,
        QueryPoolCreateInfo = vk::StructureType::eQueryPoolCreateInfo,
        BufferCreateInfo = vk::StructureType::eBufferCreateInfo,
        BufferViewCreateInfo = vk::StructureType::eBufferViewCreateInfo,
        ImageCreateInfo = vk::StructureType::eImageCreateInfo,
        ImageViewCreateInfo = vk::StructureType::eImageViewCreateInfo,
        ShaderModuleCreateInfo = vk::StructureType::eShaderModuleCreateInfo,
        PipelineCacheCreateInfo = vk::StructureType::ePipelineCacheCreateInfo,
        PipelineShaderStageCreateInfo = vk::StructureType::ePipelineShaderStageCreateInfo,
        PipelineVertexInputStateCreateInfo = vk::StructureType::ePipelineVertexInputStateCreateInfo,
        PipelineInputAssemblyStateCreateInfo = vk::StructureType::ePipelineInputAssemblyStateCreateInfo,
        PipelineTessellationStateCreateInfo = vk::StructureType::ePipelineTessellationStateCreateInfo,
        PipelineViewportStateCreateInfo = vk::StructureType::ePipelineViewportStateCreateInfo,
        PipelineRasterizationStateCreateInfo = vk::StructureType::ePipelineRasterizationStateCreateInfo,
        PipelineMultisampleStateCreateInfo = vk::StructureType::ePipelineMultisampleStateCreateInfo,
        PipelineDepthStencilStateCreateInfo = vk::StructureType::ePipelineDepthStencilStateCreateInfo,
        PipelineColorBlendStateCreateInfo = vk::StructureType::ePipelineColorBlendStateCreateInfo,
        PipelineDynamicStateCreateInfo = vk::StructureType::ePipelineDynamicStateCreateInfo,
        GraphicsPipelineCreateInfo = vk::StructureType::eGraphicsPipelineCreateInfo,
        ComputePipelineCreateInfo = vk::StructureType::eComputePipelineCreateInfo,
        PipelineLayoutCreateInfo = vk::StructureType::ePipelineLayoutCreateInfo,
        SamplerCreateInfo = vk::StructureType::eSamplerCreateInfo,
        DescriptorSetLayoutCreateInfo = vk::StructureType::eDescriptorSetLayoutCreateInfo,
        DescriptorPoolCreateInfo = vk::StructureType::eDescriptorPoolCreateInfo,
        DescriptorSetAllocateInfo = vk::StructureType::eDescriptorSetAllocateInfo,
        WriteDescriptorSet = vk::StructureType::eWriteDescriptorSet,
        CopyDescriptorSet = vk::StructureType::eCopyDescriptorSet,
        FramebufferCreateInfo = vk::StructureType::eFramebufferCreateInfo,
        RenderPassCreateInfo = vk::StructureType::eRenderPassCreateInfo,
        CommandPoolCreateInfo = vk::StructureType::eCommandPoolCreateInfo,
        CommandBufferAllocateInfo = vk::StructureType::eCommandBufferAllocateInfo,
        CommandBufferInheritanceInfo = vk::StructureType::eCommandBufferInheritanceInfo,
        CommandBufferBeginInfo = vk::StructureType::eCommandBufferBeginInfo,
        RenderPassBeginInfo = vk::StructureType::eRenderPassBeginInfo,
        BufferMemoryBarrier = vk::StructureType::eBufferMemoryBarrier,
        ImageMemoryBarrier = vk::StructureType::eImageMemoryBarrier,
        MemoryBarrier = vk::StructureType::eMemoryBarrier,
        SwapchainCreateInfoKHR = vk::StructureType::eSwapchainCreateInfoKHR,
        PresentInfoKHR = vk::StructureType::ePresentInfoKHR,
        DebugUtilsObjectNameInfoEXT = vk::StructureType::eDebugUtilsObjectNameInfoEXT,
        DebugUtilsObjectTagInfoEXT = vk::StructureType::eDebugUtilsObjectTagInfoEXT,
        DebugUtilsLabelEXT = vk::StructureType::eDebugUtilsLabelEXT,
        DebugUtilsMessengerCallbackDataEXT = vk::StructureType::eDebugUtilsMessengerCallbackDataEXT,
        DebugUtilsMessengerCreateInfoEXT = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT,
        AccelerationStructureBuildGeometryInfoKHR = vk::StructureType::eAccelerationStructureBuildGeometryInfoKHR,

        //device property structs
        PhysicalDeviceProperties2 = vk::StructureType::ePhysicalDeviceProperties2,
        PhysicalDeviceFeatures2 = vk::StructureType::ePhysicalDeviceFeatures2,
        PhysicalDeviceMemoryProperties2 = vk::StructureType::ePhysicalDeviceMemoryProperties2,
        PhysicalDeviceVulkan11Features = vk::StructureType::ePhysicalDeviceVulkan11Features,
        PhysicalDeviceVulkan11Properties = vk::StructureType::ePhysicalDeviceVulkan11Properties,
        PhysicalDeviceVulkan12Features = vk::StructureType::ePhysicalDeviceVulkan12Features,
        PhysicalDeviceVulkan12Properties = vk::StructureType::ePhysicalDeviceVulkan12Properties,
        PhysicalDeviceVulkan13Features = vk::StructureType::ePhysicalDeviceVulkan13Features,
        PhysicalDeviceVulkan13Properties = vk::StructureType::ePhysicalDeviceVulkan13Properties,
        PhysicalDevice16BitStorageFeatures = vk::StructureType::ePhysicalDevice16BitStorageFeatures,
        PhysicalDeviceMultiviewFeatures = vk::StructureType::ePhysicalDeviceMultiviewFeatures,
        PhysicalDeviceMultiviewProperties = vk::StructureType::ePhysicalDeviceMultiviewProperties,
        PhysicalDeviceVariablePointersFeatures = vk::StructureType::ePhysicalDeviceVariablePointersFeatures,
        PhysicalDeviceProtectedMemoryFeatures = vk::StructureType::ePhysicalDeviceProtectedMemoryFeatures,
        PhysicalDeviceProtectedMemoryProperties = vk::StructureType::ePhysicalDeviceProtectedMemoryProperties,
        PhysicalDeviceSamplerYcbcrConversionFeatures = vk::StructureType::ePhysicalDeviceSamplerYcbcrConversionFeatures,
        PhysicalDeviceShaderDrawParametersFeatures = vk::StructureType::ePhysicalDeviceShaderDrawParametersFeatures,
        PhysicalDevice8BitStorageFeatures = vk::StructureType::ePhysicalDevice8BitStorageFeatures,
        PhysicalDeviceShaderAtomicInt64Features = vk::StructureType::ePhysicalDeviceShaderAtomicInt64Features,
        PhysicalDeviceShaderFloat16Int8Features = vk::StructureType::ePhysicalDeviceShaderFloat16Int8Features,
        PhysicalDeviceFloatControlsProperties = vk::StructureType::ePhysicalDeviceFloatControlsProperties,
        PhysicalDeviceDescriptorIndexingFeatures = vk::StructureType::ePhysicalDeviceDescriptorIndexingFeatures,
        PhysicalDeviceDescriptorIndexingProperties = vk::StructureType::ePhysicalDeviceDescriptorIndexingProperties,
        PhysicalDeviceScalarBlockLayoutFeatures = vk::StructureType::ePhysicalDeviceScalarBlockLayoutFeatures,
        PhysicalDeviceVulkanMemoryModelFeatures = vk::StructureType::ePhysicalDeviceVulkanMemoryModelFeatures,
        PhysicalDeviceImagelessFramebufferFeatures = vk::StructureType::ePhysicalDeviceImagelessFramebufferFeatures,
        PhysicalDeviceUniformBufferStandardLayoutFeatures = vk::StructureType::ePhysicalDeviceUniformBufferStandardLayoutFeatures,
        PhysicalDeviceShaderSubgroupExtendedTypesFeatures = vk::StructureType::ePhysicalDeviceShaderSubgroupExtendedTypesFeatures,
        PhysicalDeviceSeparateDepthStencilLayoutsFeatures = vk::StructureType::ePhysicalDeviceSeparateDepthStencilLayoutsFeatures,
        PhysicalDeviceHostQueryResetFeatures = vk::StructureType::ePhysicalDeviceHostQueryResetFeatures,
        PhysicalDeviceTimelineSemaphoreFeatures = vk::StructureType::ePhysicalDeviceTimelineSemaphoreFeatures,
        PhysicalDeviceTimelineSemaphoreProperties = vk::StructureType::ePhysicalDeviceTimelineSemaphoreProperties,
        PhysicalDeviceBufferDeviceAddressFeatures = vk::StructureType::ePhysicalDeviceBufferDeviceAddressFeatures,
        PhysicalDeviceBufferDeviceAddressFeaturesEXT = vk::StructureType::ePhysicalDeviceBufferDeviceAddressFeaturesEXT,
        PhysicalDeviceImageRobustnessFeatures = vk::StructureType::ePhysicalDeviceImageRobustnessFeatures,
        PhysicalDeviceInlineUniformBlockFeatures = vk::StructureType::ePhysicalDeviceInlineUniformBlockFeatures,
        PhysicalDeviceInlineUniformBlockProperties = vk::StructureType::ePhysicalDeviceInlineUniformBlockProperties,
        PhysicalDevicePipelineCreationCacheControlFeatures = vk::StructureType::ePhysicalDevicePipelineCreationCacheControlFeatures,
        PhysicalDevicePrivateDataFeatures = vk::StructureType::ePhysicalDevicePrivateDataFeatures,
        PhysicalDeviceShaderDemoteToHelperInvocationFeatures = vk::StructureType::ePhysicalDeviceShaderDemoteToHelperInvocationFeatures,
        PhysicalDeviceShaderTerminateInvocationFeatures = vk::StructureType::ePhysicalDeviceShaderTerminateInvocationFeatures,
        PhysicalDeviceSubgroupSizeControlFeatures = vk::StructureType::ePhysicalDeviceSubgroupSizeControlFeatures,
        PhysicalDeviceSubgroupSizeControlProperties = vk::StructureType::ePhysicalDeviceSubgroupSizeControlProperties,
        PhysicalDeviceSynchronization2Features = vk::StructureType::ePhysicalDeviceSynchronization2Features,
        //PhysicalDeviceTextureCompressionASTCHDRFeatures = vk::StructureType::ePhysicalDeviceTextureCompressionASTCHDRFeatures,
        PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures = vk::StructureType::ePhysicalDeviceZeroInitializeWorkgroupMemoryFeatures,
        PhysicalDeviceDynamicRenderingFeatures = vk::StructureType::ePhysicalDeviceDynamicRenderingFeatures,
        PhysicalDeviceShaderIntegerDotProductFeatures = vk::StructureType::ePhysicalDeviceShaderIntegerDotProductFeatures,
        PhysicalDeviceShaderIntegerDotProductProperties = vk::StructureType::ePhysicalDeviceShaderIntegerDotProductProperties,
        PhysicalDeviceMaintenance4Features = vk::StructureType::ePhysicalDeviceMaintenance4Features,
        PhysicalDeviceMaintenance4Properties = vk::StructureType::ePhysicalDeviceMaintenance4Properties,
        PhysicalDeviceMeshShaderFeaturesEXT = vk::StructureType::ePhysicalDeviceMeshShaderFeaturesEXT,
        PhysicalDeviceMeshShaderFeaturesNV = vk::StructureType::ePhysicalDeviceMeshShaderFeaturesNV,
        PhysicalDeviceRobustness2FeaturesEXT = vk::StructureType::ePhysicalDeviceRobustness2FeaturesEXT,

        //queue property structs
        QueueFamilyProperties2 = vk::StructureType::eQueueFamilyProperties2,
        QueueFamilyGlobalPriorityPropertiesKHR = vk::StructureType::eQueueFamilyGlobalPriorityPropertiesKHR,
        QueueFamilyCheckpointPropertiesNV = vk::StructureType::eQueueFamilyCheckpointPropertiesNV,
        QueueFamilyCheckpointProperties2NV = vk::StructureType::eQueueFamilyCheckpointProperties2NV
    };

    template<>
    struct EnumVulkanConnect<StructureType> {
        using CorrespondingType = vk::StructureType;
    };

    template<>
    struct EnumVulkanConnect<vk::StructureType> {
        using CorrespondingType = StructureType;
    };

    template<>
    struct EnumCVulkanConnect<StructureType> {
        using CorrespondingType = VkStructureType;
    };

    template<>
    struct EnumCVulkanConnect<VkStructureType> {
        using CorrespondingType = StructureType;
    };


    enum class ColorSpace
    {
        SrgbNonlinear = vk::ColorSpaceKHR::eSrgbNonlinear,
        DisplayP3NonlinearEXT = vk::ColorSpaceKHR::eDisplayP3NonlinearEXT,
        ExtendedSrgbLinearEXT = vk::ColorSpaceKHR::eExtendedSrgbLinearEXT,
        DisplayP3LinearEXT = vk::ColorSpaceKHR::eDisplayP3LinearEXT,
        DciP3NonlinearEXT = vk::ColorSpaceKHR::eDciP3NonlinearEXT,
        Bt709LinearEXT = vk::ColorSpaceKHR::eBt709LinearEXT,
        Bt709NonlinearEXT = vk::ColorSpaceKHR::eBt709NonlinearEXT,
        Bt2020LinearEXT = vk::ColorSpaceKHR::eBt2020LinearEXT,
        Hdr10St2084EXT = vk::ColorSpaceKHR::eHdr10St2084EXT,
        //DolbyvisionEXT = vk::ColorSpaceKHR::eDolbyvisionEXT,
        Hdr10HlgEXT = vk::ColorSpaceKHR::eHdr10HlgEXT,
        AdobergbLinearEXT = vk::ColorSpaceKHR::eAdobergbLinearEXT,
        AdobergbNonlinearEXT = vk::ColorSpaceKHR::eAdobergbNonlinearEXT,
        PassThroughEXT = vk::ColorSpaceKHR::ePassThroughEXT,
        ExtendedSrgbNonlinearEXT = vk::ColorSpaceKHR::eExtendedSrgbNonlinearEXT,
        DisplayNativeAMD = vk::ColorSpaceKHR::eDisplayNativeAMD
    };

    template<>
    struct EnumVulkanConnect<ColorSpace> {
        using CorrespondingType = vk::ColorSpaceKHR;
    };

    template<>
    struct EnumVulkanConnect<vk::ColorSpaceKHR> {
        using CorrespondingType = ColorSpace;
    };

    template<>
    struct EnumCVulkanConnect<ColorSpace> {
        using CorrespondingType = VkColorSpaceKHR;
    };

    template<>
    struct EnumCVulkanConnect<VkColorSpaceKHR> {
        using CorrespondingType = ColorSpace;
    };

    enum class ImageType
    {
        T1D = vk::ImageType::e1D,
        T2D = vk::ImageType::e2D,
        T3D = vk::ImageType::e3D
    };

    template<>
    struct EnumVulkanConnect<ImageType> {
        using CorrespondingType = vk::ImageType;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageType> {
        using CorrespondingType = ImageType;
    };

    template<>
    struct EnumCVulkanConnect<ImageType> {
        using CorrespondingType = VkImageType;
    };

    template<>
    struct EnumCVulkanConnect<VkImageType> {
        using CorrespondingType = ImageType;
    };

    enum class ImageViewType
    {
        T1D = vk::ImageViewType::e1D,
        T1DArray = vk::ImageViewType::e1DArray,
        T2D = vk::ImageViewType::e2D,
        T2DArray = vk::ImageViewType::e2DArray,
        T3D = vk::ImageViewType::e3D,
        Cube = vk::ImageViewType::eCube,
		CubeArray = vk::ImageViewType::eCubeArray,
    };

    template<>
    struct EnumVulkanConnect<ImageViewType> {
        using CorrespondingType = vk::ImageViewType;
    };

    template<>
    struct EnumVulkanConnect<vk::ImageViewType> {
        using CorrespondingType = ImageViewType;
    };

    template<>
    struct EnumCVulkanConnect<ImageViewType> {
        using CorrespondingType = VkImageViewType;
    };

    template<>
    struct EnumCVulkanConnect<VkImageViewType> {
        using CorrespondingType = ImageViewType;
    };

    enum class ComponentSwizzle
    {
        R = vk::ComponentSwizzle::eR,
        G = vk::ComponentSwizzle::eG,
        B = vk::ComponentSwizzle::eB,
        A = vk::ComponentSwizzle::eA,
        Identity = vk::ComponentSwizzle::eIdentity,
		Zero = vk::ComponentSwizzle::eZero,
		One = vk::ComponentSwizzle::eOne,
    };

    template<>
    struct EnumVulkanConnect<ComponentSwizzle> {
        using CorrespondingType = vk::ComponentSwizzle;
    };

    template<>
    struct EnumVulkanConnect<vk::ComponentSwizzle> {
        using CorrespondingType = ComponentSwizzle;
    };

    template<>
    struct EnumCVulkanConnect<ComponentSwizzle> {
        using CorrespondingType = VkComponentSwizzle;
    };

    template<>
    struct EnumCVulkanConnect<VkComponentSwizzle> {
        using CorrespondingType = ComponentSwizzle;
    };

    enum class AttachmentLoadOp
    {
        None = vk::AttachmentLoadOp::eNone,
        NoneEXT = vk::AttachmentLoadOp::eNoneEXT,
        NoneKHR = vk::AttachmentLoadOp::eNoneKHR,
        DontCare = vk::AttachmentLoadOp::eDontCare,
        Load = vk::AttachmentLoadOp::eLoad,
        Clear = vk::AttachmentLoadOp::eClear
    };

    template<>
    struct EnumVulkanConnect<AttachmentLoadOp> {
        using CorrespondingType = vk::AttachmentLoadOp;
    };

    template<>
    struct EnumVulkanConnect<vk::AttachmentLoadOp> {
        using CorrespondingType = AttachmentLoadOp;
    };

    template<>
    struct EnumCVulkanConnect<AttachmentLoadOp> {
        using CorrespondingType = VkAttachmentLoadOp;
    };

    template<>
    struct EnumCVulkanConnect<VkAttachmentLoadOp> {
        using CorrespondingType = AttachmentLoadOp;
    };

    enum class AttachmentStoreOp
    {
        None = vk::AttachmentStoreOp::eNone,
        NoneEXT = vk::AttachmentStoreOp::eNoneEXT,
        NoneKHR = vk::AttachmentStoreOp::eNoneKHR,
        NoneQCOM = vk::AttachmentStoreOp::eNoneQCOM,
        DontCare = vk::AttachmentStoreOp::eDontCare,
        Store = vk::AttachmentStoreOp::eStore,
    };

    template<>
    struct EnumVulkanConnect<AttachmentStoreOp> {
        using CorrespondingType = vk::AttachmentStoreOp;
    };

    template<>
    struct EnumVulkanConnect<vk::AttachmentStoreOp> {
        using CorrespondingType = AttachmentStoreOp;
    };

    template<>
    struct EnumCVulkanConnect<AttachmentStoreOp> {
        using CorrespondingType = VkAttachmentStoreOp;
    };

    template<>
    struct EnumCVulkanConnect<VkAttachmentStoreOp> {
        using CorrespondingType = AttachmentStoreOp;
    };

    enum class PipelineBindPoint
    {
        Graphics = vk::PipelineBindPoint::eGraphics,
        Compute = vk::PipelineBindPoint::eCompute,
        RayTracingKHR = vk::PipelineBindPoint::eRayTracingKHR,
        RayTracingNV = vk::PipelineBindPoint::eRayTracingNV,
		SubpassShadingHUAWEI = vk::PipelineBindPoint::eSubpassShadingHUAWEI
	};

    template<>
    struct EnumVulkanConnect<PipelineBindPoint> {
        using CorrespondingType = vk::PipelineBindPoint;
    };

    template<>
    struct EnumVulkanConnect<vk::PipelineBindPoint> {
        using CorrespondingType = PipelineBindPoint;
    };

    template<>
    struct EnumCVulkanConnect<PipelineBindPoint> {
        using CorrespondingType = VkPipelineBindPoint;
    };

    template<>
    struct EnumCVulkanConnect<VkPipelineBindPoint> {
        using CorrespondingType = PipelineBindPoint;
    };

    enum class BorderColor
    {
        FloatCustomEXT = vk::BorderColor::eFloatCustomEXT,
		IntCustomEXT = vk::BorderColor::eIntCustomEXT,
        FloatTransparentBlack = vk::BorderColor::eFloatTransparentBlack,
        IntTransparentBlack = vk::BorderColor::eIntTransparentBlack,
        FloatOpaqueBlack = vk::BorderColor::eFloatOpaqueBlack,
        IntOpaqueBlack = vk::BorderColor::eIntOpaqueBlack,
        FloatOpaqueWhite = vk::BorderColor::eFloatOpaqueWhite,
		IntOpaqueWhite = vk::BorderColor::eIntOpaqueWhite,
    };

    template<>
    struct EnumVulkanConnect<BorderColor> {
        using CorrespondingType = vk::BorderColor;
    };

    template<>
    struct EnumVulkanConnect<vk::BorderColor> {
        using CorrespondingType = BorderColor;
    };

    template<>
    struct EnumCVulkanConnect<BorderColor> {
        using CorrespondingType = VkBorderColor;
    };

    template<>
    struct EnumCVulkanConnect<VkBorderColor> {
        using CorrespondingType = BorderColor;
    };

    enum class VertexInputRate
    {
        Instance = vk::VertexInputRate::eInstance,
        Vertex = vk::VertexInputRate::eVertex,
    };

    template<>
    struct EnumVulkanConnect<VertexInputRate> {
        using CorrespondingType = vk::VertexInputRate;
    };

    template<>
    struct EnumVulkanConnect<vk::VertexInputRate> {
        using CorrespondingType = VertexInputRate;
    };

    template<>
    struct EnumCVulkanConnect<VertexInputRate> {
        using CorrespondingType = VkVertexInputRate;
    };

    template<>
    struct EnumCVulkanConnect<VkVertexInputRate> {
        using CorrespondingType = VertexInputRate;
    };

    enum class PrimitiveTopology
    {
        PointList = vk::PrimitiveTopology::ePointList,
        LineList = vk::PrimitiveTopology::eLineList,
        LineStrip = vk::PrimitiveTopology::eLineStrip,
        TriangleList = vk::PrimitiveTopology::eTriangleList,
        TriangleStrip = vk::PrimitiveTopology::eTriangleStrip,
        TriangleFan = vk::PrimitiveTopology::eTriangleFan,
        LineListWithAdjacency = vk::PrimitiveTopology::eLineListWithAdjacency,
        LineStripWithAdjacency = vk::PrimitiveTopology::eLineStripWithAdjacency,
        TriangleListWithAdjacency = vk::PrimitiveTopology::eTriangleListWithAdjacency,
        TriangleStripWithAdjacency = vk::PrimitiveTopology::eTriangleStripWithAdjacency,
        PatchList = vk::PrimitiveTopology::ePatchList,
	};

    template<>
    struct EnumVulkanConnect<PrimitiveTopology> {
        using CorrespondingType = vk::PrimitiveTopology;
    };

    template<>
    struct EnumVulkanConnect<vk::PrimitiveTopology> {
        using CorrespondingType = PrimitiveTopology;
    };

    template<>
    struct EnumCVulkanConnect<PrimitiveTopology> {
        using CorrespondingType = VkPrimitiveTopology;
    };

    template<>
    struct EnumCVulkanConnect<VkPrimitiveTopology> {
        using CorrespondingType = PrimitiveTopology;
    };

    enum class PolygonMode
    {
        Fill = vk::PolygonMode::eFill,
		Line = vk::PolygonMode::eLine,
		Point = vk::PolygonMode::ePoint,
		FillRectangleNV = vk::PolygonMode::eFillRectangleNV
    };

    template<>
    struct EnumVulkanConnect<PolygonMode> {
        using CorrespondingType = vk::PolygonMode;
    };

    template<>
    struct EnumVulkanConnect<vk::PolygonMode> {
        using CorrespondingType = PolygonMode;
    };

    template<>
    struct EnumCVulkanConnect<PolygonMode> {
        using CorrespondingType = VkPolygonMode;
    };

    template<>
    struct EnumCVulkanConnect<VkPolygonMode> {
        using CorrespondingType = PolygonMode;
    };

    enum class FrontFace
    {
        CounterClockwise = vk::FrontFace::eCounterClockwise,
        Clockwise = vk::FrontFace::eClockwise,
	};

    template<>
    struct EnumVulkanConnect<FrontFace> {
        using CorrespondingType = vk::FrontFace;
    };

    template<>
    struct EnumVulkanConnect<vk::FrontFace> {
        using CorrespondingType = FrontFace;
    };

    template<>
    struct EnumCVulkanConnect<FrontFace> {
        using CorrespondingType = VkFrontFace;
    };

    template<>
    struct EnumCVulkanConnect<VkFrontFace> {
        using CorrespondingType = FrontFace;
    };

    enum class CompareOp
    {
        Always = vk::CompareOp::eAlways,
		Equal = vk::CompareOp::eEqual,
		Greater = vk::CompareOp::eGreater,
		GreaterOrEqual = vk::CompareOp::eGreaterOrEqual,
		Less = vk::CompareOp::eLess,
		LessOrEqual = vk::CompareOp::eLessOrEqual,
		Never = vk::CompareOp::eNever,
		NotEqual = vk::CompareOp::eNotEqual
    };

    template<>
    struct EnumVulkanConnect<CompareOp> {
        using CorrespondingType = vk::CompareOp;
    };

    template<>
    struct EnumVulkanConnect<vk::CompareOp> {
        using CorrespondingType = CompareOp;
    };

    template<>
    struct EnumCVulkanConnect<CompareOp> {
        using CorrespondingType = VkCompareOp;
    };

    template<>
    struct EnumCVulkanConnect<VkCompareOp> {
        using CorrespondingType = CompareOp;
    };

    enum class StencilOp
    {
        DecrementAndClamp = vk::StencilOp::eDecrementAndClamp,
		DecrementAndWrap = vk::StencilOp::eDecrementAndWrap,
		Invert = vk::StencilOp::eInvert,
		IncrementAndClamp = vk::StencilOp::eIncrementAndClamp,
		IncrementAndWrap = vk::StencilOp::eIncrementAndWrap,
		Keep = vk::StencilOp::eKeep,
		Replace = vk::StencilOp::eReplace,
		Zero = vk::StencilOp::eZero
    };

    template<>
    struct EnumVulkanConnect<StencilOp> {
        using CorrespondingType = vk::StencilOp;
    };

    template<>
    struct EnumVulkanConnect<vk::StencilOp> {
        using CorrespondingType = StencilOp;
    };

    template<>
    struct EnumCVulkanConnect<StencilOp> {
        using CorrespondingType = VkStencilOp;
    };

    template<>
    struct EnumCVulkanConnect<VkStencilOp> {
        using CorrespondingType = StencilOp;
    };

    enum class LogicOp
    {
        And = vk::LogicOp::eAnd,
        AndInverted = vk::LogicOp::eAndInverted,
		AndReverse = vk::LogicOp::eAndReverse,
		Clear = vk::LogicOp::eClear,
		Copy = vk::LogicOp::eCopy,
		CopyInverted = vk::LogicOp::eCopyInverted,
		Nand = vk::LogicOp::eNand,
		NoOp = vk::LogicOp::eNoOp,
		Nor = vk::LogicOp::eNor,
		Or = vk::LogicOp::eOr,
		OrInverted = vk::LogicOp::eOrInverted,
		OrReverse = vk::LogicOp::eOrReverse,
		Set = vk::LogicOp::eSet,
		Xor = vk::LogicOp::eXor,
        Invert = vk::LogicOp::eInvert,
        Equivalent = vk::LogicOp::eEquivalent,
    };

    template<>
    struct EnumVulkanConnect<LogicOp> {
        using CorrespondingType = vk::LogicOp;
    };

    template<>
    struct EnumVulkanConnect<vk::LogicOp> {
        using CorrespondingType = LogicOp;
    };

    template<>
    struct EnumCVulkanConnect<LogicOp> {
        using CorrespondingType = VkLogicOp;
    };

    template<>
    struct EnumCVulkanConnect<VkLogicOp> {
        using CorrespondingType = LogicOp;
    };

    enum class BlendFactor
    {
        ConstantAlpha = vk::BlendFactor::eConstantAlpha,
		ConstantColor = vk::BlendFactor::eConstantColor,
		DstAlpha = vk::BlendFactor::eDstAlpha,
		DstColor = vk::BlendFactor::eDstColor,
		One = vk::BlendFactor::eOne,
		OneMinusConstantAlpha = vk::BlendFactor::eOneMinusConstantAlpha,
		OneMinusConstantColor = vk::BlendFactor::eOneMinusConstantColor,
		OneMinusDstAlpha = vk::BlendFactor::eOneMinusDstAlpha,
		OneMinusDstColor = vk::BlendFactor::eOneMinusDstColor,
		OneMinusSrcAlpha = vk::BlendFactor::eOneMinusSrcAlpha,
		OneMinusSrcColor = vk::BlendFactor::eOneMinusSrcColor,
		SrcAlpha = vk::BlendFactor::eSrcAlpha,
		SrcAlphaSaturate = vk::BlendFactor::eSrcAlphaSaturate,
		SrcColor = vk::BlendFactor::eSrcColor,
		Zero = vk::BlendFactor::eZero
    };

    template<>
    struct EnumVulkanConnect<BlendFactor> {
        using CorrespondingType = vk::BlendFactor;
    };

    template<>
    struct EnumVulkanConnect<vk::BlendFactor> {
        using CorrespondingType = BlendFactor;
    };

    template<>
    struct EnumCVulkanConnect<BlendFactor> {
        using CorrespondingType = VkBlendFactor;
    };

    template<>
    struct EnumCVulkanConnect<VkBlendFactor> {
        using CorrespondingType = BlendFactor;
    };
    
    enum class BlendOp
    {
        Add = vk::BlendOp::eAdd,
        Subtract = vk::BlendOp::eSubtract,
        ReverseSubtract = vk::BlendOp::eReverseSubtract,
        Min = vk::BlendOp::eMin,
        Max = vk::BlendOp::eMax,
        ZeroEXT = vk::BlendOp::eZeroEXT,
        SrcEXT = vk::BlendOp::eSrcEXT,
        DstEXT = vk::BlendOp::eDstEXT,
        SrcOverEXT = vk::BlendOp::eSrcOverEXT,
        DstOverEXT = vk::BlendOp::eDstOverEXT,
        SrcInEXT = vk::BlendOp::eSrcInEXT,
        DstInEXT = vk::BlendOp::eDstInEXT,
        SrcOutEXT = vk::BlendOp::eSrcOutEXT,
        DstOutEXT = vk::BlendOp::eDstOutEXT,
        SrcAtopEXT = vk::BlendOp::eSrcAtopEXT,
        DstAtopEXT = vk::BlendOp::eDstAtopEXT,
        XorEXT = vk::BlendOp::eXorEXT,
        MultiplyEXT = vk::BlendOp::eMultiplyEXT,
        ScreenEXT = vk::BlendOp::eScreenEXT,
        OverlayEXT = vk::BlendOp::eOverlayEXT,
        DarkenEXT = vk::BlendOp::eDarkenEXT,
        LightenEXT = vk::BlendOp::eLightenEXT,
        ColordodgeEXT = vk::BlendOp::eColordodgeEXT,
        ColorburnEXT = vk::BlendOp::eColorburnEXT,
        HardlightEXT = vk::BlendOp::eHardlightEXT,
        SoftlightEXT = vk::BlendOp::eSoftlightEXT,
        DifferenceEXT = vk::BlendOp::eDifferenceEXT,
        ExclusionEXT = vk::BlendOp::eExclusionEXT,
        InvertEXT = vk::BlendOp::eInvertEXT,
        InvertRgbEXT = vk::BlendOp::eInvertRgbEXT,
        LineardodgeEXT = vk::BlendOp::eLineardodgeEXT,
        LinearburnEXT = vk::BlendOp::eLinearburnEXT,
        VividlightEXT = vk::BlendOp::eVividlightEXT,
        LinearlightEXT = vk::BlendOp::eLinearlightEXT,
        PinlightEXT = vk::BlendOp::ePinlightEXT,
        HardmixEXT = vk::BlendOp::eHardmixEXT,
        HslHueEXT = vk::BlendOp::eHslHueEXT,
        HslSaturationEXT = vk::BlendOp::eHslSaturationEXT,
        HslColorEXT = vk::BlendOp::eHslColorEXT,
        HslLuminosityEXT = vk::BlendOp::eHslLuminosityEXT,
        PlusEXT = vk::BlendOp::ePlusEXT,
        PlusClampedEXT = vk::BlendOp::ePlusClampedEXT,
        PlusClampedAlphaEXT = vk::BlendOp::ePlusClampedAlphaEXT,
        PlusDarkerEXT = vk::BlendOp::ePlusDarkerEXT,
        MinusEXT = vk::BlendOp::eMinusEXT,
        MinusClampedEXT = vk::BlendOp::eMinusClampedEXT,
        ContrastEXT = vk::BlendOp::eContrastEXT,
        InvertOvgEXT = vk::BlendOp::eInvertOvgEXT,
        RedEXT = vk::BlendOp::eRedEXT,
        GreenEXT = vk::BlendOp::eGreenEXT,
        BlueEXT = vk::BlendOp::eBlueEXT,
    };

    template<>
    struct EnumVulkanConnect<BlendOp> {
        using CorrespondingType = vk::BlendOp;
    };

    template<>
    struct EnumVulkanConnect<vk::BlendOp> {
        using CorrespondingType = BlendOp;
    };

    template<>
    struct EnumCVulkanConnect<BlendOp> {
        using CorrespondingType = VkBlendOp;
    };

    template<>
    struct EnumCVulkanConnect<VkBlendOp> {
        using CorrespondingType = BlendOp;
    };

    enum class DynamicState
    {
        Viewport = vk::DynamicState::eViewport,
        Scissor = vk::DynamicState::eScissor,
        LineWidth = vk::DynamicState::eLineWidth,
        DepthBias = vk::DynamicState::eDepthBias,
        BlendConstants = vk::DynamicState::eBlendConstants,
        DepthBounds = vk::DynamicState::eDepthBounds,
        StencilCompareMask = vk::DynamicState::eStencilCompareMask,
        StencilWriteMask = vk::DynamicState::eStencilWriteMask,
        StencilReference = vk::DynamicState::eStencilReference,
        CullMode = vk::DynamicState::eCullMode,
        FrontFace = vk::DynamicState::eFrontFace,
        PrimitiveTopology = vk::DynamicState::ePrimitiveTopology,
        ViewportWithCount = vk::DynamicState::eViewportWithCount,
        ScissorWithCount = vk::DynamicState::eScissorWithCount,
        VertexInputBindingStride = vk::DynamicState::eVertexInputBindingStride,
        DepthTestEnable = vk::DynamicState::eDepthTestEnable,
        DepthWriteEnable = vk::DynamicState::eDepthWriteEnable,
        DepthCompareOp = vk::DynamicState::eDepthCompareOp,
        DepthBoundsTestEnable = vk::DynamicState::eDepthBoundsTestEnable,
        StencilTestEnable = vk::DynamicState::eStencilTestEnable,
        StencilOp = vk::DynamicState::eStencilOp,
        RasterizerDiscardEnable = vk::DynamicState::eRasterizerDiscardEnable,
        DepthBiasEnable = vk::DynamicState::eDepthBiasEnable,
        PrimitiveRestartEnable = vk::DynamicState::ePrimitiveRestartEnable,
        ViewportWScalingNV = vk::DynamicState::eViewportWScalingNV,
        DiscardRectangleEXT = vk::DynamicState::eDiscardRectangleEXT,
        SampleLocationsEXT = vk::DynamicState::eSampleLocationsEXT,
        RayTracingPipelineStackSizeKHR = vk::DynamicState::eRayTracingPipelineStackSizeKHR,
        ViewportShadingRatePaletteNV = vk::DynamicState::eViewportShadingRatePaletteNV,
        ViewportCoarseSampleOrderNV = vk::DynamicState::eViewportCoarseSampleOrderNV,
        ExclusiveScissorNV = vk::DynamicState::eExclusiveScissorNV,
        FragmentShadingRateKHR = vk::DynamicState::eFragmentShadingRateKHR,
        LineStippleEXT = vk::DynamicState::eLineStippleEXT,
        VertexInputEXT = vk::DynamicState::eVertexInputEXT,
        PatchControlPointsEXT = vk::DynamicState::ePatchControlPointsEXT,
        LogicOpEXT = vk::DynamicState::eLogicOpEXT,
        ColorWriteEnableEXT = vk::DynamicState::eColorWriteEnableEXT,
        DepthClampEnableEXT = vk::DynamicState::eDepthClampEnableEXT,
        PolygonModeEXT = vk::DynamicState::ePolygonModeEXT,
        RasterizationSamplesEXT = vk::DynamicState::eRasterizationSamplesEXT,
        SampleMaskEXT = vk::DynamicState::eSampleMaskEXT,
        AlphaToCoverageEnableEXT = vk::DynamicState::eAlphaToCoverageEnableEXT,
        AlphaToOneEnableEXT = vk::DynamicState::eAlphaToOneEnableEXT,
        LogicOpEnableEXT = vk::DynamicState::eLogicOpEnableEXT,
        ColorBlendEnableEXT = vk::DynamicState::eColorBlendEnableEXT,
        ColorBlendEquationEXT = vk::DynamicState::eColorBlendEquationEXT,
        ColorWriteMaskEXT = vk::DynamicState::eColorWriteMaskEXT,
        TessellationDomainOriginEXT = vk::DynamicState::eTessellationDomainOriginEXT,
        RasterizationStreamEXT = vk::DynamicState::eRasterizationStreamEXT,
        ConservativeRasterizationModeEXT = vk::DynamicState::eConservativeRasterizationModeEXT,
        ExtraPrimitiveOverestimationSizeEXT = vk::DynamicState::eExtraPrimitiveOverestimationSizeEXT,
        DepthClipEnableEXT = vk::DynamicState::eDepthClipEnableEXT,
        SampleLocationsEnableEXT = vk::DynamicState::eSampleLocationsEnableEXT,
        ColorBlendAdvancedEXT = vk::DynamicState::eColorBlendAdvancedEXT,
        ProvokingVertexModeEXT = vk::DynamicState::eProvokingVertexModeEXT,
        LineRasterizationModeEXT = vk::DynamicState::eLineRasterizationModeEXT,
        LineStippleEnableEXT = vk::DynamicState::eLineStippleEnableEXT,
        DepthClipNegativeOneToOneEXT = vk::DynamicState::eDepthClipNegativeOneToOneEXT,
        ViewportWScalingEnableNV = vk::DynamicState::eViewportWScalingEnableNV,
        ViewportSwizzleNV = vk::DynamicState::eViewportSwizzleNV,
        CoverageToColorEnableNV = vk::DynamicState::eCoverageToColorEnableNV,
        CoverageToColorLocationNV = vk::DynamicState::eCoverageToColorLocationNV,
        CoverageModulationModeNV = vk::DynamicState::eCoverageModulationModeNV,
        CoverageModulationTableEnableNV = vk::DynamicState::eCoverageModulationTableEnableNV,
        CoverageModulationTableNV = vk::DynamicState::eCoverageModulationTableNV,
        ShadingRateImageEnableNV = vk::DynamicState::eShadingRateImageEnableNV,
        RepresentativeFragmentTestEnableNV = vk::DynamicState::eRepresentativeFragmentTestEnableNV,
        CoverageReductionModeNV = vk::DynamicState::eCoverageReductionModeNV,
    };

    template<>
    struct EnumVulkanConnect<DynamicState> {
        using CorrespondingType = vk::DynamicState;
    };

    template<>
    struct EnumVulkanConnect<vk::DynamicState> {
        using CorrespondingType = DynamicState;
    };

    template<>
    struct EnumCVulkanConnect<DynamicState> {
        using CorrespondingType = VkDynamicState;
    };

    template<>
    struct EnumCVulkanConnect<VkDynamicState> {
        using CorrespondingType = DynamicState;
    };

    enum class SubpassContents
    {
        Inline = vk::SubpassContents::eInline,
        SecondaryCommandBuffers = vk::SubpassContents::eSecondaryCommandBuffers,
        InlineAndSecondaryCommandBuffersEXT = vk::SubpassContents::eInlineAndSecondaryCommandBuffersEXT,
        InlineAndSecondaryCommandBuffersKHR = vk::SubpassContents::eInlineAndSecondaryCommandBuffersKHR,
    };

    template<>
    struct EnumVulkanConnect<SubpassContents> {
        using CorrespondingType = vk::SubpassContents;
    };

    template<>
    struct EnumVulkanConnect<vk::SubpassContents> {
        using CorrespondingType = SubpassContents;
    };

    template<>
    struct EnumCVulkanConnect<SubpassContents> {
        using CorrespondingType = VkSubpassContents;
    };

    template<>
    struct EnumCVulkanConnect<VkSubpassContents> {
        using CorrespondingType = SubpassContents;
    };    

    enum class IndexType
    {
        NoneKHR = vk::IndexType::eNoneKHR,
        NoneNV = vk::IndexType::eNoneNV,
        Uint16 = vk::IndexType::eUint16,
        Uint32 = vk::IndexType::eUint32,
        Uint8 = vk::IndexType::eUint8,
        Uint8EXT = vk::IndexType::eUint8EXT,
        Uint8KHR = vk::IndexType::eUint8KHR,
    };

    template<>
    struct EnumVulkanConnect<IndexType> {
        using CorrespondingType = vk::IndexType;
    };

    template<>
    struct EnumVulkanConnect<vk::IndexType> {
        using CorrespondingType = IndexType;
    };

    template<>
    struct EnumCVulkanConnect<IndexType> {
        using CorrespondingType = VkIndexType;
    };

    template<>
    struct EnumCVulkanConnect<VkIndexType> {
        using CorrespondingType = IndexType;
    };

    //converts enums to and from vulkan
    template<typename Enum>
    auto convertEnum(const Enum& val) { 
        return static_cast<typename EnumVulkanConnect<Enum>::CorrespondingType>(val); 
    };

    template<typename Enum>
    auto convertCEnum(const Enum& val) {
        return static_cast<typename EnumCVulkanConnect<Enum>::CorrespondingType>(val);
    };

    template<typename Enum>
    auto convertEnum(const Enum* val) {
        return reinterpret_cast<const typename EnumVulkanConnect<Enum>::CorrespondingType*>(val);
    };

    template<typename Enum>
    auto convertCEnum(const Enum* val) {
        return reinterpret_cast<const typename EnumCVulkanConnect<Enum>::CorrespondingType*>(val);
    };

    template<typename Enum>
    auto enumToInt(const Enum& val) { return static_cast<std::underlying_type_t<Enum>>(val); };
}