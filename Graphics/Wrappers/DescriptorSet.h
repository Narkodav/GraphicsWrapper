#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "Sampler.h"
#include "Image.h"
#include "Buffer.h"

namespace Graphics
{
	class DescriptorImageInfo : public StructBase<VkDescriptorImageInfo, DescriptorImageInfo>
	{
		using Base = StructBase<VkDescriptorImageInfo, DescriptorImageInfo>;
	public:
		using Base::Base;
		DescriptorImageInfo(const SamplerRef& sampler, const Image::ViewRef& imageView,
			ImageLayout layout) : Base() {
			this->sampler = sampler;
			this->imageView = imageView;
			this->imageLayout = convertCEnum(layout);
		}
		DescriptorImageInfo& setSampler(const SamplerRef& sampler) {
			this->sampler = sampler;
			return *this;
		}
		DescriptorImageInfo& setImageView(const Image::ViewRef& imageView) {
			this->imageView = imageView;
			return *this;
		}
		DescriptorImageInfo& setImageLayout(ImageLayout layout) {
			this->imageLayout = convertCEnum(layout);
			return *this;
		}
	};

	class DescriptorBufferInfo : public StructBase<VkDescriptorBufferInfo, DescriptorBufferInfo>
	{
		using Base = StructBase<VkDescriptorBufferInfo, DescriptorBufferInfo>;
	public:
		using Base::Base;
		DescriptorBufferInfo(const BufferRef& buffer,
			DeviceSize_t offset, DeviceSize_t range) : Base() {
			this->buffer = buffer;
			this->offset = offset;
			this->range = range;
		}
		DescriptorBufferInfo& setSampler(const BufferRef& buffer) {
			this->buffer = buffer;
			return *this;
		}
		DescriptorBufferInfo& setImageView(DeviceSize_t offset) {
			this->offset = offset;
			return *this;
		}
		DescriptorBufferInfo& setImageLayout(DeviceSize_t range) {
			this->range = range;
			return *this;
		}

	};

	class DescriptorSet : public BaseComponent<VkDescriptorSet, DescriptorSet>
	{
		using Base = BaseComponent<VkDescriptorSet, DescriptorSet>;
	public:
		using Base::Base;

		class LayoutRef : public BaseComponent<VkDescriptorSetLayout, LayoutRef>
		{
			using Base = BaseComponent<VkDescriptorSetLayout, LayoutRef>;
		public:
			using Base::Base;
			static inline const std::string s_typeName = "DescriptorSet::Layout";
		};

		class Layout : public VerificatorComponent<VkDescriptorSetLayout, LayoutRef>
		{
			using Base = VerificatorComponent<VkDescriptorSetLayout, LayoutRef>;
		public:
			using Base::Base;

			class Binding : public StructBase<VkDescriptorSetLayoutBinding, Binding>
			{
				using Base = StructBase<VkDescriptorSetLayoutBinding, Binding>;
			public:
				using Base::Base;
				Binding(uint32_t binding, DescriptorType descriptorType,
					uint32_t descriptorCount, ShaderStage::Flags stageFlags,
					std::span<const SamplerRef> immutableSamplers = {}) : Base() {
					this->binding = binding;
					this->descriptorType = convertCEnum(descriptorType);
					this->descriptorCount = descriptorCount;
					this->stageFlags = stageFlags;
					this->pImmutableSamplers = SamplerRef::underlyingCast(immutableSamplers.data());
				}

				Binding& setBinding(uint32_t binding) {
					this->binding = binding;
					return *this;
				}
				Binding& setDescriptorType(DescriptorType descriptorType) {
					this->descriptorType = convertCEnum(descriptorType);
					return *this;
				}
				Binding& setDescriptorCount(uint32_t descriptorCount) {
					this->descriptorCount = descriptorCount;
					return *this;
				}
				Binding& setStageFlags(ShaderStage::Flags stageFlags) {
					this->stageFlags = stageFlags;
					return *this;
				}
				Binding& setImmutableSamplers(std::span<const SamplerRef> immutableSamplers) {
					this->pImmutableSamplers = SamplerRef::underlyingCast(immutableSamplers.data());
					this->descriptorCount = immutableSamplers.size();
					return *this;
				}
			};

			class CreateInfo : public StructBase<VkDescriptorSetLayoutCreateInfo, CreateInfo>
			{
				using Base = StructBase<VkDescriptorSetLayoutCreateInfo, CreateInfo>;
			public:
				using Base::Base;

				CreateInfo(std::span<const DescriptorSet::Layout::Binding> bindings,
					DescriptorSetLayoutCreate::Flags flags = DescriptorSetLayoutCreate::Bits::None) : Base() {
					this->bindingCount = static_cast<uint32_t>(bindings.size());
					this->pBindings = DescriptorSet::Layout::Binding::underlyingCast(bindings.data());
					this->flags = flags;
				}

				CreateInfo& setFlags(DescriptorSetLayoutCreate::Flags flags) {
					this->flags = flags;
					return *this;
				}

				CreateInfo& setBindings(std::span<const DescriptorSet::Layout::Binding> bindings) {
					this->bindingCount = static_cast<uint32_t>(bindings.size());
					this->pBindings = DescriptorSet::Layout::Binding::underlyingCast(bindings.data());
					return *this;
				}
			};

			void create(const DeviceFunctionTable& functions,
				const DeviceRef& device, const CreateInfo& createInfo);
			void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);
		};

		class Copy : public StructBase<VkCopyDescriptorSet, Copy>
		{
			using Base = StructBase<VkCopyDescriptorSet, Copy>;
		public:
			using Base::Base;

