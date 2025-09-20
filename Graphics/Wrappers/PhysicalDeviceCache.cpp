#include "PhysicalDeviceCache.h"

namespace Graphics
{
	PhysicalDeviceCache::PhysicalDeviceCache(const InstanceRef& instance, const InstanceFunctionTable& functions)
	{
		auto devices = instance.getPhysicalDevices(functions);
		m_data.reserve(devices.size());
		for (auto& device : devices)
			addDevice(functions, device);
	};

	void PhysicalDeviceCache::addDevice(const InstanceFunctionTable& functions, const PhysicalDevice& device)
	{
		Data data;
		device.getProperties(functions, data.m_properties);
		device.getFeatures(functions, data.m_features);
		device.getQueueFamilyProperties(functions, data.m_families);
		data.m_availableExtensions = device.getExtensions(functions);
		m_data.emplace_back(std::make_pair(device, std::move(data)));
	};

	PhysicalDeviceCache::SearchResult PhysicalDeviceCache::getFittingDevice(
		const InstanceFunctionTable& functions,
		const Surface& surface, const DeviceRequirements& requirements) const
	{
		SearchResult result;
		for (size_t i = 0; i < m_data.size(); ++i)
		{
			checkDeviceSuitability(result, functions, surface, m_data[i], requirements);
			if (result.isSuitable())
			{
				result.device = m_data[i].first;
				PhysicalDevice::CompleteFeatureChain chain;
				for (auto& feature : requirements.features)
					chain.setFeature(feature.first, feature.second);
				result.enabledFeatures = std::move(chain);
				return result;
			}
		}

		return SearchResult::unsuitable();
	}

	PhysicalDeviceCache::SearchResult PhysicalDeviceCache::getFittingDevice(
		const DeviceRequirements& requirements) const
	{
		SearchResult result;
		for (size_t i = 0; i < m_data.size(); ++i)
		{
			checkDeviceSuitability(result, requirements, m_data[i]);
			if (result.isSuitable())
			{
				result.device = m_data[i].first;
				PhysicalDevice::CompleteFeatureChain chain;
				for (auto& feature : requirements.features)
					chain.setFeature(feature.first, feature.second);
				result.enabledFeatures = std::move(chain);
				return result;
			}
		}

		return SearchResult::unsuitable();
	}

	void PhysicalDeviceCache::checkDeviceSuitability(
		SearchResult& result, const InstanceFunctionTable& functions, const Surface& surface, 
		const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data,
		const DeviceRequirements& requirements) const
	{		
		for (const auto& feature : requirements.features)
			if (!PhysicalDevice::CompleteFeatureChain::s_featureCheck[static_cast<size_t>(feature.first)]
			(feature.second, data.second.m_features.getFeature(feature.first)))
			{
				result = SearchResult::unsuitable();
				return;
			}

		for (const auto& property : requirements.properties)
			if (!PhysicalDevice::CompletePropertyChain::s_propertyCheck[static_cast<size_t>(property.first)]
			(property.second, data.second.m_properties.getProperty(property.first)))
			{
				result = SearchResult::unsuitable();
				return;
			}

		for (const auto& extension : requirements.extensions)
		{
			bool found = false;
			for (const auto& availibleExtension : data.second.m_availableExtensions)
				if (std::strcmp(extension, availibleExtension.getExtensionName().data()) == 0)
				{
					found = true;
					break;
				}
			if (!found)
			{
				result = SearchResult::unsuitable();
				return;
			}
		}

		std::vector<std::vector<uint32_t>> queueFamilyIndices;
		const auto& queueFamilies = data.second.m_families;

		for (const auto& requirement : requirements.queueProperties)
		{
			queueFamilyIndices.push_back(std::vector<uint32_t>());
			if (requirement.shouldSupportPresent)
				for (size_t i = 0; i < queueFamilies.size(); ++i)
				{
					if (checkQueueFamilySuitability(functions, surface,
						requirement.queueProperties, data, i))
						queueFamilyIndices.back().push_back(i);
				}
			else
				for (size_t i = 0; i < queueFamilies.size(); ++i)
					if (checkQueueFamilySuitability(requirement.queueProperties, data, i))
						queueFamilyIndices.back().push_back(i);
			if (queueFamilyIndices.back().size() == 0)
			{
				result = SearchResult::unsuitable();
				return;
			}
		}
		result.queueFamilyIndices = std::move(queueFamilyIndices);
		result.device = data.first;
	}

