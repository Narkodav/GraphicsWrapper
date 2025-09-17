#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class FenceRef : public BaseComponent<VkFence, FenceRef>
    {
        using Base = BaseComponent<VkFence, FenceRef>;
    public:
        using Base::Base;

        void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            size_t timeout = std::numeric_limits<size_t>::max()) const;        

        static void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            const std::vector<Fence>& fences, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true);

        static void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            const std::vector<FenceRef>& fences, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true);

        static void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            const FenceRef* fences, size_t count, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true);
    };

    class Fence : public VerificatorComponent<VkFence, FenceRef>
    {
        using Base = VerificatorComponent<VkFence, FenceRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkFenceCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkFenceCreateInfo, CreateInfo>;
        public:
            using Base::Base;
            CreateInfo(FenceCreate::Flags flags = FenceCreate::Bits::Signaled) : Base() {
                this->flags = flags;
            }

            CreateInfo& setFlags(FenceCreate::Flags flags) {
                this->flags = flags;
                return *this;
            }

            FenceCreate::Flags getFlags() const {
                return this->flags;
            }
        };

        void create(const DeviceRef& device, const DeviceFunctionTable& functions,
            const CreateInfo& createInfo = CreateInfo());

        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);

        void reset(const DeviceRef& device, const DeviceFunctionTable& functions);

        static void reset(const DeviceRef& device, const DeviceFunctionTable& functions,
            const std::vector<Fence>& fences);

        static void reset(const DeviceRef& device, const DeviceFunctionTable& functions,
            const std::vector<FenceRef>& fences);

        static void reset(const DeviceRef& device, const DeviceFunctionTable& functions,
            const FenceRef* fences, size_t count);
    };
}



