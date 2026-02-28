#include "Graphics/Graphics.h"

namespace Graphics
{
	void DescriptorSetLayout::create(const DeviceFunctionTable& functions,
		DeviceRef device, const DescriptorSetLayoutCreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid descriptor set layout");
		auto result = functions.execute<DeviceFunction::CreateDescriptorSetLayout>(
			device.getHandle(), createInfo.getUnderlyingPointer(),
			nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY_RESULT(result, "Failed to create descriptor set layout");
	}

	void DescriptorSetLayout::destroy(const DeviceFunctionTable& functions, DeviceRef device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid descriptor set layout");
		functions.execute<DeviceFunction::DestroyDescriptorSetLayout>(device.getHandle(), getHandle(), nullptr);
		reset();
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, DeviceRef device,
		std::span<const DescriptorSetWrite> descriptorWrites, std::span<const DescriptorSetCopy> descriptorCopies)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			descriptorWrites.size(), DescriptorSetWrite::underlyingCast(descriptorWrites.data()),
			descriptorCopies.size(), DescriptorSetCopy::underlyingCast(descriptorCopies.data()));
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, DeviceRef device,
		std::span<const DescriptorSetWrite> descriptorWrites)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			descriptorWrites.size(), DescriptorSetWrite::underlyingCast(descriptorWrites.data()), 0, nullptr);
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, DeviceRef device,
		std::span<const DescriptorSetCopy> descriptorCopies)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			0, nullptr, descriptorCopies.size(), DescriptorSetCopy::underlyingCast(descriptorCopies.data()));
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, DeviceRef device,
		const DescriptorSetWrite& descriptorWrite)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			1, descriptorWrite.getUnderlyingPointer(), 0, nullptr);
	}

	void DescriptorSet::update(const DeviceFunctionTable& functions, DeviceRef device,
		const DescriptorSetCopy& descriptorCopy)
	{
		functions.execute<DeviceFunction::UpdateDescriptorSets>(device.getHandle(),
			0, nullptr, 1, descriptorCopy.getUnderlyingPointer());
	}

	void DescriptorSet::write(const DeviceFunctionTable& functions, DeviceRef device,
		uint32_t dstBinding, uint32_t dstArrayElement, std::span<const DescriptorBufferInfo> bufferInfo,
		DescriptorType descriptorType)
	{
		DescriptorSetWrite descriptorWrite(*this, dstBinding, dstArrayElement, bufferInfo, descriptorType);
		DescriptorSet::update(functions, device, descriptorWrite);
	}

	void DescriptorSet::write(const DeviceFunctionTable& functions, DeviceRef device,
		uint32_t dstBinding, uint32_t dstArrayElement, std::span<const DescriptorImageInfo> imageInfo,
		DescriptorType descriptorType)
	{
		DescriptorSetWrite descriptorWrite(*this, dstBinding, dstArrayElement, imageInfo, descriptorType);
		DescriptorSet::update(functions, device, descriptorWrite);
	}

	void DescriptorSet::write(const DeviceFunctionTable& functions, DeviceRef device,
		uint32_t dstBinding, uint32_t dstArrayElement, std::span<const BufferViewRef> texelBufferViews,
		DescriptorType descriptorType)
	{
		DescriptorSetWrite descriptorWrite(*this, dstBinding, dstArrayElement, texelBufferViews, descriptorType);
		DescriptorSet::update(functions, device, descriptorWrite);
	}

	void DescriptorSet::write(const DeviceFunctionTable& functions, DeviceRef device,
		uint32_t dstBinding, uint32_t dstArrayElement, const DescriptorBufferInfo& bufferInfo,
		DescriptorType descriptorType)
	{
		DescriptorSetWrite descriptorWrite(*this, dstBinding, dstArrayElement, bufferInfo, descriptorType);
		DescriptorSet::update(functions, device, descriptorWrite);
	}

	void DescriptorSet::write(const DeviceFunctionTable& functions, DeviceRef device,
		uint32_t dstBinding, uint32_t dstArrayElement, const DescriptorImageInfo& imageInfo,
		DescriptorType descriptorType)
	{
		DescriptorSetWrite descriptorWrite(*this, dstBinding, dstArrayElement, imageInfo, descriptorType);
		DescriptorSet::update(functions, device, descriptorWrite);
	}

	void DescriptorSet::write(const DeviceFunctionTable& functions, DeviceRef device,
		uint32_t dstBinding, uint32_t dstArrayElement, BufferViewRef texelBufferView,
		DescriptorType descriptorType)
	{
		DescriptorSetWrite descriptorWrite(*this, dstBinding, dstArrayElement, texelBufferView, descriptorType);
		DescriptorSet::update(functions, device, descriptorWrite);
	}

	void DescriptorSet::copy(const DeviceFunctionTable& functions, DeviceRef device, DescriptorSet srcSet,
		uint32_t srcBinding, uint32_t dstBinding, uint32_t srcArrayElement, uint32_t dstArrayElement,
		uint32_t descriptorCount)
	{
		DescriptorSetCopy descriptorCopy(srcSet, *this, srcBinding, dstBinding, 
			srcArrayElement, dstArrayElement, descriptorCount);
		DescriptorSet::update(functions, device, descriptorCopy);		
	}
}