#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "CommandBuffer.h"
#include "Queue.h"

namespace Graphics
{
	class CommandPoolRef : public BaseComponent<VkCommandPool, CommandPoolRef>
	{
		using Base = BaseComponent<VkCommandPool, CommandPoolRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "CommandPool";

		CommandBuffer allocateCommandBuffer(const DeviceFunctionTable& functions, const DeviceRef& device,
			CommandBufferLevel level = CommandBufferLevel::Primary);

		void freeCommandBuffer(const DeviceFunctionTable& functions, const DeviceRef& device,
			CommandBuffer& buffer);

		std::vector<CommandBuffer> allocateCommandBuffers(const DeviceFunctionTable& functions, const DeviceRef& device,
			size_t count, CommandBufferLevel level = CommandBufferLevel::Primary);

		void freeCommandBuffers(const DeviceFunctionTable& functions, const DeviceRef& device,
			std::span<const CommandBuffer> buffer);

		void reset(const DeviceFunctionTable& functions, const DeviceRef& device,
			Flags::CommandPoolReset flags = Flags::CommandPoolReset::Bits::ReleaseResources);

		template<typename Func>
		Result oneTimeSubmit(const DeviceFunctionTable& functions, const DeviceRef& device,
			const Queue& submitQueue, Func&& func)
		{
			GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
			auto temporary = allocateCommandBuffer(functions, device);

			auto result = temporary.record(functions, { Flags::CommandBufferUsage::Bits::OneTimeSubmit },
				std::forward<Func>(func));
			if (result != Result::Success)
				return result;

			QueueSubmitInfo submitInfo;
			submitInfo.setCommandBuffer(temporary);
			submitQueue.submit(functions, submitInfo, nullptr);
			submitQueue.waitIdle(functions);
			freeCommandBuffer(functions, device, temporary);
			return Result::Success;
		};
	};

	class CommandBufferAllocateInfo : public StructBase<VkCommandBufferAllocateInfo, CommandBufferAllocateInfo>
	{
		using Base = StructBase<VkCommandBufferAllocateInfo, CommandBufferAllocateInfo>;
	public:
		using Base::Base;

		CommandBufferAllocateInfo(const CommandPoolRef& commandPool,
			CommandBufferLevel level = CommandBufferLevel::Primary,
			uint32_t commandBufferCount = 1) : Base() {
			this->commandPool = commandPool.getHandle();
			this->level = convertCEnum(level);
			this->commandBufferCount = commandBufferCount;
		}
		CommandBufferAllocateInfo& setCommandPool(const CommandPoolRef& commandPool) {
			this->commandPool = commandPool.getHandle();
			return *this;
		}
		CommandBufferAllocateInfo& setLevel(CommandBufferLevel level) {
			this->level = convertCEnum(level);
			return *this;
		}
		CommandBufferAllocateInfo& setCommandBufferCount(uint32_t commandBufferCount) {
			this->commandBufferCount = commandBufferCount;
			return *this;
		}
		CommandPoolRef getCommandPool() const { return CommandPoolRef(this->commandPool); }
		CommandBufferLevel getLevel() const { return convertCEnum(this->level); }
		uint32_t getCommandBufferCount() const { return this->commandBufferCount; }
	};

	class CommandPoolCreateInfo : public StructBase<VkCommandPoolCreateInfo, CommandPoolCreateInfo>
	{
		using Base = StructBase<VkCommandPoolCreateInfo, CommandPoolCreateInfo>;
	public:
		using Base::Base;
		CommandPoolCreateInfo(uint32_t queueFamilyIndex,
			Flags::CommandPoolCreate flags = Flags::CommandPoolCreate::Bits::ResetCommandBuffer) : Base()
		{
			this->queueFamilyIndex = queueFamilyIndex;
			this->flags = flags;
		}
		CommandPoolCreateInfo& setQueueFamilyIndex(uint32_t queueFamilyIndex) {
			this->queueFamilyIndex = queueFamilyIndex;
			return *this;
		}
		CommandPoolCreateInfo& setFlags(Flags::CommandPoolCreate flags) {
			this->flags = flags;
			return *this;
		}
		uint32_t getQueueFamilyIndex() const { return this->queueFamilyIndex; }
		Flags::CommandPoolCreate getFlags() const { return this->flags; }
	};

	class CommandPool : public VerificatorComponent<VkCommandPool, CommandPoolRef> {
		using Base = VerificatorComponent<VkCommandPool, CommandPoolRef>;
	public:
		using Base::Base;		

		void create(const DeviceFunctionTable& functions, const DeviceRef& device, const CommandPoolCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);

		static std::vector<CommandBuffer> allocateCommandBuffers(const DeviceFunctionTable& functions, 
			const DeviceRef& device, const CommandBufferAllocateInfo& allocInfo);
	};
}



