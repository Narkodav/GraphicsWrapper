#pragma once
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <type_traits>

namespace Graphics
{
    template <typename Definition>
    class Flags
    {
    protected:
        uint32_t m_flags;

    public:
        using Bits = Definition::Bits;
        using VulkanFlags = Definition::VulkanFlags;

        Flags() : m_flags(0) {};
        Flags(typename Bits bit) : m_flags(static_cast<uint32_t>(bit)) {}
        Flags(uint32_t flags) : m_flags(flags) {}
        Flags(typename VulkanFlags flags) : m_flags(static_cast<uint32_t>(flags)) {};

        operator VulkanFlags() const { return VulkanFlags(this->m_flags); }
        operator uint32_t() const { return m_flags; }

        inline Flags operator|(const Bits& other) const {
            return static_cast<Flags>(m_flags | static_cast<uint32_t>(other));
        }

        inline Flags operator|(const Flags& other) const {
            return static_cast<Flags>(m_flags | other.m_flags);
        }

        inline Flags& operator|=(const Bits& other) {
            m_flags |= static_cast<uint32_t>(other);
            return *this;
        }

        inline Flags& operator|=(const Flags& other) {
            m_flags |= static_cast<uint32_t>(other);
            return *this;
        }

        inline Flags operator&(const Bits& other) const {
            return static_cast<Flags>(m_flags & static_cast<uint32_t>(other));
        }

        inline Flags operator&(const Flags& other) const {
            return static_cast<Flags>(m_flags & other.m_flags);
        }

        inline Flags& operator&=(const Bits& other) {
            m_flags &= static_cast<uint32_t>(other);
            return *this;
        }

        inline Flags& operator&=(const Flags& other) {
            m_flags &= static_cast<uint32_t>(other);
            return *this;
        }

        inline bool operator==(const Flags& other) const {
            return m_flags == static_cast<uint32_t>(other);
        }

        inline bool operator!=(const Flags& other) const {
            return m_flags != static_cast<uint32_t>(other);
        }

        inline bool operator==(const Bits& other) const {
            return m_flags == static_cast<uint32_t>(other);
        }

        inline bool operator!=(const Bits& other) const {
            return m_flags != static_cast<uint32_t>(other);
        }

        inline bool operator==(const uint32_t& other) const {
            return m_flags == other;
        }

        inline bool operator!=(const uint32_t& other) const {
            return m_flags != other;
        }

        bool hasFlag(const Bits& flag) const {
            return (m_flags & static_cast<uint32_t>(flag)) == static_cast<uint32_t>(flag);
        }

        bool hasFlags(const Flags& flags) const {
            return (m_flags & flags.m_flags) == flags.m_flags;
        }

        void clear() { m_flags = 0; };
    };

    template<typename T>
    struct BitTraits;

