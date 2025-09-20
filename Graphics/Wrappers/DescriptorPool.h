#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "Sampler.h"
#include "DescriptorSet.h"

namespace Graphics
{
	class DescriptorPoolRef : public BaseComponent<VkDescriptorPool, DescriptorPoolRef>
	{
		using Base = BaseComponent<VkDescriptorPool, DescriptorPoolRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "DescriptorPool";

		void reset(const DeviceFunctionTable& functions, const DeviceRef& device,
			DescriptorPoolReset::Flags flags = DescriptorPoolReset::Bits::None);

		template<typename LayoutType>
		std::vector<DescriptorSet> allocateSets(const DeviceFunctionTable& functions,
			const DeviceRef& device, std::span<const LayoutType> layouts) requires
			std::is_same_v<LayoutType, DescriptorSet::LayoutRef> || std::is_same_v<LayoutType, DescriptorSet::Layout>
		{
			GRAPHICS_VERIFY(isSet(), "Trying to allocate from an invalid descriptor pool");
			vk::DescriptorSetAllocateInfo allocateInfo;
			allocateInfo.descriptorPool = this;
			allocateInfo.descriptorSetCount = layouts.size();
			allocateInfo.pSetLayouts = DescriptorSet::Layout::underlyingCast(layouts.data());

			std::vector<DescriptorSet> sets(layouts.size());
			auto result = functions.execute<DeviceFunction::AllocateDescriptorSets>(
				device.getHandle(), reinterpret_cast<VkDescriptorSetAllocateInfo*>(&allocateInfo),
				DescriptorSet::underlyingCast(sets.data()));
			GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to allocate sets from descriptor pool: " +
				s_resultMessages.at(result));
			return sets;
		}

		template<typename LayoutType>
		DescriptorSet allocateSet(const DeviceFunctionTable& functions,
			const DeviceRef& device, const LayoutType& layout) requires
			std::is_same_v<LayoutType, DescriptorSet::LayoutRef> || std::is_same_v<LayoutType, DescriptorSet::Layout>
		{
			GRAPHICS_VERIFY(isSet(), "Trying to allocate from an invalid descriptor pool");
			vk::DescriptorSetAllocateInfo allocateInfo;
			allocateInfo.descriptorPool = this;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = layout.getUnderlyingPointer();

			DescriptorSet set;
			auto result = functions.execute<DeviceFunction::AllocateDescriptorSets>(
				device.getHandle(), reinterpret_cast<VkDescriptorSetAllocateInfo*>(&allocateInfo),
				set.getUnderlyingPointer());
			GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to allocate sets from descriptor pool: " +
				s_resultMessages.at(result));
			return set;
		}

		void freeSets(const DeviceFunctionTable& functions, const DeviceRef& device,
			std::span<const DescriptorSet> sets) {
			GRAPHICS_VERIFY(isSet(), "Trying to free sets from an invalid descriptor pool");
			auto result = functions.execute<DeviceFunction::FreeDescriptorSets>(
				device.getHandle(), getHandle(), sets.size(), DescriptorSet::underlyingCast(sets.data()));
			GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to free sets from descriptor pool: " +
				s_resultMessages.at(result));
		}

		void freeSet(const DeviceFunctionTable& functions, const DeviceRef& device,
			const DescriptorSet& set) {
			GRAPHICS_VERIFY(isSet(), "Trying to free set from an invalid descriptor pool");
			auto result = functions.execute<DeviceFunction::FreeDescriptorSets>(
				device.getHandle(), getHandle(), 1, set.getUnderlyingPointer());
			GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to free set from descriptor pool: " +
				s_resultMessages.at(result));
		}
	};

	class DescriptorPool : public VerificatorComponent<VkDescriptorPool, DescriptorPoolRef>
	{
		using Base = VerificatorComponent<VkDescriptorPool, DescriptorPoolRef>;
	public:
		using Base::Base;

		class Size : public StructBase<VkDescriptorPoolSize, Size>
		{
			using Base = StructBase<VkDescriptorPoolSize, Size>;
		public:
			using Base::Base;

			Size& setDescriptorCount(uint32_t descriptorCount) {
				this->descriptorCount = descriptorCount;
				return *this;
			}

			Size& setType(DescriptorType type) {
				this->type = convertCEnum(type);
				return *this;
			}
		};

		class CreateInfo : public StructBase<VkDescriptorPoolCreateInfo, CreateInfo>
		{
			using Base = StructBase<VkDescriptorPoolCreateInfo, CreateInfo>;
		public:
			using Base::Base;

			CreateInfo(uint32_t maxSets, std::span<const Size> poolSizes,
				DescriptorPoolCreate::Flags flags = DescriptorPoolCreate::Bits::None) : Base() {
				this->maxSets = maxSets;
				this->pPoolSizes = Size::underlyingCast(poolSizes.data());
				this->poolSizeCount = poolSizes.size();
				this->flags = flags;
			}

			CreateInfo& setMaxSets(uint32_t maxSets) {
				this->maxSets = maxSets;
				return *this;
			}

			CreateInfo& setPoolSizes(std::span<const Size> poolSizes) {
				this->pPoolSizes = Size::underlyingCast(poolSizes.data());
				this->poolSizeCount = poolSizes.size();
				return *this;
			}

			CreateInfo& setFlags(DescriptorPoolCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
		};

		void create(const DeviceFunctionTable& functions,
			const DeviceRef& device, const CreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);
	};
}

