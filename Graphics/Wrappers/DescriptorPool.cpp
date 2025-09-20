#include "DescriptorPool.h"

namespace Graphics
{
	void DescriptorPool::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const CreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid descriptor pool");
		auto result = functions.execute<DeviceFunction::CreateDescriptorPool>(
			device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create descriptor pool: " +
			s_resultMessages.at(result));
	}

	void DescriptorPool::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid descriptor pool");
		functions.execute<DeviceFunction::DestroyDescriptorPool>(
			device.getHandle(), getHandle(), nullptr);
		BaseComponent<VkDescriptorPool, DescriptorPoolRef>::reset();
	}

	void DescriptorPoolRef::reset(const DeviceFunctionTable& functions, const DeviceRef& device,
		DescriptorPoolReset::Flags flags /*= DescriptorPoolReset::Bits::None*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to reset an invalid descriptor pool");
		auto result = functions.execute<DeviceFunction::ResetDescriptorPool>(
			device.getHandle(), getHandle(), flags);
		GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to reset descriptor pool: " +
			s_resultMessages.at(result));
	}

}