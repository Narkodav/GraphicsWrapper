#pragma once
#include "../Common.h"
#include "../Flags.h"
#include "../Rendering/Device.h"
#include "../Rendering/Instance.h"
#include "Buffer.h"
#include "Image.h"

namespace Graphics {

    class Memory
    {
    private:
        VkDeviceMemory m_memory = nullptr;
        MemoryProperty::Flags m_memoryProperties;
        size_t m_capacity = 0;
        size_t m_alignment = 0;
        uint32_t m_memoryTypeIndex = 0;
    public:

        Memory() noexcept = default;

        Memory(const Instance& instance, const Device& device,
            const vk::MemoryRequirements& memRequirements,
            MemoryProperty::Flags memoryProperties, size_t capacity);

        Memory(Memory&& other) noexcept {
            m_memory = std::exchange(other.m_memory, nullptr);
            m_memoryProperties = std::exchange(other.m_memoryProperties, 0);
            m_capacity = std::exchange(other.m_capacity, 0);
            m_alignment = std::exchange(other.m_alignment, 0);
            m_memoryTypeIndex = std::exchange(other.m_memoryTypeIndex, 0);
        };

        Memory& operator=(Memory&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(m_memory == nullptr && "Memory& Memory::operator=(Memory&& other) noexcept - Memory already initialized");

            m_memory = std::exchange(other.m_memory, nullptr);
            m_memoryProperties = std::exchange(other.m_memoryProperties, 0);
            m_capacity = std::exchange(other.m_capacity, 0);
            m_alignment = std::exchange(other.m_alignment, 0);
            m_memoryTypeIndex = std::exchange(other.m_memoryTypeIndex, 0);

            return *this;
        };

        Memory(const Memory&) noexcept = delete;
        Memory& operator=(const Memory&) noexcept = delete;

        ~Memory() { assert(m_memory == nullptr && "Memory::~Memory() - Memory was not destroyed!"); };

        void destroy(const Device& device) {
            if (m_memory == nullptr)
                return;
            device.getFunctionTable().execute<DeviceFunction::FreeMemory>(
                device.getHandle(), m_memory, nullptr);
            m_memory = nullptr;
#ifdef _DEBUG
            std::cout << "void Memory::destroy(const Device& device) - Freed memory" << std::endl;
#endif
        }

        void bindBuffer(const Device& device, const Buffer& buffer, size_t offset = 0);
        void bindImage(const Device& device, const Image& image, size_t offset = 0);
        VkDeviceMemory getHandle() { return m_memory; };

        size_t getCapacity() const { return m_capacity; };
        size_t getAlignment() const { return m_alignment; };
    };

}