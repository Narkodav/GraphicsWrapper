#include "Semaphore.h"

namespace Graphics {

	Semaphore::Semaphore(const Device& device)
	{
		vk::SemaphoreCreateInfo semaphoreInfo;
		auto result = device.getFunctionTable().execute<DeviceFunction::CreateSemaphore>(
			device.getHandle(), &semaphoreInfo, nullptr, &m_semaphore);
		if (result != VK_SUCCESS) throw std::runtime_error("Failed to create semaphore: " +
			s_resultMessages.at(result));
	}

}