#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
	DescriptorSetAllocateInfo::DescriptorSetAllocateInfo(const DescriptorPoolRef& descriptorPool,
		std::span<const DescriptorSetLayoutRef> layouts) : Base() {
		this->descriptorSetCount = layouts.size();
		this->pSetLayouts = DescriptorSetLayoutRef::underlyingCast(layouts.data());
		this->descriptorPool = descriptorPool.getHandle();
	}

	DescriptorSetAllocateInfo::DescriptorSetAllocateInfo(const DescriptorPoolRef& descriptorPool,
		std::span<const DescriptorSetLayout> layouts) : Base() {
		this->descriptorSetCount = layouts.size();
		this->pSetLayouts = DescriptorSetLayoutRef::underlyingCast(layouts.data());
		this->descriptorPool = descriptorPool.getHandle();
	}

	DescriptorSetAllocateInfo& DescriptorSetAllocateInfo::setDescriptorPool(
		const DescriptorPoolRef& descriptorPool) {
		this->descriptorPool = descriptorPool.getHandle();
		return *this;
	}

	void DescriptorPool::create(const DeviceFunctionTable& functions,
		const DeviceRef& device, const DescriptorPoolCreateInfo& createInfo)
	{
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid descriptor pool");
		auto result = functions.execute<DeviceFunction::CreateDescriptorPool>(
			device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY_RESULT(result, "Failed to create descriptor pool");
	}

	void DescriptorPool::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
	{
		GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid descriptor pool");
		functions.execute<DeviceFunction::DestroyDescriptorPool>(
			device.getHandle(), getHandle(), nullptr);
		BaseComponent<VkDescriptorPool, DescriptorPoolRef>::reset();
	}

	void DescriptorPoolRef::reset(const DeviceFunctionTable& functions, const DeviceRef& device,
		Flags::DescriptorPoolReset flags /*= Flags::DescriptorPoolReset::Bits::None*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to reset an invalid descriptor pool");
		auto result = functions.execute<DeviceFunction::ResetDescriptorPool>(
			device.getHandle(), getHandle(), flags);
		GRAPHICS_VERIFY_RESULT(result, "Failed to reset descriptor pool");
	}

	void DescriptorPoolRef::freeSets(const DeviceFunctionTable& functions, const DeviceRef& device,
		std::span<const DescriptorSet> sets) {
		GRAPHICS_VERIFY(isSet(), "Trying to free sets from an invalid descriptor pool");
		auto result = functions.execute<DeviceFunction::FreeDescriptorSets>(
			device.getHandle(), getHandle(), sets.size(), DescriptorSet::underlyingCast(sets.data()));
		GRAPHICS_VERIFY_RESULT(result, "Failed to free sets from descriptor pool");
	}

	void DescriptorPoolRef::freeSet(const DeviceFunctionTable& functions, const DeviceRef& device,
		const DescriptorSet& set) {
		GRAPHICS_VERIFY(isSet(), "Trying to free set from an invalid descriptor pool");
		auto result = functions.execute<DeviceFunction::FreeDescriptorSets>(
			device.getHandle(), getHandle(), 1, set.getUnderlyingPointer());
		GRAPHICS_VERIFY_RESULT(result, "Failed to free set from descriptor pool");
	}

	std::vector<DescriptorSet> DescriptorPoolRef::allocateSets(const DeviceFunctionTable& functions,
		const DeviceRef& device, const DescriptorSetAllocateInfo& allocateInfo)
	{
		std::vector<DescriptorSet> sets(allocateInfo.getDescriptorSetCount());
		auto result = functions.execute<DeviceFunction::AllocateDescriptorSets>(
			device.getHandle(), allocateInfo.getUnderlyingPointer(),
			DescriptorSet::underlyingCast(sets.data()));
		GRAPHICS_VERIFY_RESULT(result, "Failed to allocate sets from descriptor pool");
		return sets;
	}
}