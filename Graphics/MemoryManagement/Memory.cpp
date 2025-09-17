#include "Memory.h"

namespace Graphics {

    Memory::Memory(const Instance& instance, const Device& device,
        const vk::MemoryRequirements& memRequirements,
        MemoryProperty::Flags memoryProperties, size_t capacity) :
        m_memoryProperties(memoryProperties)
    {
        m_capacity = capacity;
        m_alignment = memRequirements.alignment;
        m_memoryTypeIndex = device.findMemoryType(instance, memRequirements.memoryTypeBits, memoryProperties);
        m_memory = device.allocateMemory(m_capacity, m_memoryTypeIndex);
    }

    void Memory::bindBuffer(const Device& device, const Buffer& buffer, size_t offset /*= 0*/) {
        auto result = device.getFunctionTable().execute<DeviceFunction::BindBufferMemory>(
			device.getHandle(), buffer.getHandle(), m_memory, offset);
        if (result != VK_SUCCESS) throw std::runtime_error(
            "void Memory::bindBuffer(const Device& device, const Buffer& buffer, size_t offset /*= 0*/)"
            "- failed to bind buffer memory: " +
            s_resultMessages.at(result));

#ifdef _DEBUG
        std::cout << "void Memory::bindBuffer(const Device& device, const Buffer& buffer, size_t offset /*= 0*/) - Bound buffer memory" << std::endl;
#endif
    }

    void Memory::bindImage(const Device& device, const Image& image, size_t offset /*= 0*/)
    {
        auto result = device.getFunctionTable().execute<DeviceFunction::BindImageMemory>(
            device.getHandle(), image.getHandle(), m_memory, offset);
        if (result != VK_SUCCESS) throw std::runtime_error(
            "void Memory::bindBuffer(const Device& device, const Buffer& buffer, size_t offset /*= 0*/)"
            "- failed to bind image memory: " +
            s_resultMessages.at(result));

#ifdef _DEBUG
        std::cout << "void Memory::bindImage(const Device& device, const Image& image, size_t offset /*= 0*/) - Bound image memory" << std::endl;
#endif
    }

}