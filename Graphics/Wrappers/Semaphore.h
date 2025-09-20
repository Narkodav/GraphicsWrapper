#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
	class SemaphoreRef : public BaseComponent<VkSemaphore, SemaphoreRef>
	{
		using Base = BaseComponent<VkSemaphore, SemaphoreRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Semaphore";
	};

    class Semaphore : public VerificatorComponent<VkSemaphore, SemaphoreRef>
    {
        using Base = VerificatorComponent<VkSemaphore, SemaphoreRef>;
    public:
		using Base::Base;

		class CreateInfo : public StructBase<VkSemaphoreCreateInfo, CreateInfo>
		{
			using Base = StructBase<VkSemaphoreCreateInfo, CreateInfo>;
		public:
			using Base::Base;
			CreateInfo(SemaphoreCreate::Flags flags = SemaphoreCreate::Bits::None) : Base() {
				this->flags = flags;
			}
			CreateInfo& setFlags(SemaphoreCreate::Flags flags) {
				this->flags = flags;
				return *this;
			}
			SemaphoreCreate::Flags getFlags() const {
				return this->flags;
			}
		};

		void create(const DeviceRef& device, const DeviceFunctionTable& functions,
			const CreateInfo& createInfo = CreateInfo()); // CreateInfo is currently useless, as there are no flags defined
        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };
}
