#include "DescriptorSet.h"

namespace Graphics
{
	void DescriptorSet::Layout::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const CreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid descriptor set layout");
		auto result = functions.execute<DeviceFunction::CreateDescriptorSetLayout>(
			device.getHandle(), createInfo.getUnderlyingPointer(),
			nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY(result == VK_SUCCESS,
			"Failed to create descriptor set layout: " + s_resultMessages.at(result));
	}

	void DescriptorSet::Layout::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid descriptor set layout");
		functions.execute<DeviceFunction::DestroyDescriptorSetLayout>(device.getHandle(), getHandle(), nullptr);
		reset();
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, const DeviceRef& device,
		std::span<const Write> descriptorWrites, std::span<const Copy> descriptorCopies)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			descriptorWrites.size(), Write::underlyingCast(descriptorWrites.data()),
			descriptorCopies.size(), Copy::underlyingCast(descriptorCopies.data()));
	}
}