	void PhysicalDeviceCache::checkDeviceSuitability(
		SearchResult& result, const DeviceRequirements& requirements,
		const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data) const
	{
		for (const auto& feature : requirements.features)
			if (!PhysicalDevice::CompleteFeatureChain::s_featureCheck[static_cast<size_t>(feature.first)]
			(feature.second, data.second.m_features.getFeature(feature.first)))
			{
				result = SearchResult::unsuitable();
				return;
			}

		for (const auto& property : requirements.properties)
			if (!PhysicalDevice::CompletePropertyChain::s_propertyCheck[static_cast<size_t>(property.first)]
			(property.second, data.second.m_properties.getProperty(property.first)))
			{
				result = SearchResult::unsuitable();
				return;
			}

		for (const auto& extension : requirements.extensions)
		{
			bool found = false;
			for (const auto& availibleExtension : data.second.m_availableExtensions)
				if (std::strcmp(extension, availibleExtension.getExtensionName().data()) == 0)
				{
					found = true;
					break;
				}
			if (!found)
			{
				result = SearchResult::unsuitable();
				return;
			}
		}

		std::vector<std::vector<uint32_t>> queueFamilyIndices;
		const auto& queueFamilies = data.second.m_families;

		for (const auto& requirement : requirements.queueProperties)
		{
			queueFamilyIndices.push_back({});
			for (size_t i = 0; i < queueFamilies.size(); ++i)
				if (checkQueueFamilySuitability(requirement.queueProperties, data, i))
					queueFamilyIndices.back().push_back(i);
			if (queueFamilyIndices.back().size() == 0)
			{
				result = SearchResult::unsuitable();
				return;
			}
		}
		result.queueFamilyIndices = std::move(queueFamilyIndices);
	}

	bool PhysicalDeviceCache::checkQueueFamilySuitability(
		const InstanceFunctionTable& functions, const Surface& surface,
		const std::unordered_map<QueueProperty, std::any>& props,
		const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data,
		size_t familyIndex) const
	{
		if (!checkQueueFamilySuitability(props, data, familyIndex)) return false;
		if (!data.first.getSurfaceSupport(functions, surface, familyIndex))
			return false;
		return true;
	}

	bool PhysicalDeviceCache::checkQueueFamilySuitability(
		const std::unordered_map<QueueProperty, std::any>& props,
		const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data,
		size_t familyIndex) const
	{
		for (const auto& property : props)
			if (!CompleteQueueFamilyPropertyChain::s_queuePropertyCheck[static_cast<size_t>(property.first)]
			(property.second, data.second.m_families[familyIndex].getProperty(property.first)))
				return false;
		return true;
	}


}





