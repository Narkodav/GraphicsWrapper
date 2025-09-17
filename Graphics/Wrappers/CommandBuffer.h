#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
	class CommandBuffer : public BaseComponent<VkCommandBuffer, CommandBuffer>
	{
		using Base = BaseComponent<VkCommandBuffer, CommandBuffer>;
	public:
		using Base::Base;
	};
}