			Copy(const DescriptorSet& srcSet, const DescriptorSet& dstSet,
				uint32_t srcBinding, uint32_t dstBinding,
				uint32_t srcArrayElement, uint32_t dstArrayElement,
				uint32_t descriptorCount) : Base()
			{
				this->srcSet = srcSet;
				this->dstSet = dstSet;
				this->srcBinding = srcBinding;
				this->dstBinding = dstBinding;
				this->srcArrayElement = srcArrayElement;
				this->dstArrayElement = dstArrayElement;
				this->descriptorCount = descriptorCount;
			}

			Copy& setSrcSet(const DescriptorSet& srcSet) {
				this->srcSet = srcSet;
				return *this;
			}
			Copy& setDstSet(const DescriptorSet& dstSet) {
				this->dstSet = dstSet;
				return *this;
			}
			Copy& setSrcBinding(uint32_t srcBinding) {
				this->srcBinding = srcBinding;
				return *this;
			}
			Copy& setDstBinding(uint32_t dstBinding) {
				this->dstBinding = dstBinding;
				return *this;
			}
			Copy& setSrcArrayElement(uint32_t srcArrayElement) {
				this->srcArrayElement = srcArrayElement;
				return *this;
			}
			Copy& setDstArrayElement(uint32_t dstArrayElement) {
				this->dstArrayElement = dstArrayElement;
				return *this;
			}
			Copy& setDescriptorCount(uint32_t descriptorCount) {
				this->descriptorCount = descriptorCount;
				return *this;
			}
		};

		class Write : public StructBase<VkWriteDescriptorSet, Write>
		{
			using Base = StructBase<VkWriteDescriptorSet, Write>;
		public:
			using Base::Base;

			Write(const DescriptorSet& dstSet, uint32_t dstBinding,
				uint32_t dstArrayElement, uint32_t descriptorCount,
				std::span<const DescriptorImageInfo> imageInfo) : Base() {
				this->dstSet = dstSet;
				this->dstBinding = dstBinding;
				this->dstArrayElement = dstArrayElement;
				this->pImageInfo = DescriptorImageInfo::underlyingCast(imageInfo.data());
				this->descriptorCount = imageInfo.size();
				this->pTexelBufferView = nullptr;
				this->pBufferInfo = nullptr;
			}

			Write(const DescriptorSet& dstSet, uint32_t dstBinding,
				uint32_t dstArrayElement, uint32_t descriptorCount,
				std::span<const DescriptorImageInfo> imageInfo) : Base() {
				this->dstSet = dstSet;
				this->dstBinding = dstBinding;
				this->dstArrayElement = dstArrayElement;
				this->pImageInfo = DescriptorImageInfo::underlyingCast(imageInfo.data());
				this->descriptorCount = imageInfo.size();
				this->pTexelBufferView = nullptr;
				this->pBufferInfo = nullptr;
			}

			Write(const DescriptorSet& dstSet, uint32_t dstBinding,
				uint32_t dstArrayElement, uint32_t descriptorCount,
				std::span<const DescriptorBufferInfo> bufferInfo) : Base() {
				this->dstSet = dstSet;
				this->dstBinding = dstBinding;
				this->dstArrayElement = dstArrayElement;
				this->pBufferInfo = DescriptorBufferInfo::underlyingCast(bufferInfo.data());
				this->descriptorCount = bufferInfo.size();
				this->pTexelBufferView = nullptr;
				this->pImageInfo = nullptr;
			}

			Write(const DescriptorSet& dstSet, uint32_t dstBinding,
				uint32_t dstArrayElement, uint32_t descriptorCount,
				std::span<const Buffer::ViewRef> texelBufferView) : Base() {
				this->dstSet = dstSet;
				this->dstBinding = dstBinding;
				this->dstArrayElement = dstArrayElement;
				this->pTexelBufferView = Buffer::ViewRef::underlyingCast(texelBufferView.data());
				this->descriptorCount = texelBufferView.size();
				this->pImageInfo = nullptr;
				this->pBufferInfo = nullptr;
			}

			Write& setDescriptorType(DescriptorType descriptorType) {
				this->descriptorType = convertCEnum(descriptorType);
				return *this;
			}
			Write& setDstArrayElement(uint32_t dstArrayElement) {
				this->dstArrayElement = dstArrayElement;
				return *this;
			}
			Write& setDstBinding(uint32_t dstBinding) {
				this->dstBinding = dstBinding;
				return *this;
			}
			Write& setDstSet(const DescriptorSet& dstSet) {
				this->dstSet = dstSet;
				return *this;
			}
			Write& setImageInfo(std::span<const DescriptorImageInfo> imageInfo) {
				this->pImageInfo = DescriptorImageInfo::underlyingCast(imageInfo.data());
				this->descriptorCount = imageInfo.size();
				this->pTexelBufferView = nullptr;
				this->pBufferInfo = nullptr;
				return *this;
			}
			Write& setBufferInfo(std::span<const DescriptorBufferInfo> bufferInfo) {
				this->pBufferInfo = DescriptorBufferInfo::underlyingCast(bufferInfo.data());
				this->descriptorCount = bufferInfo.size();
				this->pTexelBufferView = nullptr;
				this->pImageInfo = nullptr;
				return *this;
			}
			Write& setTexelBufferView(std::span<const Buffer::ViewRef> texelBufferView) {
				this->pTexelBufferView = Buffer::ViewRef::underlyingCast(texelBufferView.data());
				this->descriptorCount = texelBufferView.size();
				this->pImageInfo = nullptr;
				this->pBufferInfo = nullptr;
				return *this;
			}
		};

		static void update(const DeviceFunctionTable& functions, const DeviceRef& device,
			std::span<const Write> descriptorWrites, std::span<const Copy> descriptorCopies);
	};
}

