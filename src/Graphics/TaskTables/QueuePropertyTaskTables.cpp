#include "Graphics/HandleTypes/PhysicalDevice.h"

namespace Graphics {

	template<size_t index, typename T>
	std::any getCorrectAnyQueueFamilyProperty(const T& data) {
		return std::any(static_cast<typename QueuePropertyTypeTrait<static_cast<QueueProperty>(index)>::Type>(data));
	}

	const std::array<CompleteQueueFamilyPropertyChain::QueueFamilyPropertySetFunc,
		static_cast<size_t>(QueueProperty::Num)> CompleteQueueFamilyPropertyChain::s_queuePropertySet = {
		//QueueFlags
		[](CompleteQueueFamilyPropertyChain& chain, const std::any& data) {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			props.queueFamilyProperties.queueFlags = std::any_cast<Flags::Queue>(data);
		},
		//QueueCount
		[](CompleteQueueFamilyPropertyChain& chain, const std::any& data) {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			props.queueFamilyProperties.queueCount = std::any_cast<uint32_t>(data);
		},
		//TimestampValidBits
		[](CompleteQueueFamilyPropertyChain& chain, const std::any& data) {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			props.queueFamilyProperties.timestampValidBits = std::any_cast<uint32_t>(data);
		},
		//MinImageTransferGranularity
		[](CompleteQueueFamilyPropertyChain& chain, const std::any& data) {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			props.queueFamilyProperties.minImageTransferGranularity =
				std::any_cast<Extent3D>(data);
		},
	};

	const std::array<CompleteQueueFamilyPropertyChain::GetQueueFamilyPropertyFunc,
		static_cast<size_t>(QueueProperty::Num)> CompleteQueueFamilyPropertyChain::s_queuePropertyGet = {
		//QueueFlags
		[](const CompleteQueueFamilyPropertyChain& chain) -> std::any {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			return getCorrectAnyQueueFamilyProperty<0>(props.queueFamilyProperties.queueFlags);
		},
		//QueueCount
		[](const CompleteQueueFamilyPropertyChain& chain) -> std::any {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			return getCorrectAnyQueueFamilyProperty<1>(props.queueFamilyProperties.queueCount);
		},
		//TimestampValidBits
		[](const CompleteQueueFamilyPropertyChain& chain) -> std::any {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			return getCorrectAnyQueueFamilyProperty<2>(props.queueFamilyProperties.timestampValidBits);
		},
		//MinImageTransferGranularity
		[](const CompleteQueueFamilyPropertyChain& chain) -> std::any {
			auto& props = chain.get<StructureType::QueueFamilyProperties2>();
			return getCorrectAnyQueueFamilyProperty<3>(props.queueFamilyProperties.minImageTransferGranularity);
		},
	};

	const std::array<CompleteQueueFamilyPropertyChain::CheckFunc,
		static_cast<size_t>(QueueProperty::Num)> CompleteQueueFamilyPropertyChain::s_queuePropertyCheck = {
			[](const std::any& required, const std::any& available)
			{
				return std::any_cast<Flags::Queue>(available)
				.hasFlags(std::any_cast<Flags::Queue>(required)); },

			[](const std::any& required, const std::any& available)
			{ return std::any_cast<const uint32_t&>(required) <=
			std::any_cast<const uint32_t&>(available); },

			[](const std::any& required, const std::any& available)
			{ return std::any_cast<const uint32_t&>(required) ==
			std::any_cast<const uint32_t&>(available); },

			[](const std::any& required, const std::any& available)
			{
				const std::span<uint32_t, 3>& requiredGranularity =
					std::any_cast<const std::span<uint32_t, 3>&>(required);
				const std::span<uint32_t, 3>& availableGranularity =
					std::any_cast<const std::span<uint32_t, 3>&>(available);
				return requiredGranularity[0] == availableGranularity[0] &&
					requiredGranularity[1] == availableGranularity[1] &&
					requiredGranularity[2] == availableGranularity[2]; },
	};

}