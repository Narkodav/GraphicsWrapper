#pragma once
#include "../Common.h"
#include "../PlatformManagement/Window.h"
#include "Instance.h"
#include "../DeviceCaching/PhysicalDeviceCache.h"
#include "../DeviceFunctionTable.h"
#include "../Wrappers.h"

namespace Graphics {

	class Device
	{
	public:



	private:
		const PhysicalDevice* m_physicalDevice = nullptr;
		Wrappers::Device m_device;
		DeviceFunctionTable m_deviceFunctions;

	public:
		Device() = default;

		Device(const Instance& instance,
			const PhysicalDevice& physicalDevice,
			const DeviceRequirements& requirements,
			const std::vector<uint32_t>& queueFamilyIndices,
			const std::vector<uint32_t>& queueCounts,
			const std::vector<std::vector<float>>& queuePriorities,
			const std::vector<const char*>& enabledLayers) :
			m_physicalDevice(&physicalDevice)
		{
			m_device.create(instance.getHandle(),
				physicalDevice.getHandle(), instance.getFunctionTable(),
				requirements, queueFamilyIndices, queueCounts,
				queuePriorities, enabledLayers);
			m_deviceFunctions = m_device.getFunctionTable(instance.getFunctionTable());
#ifdef _DEBUG
			std::cout << __FUNCTION__ << " - Created Device" << std::endl;
#endif
		};

		Device(Device&& other) noexcept {
			m_physicalDevice = std::exchange(other.m_physicalDevice, nullptr);
			m_device = std::exchange(other.m_device, nullptr);
			m_deviceFunctions = std::exchange(other.m_deviceFunctions, DeviceFunctionTable());
		};

		//moving to an initialized device is undefined behavior, destroy before moving
		Device& operator=(Device&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_physicalDevice = std::exchange(other.m_physicalDevice, nullptr);
			m_device = std::exchange(other.m_device, nullptr);
			m_deviceFunctions = std::exchange(other.m_deviceFunctions, DeviceFunctionTable());

			return *this;
		};

		Device(const Device&) noexcept = delete;
		Device& operator=(const Device&) noexcept = delete;

		~Device() = default;

		void destroy() {			
			m_device.destroy(m_deviceFunctions);
#ifdef _DEBUG
			std::cout << __FUNCTION__ << " - Destroyed Device" << std::endl;
#endif
		}

		const auto& getHandle() const { return m_device; }
		const auto& getPhysicalDevice() const { return *m_physicalDevice; }
		const auto& getFunctionTable() const { return m_deviceFunctions; }

		void waitIdle() const {
			m_device.waitIdle(m_deviceFunctions);
		};				

		VkCommandBuffer allocateCommandBuffer(const Instance& instance, const vk::CommandPool& pool,
			CommandBufferLevel level = CommandBufferLevel::Primary) const {
			vk::CommandBufferAllocateInfo allocInfo{};
			allocInfo.setCommandPool(pool)
				.setLevel(convertEnum(level))
				.setCommandBufferCount(1);

			VkCommandBuffer buffer;
			auto result = m_deviceFunctions.execute<DeviceFunction::AllocateCommandBuffers>(m_device,
				reinterpret_cast<VkCommandBufferAllocateInfo*>(&allocInfo), &buffer);

			if (result != VK_SUCCESS)
				throw std::runtime_error("VkCommandBuffer Device::allocateCommandBuffer(const Instance& instance, const vk::CommandPool& pool,"
					"CommandBufferLevel level = CommandBufferLevel::Primary) const - Failed to allocate command buffer: " +
					s_resultMessages.at(result));
			return buffer;
		}

		void freeCommandBuffer(const VkCommandPool& pool, VkCommandBuffer& buffer) const {
			m_deviceFunctions.execute<DeviceFunction::FreeCommandBuffers>(m_device, pool, 1, &buffer);
		}
	};

}