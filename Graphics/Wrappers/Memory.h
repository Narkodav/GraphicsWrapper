#pragma once
#include "../Common.h"
#include "../Structs.h"
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
        static inline const std::string s_typeName = "Memory";

        void bindBuffer(const DeviceRef& device, const DeviceFunctionTable& functions,
            const BufferRef& buffer, size_t offset = 0);
        void bindImage(const DeviceRef& device, const DeviceFunctionTable& functions,
            const ImageRef& image, size_t offset = 0);
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

            std::span<const Memory::Type> getMemoryTypes() const {
                return std::span<const Memory::Type>(
                    reinterpret_cast<const Memory::Type*>(memoryTypes), memoryTypeCount);
            };

            std::span<const Memory::Heap> getMemoryHeaps() const {
                return std::span<const Memory::Heap>(
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

        class Barrier : public StructBase<VkMemoryBarrier, Barrier>
        {
            using Base = StructBase<VkMemoryBarrier, Barrier>;
        public:
            using Base::Base;
            Barrier(Access::Flags srcAccessMask, Access::Flags dstAccessMask) : Base() {
                this->srcAccessMask = srcAccessMask;
                this->dstAccessMask = dstAccessMask;
            }
            Barrier& setSrcAccessMask(Access::Flags srcAccessMask) {
                this->srcAccessMask = srcAccessMask;
                return *this;
            }
            Barrier& setDstAccessMask(Access::Flags dstAccessMask) {
                this->dstAccessMask = dstAccessMask;
                return *this;
            }
        };

        class Mapping {
            friend class Memory;
        private:
            void* m_mapping = nullptr;

#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
            MemoryRef m_owner = nullptr;
#endif
            Mapping(void* mapping) :
                m_mapping(mapping) {
            };

            void invalidate() {
                m_mapping = nullptr;
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
                m_owner = nullptr;
#endif  
            }

        public:
            static inline const DeviceSize_t s_wholeSize = VK_WHOLE_SIZE;

            Mapping() = default;
            Mapping(Mapping&&) = default;
            Mapping& operator=(Mapping&& other)
            {
                if (this == &other)
                    return;
                GRAPHICS_VERIFY(!isValid(), "Mapping was not unmapped before being moved to");
                m_mapping = std::exchange(other.m_mapping, nullptr);
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
                m_owner = std::exchange(other.m_owner, nullptr);
#endif
            }
            Mapping(const Mapping&) = delete;
            Mapping& operator=(const Mapping&) = delete;
            ~Mapping() {
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
                GRAPHICS_VERIFY(!isValid(), "Mapping was not unmapped before being destroyed, "
                    "memory handle: " + std::to_string(
                        reinterpret_cast<uintptr_t>(std::as_const(m_owner).getHandle())));
#endif
            }

            void unmap(const DeviceFunctionTable& functions, const DeviceRef& device,
                const MemoryRef& memory) {
                GRAPHICS_VERIFY(isValid(), "Mapping was not mapped before being unmapped");
                functions.execute<DeviceFunction::UnmapMemory>(device, memory);
            }

            bool isValid() const {
                return m_mapping != nullptr;
            }

            template<typename T>
            std::span<T> get(size_t offset, size_t size)
            {
                return std::span<T>(reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(m_mapping)
                    + offset), size);
            }

            template<typename T>
            const std::span<const T> get(size_t offset, size_t size) const
            {
                return std::span<const T>(reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(m_mapping)
                    + offset), size);
            }

            template<typename T>
            T& get(size_t offset)
            {
                return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(m_mapping) + offset);
            }

            template<typename T>
            const T& get(size_t offset) const
            {
                return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(m_mapping) + offset);
            }
        };

        void create(const DeviceRef& device, const DeviceFunctionTable& functions,
            const AllocateInfo& allocInfo);

        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);

        Mapping map(const DeviceFunctionTable& functions, const DeviceRef& device,
            DeviceSize_t offset = 0, DeviceSize_t size = Mapping::s_wholeSize,
            MemoryMap::Flags flags = MemoryMap::Bits::None)
        {
            GRAPHICS_VERIFY(isValid(), "Memory was not created before being mapped");
            void* mapping = nullptr;
            auto result = functions.execute<DeviceFunction::MapMemory>(device, getHandle(),
                offset, size, convertCEnum(flags), &mapping);
            GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to map memory: " +
                s_resultMessages.at(result));
            return Mapping(mapping);
        }

        void unmap(const DeviceFunctionTable& functions, const DeviceRef& device,
            Mapping& mapping) {
#ifdef _DEBUG && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
            GRAPHICS_VERIFY(mapping.m_owner == getHandle(),
                "Unmapping memory that was not mapped by this handle");
#endif
            GRAPHICS_VERIFY(isValid(), "Memory was not created before being unmapped");
            functions.execute<DeviceFunction::UnmapMemory>(device, getHandle());
            mapping.invalidate();
        }
    };



}

