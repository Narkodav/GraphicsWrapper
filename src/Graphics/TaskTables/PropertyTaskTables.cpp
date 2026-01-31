#include "Graphics/HandleTypes/PhysicalDevice.h"

namespace Graphics {

    template<size_t index, typename T>
    std::any getCorrectAnyProperty(const T& data) {
        return std::any(static_cast<typename DevicePropertyTypeTrait<static_cast<DeviceProperty>(index)>::Type>(data));
    }

	const std::array<PhysicalDevice::CompletePropertyChain::PropertySetFunc,
		static_cast<size_t>(DeviceProperty::Num)> PhysicalDevice::CompletePropertyChain::s_propertySet = {
        // ApiVersion
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.apiVersion = std::any_cast<uint32_t>(data);
        },
        // DriverVersion
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.driverVersion = std::any_cast<uint32_t>(data);
        },
        // VendorId
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.vendorID = std::any_cast<uint32_t>(data);
        },
        // PhysicalDeviceId
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.deviceID = std::any_cast<uint32_t>(data);
        },
        // PhysicalDeviceType
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.deviceType = static_cast<VkPhysicalDeviceType>(
                std::any_cast<PhysicalDeviceType>(data));
        },
        // PhysicalDeviceName
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            auto name = std::any_cast<std::string_view>(data);
            std::copy(name.begin(), name.end(), props.properties.deviceName);
        },
        // MaxImageDimension2d
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxImageDimension2D = std::any_cast<uint32_t>(data);
        },
        // MaxImageDimension3d
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxImageDimension3D = std::any_cast<uint32_t>(data);
        },
        // MaxImageDimensionCube
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxImageDimensionCube = std::any_cast<uint32_t>(data);
        },
        // MaxImageArrayLayers
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxImageArrayLayers = std::any_cast<uint32_t>(data);
        },
        // MaxBoundDescriptorSets
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxBoundDescriptorSets = std::any_cast<uint32_t>(data);
        },
        // MaxPerStageDescriptorSamplers
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxPerStageDescriptorSamplers = std::any_cast<uint32_t>(data);
        },
        // MaxPerStageDescriptorUniformBuffers
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxPerStageDescriptorUniformBuffers = std::any_cast<uint32_t>(data);
        },
        // MaxPerStageDescriptorStorageBuffers
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxPerStageDescriptorStorageBuffers = std::any_cast<uint32_t>(data);
        },
        // MaxStorageBufferRange
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxStorageBufferRange = std::any_cast<uint32_t>(data);
        },
        // MaxUniformBufferRange
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxUniformBufferRange = std::any_cast<uint32_t>(data);
        },
        // MinUniformBufferOffsetAlignment
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.minUniformBufferOffsetAlignment = std::any_cast<DeviceSize>(data);
        },
        // MinStorageBufferOffsetAlignment
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.minStorageBufferOffsetAlignment = std::any_cast<DeviceSize>(data);
        },
        // MaxViewportDimensions
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            auto dims = std::any_cast<std::span<uint32_t, 2>>(data);
            props.properties.limits.maxViewportDimensions[0] = dims[0];
            props.properties.limits.maxViewportDimensions[1] = dims[1];
        },
        // MaxViewports
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxViewports = std::any_cast<uint32_t>(data);
        },
        // ViewportBoundsRange
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            auto range = std::any_cast<std::span<float, 2>>(data);
            props.properties.limits.viewportBoundsRange[0] = range[0];
            props.properties.limits.viewportBoundsRange[1] = range[1];
        },
        // ViewportSubPixelBits
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.viewportSubPixelBits = std::any_cast<uint32_t>(data);
        },
        // MaxFramebufferLayers
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxFramebufferLayers = std::any_cast<uint32_t>(data);
        },
        // MaxSampleMaskWords
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxSampleMaskWords = std::any_cast<uint32_t>(data);
        },
        // MaxColorAttachments
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxColorAttachments = std::any_cast<uint32_t>(data);
        },
        // MaxMemoryAllocationCount
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxMemoryAllocationCount = std::any_cast<uint32_t>(data);
        },
        // MaxSamplerAllocationCount
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxSamplerAllocationCount = std::any_cast<uint32_t>(data);
        },
        // BufferImageGranularity
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.bufferImageGranularity = std::any_cast<DeviceSize>(data);
        },
        // SparseAddressSpaceSize
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.sparseAddressSpaceSize = std::any_cast<DeviceSize>(data);
        },
        // MaxSamplerAnisotropy
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxSamplerAnisotropy = std::any_cast<float>(data);
        },
        // MaxSamplerLodBias
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.maxSamplerLodBias = std::any_cast<float>(data);
        },
        // TimestampPeriod
        [](PhysicalDevice::CompletePropertyChain& chain, const std::any& data) {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            props.properties.limits.timestampPeriod = std::any_cast<float>(data);
        }

	};

	const std::array<PhysicalDevice::CompletePropertyChain::GetPropertyFunc,
		static_cast<size_t>(DeviceProperty::Num)> PhysicalDevice::CompletePropertyChain::s_propertyGet = {
        //ApiVersion
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();            
            return getCorrectAnyProperty<0>(props.properties.apiVersion);
        },
        // DriverVersion
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<1>(props.properties.driverVersion);
        },
        // VendorId
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<2>(props.properties.vendorID);
        },
        // PhysicalDeviceId
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<3>(props.properties.deviceID);
        },
        // PhysicalDeviceType
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<4>(props.properties.deviceType);
        },
        // PhysicalDeviceName
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<5>(props.properties.deviceName);
        },
        // MaxImageDimension2d
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<6>(props.properties.limits.maxImageDimension2D);
        },
        // MaxImageDimension3d
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<7>(props.properties.limits.maxImageDimension3D);
        },
        // MaxImageDimensionCube
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<8>(props.properties.limits.maxImageDimensionCube);
        },
        // MaxImageArrayLayers
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<9>(props.properties.limits.maxImageArrayLayers);
        },
        // MaxBoundDescriptorSets
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<10>(props.properties.limits.maxBoundDescriptorSets);
        },
        // MaxPerStageDescriptorSamplers
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<11>(props.properties.limits.maxPerStageDescriptorSamplers);
        },
        // MaxPerStageDescriptorUniformBuffers
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<12>(props.properties.limits.maxPerStageDescriptorUniformBuffers);
        },
        // MaxPerStageDescriptorStorageBuffers
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<13>(props.properties.limits.maxPerStageDescriptorStorageBuffers);
        },
        // MaxStorageBufferRange
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<14>(props.properties.limits.maxStorageBufferRange);
        },
        // MaxUniformBufferRange
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<15>(props.properties.limits.maxUniformBufferRange);
        },
        // MinUniformBufferOffsetAlignment
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<16>(props.properties.limits.minUniformBufferOffsetAlignment);
        },
        // MinStorageBufferOffsetAlignment
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<17>(props.properties.limits.minStorageBufferOffsetAlignment);
        },
        // MaxViewportDimensions
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return std::array<uint32_t, 2>{ props.properties.limits.maxViewportDimensions[0],
                props.properties.limits.maxViewportDimensions[1] };
        },
        // MaxViewports
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<19>(props.properties.limits.maxViewports);
        },
        // ViewportBoundsRange
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return std::array<float, 2>{ props.properties.limits.viewportBoundsRange[0],
                props.properties.limits.viewportBoundsRange[1] };
        },
        // ViewportSubPixelBits
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<21>(props.properties.limits.viewportSubPixelBits);
        },
        // MaxFramebufferLayers
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<22>(props.properties.limits.maxFramebufferLayers);
        },
        // MaxSampleMaskWords
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<23>(props.properties.limits.maxSampleMaskWords);
        },
        // MaxColorAttachments
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<24>(props.properties.limits.maxColorAttachments);
        },
        // MaxMemoryAllocationCount
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<25>(props.properties.limits.maxMemoryAllocationCount);
        },
        // MaxSamplerAllocationCount
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<26>(props.properties.limits.maxSamplerAllocationCount);
        },
        // BufferImageGranularity
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<27>(props.properties.limits.bufferImageGranularity);
        },
        // SparseAddressSpaceSize
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<28>(props.properties.limits.sparseAddressSpaceSize);
        },
        // MaxSamplerAnisotropy
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<29>(props.properties.limits.maxSamplerAnisotropy);
        },
        // MaxSamplerLodBias
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<30>(props.properties.limits.maxSamplerLodBias);
        },
        // TimestampPeriod
        [](const PhysicalDevice::CompletePropertyChain& chain) -> std::any {
            auto& props = chain.get<StructureType::PhysicalDeviceProperties2>();
            return getCorrectAnyProperty<31>(props.properties.limits.timestampPeriod);
        }
	};

	const std::array<PhysicalDevice::CompletePropertyChain::CheckFunc,
		static_cast<size_t>(DeviceProperty::Num)> PhysicalDevice::CompletePropertyChain::s_propertyCheck = {
            [](const std::any& required, const std::any& available) //ApiVersion
        { return std::any_cast<const uint32_t&>(required) ==
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //DriverVersion
        { return std::any_cast<const uint32_t&>(required) ==
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //VendorId
        { return std::any_cast<const uint32_t&>(required) ==
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //PhysicalDeviceId
        { return std::any_cast<const uint32_t&>(required) ==
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //PhysicalDeviceType
        { return std::any_cast<const PhysicalDeviceType&>(required) ==
        std::any_cast<const PhysicalDeviceType&>(available); },
            [](const std::any& required, const std::any& available) //PhysicalDeviceName
        { return std::any_cast<const std::string_view&>(required) ==
        std::any_cast<const std::string_view&>(available); },

            [](const std::any& required, const std::any& available) //MaxImageDimension2d
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxImageDimension3d
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxImageDimensionCube
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxImageArrayLayers
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },

            [](const std::any& required, const std::any& available) //MaxBoundDescriptorSets
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxPerStageDescriptorSamplers
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxPerStageDescriptorUniformBuffers
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxPerStageDescriptorStorageBuffers
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },

            [](const std::any& required, const std::any& available) //MaxStorageBufferRange
        { return std::any_cast<const DeviceSize&>(required) <=
        std::any_cast<const DeviceSize&>(available); },
            [](const std::any& required, const std::any& available) //MaxUniformBufferRange
        { return std::any_cast<const DeviceSize&>(required) <=
        std::any_cast<const DeviceSize&>(available); },
            [](const std::any& required, const std::any& available) //MinUniformBufferOffsetAlignment
        { return std::any_cast<const DeviceSize&>(required) <=
        std::any_cast<const DeviceSize&>(available); },
            [](const std::any& required, const std::any& available) //MinStorageBufferOffsetAlignment
        { return std::any_cast<const DeviceSize&>(required) <=
        std::any_cast<const DeviceSize&>(available); },

            [](const std::any& required, const std::any& available) //MaxViewportDimensions
        {   const auto& requiredDims = std::any_cast<const std::span<uint32_t, 2>&>(required);
            const auto& availableDims = std::any_cast<const std::span<uint32_t, 2>&>(available);
            return requiredDims[0] <= availableDims[0] && requiredDims[1] <= availableDims[1]; },
            [](const std::any& required, const std::any& available) //MaxViewports
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //ViewportBoundsRange
        {   const auto& requiredRange = std::any_cast<const std::span<float, 2>&>(required);
            const auto& availableRange = std::any_cast<const std::span<float, 2>&>(available);
            return requiredRange[0] <= availableRange[0] && requiredRange[1] <= availableRange[1]; },
            [](const std::any& required, const std::any& available) //ViewportSubPixelBits
        { return std::any_cast<const uint32_t&>(required) ==
        std::any_cast<const uint32_t&>(available); },

            [](const std::any& required, const std::any& available) //MaxFramebufferLayers
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxSampleMaskWords
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxColorAttachments
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },

            [](const std::any& required, const std::any& available) //MaxMemoryAllocationCount
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //MaxSamplerAllocationCount
        { return std::any_cast<const uint32_t&>(required) <=
        std::any_cast<const uint32_t&>(available); },
            [](const std::any& required, const std::any& available) //BufferImageGranularity
        { return std::any_cast<const DeviceSize&>(required) <=
        std::any_cast<const DeviceSize&>(available); },
            [](const std::any& required, const std::any& available) //SparseAddressSpaceSize
        { return std::any_cast<const DeviceSize&>(required) <=
        std::any_cast<const DeviceSize&>(available); },

            [](const std::any& required, const std::any& available) //MaxSamplerAnisotropy
        { return std::any_cast<const float&>(required) <=
        std::any_cast<const float&>(available); },
            [](const std::any& required, const std::any& available) //MaxSamplerLodBias
        { return std::any_cast<const float&>(required) <=
        std::any_cast<const float&>(available); },
            [](const std::any& required, const std::any& available) //TimestampPeriod
        { return std::any_cast<const float&>(required) ==
        std::any_cast<const float&>(available); },
	};

}