#pragma once
#include "../Common.h"
#include "../Wrappers.h"
#include "QueueFamily.h"

#include <array>
#include <set>

namespace Graphics {

	class PhysicalDevice
	{
	private:
		Wrappers::PhysicalDevice m_physicalDevice;

		std::array<std::any, static_cast<size_t>(DeviceFeature::FeaturesNum)> m_features;
		std::array<std::any, static_cast<size_t>(DeviceProperty::PropertiesNum)> m_properties;
		std::vector<QueueFamily> m_queueFamilies;
		std::set<std::string> m_availableExtensions;

	public:

		PhysicalDevice() = default;

		PhysicalDevice(const PhysicalDevice&) = delete;
		PhysicalDevice(PhysicalDevice&&) = default;
		PhysicalDevice& operator=(const PhysicalDevice&) = delete;
		PhysicalDevice& operator=(PhysicalDevice&&) = default;

		PhysicalDevice(const InstanceFunctionTable& functions, Wrappers::PhysicalDevice device) :
			m_physicalDevice(std::move(device))
		{
			enumerateFeatures(functions);
			enumerateProperties(functions);
			enumerateExtensions(functions);
			enumerateQueueFamilies(functions);
		}

		const std::any& getFeature(DeviceFeature feature) const {
			return m_features[static_cast<size_t>(feature)];
		}

		const std::any& getProperty(DeviceProperty property) const {
			return m_properties[static_cast<size_t>(property)];
		}

		template<DeviceFeature F>
		const typename DeviceFeatureTypeTrait<F>::Type& getFeature() const {
			return std::any_cast<const typename DeviceFeatureTypeTrait<F>::Type&>(
				m_features[static_cast<size_t>(F)]);
		}

		template<DeviceProperty P>
		const typename DevicePropertyTypeTrait<P>::Type& getProperty() const {
			return std::any_cast<const typename DevicePropertyTypeTrait<P>::Type&>(
				m_properties[static_cast<size_t>(P)]);
		}

		const std::vector<QueueFamily>& getQueueFamilies() const { return m_queueFamilies; };
		const std::set<std::string>& getAvailableExtensions() const { return m_availableExtensions; };
		const Wrappers::PhysicalDevice& getHandle() const { return m_physicalDevice; };

	private:

		void enumerateFeatures(const InstanceFunctionTable& functions);
		void enumerateProperties(const InstanceFunctionTable& functions);
		void enumerateExtensions(const InstanceFunctionTable& functions);
		void enumerateQueueFamilies(const InstanceFunctionTable& functions);

		template<typename T>
		void storeFeature(DeviceFeature feature, const T& value) {
			m_features[static_cast<size_t>(feature)] = value;
		}

		template<typename T>
		void storeProperty(DeviceProperty property, const T& value) {
			m_properties[static_cast<size_t>(property)] = value;
		}
	};

}