    template <typename Bits>
    inline Flags<typename BitTraits<Bits>::ParentType> operator|(const Bits& a, const Bits& b) {
        return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline Flags<typename BitTraits<Bits>::ParentType> operator&(const Bits& a, const Bits& b) {
        return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator==(const Bits& a, const Flags<typename BitTraits<Bits>::ParentType>& b) {
        return static_cast<uint32_t>(a) == static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator!=(const Bits& a, const Flags<typename BitTraits<Bits>::ParentType>& b) {
        return static_cast<uint32_t>(a) != static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator==(const uint32_t& a, const Flags<typename BitTraits<Bits>::ParentType>& b) {
        return a == static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator!=(const uint32_t& a, const Flags<typename BitTraits<Bits>::ParentType>& b) {
        return a != static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator==(const uint32_t& a, const Bits& b) {
        return a == static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator!=(const uint32_t& a, const Bits& b) {
        return a != static_cast<uint32_t>(b);
    }

    template <typename Bits>
    inline bool operator==(const Bits& a, const uint32_t& b) {
        return b == static_cast<uint32_t>(a);
    }

    template <typename Bits>
    inline bool operator!=(const Bits& a, const uint32_t& b) {
        return b != static_cast<uint32_t>(a);
    }

    struct MemoryProperty
    {
        enum class Bits : uint32_t {
            None = 0,
            DeviceLocal = vk::MemoryPropertyFlagBits::eDeviceLocal,
            HostVisible = vk::MemoryPropertyFlagBits::eHostVisible,
            HostCoherent = vk::MemoryPropertyFlagBits::eHostCoherent,
            HostCached = vk::MemoryPropertyFlagBits::eHostCached,
            LazilyAllocated = vk::MemoryPropertyFlagBits::eLazilyAllocated,
            Protected = vk::MemoryPropertyFlagBits::eProtected,

            HostVisibleCoherent = static_cast<uint32_t>(HostVisible) | static_cast<uint32_t>(HostCoherent),
            DeviceLocalVisible = static_cast<uint32_t>(DeviceLocal) | static_cast<uint32_t>(HostVisible),
        };

        using VulkanFlags = vk::MemoryPropertyFlags;
        using Flags = Flags<MemoryProperty>;
    };

    template<>
    struct BitTraits<MemoryProperty::Bits> {
        using ParentType = MemoryProperty;
    };

	struct MemoryHeap
    {
        enum class Bits : uint32_t {
            None = 0,
            DeviceLocal = vk::MemoryHeapFlagBits::eDeviceLocal,
			MultiInstance = vk::MemoryHeapFlagBits::eMultiInstance,
			MultiInstanceKHR = vk::MemoryHeapFlagBits::eMultiInstanceKHR,
        };

        using VulkanFlags = vk::MemoryHeapFlags;
        using Flags = Flags<MemoryHeap>;
    };

    template<>
    struct BitTraits<MemoryHeap::Bits> {
        using ParentType = MemoryHeap;
    };

    struct BufferUsage
    {
        enum class Bits : uint32_t
        {
            None = 0,
            Vertex = vk::BufferUsageFlagBits::eVertexBuffer,        // Buffer can be used as a vertex buffer
            Index = vk::BufferUsageFlagBits::eIndexBuffer,          // Buffer can be used as an index buffer
            Uniform = vk::BufferUsageFlagBits::eUniformBuffer,      // Buffer can be used as a uniform buffer
            Storage = vk::BufferUsageFlagBits::eStorageBuffer,      // Buffer can be used as a storage buffer
            TransferSrc = vk::BufferUsageFlagBits::eTransferSrc,    // Buffer can be used as source in transfer commands
            TransferDst = vk::BufferUsageFlagBits::eTransferDst,    // Buffer can be used as destination in transfer commands
            Indirect = vk::BufferUsageFlagBits::eIndirectBuffer,     // For indirect drawing commands
            ShaderDeviceAddress = vk::BufferUsageFlagBits::eShaderDeviceAddress,  // Buffer can be used to get device address
            StorageTexelBuffer = vk::BufferUsageFlagBits::eStorageTexelBuffer,    // Buffer can be used as a storage texel buffer
            UniformTexelBuffer = vk::BufferUsageFlagBits::eUniformTexelBuffer,    // Buffer can be used as a uniform texel buffer

            VertexIndex = static_cast<uint32_t>(Vertex) | static_cast<uint32_t>(Index),         // Buffer can be used as a vertex and index buffer
            VertexUniform = static_cast<uint32_t>(Vertex) | static_cast<uint32_t>(Uniform),     // Buffer can be used as a vertex and uniform buffer
        };

        using VulkanFlags = vk::BufferUsageFlags;
        using Flags = Flags<BufferUsage>;
    };

    template<>
    struct BitTraits<BufferUsage::Bits> {
        using ParentType = BufferUsage;
    };

    struct CommandBufferUsage
    {
        enum class Bits : uint32_t
        {
            None = 0,
            OneTimeSubmit = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
            SimultaneousUse = vk::CommandBufferUsageFlagBits::eSimultaneousUse,
            RenderPassContinue = vk::CommandBufferUsageFlagBits::eRenderPassContinue,
            SimultaneousUseRenderPassContinue =
            static_cast<uint32_t>(SimultaneousUse) | static_cast<uint32_t>(RenderPassContinue),
        };

        using VulkanFlags = vk::CommandBufferUsageFlags;
        using Flags = Flags<CommandBufferUsage>;
    };

    template<>
    struct BitTraits<CommandBufferUsage::Bits> {
        using ParentType = CommandBufferUsage;
    };

    struct DebugMessageSeverity
    {
        enum class Bits : uint32_t
        {
            None = 0,
            Verbose = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
            Info = vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
            Warning = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
            Error = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            All = Verbose | Info | Warning | Error
        };

        using VulkanFlags = vk::DebugUtilsMessageSeverityFlagsEXT;
        using Flags = Flags<DebugMessageSeverity>;
    };

    template<>
    struct BitTraits<DebugMessageSeverity::Bits> {
        using ParentType = DebugMessageSeverity;
    };

    struct DebugMessageType
    {
        enum class Bits : uint32_t
        {
            None = 0,
            DeviceAddressBinding = vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
            General = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral,
            Performance = vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
            Validation = vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            All = General | Performance | Validation | DeviceAddressBinding,
            AllLegacy = General | Performance | Validation,
        };

        using VulkanFlags = vk::DebugUtilsMessageTypeFlagsEXT;
        using Flags = Flags<DebugMessageType>;
    };

    template<>
    struct BitTraits<DebugMessageType::Bits> {
        using ParentType = DebugMessageType;
    };

    struct DescriptorSetLayoutCreate
    {
        enum class Bits : uint32_t
        {
            None = 0,
            UpdateAfterBind = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
        };

        using VulkanFlags = vk::DescriptorSetLayoutCreateFlags;
        using Flags = Flags<DescriptorSetLayoutCreate>;
    };

    template<>
    struct BitTraits<DescriptorSetLayoutCreate::Bits> {
        using ParentType = DescriptorSetLayoutCreate;
    };

    struct QueueFlags
    {
        enum class Bits : uint32_t {
            None = 0,
            Graphics = vk::QueueFlagBits::eGraphics,
            Transfer = vk::QueueFlagBits::eTransfer,
            Compute = vk::QueueFlagBits::eCompute,
            SparseBinding = vk::QueueFlagBits::eSparseBinding,
            Protected = vk::QueueFlagBits::eProtected,
            VideoDecode = vk::QueueFlagBits::eVideoDecodeKHR,
            VideoEncode = vk::QueueFlagBits::eVideoEncodeKHR,
            OpticalFlowNv = vk::QueueFlagBits::eOpticalFlowNV,
        };

        using VulkanFlags = vk::QueueFlags;
        using Flags = Flags<QueueFlags>;
    };

    template<>
    struct BitTraits<QueueFlags::Bits> {
        using ParentType = QueueFlags;
    };

    struct DescriptorPoolCreateFlags
    {
        enum class Bits : uint32_t {
            None = 0,
            UpdateAfterBind = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
            FreeDescriptorSet = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
        };

        using VulkanFlags = vk::DescriptorPoolCreateFlags;
        using Flags = Flags<DescriptorPoolCreateFlags>;
    };

    template<>
    struct BitTraits<DescriptorPoolCreateFlags::Bits> {
        using ParentType = DescriptorPoolCreateFlags;
    };

    struct PipelineStage
    {
        enum class Bits : uint32_t {
            None = 0,
            TopOfPipe = vk::PipelineStageFlagBits::eTopOfPipe,
            DrawIndirect = vk::PipelineStageFlagBits::eDrawIndirect,
            VertexInput = vk::PipelineStageFlagBits::eVertexInput,
            VertexShader = vk::PipelineStageFlagBits::eVertexShader,
            TessellationControlShader = vk::PipelineStageFlagBits::eTessellationControlShader,
            TessellationEvaluationShader = vk::PipelineStageFlagBits::eTessellationEvaluationShader,
            GeometryShader = vk::PipelineStageFlagBits::eGeometryShader,
            FragmentShader = vk::PipelineStageFlagBits::eFragmentShader,
            EarlyFragmentTests = vk::PipelineStageFlagBits::eEarlyFragmentTests,
            LateFragmentTests = vk::PipelineStageFlagBits::eLateFragmentTests,
            ColorAttachmentOutput = vk::PipelineStageFlagBits::eColorAttachmentOutput,
            ComputeShader = vk::PipelineStageFlagBits::eComputeShader,
            Transfer = vk::PipelineStageFlagBits::eTransfer,
            BottomOfPipe = vk::PipelineStageFlagBits::eBottomOfPipe,
            Host = vk::PipelineStageFlagBits::eHost,
            AllGraphics = vk::PipelineStageFlagBits::eAllGraphics,
            AllCommands = vk::PipelineStageFlagBits::eAllCommands,
            MeshShaderEXT = vk::PipelineStageFlagBits::eMeshShaderEXT,
            TaskShaderEXT = vk::PipelineStageFlagBits::eTaskShaderEXT,
        };

        using VulkanFlags = vk::PipelineStageFlags;
        using Flags = Flags<PipelineStage>;
    };
    
    template<>
    struct BitTraits<PipelineStage::Bits> {
        using ParentType = PipelineStage;
    };

    struct ShaderStage
    {
        enum class Bits : uint32_t {
            None = 0,
            Vertex = vk::ShaderStageFlagBits::eVertex,
            TessellationControl = vk::ShaderStageFlagBits::eTessellationControl,
            TessellationEvaluation = vk::ShaderStageFlagBits::eTessellationEvaluation,
            Geometry = vk::ShaderStageFlagBits::eGeometry,
            Fragment = vk::ShaderStageFlagBits::eFragment,
            Compute = vk::ShaderStageFlagBits::eCompute,
            AllGraphics = vk::ShaderStageFlagBits::eAllGraphics,
            All = vk::ShaderStageFlagBits::eAll,
            RaygenKHR = vk::ShaderStageFlagBits::eRaygenKHR,
            AnyHitKHR = vk::ShaderStageFlagBits::eAnyHitKHR,
            ClosestHitKHR = vk::ShaderStageFlagBits::eClosestHitKHR,
            MissKHR = vk::ShaderStageFlagBits::eMissKHR,
            IntersectionKHR = vk::ShaderStageFlagBits::eIntersectionKHR,
            CallableKHR = vk::ShaderStageFlagBits::eCallableKHR,
            TaskNV = vk::ShaderStageFlagBits::eTaskNV,
            MeshNV = vk::ShaderStageFlagBits::eMeshNV,
            TaskEXT = vk::ShaderStageFlagBits::eTaskEXT,
            MeshEXT = vk::ShaderStageFlagBits::eMeshEXT
        };

        using VulkanFlags = vk::ShaderStageFlags;
        using Flags = Flags<ShaderStage>;
    };

    template<>
    struct BitTraits<ShaderStage::Bits> {
        using ParentType = ShaderStage;
    };

    struct Access
    {
        enum class Bits : uint32_t {
            None = vk::AccessFlagBits::eNone,
            IndirectCommandRead = vk::AccessFlagBits::eIndirectCommandRead,
            IndexRead = vk::AccessFlagBits::eIndexRead,
            VertexAttributeRead = vk::AccessFlagBits::eVertexAttributeRead,
            UniformRead = vk::AccessFlagBits::eUniformRead,
            InputAttachmentRead = vk::AccessFlagBits::eInputAttachmentRead,
            ShaderRead = vk::AccessFlagBits::eShaderRead,
            ShaderWrite = vk::AccessFlagBits::eShaderWrite,
            ColorAttachmentRead = vk::AccessFlagBits::eColorAttachmentRead,
            ColorAttachmentWrite = vk::AccessFlagBits::eColorAttachmentWrite,
            DepthStencilAttachmentRead = vk::AccessFlagBits::eDepthStencilAttachmentRead,
            DepthStencilAttachmentWrite = vk::AccessFlagBits::eDepthStencilAttachmentWrite,
            TransferRead = vk::AccessFlagBits::eTransferRead,
            TransferWrite = vk::AccessFlagBits::eTransferWrite,
            HostRead = vk::AccessFlagBits::eHostRead,
            HostWrite = vk::AccessFlagBits::eHostWrite,
            MemoryRead = vk::AccessFlagBits::eMemoryRead,
            MemoryWrite = vk::AccessFlagBits::eMemoryWrite,
            TransformFeedbackWriteEXT = vk::AccessFlagBits::eTransformFeedbackWriteEXT,
            TransformFeedbackCounterReadEXT = vk::AccessFlagBits::eTransformFeedbackCounterReadEXT,
            TransformFeedbackCounterWriteEXT = vk::AccessFlagBits::eTransformFeedbackCounterWriteEXT,
            ConditionalRenderingReadEXT = vk::AccessFlagBits::eConditionalRenderingReadEXT,
            ColorAttachmentReadNoncoherentEXT = vk::AccessFlagBits::eColorAttachmentReadNoncoherentEXT,
            AccelerationStructureReadKHR = vk::AccessFlagBits::eAccelerationStructureReadKHR,
            AccelerationStructureWriteKHR = vk::AccessFlagBits::eAccelerationStructureWriteKHR,
            FragmentDensityMapReadEXT = vk::AccessFlagBits::eFragmentDensityMapReadEXT,
            FragmentShadingRateAttachmentReadKHR = vk::AccessFlagBits::eFragmentShadingRateAttachmentReadKHR,
            CommandPreprocessReadNV = vk::AccessFlagBits::eCommandPreprocessReadNV,
            CommandPreprocessWriteNV = vk::AccessFlagBits::eCommandPreprocessWriteNV,
        };

        using VulkanFlags = vk::AccessFlags;
        using Flags = Flags<Access>;
    };

    template<>
    struct BitTraits<Access::Bits> {
        using ParentType = Access;
    };

    struct ImageUsage
    {
        enum class Bits : uint32_t {
            None = 0,
            TransferSrc = vk::ImageUsageFlagBits::eTransferSrc,
            TransferDst = vk::ImageUsageFlagBits::eTransferDst,
            Sampled = vk::ImageUsageFlagBits::eSampled,
            Storage = vk::ImageUsageFlagBits::eStorage,
            ColorAttachment = vk::ImageUsageFlagBits::eColorAttachment,
            DepthStencilAttachment = vk::ImageUsageFlagBits::eDepthStencilAttachment,
            TransientAttachment = vk::ImageUsageFlagBits::eTransientAttachment,
            InputAttachment = vk::ImageUsageFlagBits::eInputAttachment,
            FragmentDensityMapEXT = vk::ImageUsageFlagBits::eFragmentDensityMapEXT,
            FragmentShadingRateAttachmentKHR = vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR,
            InvocationMaskHUAWEI = vk::ImageUsageFlagBits::eInvocationMaskHUAWEI,
            SampleWeightQCOM = vk::ImageUsageFlagBits::eSampleWeightQCOM,
            SampleBlockMatchQCOM = vk::ImageUsageFlagBits::eSampleBlockMatchQCOM,

            TransferSrcDst = static_cast<uint32_t>(TransferSrc) | static_cast<uint32_t>(TransferDst),
            ColorAttachmentSampled = static_cast<uint32_t>(ColorAttachment) | static_cast<uint32_t>(Sampled),
            DepthStencilAttachmentSampled = static_cast<uint32_t>(DepthStencilAttachment) | static_cast<uint32_t>(Sampled),
            StorageSampled = static_cast<uint32_t>(Storage) | static_cast<uint32_t>(Sampled),
        };

        using VulkanFlags = vk::ImageUsageFlags;
        using Flags = Flags<ImageUsage>;
    };

    template<>
    struct BitTraits<ImageUsage::Bits> {
        using ParentType = ImageUsage;
    };

    struct ImageAspect
    {
        enum class Bits : uint32_t {
            None = 0,
            Color = vk::ImageAspectFlagBits::eColor,
            Depth = vk::ImageAspectFlagBits::eDepth,
            Stencil = vk::ImageAspectFlagBits::eStencil,
            Metadata = vk::ImageAspectFlagBits::eMetadata,
            Plane0 = vk::ImageAspectFlagBits::ePlane0,
            Plane1 = vk::ImageAspectFlagBits::ePlane1,
            Plane2 = vk::ImageAspectFlagBits::ePlane2,
            MemoryPlane0EXT = vk::ImageAspectFlagBits::eMemoryPlane0EXT,
            MemoryPlane1EXT = vk::ImageAspectFlagBits::eMemoryPlane1EXT,
            MemoryPlane2EXT = vk::ImageAspectFlagBits::eMemoryPlane2EXT,
            MemoryPlane3EXT = vk::ImageAspectFlagBits::eMemoryPlane3EXT,

            DepthStencil = static_cast<uint32_t>(Depth) | static_cast<uint32_t>(Stencil),
            AllPlanes = static_cast<uint32_t>(Plane0) | static_cast<uint32_t>(Plane1) | static_cast<uint32_t>(Plane2),
        };

        using VulkanFlags = vk::ImageAspectFlags;
        using Flags = Flags<ImageAspect>;
    };

    template<>
    struct BitTraits<ImageAspect::Bits> {
        using ParentType = ImageAspect;
    };

    struct FormatFeature
    {
        enum class Bits : uint32_t {
            None = 0,
            SampledImage = vk::FormatFeatureFlagBits::eSampledImage,
            StorageImage = vk::FormatFeatureFlagBits::eStorageImage,
            StorageImageAtomic = vk::FormatFeatureFlagBits::eStorageImageAtomic,
            UniformTexelBuffer = vk::FormatFeatureFlagBits::eUniformTexelBuffer,
            StorageTexelBuffer = vk::FormatFeatureFlagBits::eStorageTexelBuffer,
            StorageTexelBufferAtomic = vk::FormatFeatureFlagBits::eStorageTexelBufferAtomic,
            VertexBuffer = vk::FormatFeatureFlagBits::eVertexBuffer,
            ColorAttachment = vk::FormatFeatureFlagBits::eColorAttachment,
            ColorAttachmentBlend = vk::FormatFeatureFlagBits::eColorAttachmentBlend,
            DepthStencilAttachment = vk::FormatFeatureFlagBits::eDepthStencilAttachment,
            BlitSrc = vk::FormatFeatureFlagBits::eBlitSrc,
            BlitDst = vk::FormatFeatureFlagBits::eBlitDst,
            SampledImageFilterLinear = vk::FormatFeatureFlagBits::eSampledImageFilterLinear,
            TransferSrc = vk::FormatFeatureFlagBits::eTransferSrc,
            TransferDst = vk::FormatFeatureFlagBits::eTransferDst,
            MidpointChromaSamples = vk::FormatFeatureFlagBits::eMidpointChromaSamples,
            SampledImageYcbcrConversionLinearFilter = vk::FormatFeatureFlagBits::eSampledImageYcbcrConversionLinearFilter,
            SampledImageYcbcrConversionSeparateReconstructionFilter = vk::FormatFeatureFlagBits::eSampledImageYcbcrConversionSeparateReconstructionFilter,
            SampledImageYcbcrConversionChromaReconstructionExplicit = vk::FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicit,
            SampledImageYcbcrConversionChromaReconstructionExplicitForceable = vk::FormatFeatureFlagBits::eSampledImageYcbcrConversionChromaReconstructionExplicitForceable,
            Disjoint = vk::FormatFeatureFlagBits::eDisjoint,
            CositedChromaSamples = vk::FormatFeatureFlagBits::eCositedChromaSamples,
            SampledImageFilterMinmax = vk::FormatFeatureFlagBits::eSampledImageFilterMinmax,
            SampledImageFilterCubicIMG = vk::FormatFeatureFlagBits::eSampledImageFilterCubicIMG,
            AccelerationStructureVertexBufferKHR = vk::FormatFeatureFlagBits::eAccelerationStructureVertexBufferKHR,
            FragmentDensityMapEXT = vk::FormatFeatureFlagBits::eFragmentDensityMapEXT,
            FragmentShadingRateAttachmentKHR = vk::FormatFeatureFlagBits::eFragmentShadingRateAttachmentKHR,
        };

        using VulkanFlags = vk::FormatFeatureFlags;
        using Flags = Flags<FormatFeature>;
    };

    template<>
    struct BitTraits<FormatFeature::Bits> {
        using ParentType = FormatFeature;
    };

    struct FenceCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            Signaled = vk::FenceCreateFlagBits::eSignaled,
        };

        using VulkanFlags = vk::FenceCreateFlags;
        using Flags = Flags<FenceCreate>;
    };

    template<>
    struct BitTraits<FenceCreate::Bits> {
        using ParentType = FenceCreate;
    };

    struct CommandPoolCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            Protected = vk::CommandPoolCreateFlagBits::eProtected,
            ResetCommandBuffer = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            Transient = vk::CommandPoolCreateFlagBits::eTransient,
        };

        using VulkanFlags = vk::CommandPoolCreateFlags;
        using Flags = Flags<CommandPoolCreate>;
    };

    template<>
    struct BitTraits<CommandPoolCreate::Bits> {
        using ParentType = CommandPoolCreate;
    };

    struct CommandPoolReset
    {
        enum class Bits : uint32_t {
            None = 0,
            ReleaseResources = vk::CommandPoolResetFlagBits::eReleaseResources,
        };

        using VulkanFlags = vk::CommandPoolResetFlags;
        using Flags = Flags<CommandPoolReset>;
    };

    template<>
    struct BitTraits<CommandPoolReset::Bits> {
        using ParentType = CommandPoolReset;
    };

    namespace DebugUtils
    {
        struct MessengerCallbackData
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::DebugUtilsMessengerCallbackDataFlagsEXT;
            using Flags = Flags<MessengerCallbackData>;
        };
    }

    template<>
    struct BitTraits<DebugUtils::MessengerCallbackData::Bits> {
        using ParentType = DebugUtils::MessengerCallbackData;
    };

    struct SurfaceTransform
    {
        enum class Bits : uint32_t {
            None = 0,
            Identity = vk::SurfaceTransformFlagBitsKHR::eIdentity,
            Rotate90 = vk::SurfaceTransformFlagBitsKHR::eRotate90,
            Rotate180 = vk::SurfaceTransformFlagBitsKHR::eRotate180,
            Rotate270 = vk::SurfaceTransformFlagBitsKHR::eRotate270,
            HorizontalMirror = vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror,
            HorizontalMirrorRotate90 = vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90,
            HorizontalMirrorRotate180 = vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180,
            HorizontalMirrorRotate270 = vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270,
            Inherit = vk::SurfaceTransformFlagBitsKHR::eInherit
        };

        using VulkanFlags = vk::SurfaceTransformFlagsKHR;
        using Flags = Flags<SurfaceTransform>;
    };

    template<>
    struct BitTraits<SurfaceTransform::Bits> {
        using ParentType = SurfaceTransform;
    };

    struct SemaphoreCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };

        using VulkanFlags = vk::SemaphoreCreateFlags;
        using Flags = Flags<SemaphoreCreate>;
    };

