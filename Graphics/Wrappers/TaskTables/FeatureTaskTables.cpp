#include "../PhysicalDeviceCache.h"

namespace Graphics {

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
            auto& props = chain.get<StructureType::PhysicalDeviceShaderAtomicInt64Features>();
            props.shaderBufferInt64Atomics = std::any_cast<bool>(data);
        },
    // ShaderSharedInt64Atomics
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceShaderAtomicInt64Features>();
            props.shaderSharedInt64Atomics = std::any_cast<bool>(data);
        },
    // ShaderInt8
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceShaderFloat16Int8Features>();
            props.shaderInt8 = std::any_cast<bool>(data);
        },
    // DescriptorBindingPartiallyBound
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingPartiallyBound = std::any_cast<bool>(data);
        },
    // DescriptorBindingUpdateUnusedWhilePending
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingUpdateUnusedWhilePending = std::any_cast<bool>(data);
        },
    // DescriptorBindingUniformBufferUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingUniformBufferUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorBindingSampledImageUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingSampledImageUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorBindingStorageImageUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingStorageImageUpdateAfterBind = std::any_cast<bool>(data);
        },
    // DescriptorBindingVariableDescriptorCount
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingVariableDescriptorCount = std::any_cast<bool>(data);
        },
    // ShaderSampledImageArrayNonUniformIndexing
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.shaderSampledImageArrayNonUniformIndexing = std::any_cast<bool>(data);
        },
    // RuntimeDescriptorArray
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.runtimeDescriptorArray = std::any_cast<bool>(data);
        },
    // DescriptorBindingStorageBufferUpdateAfterBind
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
            props.descriptorBindingStorageBufferUpdateAfterBind = std::any_cast<bool>(data);
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
            auto& props = chain.get<StructureType::PhysicalDeviceImageRobustnessFeatures>();
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
    // MeshShaderNV
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesNV>();
            props.meshShader = std::any_cast<bool>(data);
        },
    // TaskShaderNV
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesNV>();
            props.taskShader = std::any_cast<bool>(data);
        },
    // ShaderDrawParameters
        [](PhysicalDevice::CompleteFeatureChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceShaderDrawParametersFeatures>();
            props.shaderDrawParameters = std::any_cast<bool>(data);
        }
	};

	const std::array<PhysicalDevice::CompleteFeatureChain::GetFeatureFunc,
		static_cast<size_t>(DeviceFeature::Num)> PhysicalDevice::CompleteFeatureChain::s_featureGet = {
        // GeometryShader
        [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
            return std::any(props.features.geometryShader);
        },
        // TessellationShader
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.tessellationShader);
            },
        // ShaderFloat64
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.shaderFloat64);
            },
        // SamplerAnisotropy
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.samplerAnisotropy);
            },
        // FillModeNonSolid
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.fillModeNonSolid);
            },
        // TextureCompressionBc
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.textureCompressionBC);
            },
        // TextureCompressionEtc2
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.textureCompressionETC2);
            },
        // MultiDrawIndirect
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.multiDrawIndirect);
            },
        // WideLines
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.wideLines);
            },
        // LargePoints
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.largePoints);
            },
        // MultiViewport
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.multiViewport);
            },
        // DepthClamp
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.depthClamp);
            },
        // DepthBiasClamp
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.depthBiasClamp);
            },
        // DualSrcBlend
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.dualSrcBlend);
            },
        // VertexPipelineStoresAndAtomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.vertexPipelineStoresAndAtomics);
            },
        // FragmentStoresAndAtomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.fragmentStoresAndAtomics);
            },
        // ShaderBufferInt64Atomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceShaderAtomicInt64Features>();
                return std::any(props.shaderBufferInt64Atomics);
            },
        // ShaderSharedInt64Atomics
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceShaderAtomicInt64Features>();
                return std::any(props.shaderSharedInt64Atomics);
            },
        // ShaderInt8
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceShaderFloat16Int8Features>();
                return std::any(props.shaderInt8);
            },
        // DescriptorBindingPartiallyBound
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingPartiallyBound);
            },
        // DescriptorBindingUpdateUnusedWhilePending
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingUpdateUnusedWhilePending);
            },
        // DescriptorBindingUniformBufferUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingUniformBufferUpdateAfterBind);
            },
        // DescriptorBindingSampledImageUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingSampledImageUpdateAfterBind);
            },
        // DescriptorBindingStorageImageUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingStorageImageUpdateAfterBind);
            },
        // DescriptorBindingVariableDescriptorCount
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingVariableDescriptorCount);
            },
        // ShaderSampledImageArrayNonUniformIndexing
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.shaderSampledImageArrayNonUniformIndexing);
            },
        // RuntimeDescriptorArray
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.runtimeDescriptorArray);
            },
        // DescriptorBindingStorageBufferUpdateAfterBind
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceDescriptorIndexingFeatures>();
                return std::any(props.descriptorBindingStorageBufferUpdateAfterBind);
            },
        // NullDescriptor
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceRobustness2FeaturesEXT>();
                return std::any(props.nullDescriptor);
            },
        // RobustBufferAccess
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceFeatures2>();
                return std::any(props.features.robustBufferAccess);
            },
        // RobustImageAccess
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceImageRobustnessFeatures>();
                return std::any(props.robustImageAccess);
            },
        // MeshShaderEXT - placeholder
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesEXT>();
                return std::any(props.meshShader);
            },
        // TaskShaderEXT - placeholder
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesEXT>();
                return std::any(props.taskShader);
            },
        // MeshShaderNV - placeholder
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesNV>();
                return std::any(props.meshShader);
            },
        // TaskShaderNV - placeholder
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceMeshShaderFeaturesNV>();
                return std::any(props.taskShader);
            },
        // ShaderDrawParameters
            [](const PhysicalDevice::CompleteFeatureChain& chain) -> std::any {
                auto& props = chain.get<StructureType::PhysicalDeviceShaderDrawParametersFeatures>();
                return std::any(props.shaderDrawParameters);
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
            [](const std::any& required, const std::any& available)
        { return std::any_cast<bool>(required) == std::any_cast<bool>(available); },
	};
}