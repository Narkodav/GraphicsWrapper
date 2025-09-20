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
        using VulkanBits = Definition::VulkanBits;
        using VulkanCFlags = Definition::VulkanCFlags;
        using VulkanCBits = Definition::VulkanCBits;

        Flags() : m_flags(0) {};
        Flags(typename Bits bit) : m_flags(static_cast<uint32_t>(bit)) {}
        Flags(uint32_t flags) : m_flags(flags) {}
        Flags(typename VulkanFlags flags) : m_flags(static_cast<uint32_t>(flags)) {};
        Flags(typename VulkanBits bit) : m_flags(static_cast<uint32_t>(bit)) {};

        template<typename T>
        Flags(T value) {
            if constexpr (std::is_same_v<T, VulkanCBits> && !std::is_void_v<VulkanCBits>) {
                m_flags = static_cast<uint32_t>(value);
            }
            else {
                static_assert(false, "Invalid type for Flags constructor");
            }
        }

        operator VulkanFlags() const { return VulkanFlags(this->m_flags); }
        operator VulkanBits() const { return static_cast<VulkanBits>(this->m_flags); }

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
        using VulkanBits = vk::MemoryPropertyFlagBits;
        using VulkanCFlags = VkMemoryPropertyFlags;
        using VulkanCBits = VkMemoryPropertyFlagBits;

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
        using VulkanBits = vk::MemoryHeapFlagBits;
        using VulkanCFlags = VkMemoryHeapFlags;
        using VulkanCBits = VkMemoryHeapFlagBits;

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
        using VulkanBits = vk::BufferUsageFlagBits;
        using VulkanCFlags = VkBufferUsageFlags;
        using VulkanCBits = VkBufferUsageFlagBits;

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
        using VulkanBits = vk::CommandBufferUsageFlagBits;
        using VulkanCFlags = VkCommandBufferUsageFlags;
        using VulkanCBits = VkCommandBufferUsageFlagBits;

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
        using VulkanBits = vk::DebugUtilsMessageSeverityFlagBitsEXT;
        using VulkanCFlags = VkDebugUtilsMessageSeverityFlagsEXT;
        using VulkanCBits = VkDebugUtilsMessageSeverityFlagBitsEXT;

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
        using VulkanBits = vk::DebugUtilsMessageTypeFlagBitsEXT;
        using VulkanCFlags = VkDebugUtilsMessageTypeFlagsEXT;
        using VulkanCBits = VkDebugUtilsMessageTypeFlagBitsEXT;

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
        using VulkanBits = vk::DescriptorSetLayoutCreateFlagBits;
        using VulkanCFlags = VkDescriptorSetLayoutCreateFlags;
        using VulkanCBits = VkDescriptorSetLayoutCreateFlagBits;

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
        using VulkanBits = vk::QueueFlagBits;
        using VulkanCFlags = VkQueueFlags;
        using VulkanCBits = VkQueueFlagBits;

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
        using VulkanBits = vk::DescriptorPoolCreateFlagBits;
        using VulkanCFlags = VkDescriptorPoolCreateFlags;
        using VulkanCBits = VkDescriptorPoolCreateFlagBits;

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
        using VulkanBits = vk::PipelineStageFlagBits;
        using VulkanCFlags = VkPipelineStageFlags;
        using VulkanCBits = VkPipelineStageFlagBits;

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
        using VulkanBits = vk::ShaderStageFlagBits;
        using VulkanCFlags = VkShaderStageFlags;
        using VulkanCBits = VkShaderStageFlagBits;

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
        using VulkanBits = vk::AccessFlagBits;
        using VulkanCFlags = VkAccessFlags;
        using VulkanCBits = VkAccessFlagBits;

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
        using VulkanBits = vk::ImageUsageFlagBits;
        using VulkanCFlags = VkImageUsageFlags;
        using VulkanCBits = VkImageUsageFlagBits;

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
        using VulkanBits = vk::ImageAspectFlagBits;
        using VulkanCFlags = VkImageAspectFlags;
        using VulkanCBits = VkImageAspectFlagBits;

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
        using VulkanBits = vk::FormatFeatureFlagBits;
        using VulkanCFlags = VkFormatFeatureFlags;
        using VulkanCBits = VkFormatFeatureFlagBits;

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
        using VulkanBits = vk::FenceCreateFlagBits;
        using VulkanCFlags = VkFenceCreateFlags;
        using VulkanCBits = VkFenceCreateFlagBits;

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
        using VulkanBits = vk::CommandPoolCreateFlagBits;
        using VulkanCFlags = VkCommandPoolCreateFlags;
        using VulkanCBits = VkCommandPoolCreateFlagBits;

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
        using VulkanBits = vk::CommandPoolResetFlagBits;
        using VulkanCFlags = VkCommandPoolResetFlags;
        using VulkanCBits = VkCommandPoolResetFlagBits;

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
            using VulkanBits = vk::DebugUtilsMessengerCallbackDataFlagBitsEXT;
            using VulkanCFlags = VkDebugUtilsMessengerCallbackDataFlagsEXT;
            using VulkanCBits = void;

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
        using VulkanBits = vk::SurfaceTransformFlagBitsKHR;
        using VulkanCFlags = VkSurfaceTransformFlagsKHR;
        using VulkanCBits = VkSurfaceTransformFlagBitsKHR;

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
        using VulkanBits = vk::SemaphoreCreateFlagBits;
        using VulkanCFlags = VkSemaphoreCreateFlags;
        using VulkanCBits = void;

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
        using VulkanBits = vk::SampleCountFlagBits;
        using VulkanCFlags = VkSampleCountFlags;
        using VulkanCBits = VkSampleCountFlagBits;

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
        using VulkanBits = vk::AttachmentDescriptionFlagBits;
        using VulkanCFlags = VkAttachmentDescriptionFlags;
        using VulkanCBits = VkAttachmentDescriptionFlagBits;

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
        using VulkanBits = vk::DependencyFlagBits;
        using VulkanCFlags = VkDependencyFlags;
        using VulkanCBits = VkDependencyFlagBits;

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
        using VulkanBits = vk::CompositeAlphaFlagBitsKHR;
        using VulkanCFlags = VkCompositeAlphaFlagsKHR;
        using VulkanCBits = VkCompositeAlphaFlagBitsKHR;

        using Flags = Flags<CompositeAlpha>;
    };

    template<>
    struct BitTraits<CompositeAlpha::Bits> {
        using ParentType = CompositeAlpha;
    };

    struct PipelineLayoutCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            IndependentSetsEXT = vk::PipelineLayoutCreateFlagBits::eIndependentSetsEXT,
        };

        using VulkanFlags = vk::PipelineLayoutCreateFlags;
        using VulkanBits = vk::PipelineLayoutCreateFlagBits;
        using VulkanCFlags = VkPipelineLayoutCreateFlags;
        using VulkanCBits = VkPipelineLayoutCreateFlagBits;

        using Flags = Flags<PipelineLayoutCreate>;
    };

    template<>
    struct BitTraits<PipelineLayoutCreate::Bits> {
        using ParentType = PipelineLayoutCreate;
    };

    struct PipelineCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            DisableOptimization = vk::PipelineCreateFlagBits::eDisableOptimization,
            AllowDerivatives = vk::PipelineCreateFlagBits::eAllowDerivatives,
			Derivative = vk::PipelineCreateFlagBits::eDerivative,
            ViewIndexFromDeviceIndex = vk::PipelineCreateFlagBits::eViewIndexFromDeviceIndex,
            ViewIndexFromDeviceIndexKHR = vk::PipelineCreateFlagBits::eViewIndexFromDeviceIndexKHR,
            DispatchBase = vk::PipelineCreateFlagBits::eDispatchBase,
            DispatchBaseKHR = vk::PipelineCreateFlagBits::eDispatchBaseKHR,
            FailOnPipelineCompileRequired = vk::PipelineCreateFlagBits::eFailOnPipelineCompileRequiredEXT,
            EarlyReturnOnFailure = vk::PipelineCreateFlagBits::eEarlyReturnOnFailureEXT,
            NoProtectedAccess = vk::PipelineCreateFlagBits::eNoProtectedAccessEXT,
            ProtectedAccessOnly = vk::PipelineCreateFlagBits::eProtectedAccessOnlyEXT,
            RayTracingNoNullAnyHitShadersKHR = vk::PipelineCreateFlagBits::eRayTracingNoNullAnyHitShadersKHR,
			RayTracingNoNullClosestHitShadersKHR = vk::PipelineCreateFlagBits::eRayTracingNoNullClosestHitShadersKHR,
			RayTracingNoNullMissShadersKHR = vk::PipelineCreateFlagBits::eRayTracingNoNullMissShadersKHR,
			RayTracingNoNullIntersectionShadersKHR = vk::PipelineCreateFlagBits::eRayTracingNoNullIntersectionShadersKHR,
			RayTracingSkipTrianglesKHR = vk::PipelineCreateFlagBits::eRayTracingSkipTrianglesKHR,
			RayTracingSkipAabbsKHR = vk::PipelineCreateFlagBits::eRayTracingSkipAabbsKHR,
			RayTracingShaderGroupHandleCaptureReplayKHR = vk::PipelineCreateFlagBits::eRayTracingShaderGroupHandleCaptureReplayKHR,
            DeferCompileNV = vk::PipelineCreateFlagBits::eDeferCompileNV,
            RenderingFragmentDensityMapAttachmentEXT = vk::PipelineCreateFlagBits::eRenderingFragmentDensityMapAttachmentEXT,
            RenderingFragmentShadingRateAttachmentKHR = vk::PipelineCreateFlagBits::eRenderingFragmentShadingRateAttachmentKHR,
            CaptureStatisticsKHR = vk::PipelineCreateFlagBits::eCaptureStatisticsKHR,
            CaptureInternalRepresentationsKHR = vk::PipelineCreateFlagBits::eCaptureInternalRepresentationsKHR,
            IndirectBindableNV = vk::PipelineCreateFlagBits::eIndirectBindableNV,
			Library = vk::PipelineCreateFlagBits::eLibraryKHR,
            DescriptorBufferEXT = vk::PipelineCreateFlagBits::eDescriptorBufferEXT,
            RetainLinkTimeOptimizationInfoEXT = vk::PipelineCreateFlagBits::eRetainLinkTimeOptimizationInfoEXT,
			LinkTimeOptimization = vk::PipelineCreateFlagBits::eLinkTimeOptimizationEXT,
            RayTracingAllowMotionNV = vk::PipelineCreateFlagBits::eRayTracingAllowMotionNV,
            ColorAttachmentFeedbackLoopEXT = vk::PipelineCreateFlagBits::eColorAttachmentFeedbackLoopEXT,
            DepthStencilAttachmentFeedbackLoopEXT = vk::PipelineCreateFlagBits::eDepthStencilAttachmentFeedbackLoopEXT,
            RayTracingOpacityMicromapEXT = vk::PipelineCreateFlagBits::eRayTracingOpacityMicromapEXT,
        };

        using VulkanFlags = vk::PipelineCreateFlags;
        using VulkanBits = vk::PipelineCreateFlagBits;
        using VulkanCFlags = VkPipelineCreateFlags;
        using VulkanCBits = VkPipelineCreateFlagBits;

        using Flags = Flags<PipelineCreate>;
    };

    template<>
    struct BitTraits<PipelineCreate::Bits> {
        using ParentType = PipelineCreate;
    };

    struct PipelineShaderStageCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            AllowVaryingSubgroupSize = vk::PipelineShaderStageCreateFlagBits::eAllowVaryingSubgroupSize,
			RequireFullSubgroups = vk::PipelineShaderStageCreateFlagBits::eRequireFullSubgroups,
			AllowVaryingSubgroupSizeEXT = vk::PipelineShaderStageCreateFlagBits::eAllowVaryingSubgroupSizeEXT,
			RequireFullSubgroupsEXT = vk::PipelineShaderStageCreateFlagBits::eRequireFullSubgroupsEXT,
        };

        using VulkanFlags = vk::PipelineShaderStageCreateFlags;
        using VulkanBits = vk::PipelineShaderStageCreateFlagBits;
        using VulkanCFlags = VkPipelineShaderStageCreateFlags;
        using VulkanCBits = VkPipelineShaderStageCreateFlagBits;

        using Flags = Flags<PipelineShaderStageCreate>;
    };

    template<>
    struct BitTraits<PipelineShaderStageCreate::Bits> {
        using ParentType = PipelineShaderStageCreate;
    };

    struct PipelineVertexInputStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };

        using VulkanFlags = vk::PipelineVertexInputStateCreateFlags;
        using VulkanBits = vk::PipelineVertexInputStateCreateFlagBits;
        using VulkanCFlags = VkPipelineVertexInputStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineVertexInputStateCreate>;
    };

    template<>
    struct BitTraits<PipelineVertexInputStateCreate::Bits> {
        using ParentType = PipelineVertexInputStateCreate;
    };

    struct PipelineInputAssemblyStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };

        using VulkanFlags = vk::PipelineInputAssemblyStateCreateFlags;
        using VulkanBits = vk::PipelineInputAssemblyStateCreateFlagBits;
        using VulkanCFlags = VkPipelineInputAssemblyStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineInputAssemblyStateCreate>;
    };

    template<>
    struct BitTraits<PipelineInputAssemblyStateCreate::Bits> {
        using ParentType = PipelineInputAssemblyStateCreate;
    };
    
    struct PipelineTessellationStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };
        using VulkanFlags = vk::PipelineTessellationStateCreateFlags;
        using VulkanBits = vk::PipelineTessellationStateCreateFlagBits;
        using VulkanCFlags = VkPipelineTessellationStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineTessellationStateCreate>;
	};

    template<>
    struct BitTraits<PipelineTessellationStateCreate::Bits> {
        using ParentType = PipelineTessellationStateCreate;
    };

    struct PipelineViewportStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };
        using VulkanFlags = vk::PipelineViewportStateCreateFlags;
        using VulkanBits = vk::PipelineViewportStateCreateFlagBits;
        using VulkanCFlags = VkPipelineViewportStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineViewportStateCreate>;
    };

    template<>
    struct BitTraits<PipelineViewportStateCreate::Bits> {
        using ParentType = PipelineViewportStateCreate;
    };

    struct PipelineRasterizationStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };
        using VulkanFlags = vk::PipelineRasterizationStateCreateFlags;
        using VulkanBits = vk::PipelineRasterizationStateCreateFlagBits;
        using VulkanCFlags = VkPipelineRasterizationStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineRasterizationStateCreate>;
    };

    template<>
    struct BitTraits<PipelineRasterizationStateCreate::Bits> {
        using ParentType = PipelineRasterizationStateCreate;
    };

    struct CullMode
    {
        enum class Bits : uint32_t {
            None = vk::CullModeFlagBits::eNone,
			Front = vk::CullModeFlagBits::eFront,
            Back = vk::CullModeFlagBits::eBack,
			FrontAndBack = vk::CullModeFlagBits::eFrontAndBack,
        };
        using VulkanFlags = vk::CullModeFlags;
        using VulkanBits = vk::CullModeFlagBits;
        using VulkanCFlags = VkCullModeFlags;
        using VulkanCBits = VkCullModeFlagBits;

        using Flags = Flags<CullMode>;
    };

    template<>
    struct BitTraits<CullMode::Bits> {
        using ParentType = CullMode;
    };

    struct PipelineMultisampleStateCreate
    {
        enum class Bits : uint32_t {
            None = 0
        };
        using VulkanFlags = vk::PipelineMultisampleStateCreateFlags;
        using VulkanBits = vk::PipelineMultisampleStateCreateFlagBits;
        using VulkanCFlags = VkPipelineMultisampleStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineMultisampleStateCreate>;
    };

    template<>
    struct BitTraits<PipelineMultisampleStateCreate::Bits> {
        using ParentType = PipelineMultisampleStateCreate;
    };

    struct PipelineDepthStencilStateCreate
    {
        enum class Bits : uint32_t {
            None = 0
        };
        using VulkanFlags = vk::PipelineDepthStencilStateCreateFlags;
        using VulkanBits = vk::PipelineDepthStencilStateCreateFlagBits;
        using VulkanCFlags = VkPipelineDepthStencilStateCreateFlags;
        using VulkanCBits = VkPipelineDepthStencilStateCreateFlagBits;

        using Flags = Flags<PipelineDepthStencilStateCreate>;
    };

    template<>
    struct BitTraits<PipelineDepthStencilStateCreate::Bits> {
        using ParentType = PipelineDepthStencilStateCreate;
    };
    
    struct PipelineColorBlendStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            RasterizationOrderAttachmentAccessARM = vk::PipelineColorBlendStateCreateFlagBits::eRasterizationOrderAttachmentAccessARM,
            RasterizationOrderAttachmentAccessEXT = vk::PipelineColorBlendStateCreateFlagBits::eRasterizationOrderAttachmentAccessEXT,
        };
        using VulkanFlags = vk::PipelineColorBlendStateCreateFlags;
        using VulkanBits = vk::PipelineColorBlendStateCreateFlagBits;
        using VulkanCFlags = VkPipelineColorBlendStateCreateFlags;
        using VulkanCBits = VkPipelineColorBlendStateCreateFlagBits;

        using Flags = Flags<PipelineColorBlendStateCreate>;
    };

    template<>
    struct BitTraits<PipelineColorBlendStateCreate::Bits> {
        using ParentType = PipelineColorBlendStateCreate;
    };

    struct PipelineDynamicStateCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };
        using VulkanFlags = vk::PipelineDynamicStateCreateFlags;
        using VulkanBits = vk::PipelineDynamicStateCreateFlagBits;
        using VulkanCFlags = VkPipelineDynamicStateCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<PipelineDynamicStateCreate>;
    };

    template<>
    struct BitTraits<PipelineDynamicStateCreate::Bits> {
        using ParentType = PipelineDynamicStateCreate;
    };

    struct ColorComponent
    {
        enum class Bits : uint32_t {
            None = 0,			
			R = vk::ColorComponentFlagBits::eR,
			G = vk::ColorComponentFlagBits::eG,
            B = vk::ColorComponentFlagBits::eB,
            A = vk::ColorComponentFlagBits::eA,
        };
        using VulkanFlags = vk::ColorComponentFlags;
        using VulkanBits = vk::ColorComponentFlagBits;
        using VulkanCFlags = VkColorComponentFlags;
        using VulkanCBits = VkColorComponentFlagBits;

        using Flags = Flags<ColorComponent>;
    };

    template<>
    struct BitTraits<ColorComponent::Bits> {
        using ParentType = ColorComponent;
    };

    struct DescriptorPoolCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            FreeDescriptorSet = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            UpdateAfterBindEXT = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBindEXT,
            HostOnlyValve = vk::DescriptorPoolCreateFlagBits::eHostOnlyVALVE,
            HostOnlyEXT = vk::DescriptorPoolCreateFlagBits::eHostOnlyEXT,
            AllowOverallocationPoolsNV = vk::DescriptorPoolCreateFlagBits::eAllowOverallocationPoolsNV,
            AllowOverallocationSetsNV = vk::DescriptorPoolCreateFlagBits::eAllowOverallocationSetsNV,
            UpdateAfterBind = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
        };

        using VulkanFlags = vk::DescriptorPoolCreateFlags;
        using VulkanBits = vk::DescriptorPoolCreateFlagBits;
        using VulkanCFlags = VkDescriptorPoolCreateFlags;
        using VulkanCBits = VkDescriptorPoolCreateFlagBits;

        using Flags = Flags<DescriptorPoolCreate>;
    };

    template<>
    struct BitTraits<DescriptorPoolCreate::Bits> {
        using ParentType = DescriptorPoolCreate;
    };
    
    struct DescriptorPoolReset
    {
        enum class Bits : uint32_t {
            None = 0,
        };

        using VulkanFlags = vk::DescriptorPoolResetFlags;
        using VulkanBits = vk::DescriptorPoolResetFlagBits;
        using VulkanCFlags = VkDescriptorPoolResetFlags;
        using VulkanCBits = void;

        using Flags = Flags<DescriptorPoolReset>;
    };

    template<>
    struct BitTraits<DescriptorPoolReset::Bits> {
        using ParentType = DescriptorPoolReset;
    };

    struct QueryControl
    {
        enum class Bits : uint32_t {
            None = 0,
            Precise = vk::QueryControlFlagBits::ePrecise,
        };

        using VulkanFlags = vk::QueryControlFlags;
        using VulkanBits = vk::QueryControlFlagBits;
        using VulkanCFlags = VkQueryControlFlags;
        using VulkanCBits = VkQueryControlFlagBits;

        using Flags = Flags<QueryControl>;
    };

    template<>
    struct BitTraits<QueryControl::Bits> {
        using ParentType = QueryControl;
    };

    struct QueryPipelineStatistic
    {
        enum class Bits : uint32_t {
            None = 0,
            ClippingInvocations = vk::QueryPipelineStatisticFlagBits::eClippingInvocations,
            ClippingPrimitives = vk::QueryPipelineStatisticFlagBits::eClippingPrimitives,
            FragmentShaderInvocations = vk::QueryPipelineStatisticFlagBits::eFragmentShaderInvocations,
            //FragmentShaderInvocationsARM = vk::QueryPipelineStatisticFlagBits::eFragmentShaderInvocationsARM,
            //FragmentShaderInvocationsEXT = vk::QueryPipelineStatisticFlagBits::eFragmentShaderInvocationsEXT,
            GeometryShaderInvocations = vk::QueryPipelineStatisticFlagBits::eGeometryShaderInvocations,
            GeometryShaderPrimitives = vk::QueryPipelineStatisticFlagBits::eGeometryShaderPrimitives,
            //InstanceCount = vk::QueryPipelineStatisticFlagBits::eInstanceCount,
            //PrimitiveCount = vk::QueryPipelineStatisticFlagBits::ePrimitiveCount,
            VertexShaderInvocations = vk::QueryPipelineStatisticFlagBits::eVertexShaderInvocations,
        };

        using VulkanFlags = vk::QueryPipelineStatisticFlags;
        using VulkanBits = vk::QueryPipelineStatisticFlagBits;
        using VulkanCFlags = VkQueryPipelineStatisticFlags;
        using VulkanCBits = VkQueryPipelineStatisticFlagBits;

        using Flags = Flags<QueryPipelineStatistic>;
    };

    template<>
    struct BitTraits<QueryPipelineStatistic::Bits> {
        using ParentType = QueryPipelineStatistic;
    };

    struct SamplerCreate
    {
        enum class Bits : uint32_t {
            None = 0,
            DescriptorBufferCaptureReplayEXT = vk::SamplerCreateFlagBits::eDescriptorBufferCaptureReplayEXT,
            SubsampledCoarseReconstruction = vk::SamplerCreateFlagBits::eSubsampledCoarseReconstructionEXT,
            NonSeamlessCubeMap = vk::SamplerCreateFlagBits::eNonSeamlessCubeMapEXT,
            SubsampledEXT = vk::SamplerCreateFlagBits::eSubsampledEXT,
            ImageProcessingQCOM = vk::SamplerCreateFlagBits::eImageProcessingQCOM,
        };

        using VulkanFlags = vk::SamplerCreateFlags;
        using VulkanBits = vk::SamplerCreateFlagBits;
        using VulkanCFlags = VkSamplerCreateFlags;
        using VulkanCBits = VkSamplerCreateFlagBits;

        using Flags = Flags<SamplerCreate>;
    };

    template<>
    struct BitTraits<SamplerCreate::Bits> {
        using ParentType = SamplerCreate;
    };
    
    struct CommandBufferReset
    {
        enum class Bits : uint32_t {
            None = 0,
            ReleaseResources = vk::CommandBufferResetFlagBits::eReleaseResources
        };

        using VulkanFlags = vk::CommandBufferResetFlags;
        using VulkanBits = vk::CommandBufferResetFlagBits;
        using VulkanCFlags = VkCommandBufferResetFlags;
        using VulkanCBits = VkCommandBufferResetFlagBits;

        using Flags = Flags<CommandBufferReset>;
    };

    template<>
    struct BitTraits<CommandBufferReset::Bits> {
        using ParentType = CommandBufferReset;
    };

    struct BufferViewCreate
    {
        enum class Bits : uint32_t {
            None = 0,
        };

        using VulkanFlags = vk::BufferViewCreateFlags;
        using VulkanBits = vk::BufferViewCreateFlagBits;
        using VulkanCFlags = VkBufferViewCreateFlags;
        using VulkanCBits = void;

        using Flags = Flags<BufferViewCreate>;
    };

    template<>
    struct BitTraits<BufferViewCreate::Bits> {
        using ParentType = BufferViewCreate;
    };

    struct MemoryMap
    {
        enum class Bits : uint32_t {
            None = 0,
            PlacedEXT = vk::MemoryMapFlagBits::ePlacedEXT,
        };

        using VulkanFlags = vk::MemoryMapFlags;
        using VulkanBits = vk::MemoryMapFlagBits;
        using VulkanCFlags = VkMemoryMapFlags;
        using VulkanCBits = VkMemoryMapFlagBits;

        using Flags = Flags<MemoryMap>;
    };

    template<>
    struct BitTraits<MemoryMap::Bits> {
        using ParentType = MemoryMap;
    };
    
    template<typename Bit>
    typename BitTraits<Bit>::ParentType::VulkanBits convertBit(Bit bit) {
        return static_cast<BitTraits<Bit>::ParentType::VulkanBits>(bit);
    }

    template<typename Bit>
    typename BitTraits<Bit>::ParentType::VulkanCBits convertCBit(Bit bit) {
        return static_cast<BitTraits<Bit>::ParentType::VulkanCBits>(bit);
    }
}