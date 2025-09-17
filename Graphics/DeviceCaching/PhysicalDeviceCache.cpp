#include "PhysicalDeviceCache.h"

namespace Graphics {

	PhysicalDeviceCache::PhysicalDeviceCache(const Instance& instance)
	{
		
		uint32_t physicalDeviceCount = 0;
		auto result = instance.getFunctionTable().execute<InstanceFunction::EnumeratePhysicalDevices>(instance.getHandle(), &physicalDeviceCount, nullptr);

		if (result != VK_SUCCESS) throw std::runtime_error("Failed to get enumerate physical devices: " +
			s_resultMessages.at(result));

		std::vector<VkPhysicalDevice> devices(physicalDeviceCount);

		result = instance.getFunctionTable().execute<InstanceFunction::EnumeratePhysicalDevices>(instance.getHandle(), &physicalDeviceCount, devices.data());

		if (result != VK_SUCCESS) throw std::runtime_error("Failed to get enumerate physical devices: " +
			s_resultMessages.at(result));

		if (devices.size() == 0)
			throw std::runtime_error("Failed to find available Devices");

		for (const auto& physicalDevice : devices)
		{
			PhysicalDevice device(instance.getFunctionTable(), physicalDevice);
			m_systemDevices.push_back(std::move(device));
		}
	}

	DeviceSearchResult PhysicalDeviceCache::getFittingDevice(const Instance& instance, 
	const Surface& surface, const DeviceRequirements& requirements) const
	{
		for (size_t i = 0; i < m_systemDevices.size(); ++i)
		{
			DeviceSearchResult result = checkDeviceSuitability(instance, surface, i, requirements);
			if (result.isSuitable())
				return result;
		}

		return DeviceSearchResult::unsuitable();
	}

	DeviceSearchResult PhysicalDeviceCache::getFittingDevice(const DeviceRequirements& requirements) const
	{
		for (size_t i = 0; i < m_systemDevices.size(); ++i)
		{
			DeviceSearchResult result = checkDeviceSuitability(requirements, i);
			if (result.isSuitable())
				return result;
		}

		return DeviceSearchResult::unsuitable();
	}

	DeviceSearchResult PhysicalDeviceCache::checkDeviceSuitability(const Instance& instance,
		const Surface& surface, size_t deviceIndex, const DeviceRequirements& requirements) const
	{
		const auto& device = m_systemDevices[deviceIndex];
		//device checks
		for (const auto& feature : requirements.features)
			if (!featureChecks[static_cast<size_t>(feature.first)]
			(feature.second, device.getFeature(feature.first)))
				return DeviceSearchResult::unsuitable();

		for (const auto& property : requirements.properties)
			if (!propertyChecks[static_cast<size_t>(property.first)]
			(property.second, device.getProperty(property.first)))
				return DeviceSearchResult::unsuitable();

		for (const auto& extension : requirements.extensions)
			if (device.getAvailableExtensions().find(extension) == device.getAvailableExtensions().end())
				return DeviceSearchResult::unsuitable();

		std::vector<std::vector<uint32_t>> queueFamilyIndices;
		const auto& queueFamilies = device.getQueueFamilies();

		//queue family checks
		for (const auto& requirement : requirements.queueProperties)
		{
			queueFamilyIndices.push_back(std::vector<uint32_t>());
			if (requirement.shouldSupportPresent)
				for (const auto& queueFamily : queueFamilies)
				{
					if (checkQueueFamilySuitability(instance, surface, deviceIndex,
						requirement.queueProperties, queueFamily))
						queueFamilyIndices.back().push_back(queueFamily.getFamilyIndex());
				}
			else
				for (const auto& queueFamily : queueFamilies)
					if (checkQueueFamilySuitability(requirement.queueProperties, queueFamily))
						queueFamilyIndices.back().push_back(queueFamily.getFamilyIndex());
			if (queueFamilyIndices.back().size() == 0)
				return DeviceSearchResult::unsuitable();
		}
		return DeviceSearchResult::suitable(deviceIndex, std::move(queueFamilyIndices));
	}

	DeviceSearchResult PhysicalDeviceCache::checkDeviceSuitability(const DeviceRequirements& requirements, size_t deviceIndex) const
	{
		const auto& device = m_systemDevices[deviceIndex];
		//device checks
		for (const auto& feature : requirements.features)
			if (!featureChecks[static_cast<size_t>(feature.first)]
			(feature.second, device.getFeature(feature.first)))
				return DeviceSearchResult::unsuitable();

		for (const auto& property : requirements.properties)
			if (!propertyChecks[static_cast<size_t>(property.first)]
			(property.second, device.getProperty(property.first)))
				return DeviceSearchResult::unsuitable();

		for (const auto& extension : requirements.extensions)
			if (device.getAvailableExtensions().find(extension) == device.getAvailableExtensions().end())
				return DeviceSearchResult::unsuitable();

		std::vector<std::vector<uint32_t>> queueFamilyIndices;

		//queue family checks
		for (const auto& requirement : requirements.queueProperties)
		{
			queueFamilyIndices.push_back({});
			for (const auto& queueFamily : device.getQueueFamilies())
			{
				if (checkQueueFamilySuitability(requirement.queueProperties, queueFamily))
					queueFamilyIndices.back().push_back(queueFamily.getFamilyIndex());
			}
			if (queueFamilyIndices.back().size() == 0)
				return DeviceSearchResult::unsuitable();
		}
		return DeviceSearchResult::suitable(deviceIndex, std::move(queueFamilyIndices));
	}

	bool PhysicalDeviceCache::checkQueueFamilySuitability(const Instance& instance,
		const Surface& surface, size_t deviceIndex,
		const std::map<QueueProperty, std::any>& requirements,
		const QueueFamily& queueFamily) const
	{
		if (!checkQueueFamilySuitability(requirements, queueFamily)) return false;
		if (!queueFamily.getSurfaceSupport(surface.getHandle(), m_systemDevices[deviceIndex].getHandle(), instance.getFunctionTable()))
			return false;
		return true;
	}

	bool PhysicalDeviceCache::checkQueueFamilySuitability(const std::map<QueueProperty,
		std::any>& requirements, const QueueFamily& queueFamily) const
	{
		for (const auto& property : requirements)
			if (!queuePropertyChecks[static_cast<size_t>(property.first)]
			(property.second, queueFamily.getProperty(property.first)))
				return false;
		return true;
	}
}