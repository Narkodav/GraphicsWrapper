#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
	class SamplerRef : public BaseComponent<VkSampler, SamplerRef>
	{
		using Base = BaseComponent<VkSampler, SamplerRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Sampler";
	};

	class Sampler : public VerificatorComponent<VkSampler, SamplerRef>
	{
		using Base = VerificatorComponent<VkSampler, SamplerRef>;
	public:
		using Base::Base;

		class CreateInfo : public StructBase<VkSamplerCreateInfo, CreateInfo>
		{
			using Base = StructBase<VkSamplerCreateInfo, CreateInfo>;
		public:
			using Base::Base;

			CreateInfo(Filter magFilter = Filter::Linear, Filter minFilter = Filter::Linear,
				SamplerMipmapMode mipmapMode = SamplerMipmapMode::Linear,
				SamplerAddressMode modeU = SamplerAddressMode::Repeat,
				SamplerAddressMode modeV = SamplerAddressMode::Repeat,
				SamplerAddressMode modeW = SamplerAddressMode::Repeat,
				float mipLodBias = 0.0f,
				bool anisotropyEnable = false,
				float maxAnisotropy = 1.0f,
				bool compareEnable = false,
				CompareOp compareOp = CompareOp::Always,
				float minLod = 0.0f,
				float maxLod = VK_LOD_CLAMP_NONE,
				BorderColor borderColor = BorderColor::IntOpaqueBlack,
				bool unnormalizedCoordinates = false,
				SamplerCreate::Flags flags = SamplerCreate::Bits::None)
				: Base()
			{
				this->flags = flags;
				this->magFilter = static_cast<VkFilter>(magFilter);
				this->minFilter = static_cast<VkFilter>(minFilter);
				this->mipmapMode = static_cast<VkSamplerMipmapMode>(mipmapMode);
				this->addressModeU = static_cast<VkSamplerAddressMode>(modeU);
				this->addressModeV = static_cast<VkSamplerAddressMode>(modeV);
				this->addressModeW = static_cast<VkSamplerAddressMode>(modeW);
				this->mipLodBias = mipLodBias;
				this->anisotropyEnable = anisotropyEnable;
				this->maxAnisotropy = maxAnisotropy;
				this->compareEnable = compareEnable;
				this->compareOp = static_cast<VkCompareOp>(compareOp);
				this->minLod = minLod;
				this->maxLod = maxLod;
				this->borderColor = static_cast<VkBorderColor>(borderColor);
				this->unnormalizedCoordinates = unnormalizedCoordinates;
			}

			CreateInfo& setMagFilter(Filter magFilter) {
				this->magFilter = static_cast<VkFilter>(magFilter);
				return *this;
			}

			CreateInfo& setMinFilter(Filter minFilter) {
				this->minFilter = static_cast<VkFilter>(minFilter);
				return *this;
			}

			CreateInfo& setMipmapMode(SamplerMipmapMode mipmapMode) {
				this->mipmapMode = static_cast<VkSamplerMipmapMode>(mipmapMode);
				return *this;
			}

			CreateInfo& setAddressModeU(SamplerAddressMode modeU) {
				this->addressModeU = static_cast<VkSamplerAddressMode>(modeU);
				return *this;
			}

			CreateInfo& setAddressModeV(SamplerAddressMode modeV) {
				this->addressModeV = static_cast<VkSamplerAddressMode>(modeV);
				return *this;
			}

			CreateInfo& setAddressModeW(SamplerAddressMode modeW) {
				this->addressModeW = static_cast<VkSamplerAddressMode>(modeW);
				return *this;
			}

			CreateInfo& setMipLodBias(float mipLodBias) {
				this->mipLodBias = mipLodBias;
				return *this;
			}

			CreateInfo& setAnisotropyEnable(bool enable) {
				this->anisotropyEnable = enable;
				return *this;
			}

			CreateInfo& setMaxAnisotropy(float maxAnisotropy) {
				this->maxAnisotropy = maxAnisotropy;
				return *this;
			}

			CreateInfo& setCompareEnable(bool enable) {
				this->compareEnable = enable;
				return *this;
			}

			CreateInfo& setCompareOp(CompareOp compareOp) {
				this->compareOp = static_cast<VkCompareOp>(compareOp);
				return *this;
			}

			CreateInfo& setMinLod(float minLod) {
				this->minLod = minLod;
				return *this;
			}

			CreateInfo& setMaxLod(float maxLod) {
				this->maxLod = maxLod;
				return *this;
			}

			CreateInfo& setBorderColor(BorderColor borderColor) {
				this->borderColor = static_cast<VkBorderColor>(borderColor);
				return *this;
			}

			CreateInfo& setUnnormalizedCoordinates(bool unnormalizedCoordinates) {
				this->unnormalizedCoordinates = unnormalizedCoordinates;
				return *this;
			}
		};

		void create(const DeviceRef& device, const DeviceFunctionTable& functions, const CreateInfo& createInfo);
		void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
	};
}

