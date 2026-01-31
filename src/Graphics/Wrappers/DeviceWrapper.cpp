#include "Graphics/Wrappers/DeviceWrapper.h"

namespace Graphics::Wrappers {

	void Device::create(const Instance& instance, const PhysicalDevice& physicalDevice, const DeviceCreateInfo& createInfo) {
		m_physicalDevice = physicalDevice;
		m_device.create(instance.getFunctionTable(), m_physicalDevice, createInfo);
		m_deviceFunctions = m_device.getFunctionTable(instance.getFunctionTable());
	}

	void Device::waitIdle() {
		m_device.waitIdle(m_deviceFunctions);
	}

	Queue Device::getQueue(uint32_t familyIndex, uint32_t queueIndex) {
		return m_device.getQueue(familyIndex, queueIndex, m_deviceFunctions);
	}
}