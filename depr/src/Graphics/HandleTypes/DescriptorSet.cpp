#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
	void DescriptorSetLayout::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const DescriptorSetLayoutCreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid descriptor set layout");
		auto result = functions.execute<DeviceFunction::CreateDescriptorSetLayout>(
			device.getHandle(), createInfo.getUnderlyingPointer(),
			nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY_RESULT(result, "Failed to create descriptor set layout");
	}

	void DescriptorSetLayout::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid descriptor set layout");
		functions.execute<DeviceFunction::DestroyDescriptorSetLayout>(device.getHandle(), getHandle(), nullptr);
		reset();
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, const DeviceRef& device,
		std::span<const DescriptorSetWrite> descriptorWrites, std::span<const DescriptorSetCopy> descriptorCopies)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			descriptorWrites.size(), DescriptorSetWrite::underlyingCast(descriptorWrites.data()),
			descriptorCopies.size(), DescriptorSetCopy::underlyingCast(descriptorCopies.data()));
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, const DeviceRef& device,
		std::span<const DescriptorSetWrite> descriptorWrites)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			descriptorWrites.size(), DescriptorSetWrite::underlyingCast(descriptorWrites.data()), 0, nullptr);
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, const DeviceRef& device,
		std::span<const DescriptorSetCopy> descriptorCopies)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			0, nullptr, descriptorCopies.size(), DescriptorSetCopy::underlyingCast(descriptorCopies.data()));
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, const DeviceRef& device,
		const DescriptorSetWrite& descriptorWrite)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			1, descriptorWrite.getUnderlyingPointer(), 0, nullptr);
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, const DeviceRef& device,
		const DescriptorSetCopy& descriptorCopy)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			0, nullptr, 1, descriptorCopy.getUnderlyingPointer());
	}
}