#pragma once
#include "../Common.h"

#include "PhysicalDevice.h"
#include "../Rendering/Surface.h"

#include <array>
#include <set>
#include <map>
#include <any>
#include <functional>

namespace Graphics {	
		
	struct DeviceSearchResult
	{
		size_t deviceIndex = std::numeric_limits<size_t>::max();
		std::vector<std::vector<uint32_t>> queueFamilyIndices;

		inline bool isSuitable() { return deviceIndex != std::numeric_limits<size_t>::max(); };

		static DeviceSearchResult unsuitable() { return DeviceSearchResult(); };
		static DeviceSearchResult suitable(size_t deviceIndex, std::vector<std::vector<uint32_t>>&& queueFamilyIndices)
		{
			return DeviceSearchResult{ deviceIndex , std::move(queueFamilyIndices) };
		};
	};

	class PhysicalDeviceCache
	{		
	private:
		std::vector<PhysicalDevice> m_systemDevices;

	public:
		PhysicalDeviceCache() = default;

		PhysicalDeviceCache(const PhysicalDeviceCache&) = delete;
		PhysicalDeviceCache& operator=(const PhysicalDeviceCache&) = delete;

		PhysicalDeviceCache(PhysicalDeviceCache&&) = default;
		PhysicalDeviceCache& operator=(PhysicalDeviceCache&&) = default;

		PhysicalDeviceCache(const Instance& instance);

		//this version will check for queue present support if required
		DeviceSearchResult getFittingDevice(const Instance& instance,
			const Surface& surface, const DeviceRequirements& requirements) const;

		//this version will not check for queue present support
		DeviceSearchResult getFittingDevice(const DeviceRequirements& requirements) const;

		const std::vector<PhysicalDevice>& getCachedDevices() const { return m_systemDevices; };

		//this version will check for queue present support if required
		DeviceSearchResult checkDeviceSuitability(const Instance& instance,
			const Surface& surface, size_t deviceIndex, const DeviceRequirements& requirements) const;

		//this version will not check for queue present support
		DeviceSearchResult checkDeviceSuitability(const DeviceRequirements& requirements, size_t deviceIndex) const;

		//this version will check for queue present support
		bool checkQueueFamilySuitability(const Instance& instance,
			const Surface& surface, size_t deviceIndex,
			const std::map<QueueProperty, std::any>& requirements,
			const QueueFamily& queueFamily) const;

		//this version will not check for queue present support
		bool checkQueueFamilySuitability(const std::map<QueueProperty,
			std::any>& requirements, const QueueFamily& queueFamily) const;
	};

}