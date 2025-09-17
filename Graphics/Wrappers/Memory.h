#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"

namespace Graphics
{
    class DeviceRef;
	class BufferRef;
	class ImageRef;

    class MemoryRef : public BaseComponent<VkDeviceMemory, MemoryRef>
    {
        using Base = BaseComponent<VkDeviceMemory, MemoryRef>;
    public:
        using Base::Base;
    };

    class Memory : public VerificatorComponent<VkDeviceMemory, MemoryRef>
    {
        using Base = VerificatorComponent<VkDeviceMemory, MemoryRef>;
    public:
        using Base::Base;
        
		static inline const size_t s_maxMemoryTypes = VK_MAX_MEMORY_TYPES;
        static inline const size_t s_maxMemoryHeaps = VK_MAX_MEMORY_HEAPS;

        class Type : public StructBase<VkMemoryType, Type>
        {
            using Base = StructBase<VkMemoryType, Type>;
        public:
            using Base::Base;
            MemoryProperty::Flags getPropertyFlags() const {
                return static_cast<MemoryProperty::Flags>(this->propertyFlags);
            };
            uint32_t getHeapIndex() const {
                return this->heapIndex;
            };
        };

        class Heap : public StructBase<VkMemoryHeap, Heap>
        {
            using Base = StructBase<VkMemoryHeap, Heap>;
        public:
            using Base::Base;
            MemoryHeap::Flags getFlags() const {
                return static_cast<MemoryHeap::Flags>(this->flags);
            };
            VkDeviceSize getSize() const {
                return this->size;
            };
        };

        class Properties : public StructBase<VkPhysicalDeviceMemoryProperties, Properties>
        {
            using Base = StructBase<VkPhysicalDeviceMemoryProperties, Properties>;
        public:
            using Base::Base;

            uint32_t getMemoryTypeCount() const {
                return memoryTypeCount;
            };

            uint32_t getMemoryHeapCount() const {
                return memoryHeapCount;
            };

            std::span<const Memory::Type, s_maxMemoryTypes> getMemoryTypes() const {
                return std::span<const Memory::Type, s_maxMemoryTypes>(
                    reinterpret_cast<const Memory::Type*>(memoryTypes), memoryTypeCount);
            };

            std::span<const Memory::Heap, s_maxMemoryHeaps> getMemoryHeaps() const {
                return std::span<const Memory::Heap, s_maxMemoryHeaps>(
                    reinterpret_cast<const Memory::Heap*>(memoryHeaps), memoryHeapCount);
            };
        };

        class AllocateInfo : public StructBase<VkMemoryAllocateInfo, AllocateInfo>
        {
            using Base = StructBase<VkMemoryAllocateInfo, AllocateInfo>;
        public:
            using Base::Base;
            AllocateInfo(size_t allocationSize, uint32_t memoryTypeIndex) : Base()
            {
                this->allocationSize = static_cast<VkDeviceSize>(allocationSize);
                this->memoryTypeIndex = memoryTypeIndex;
            }
            AllocateInfo& setAllocationSize(size_t allocationSize) {
                this->allocationSize = static_cast<VkDeviceSize>(allocationSize);
                return *this;
            }
            AllocateInfo& setMemoryTypeIndex(uint32_t memoryTypeIndex) {
                this->memoryTypeIndex = memoryTypeIndex;
                return *this;
            }
            size_t getAllocationSize() const {
                return static_cast<size_t>(this->allocationSize);
            }
            uint32_t getMemoryTypeIndex() const {
                return this->memoryTypeIndex;
            }
        };

        void create(const DeviceRef& device, const DeviceFunctionTable& functions,
            const AllocateInfo& allocInfo);

        void bindBuffer(const DeviceRef& device, const DeviceFunctionTable& functions,
            const BufferRef& buffer, size_t offset = 0);
        void bindImage(const DeviceRef& device, const DeviceFunctionTable& functions,
            const ImageRef& image, size_t offset = 0);

        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };

}

