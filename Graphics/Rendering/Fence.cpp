#include "Fence.h"

namespace Graphics {

	Fence::Fence(const Device& device, bool createSignaled/* = false*/)
	{
		vk::FenceCreateInfo fenceInfo{};
		fenceInfo.sType = vk::StructureType::eFenceCreateInfo;
		if (createSignaled)
			fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		auto result = device.getFunctionTable().execute<DeviceFunction::CreateSemaphore>(
			device.getHandle(), &fenceInfo, nullptr, &m_fence);
		if (result != VK_SUCCESS) throw std::runtime_error("Failed to create fence: " +
			s_resultMessages.at(result));
	}

	void Fence::wait(const Device& device, size_t timeout /*= std::numeric_limits<size_t>::max()*/)
	{
		auto result = device.getFunctionTable().execute<DeviceFunction::WaitForFences>(
			device.getHandle(), 1, &m_fence, VK_TRUE, timeout);
		if (result != VK_SUCCESS) throw std::runtime_error("Failed to wait for fence: " +
			s_resultMessages.at(result));
	}

	void Fence::reset(const Device& device)
	{
		auto result = device.getFunctionTable().execute<DeviceFunction::ResetFences>(
			device.getHandle(), 1, &m_fence);
		if (result != VK_SUCCESS) throw std::runtime_error("Failed to reset fence: " +
			s_resultMessages.at(result));
	}

	static void wait(const Device& device, std::vector<Fence>& fences, size_t timeout = std::numeric_limits<size_t>::max())
	{
		VkFence* vkFences = reinterpret_cast<VkFence*>(fences.data());

		auto result = device.getFunctionTable().execute<DeviceFunction::WaitForFences>(
			device.getHandle(), fences.size(), vkFences, VK_TRUE, timeout);
		if (result != VK_SUCCESS) throw std::runtime_error("Failed to wait for fences: " +
			s_resultMessages.at(result));
	}
	
	static void reset(const Device& device, std::vector<Fence>& fences)
	{
		VkFence* vkFences = reinterpret_cast<VkFence*>(fences.data());

		auto result = device.getFunctionTable().execute<DeviceFunction::ResetFences>(
			device.getHandle(), fences.size(), vkFences);
		if (result != VK_SUCCESS) throw std::runtime_error("Failed to reset fences: " +
			s_resultMessages.at(result));
	}
}