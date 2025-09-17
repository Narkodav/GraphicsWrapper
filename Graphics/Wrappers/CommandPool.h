#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "CommandBuffer.h"

namespace Graphics
{
	class CommandPoolRef : public BaseComponent<VkCommandPool, CommandPoolRef>
	{
		using Base = BaseComponent<VkCommandPool, CommandPoolRef>;
	public:
		using Base::Base;
	};

	class CommandPool : public VerificatorComponent<VkCommandPool, CommandPoolRef> {
		using Base = VerificatorComponent<VkCommandPool, CommandPoolRef>;
	public:
		using Base::Base;
		class CreateInfo : public StructBase<VkCommandPoolCreateInfo, CreateInfo>
		{
			using Base = StructBase<VkCommandPoolCreateInfo, CreateInfo>;
		public:
			using Base::Base;
			CreateInfo(uint32_t queueFamilyIndex,
				CommandPoolCreate::Flags flags = CommandPoolCreate::Bits::ResetCommandBuffer) : Base()
			{
				this->queueFamilyIndex = queueFamilyIndex;
				this->flags = flags;
			}
			CreateInfo& setQueueFamilyIndex(uint32_t queueFamilyIndex) {
				this->queueFamilyIndex = queueFamilyIndex;
				return *this;
			}
			CreateInfo& setFlags(CommandPoolCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			uint32_t getQueueFamilyIndex() const { return this->queueFamilyIndex; }
			CommandPoolCreate::Flags getFlags() const { return this->flags; }
		};

		void create(const DeviceRef& device, const DeviceFunctionTable& functions, const CreateInfo& createInfo);
		void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);

		CommandBuffer allocateCommandBuffer(const DeviceRef& device, const DeviceFunctionTable& functions,
			CommandBufferLevel level = CommandBufferLevel::Primary);

		void freeCommandBuffer(const DeviceRef& deviceRef, const DeviceFunctionTable& functions,
			CommandBuffer& buffer);

		std::vector<CommandBuffer> allocateCommandBuffers(const DeviceRef& device, const DeviceFunctionTable& functions,
			size_t count, CommandBufferLevel level = CommandBufferLevel::Primary);

		void freeCommandBuffers(const DeviceRef& device, const DeviceFunctionTable& functions,
			std::vector<CommandBuffer>& buffer);

		void resetPool(const DeviceRef& device, const DeviceFunctionTable& functions,
			CommandPoolReset::Flags flags = CommandPoolReset::Bits::ReleaseResources);
	};
}



