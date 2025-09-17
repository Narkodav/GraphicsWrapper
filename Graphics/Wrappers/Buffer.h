#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class BufferRef : public BaseComponent<VkBufferView, BufferRef>
    {
        using Base = BaseComponent<VkBufferView, BufferRef>;
    public:
        using Base::Base;

        MemoryRequirements getMemoryRequirements(const DeviceRef& device,
            const DeviceFunctionTable& functions) const;
    };

    class Buffer : public VerificatorComponent<VkBuffer, BufferRef>
    {
        using Base = VerificatorComponent<VkBuffer, BufferRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkBufferCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkBufferCreateInfo, CreateInfo>;
        public:
            using Base::Base;
            CreateInfo(size_t byteSize, BufferUsage::Flags usageFlags,
                SharingMode sharingMode = SharingMode::Exclusive) : Base()
            {
                this->size = static_cast<VkDeviceSize>(byteSize);
                this->usage = usageFlags;
                this->sharingMode = static_cast<VkSharingMode>(sharingMode);
                this->flags = 0;
                this->queueFamilyIndexCount = 0;
                this->pQueueFamilyIndices = nullptr;
            }
            CreateInfo& setSize(size_t byteSize) {
                this->size = static_cast<VkDeviceSize>(byteSize);
                return *this;
            }
            CreateInfo& setUsage(BufferUsage::Flags usageFlags) {
                this->usage = usageFlags;
                return *this;
            }
            CreateInfo& setSharingMode(SharingMode sharingMode) {
                this->sharingMode = static_cast<VkSharingMode>(sharingMode);
                return *this;
            }
            CreateInfo& setQueueFamilyIndices(const std::vector<uint32_t>& queueFamilyIndices) {
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                return *this;
            }

            size_t getSize() const { return static_cast<size_t>(this->size); }
            BufferUsage::Flags getUsage() const { return this->usage; }
            SharingMode getSharingMode() const { return static_cast<SharingMode>(this->sharingMode); }
            std::span<const uint32_t> getQueueFamilyIndices() const {
                return std::span<const uint32_t>(this->pQueueFamilyIndices, this->queueFamilyIndexCount);
            }
        };

        void create(const DeviceRef& device,
            const DeviceFunctionTable& functions,
            const CreateInfo& createInfo);

        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };
}