//#include "PhysicalDeviceCache.h"
//
//namespace Graphics
//{
//	PhysicalDeviceCache::PhysicalDeviceCache(const Instance& instance, const InstanceFunctionTable& functions)
//	{
//
//		uint32_t deviceCount = 0;
//		auto result = functions.execute<InstanceFunction::EnumeratePhysicalDevices>(instance.getHandle(), &deviceCount, nullptr);
//
//		GRAPHICS_VERIFY(result == VK_SUCCESS,
//			"Failed to enumerate physical devices: " + s_resultMessages.at(result));
//
//		std::vector<PhysicalDevice> devices(deviceCount);
//
//		result = functions.execute<InstanceFunction::EnumeratePhysicalDevices>(instance.getHandle(),
//			&deviceCount, PhysicalDevice::underlyingCast(devices.data()));
//
//		GRAPHICS_VERIFY(result == VK_SUCCESS,
//			"Failed to enumerate physical devices: " + s_resultMessages.at(result));
//
//		for (const auto& device : devices)
//		{
//			addDevice(functions, device);
//		}
//	}
//
//	void PhysicalDeviceCache::addDevice(const InstanceFunctionTable& functions, const PhysicalDevice& device)
//	{
//
//	}
//
//	DeviceSearchResult PhysicalDeviceCache::getFittingDevice(const Instance& instance,
//		const Surface& surface, const DeviceRequirements& requirements) const
//	{
//		for (size_t i = 0; i < m_systemDevices.size(); ++i)
//		{
//			DeviceSearchResult result = checkDeviceSuitability(instance, surface, i, requirements);
//			if (result.isSuitable())
//				return result;
//		}
//
//		return DeviceSearchResult::unsuitable();
//	}
//
//	DeviceSearchResult PhysicalDeviceCache::getFittingDevice(const DeviceRequirements& requirements) const
//	{
//		for (size_t i = 0; i < m_systemDevices.size(); ++i)
//		{
//			DeviceSearchResult result = checkDeviceSuitability(requirements, i);
//			if (result.isSuitable())
//				return result;
//		}
//
//		return DeviceSearchResult::unsuitable();
//	}
//
//	DeviceSearchResult PhysicalDeviceCache::checkDeviceSuitability(const Instance& instance,
//		const Surface& surface, size_t deviceIndex, const DeviceRequirements& requirements) const
//	{
//		const auto& device = m_systemDevices[deviceIndex];
//		//device checks
//		for (const auto& feature : requirements.features)
//			if (!featureChecks[static_cast<size_t>(feature.first)]
//			(feature.second, device.getFeature(feature.first)))
//				return DeviceSearchResult::unsuitable();
//
//		for (const auto& property : requirements.properties)
//			if (!propertyChecks[static_cast<size_t>(property.first)]
//			(property.second, device.getProperty(property.first)))
//				return DeviceSearchResult::unsuitable();
//
//		for (const auto& extension : requirements.extensions)
//			if (device.getAvailableExtensions().find(extension) == device.getAvailableExtensions().end())
//				return DeviceSearchResult::unsuitable();
//
//		std::vector<std::vector<uint32_t>> queueFamilyIndices;
//		const auto& queueFamilies = device.getQueueFamilies();
//
//		//queue family checks
//		for (const auto& requirement : requirements.queueProperties)
//		{
//			queueFamilyIndices.push_back(std::vector<uint32_t>());
//			if (requirement.shouldSupportPresent)
//				for (const auto& queueFamily : queueFamilies)
//				{
//					if (checkQueueFamilySuitability(instance, surface, deviceIndex,
//						requirement.queueProperties, queueFamily))
//						queueFamilyIndices.back().push_back(queueFamily.getFamilyIndex());
//				}
//			else
//				for (const auto& queueFamily : queueFamilies)
//					if (checkQueueFamilySuitability(requirement.queueProperties, queueFamily))
//						queueFamilyIndices.back().push_back(queueFamily.getFamilyIndex());
//			if (queueFamilyIndices.back().size() == 0)
//				return DeviceSearchResult::unsuitable();
//		}
//		return DeviceSearchResult::suitable(deviceIndex, std::move(queueFamilyIndices));
//	}
//
//	DeviceSearchResult PhysicalDeviceCache::checkDeviceSuitability(const DeviceRequirements& requirements, size_t deviceIndex) const
//	{
//		const auto& device = m_systemDevices[deviceIndex];
//		//device checks
//		for (const auto& feature : requirements.features)
//			if (!featureChecks[static_cast<size_t>(feature.first)]
//			(feature.second, device.getFeature(feature.first)))
//				return DeviceSearchResult::unsuitable();
//
//		for (const auto& property : requirements.properties)
//			if (!propertyChecks[static_cast<size_t>(property.first)]
//			(property.second, device.getProperty(property.first)))
//				return DeviceSearchResult::unsuitable();
//
//		for (const auto& extension : requirements.extensions)
//			if (device.getAvailableExtensions().find(extension) == device.getAvailableExtensions().end())
//				return DeviceSearchResult::unsuitable();
//
//		std::vector<std::vector<uint32_t>> queueFamilyIndices;
//
//		//queue family checks
//		for (const auto& requirement : requirements.queueProperties)
//		{
//			queueFamilyIndices.push_back({});
//			for (const auto& queueFamily : device.getQueueFamilies())
//			{
//				if (checkQueueFamilySuitability(requirement.queueProperties, queueFamily))
//					queueFamilyIndices.back().push_back(queueFamily.getFamilyIndex());
//			}
//			if (queueFamilyIndices.back().size() == 0)
//				return DeviceSearchResult::unsuitable();
//		}
//		return DeviceSearchResult::suitable(deviceIndex, std::move(queueFamilyIndices));
//	}
//
//	bool PhysicalDeviceCache::checkQueueFamilySuitability(const Instance& instance,
//		const Surface& surface, size_t deviceIndex,
//		const std::map<QueueProperty, std::any>& requirements,
//		const QueueFamily& queueFamily) const
//	{
//		if (!checkQueueFamilySuitability(requirements, queueFamily)) return false;
//		if (!queueFamily.getSurfaceSupport(surface.getHandle(), m_systemDevices[deviceIndex].getHandle(), instance.getFunctionTable()))
//			return false;
//		return true;
//	}
//
//	bool PhysicalDeviceCache::checkQueueFamilySuitability(const std::map<QueueProperty,
//		std::any>& requirements, const QueueFamily& queueFamily) const
//	{
//		for (const auto& property : requirements)
//			if (!queuePropertyChecks[static_cast<size_t>(property.first)]
//			(property.second, queueFamily.getProperty(property.first)))
//				return false;
//		return true;
//	}
//}