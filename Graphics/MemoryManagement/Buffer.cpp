#include "Buffer.h"

namespace Graphics {

    Buffer::Buffer(const Device& device, size_t byteSize,
        BufferUsage::Flags usageFlags, bool allowConcurrentAccess /*= false*/) :
        m_capacity(byteSize)
    {
        m_descriptor = Descriptor(byteSize, usageFlags, allowConcurrentAccess);

        auto result = device.getFunctionTable().execute<DeviceFunction::CreateBuffer>(
            device.getHandle(), static_cast<vk::BufferCreateInfo>(m_descriptor), nullptr, &m_buffer);

        if (result != VK_SUCCESS) throw std::runtime_error("Buffer::Buffer(const Device& device, size_t byteSize, "
            "BufferUsage::Flags usageFlags, bool allowConcurrentAccess /*= false*/) - failed to create buffer: " +
            s_resultMessages.at(result));

        device.getFunctionTable().execute<DeviceFunction::GetBufferMemoryRequirements>(
            device.getHandle(), m_buffer, &m_memRequirements);
    }

    Buffer::Buffer(const Device& device, Descriptor descriptor) :
        m_descriptor(descriptor), m_capacity(m_descriptor.getBufferInfo().size)
    {
        m_capacity = m_descriptor.getBufferInfo().size;

        auto result = device.getFunctionTable().execute<DeviceFunction::CreateBuffer>(
            device.getHandle(), static_cast<vk::BufferCreateInfo>(m_descriptor), nullptr, &m_buffer);

        if (result != VK_SUCCESS) throw std::runtime_error("Buffer::Buffer(const Device& device, size_t byteSize, "
            "BufferUsage::Flags usageFlags, bool allowConcurrentAccess /*= false*/) - failed to create buffer: " +
            s_resultMessages.at(result));

        device.getFunctionTable().execute<DeviceFunction::GetBufferMemoryRequirements>(
            device.getHandle(), m_buffer, &m_memRequirements);
    }
}