#include "../PhysicalDeviceCache.h"

namespace Graphics {

    template<size_t index, typename T>
    std::any getCorrectAnyFeature(const T& data) {
        return std::any(static_cast<typename DeviceFeatureTypeTrait<static_cast<DeviceFeature>(index)>::Type>(data));
    }

	const std::array<PhysicalDevice::CompleteFeatureChain::FeatureSetFunc,
		static_cast<size_t>(DeviceFeature::Num)> PhysicalDevice::CompleteFeatureChain::s_featureSet = {
    //GeometryShader,
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.geometryShader = std::any_cast<bool>(data);
        },
    //TessellationShader,
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.tessellationShader = std::any_cast<bool>(data);
        },
    // ShaderFloat64
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.shaderFloat64 = std::any_cast<bool>(data);
        },
    // SamplerAnisotropy
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.samplerAnisotropy = std::any_cast<bool>(data);
        },
    // FillModeNonSolid
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.fillModeNonSolid = std::any_cast<bool>(data);
        },
    // TextureCompressionBc
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.textureCompressionBC = std::any_cast<bool>(data);
        },
    // TextureCompressionEtc2
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.textureCompressionETC2 = std::any_cast<bool>(data);
        },
    // MultiDrawIndirect
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.multiDrawIndirect = std::any_cast<bool>(data);
        },
    // WideLines
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.wideLines = std::any_cast<bool>(data);
        },
    // LargePoints
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.largePoints = std::any_cast<bool>(data);
        },
    // MultiViewport
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.multiViewport = std::any_cast<bool>(data);
        },
    // DepthClamp
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.depthClamp = std::any_cast<bool>(data);
        },
    // DepthBiasClamp
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.depthBiasClamp = std::any_cast<bool>(data);
        },
    // DualSrcBlend
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.dualSrcBlend = std::any_cast<bool>(data);
        },
    // VertexPipelineStoresAndAtomics
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.vertexPipelineStoresAndAtomics = std::any_cast<bool>(data);
        },
    // FragmentStoresAndAtomics
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.fragmentStoresAndAtomics = std::any_cast<bool>(data);
        },
    // ShaderBufferInt64Atomics
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.shaderBufferInt64Atomics = std::any_cast<bool>(data);
        },
    // ShaderSharedInt64Atomics
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.shaderSharedInt64Atomics = std::any_cast<bool>(data);
        },
    // ShaderInt8
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.shaderInt8 = std::any_cast<bool>(data);
        },
    // DescriptorBindingPartiallyBound
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingPartiallyBound = std::any_cast<bool>(data);
        },
    // DescriptorBindingUpdateUnusedWhilePending
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingUpdateUnusedWhilePending = std::any_cast<bool>(data);
        },
    // DescriptorBindingUniformBufferUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingUniformBufferUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorBindingSampledImageUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingSampledImageUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorBindingStorageImageUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingStorageImageUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorBindingVariableDescriptorCount
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingVariableDescriptorCount = std::any_cast<bool>(data);
        },
    // ShaderSampledImageArrayNonUniformIndexing
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.shaderSampledImageArrayNonUniformIndexing = std::any_cast<bool>(data);
        },
    // RuntimeDescriptorArray
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.runtimeDescriptorArray = std::any_cast<bool>(data);
        },
    // DescriptorBindingStorageBufferUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorBindingStorageBufferUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorIndexing
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
            props.descriptorIndexing = std::any_cast<bool>(data);
        },
    // NullDescriptor
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceRobustness2FeaturesEXT>();
            props.nullDescriptor = std::any_cast<bool>(data);
        },
    // RobustBufferAccess
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            props.features.robustBufferAccess = std::any_cast<bool>(data);
        },
    // RobustImageAccess
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan13Features>();
            props.robustImageAccess = std::any_cast<bool>(data);
        },
    // MeshShaderEXT
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {            
            auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesEXT>();
            props.meshShader = std::any_cast<bool>(data);
        },
    // TaskShaderEXT
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesEXT>();
            props.taskShader = std::any_cast<bool>(data);
        },
    // ShaderDrawParameters
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceVulkan11Features>();
            props.shaderDrawParameters = std::any_cast<bool>(data);
        }
	};

	const std::array<PhysicalDevice::CompleteFeatureChain::GetFeatureFunc,
		static_cast<size_t>(DeviceFeature::Num)> PhysicalDevice::CompleteFeatureChain::s_featureGet = {
        // GeometryShader
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<0>(props.features.geometryShader);
            },
        // TessellationShader
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<1>(props.features.tessellationShader);
            },
        // ShaderFloat64
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<2>(props.features.shaderFloat64);
            },
        // SamplerAnisotropy
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<3>(props.features.samplerAnisotropy);
            },
        // FillModeNonSolid
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<4>(props.features.fillModeNonSolid);
            },
        // TextureCompressionBc
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<5>(props.features.textureCompressionBC);
            },
        // TextureCompressionEtc2
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<6>(props.features.textureCompressionETC2);
            },
        // MultiDrawIndirect
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<7>(props.features.multiDrawIndirect);
            },
        // WideLines
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<8>(props.features.wideLines);
            },
        // LargePoints
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<9>(props.features.largePoints);
            },
        // MultiViewport
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<10>(props.features.multiViewport);
            },
        // DepthClamp
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<11>(props.features.depthClamp);
            },
        // DepthBiasClamp
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<12>(props.features.depthBiasClamp);
            },
        // DualSrcBlend
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<13>(props.features.dualSrcBlend);
            },
        // VertexPipelineStoresAndAtomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<14>(props.features.vertexPipelineStoresAndAtomics);
            },
        // FragmentStoresAndAtomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<15>(props.features.fragmentStoresAndAtomics);
            },
        // ShaderBufferInt64Atomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<16>(props.shaderBufferInt64Atomics);
            },
        // ShaderSharedInt64Atomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<17>(props.shaderSharedInt64Atomics);
            },
        // ShaderInt8
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<18>(props.shaderInt8);
            },
        // DescriptorBindingPartiallyBound
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<19>(props.descriptorBindingPartiallyBound);
            },
        // DescriptorBindingUpdateUnusedWhilePending
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<20>(props.descriptorBindingUpdateUnusedWhilePending);
            },
        // DescriptorBindingUniformBufferUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<21>(props.descriptorBindingUniformBufferUpdateAfterBind);
            },
        // DescriptorBindingSampledImageUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<22>(props.descriptorBindingSampledImageUpdateAfterBind);
            },
        // DescriptorBindingStorageImageUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<23>(props.descriptorBindingStorageImageUpdateAfterBind);
            },
        // DescriptorBindingVariableDescriptorCount
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<24>(props.descriptorBindingVariableDescriptorCount);
            },
        // ShaderSampledImageArrayNonUniformIndexing
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<25>(props.shaderSampledImageArrayNonUniformIndexing);
            },
        // RuntimeDescriptorArray
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<26>(props.runtimeDescriptorArray);
            },
        // DescriptorBindingStorageBufferUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<27>(props.descriptorBindingStorageBufferUpdateAfterBind);
            },
        // DescriptorIndexing
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan12Features>();
                return getCorrectAnyFeature<27>(props.descriptorIndexing);
            },
        // NullDescriptor
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceRobustness2FeaturesEXT>();
                return getCorrectAnyFeature<28>(props.nullDescriptor);
            },
        // RobustBufferAccess
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return getCorrectAnyFeature<29>(props.features.robustBufferAccess);
            },
        // RobustImageAccess
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan13Features>();
                return getCorrectAnyFeature<30>(props.robustImageAccess);
            },
        // MeshShaderEXT
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesEXT>();
                return getCorrectAnyFeature<31>(props.meshShader);
            },
        // TaskShaderEXT
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesEXT>();
                return getCorrectAnyFeature<32>(props.taskShader);
            },
        // ShaderDrawParameters
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceVulkan11Features>();
                return getCorrectAnyFeature<33>(props.shaderDrawParameters);
            }
	};

	const std::array<PhysicalDevice::CompleteFeatureChain::CheckFunc,
		static_cast<size_t>(DeviceFeature::Num)> PhysicalDevice::CompleteFeatureChain::s_featureCheck = {
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
	};
}