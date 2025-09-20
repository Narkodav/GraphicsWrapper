#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class FenceRef : public BaseComponent<VkFence, FenceRef>
    {
        using Base = BaseComponent<VkFence, FenceRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Fence";

        void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            size_t timeout = std::numeric_limits<size_t>::max()) const;
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

        template<typename FenceType>
        static void reset(const DeviceRef& device, const DeviceFunctionTable& functions,
            std::span<const FenceType> fences) requires 
            std::is_base_of_v<Fence, FenceType> || std::is_base_of_v<FenceRef, FenceType>
        {
            reset(device, functions, reinterpret_cast<const FenceType*>(fences.data()), fences.size());
        }

        static void reset(const DeviceRef& device, const DeviceFunctionTable& functions,
            const FenceRef* fences, size_t count);

        template<typename FenceType>
        static void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            std::span<const FenceType> fences, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true) requires
            std::is_base_of_v<Fence, FenceType> || std::is_base_of_v<FenceRef, FenceType>
        {
            wait(device, functions, fences.data(), fences.size(), timeout, waitAll);
        }

        static void wait(const DeviceRef& device, const DeviceFunctionTable& functions,
            const FenceRef* fences, size_t count, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true);
    };
}