    template<>
    struct BitTraits<SemaphoreCreate::Bits> {
        using ParentType = SemaphoreCreate;
    };

    struct SampleCount
    {
        enum class Bits : uint32_t {
            None = 0,
			SC1 = vk::SampleCountFlagBits::e1,
            SC2 = vk::SampleCountFlagBits::e2,
            SC4 = vk::SampleCountFlagBits::e4,
            SC8 = vk::SampleCountFlagBits::e8,
            SC16 = vk::SampleCountFlagBits::e16,
            SC32 = vk::SampleCountFlagBits::e32,
            SC64 = vk::SampleCountFlagBits::e64,
			SampleCountAll = SC1 | SC2 | SC4 | SC8 | SC16 | SC32 | SC64
        };

        using VulkanFlags = vk::SampleCountFlags;
        using Flags = Flags<SampleCount>;
    };

    template<>
    struct BitTraits<SampleCount::Bits> {
        using ParentType = SampleCount;
    };

    struct AttachmentCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            MayAlias = vk::AttachmentDescriptionFlagBits::eMayAlias,
        };

        using VulkanFlags = vk::AttachmentDescriptionFlags;
        using Flags = Flags<AttachmentCreate>;
    };

    template<>
    struct BitTraits<AttachmentCreate::Bits> {
        using ParentType = AttachmentCreate;
    };

    struct Dependency
    {
        enum class Bits : uint32_t {
            None = 0,
            ByRegion = vk::DependencyFlagBits::eByRegion,
			ViewLocal = vk::DependencyFlagBits::eViewLocal,
            ViewLocalKHR = vk::DependencyFlagBits::eViewLocalKHR,
			DeviceGroup = vk::DependencyFlagBits::eDeviceGroup,
            DeviceGroupKHR = vk::DependencyFlagBits::eDeviceGroupKHR,
            FeedbackLoopEXT = vk::DependencyFlagBits::eFeedbackLoopEXT,
            QueueFamilyOwnershipTransferUseAllStagesKHR = vk::DependencyFlagBits::eQueueFamilyOwnershipTransferUseAllStagesKHR,
        };

        using VulkanFlags = vk::DependencyFlags;
        using Flags = Flags<Dependency>;
    };

    template<>
    struct BitTraits<Dependency::Bits> {
        using ParentType = Dependency;
    };

    struct CompositeAlpha
    {
        enum class Bits : uint32_t {
            None = 0,
            Inherit = vk::CompositeAlphaFlagBitsKHR::eInherit,
            Opaque = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            PostMultiplied = vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
            PreMultiplied = vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
        };

        using VulkanFlags = vk::CompositeAlphaFlagsKHR;
        using Flags = Flags<CompositeAlpha>;
    };

    template<>
    struct BitTraits<CompositeAlpha::Bits> {
        using ParentType = CompositeAlpha;
    };
}