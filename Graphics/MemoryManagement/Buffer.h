#pragma once
#include "../Common.h"
#include "../Flags.h"
#include "../Rendering/Instance.h"
#include "../Rendering/Device.h"
#include "../BufferDataLayouts.h"

namespace Graphics {

    class Buffer
    {
    public:
        class Descriptor
        {
        private:
            vk::BufferCreateInfo m_bufferInfo;

        public:

            operator vk::BufferCreateInfo() const { return m_bufferInfo; };

            Descriptor() {};
            Descriptor(size_t byteSize, BufferUsage::Flags usageFlags, bool allowConcurrentAccess = false)
            {
                m_bufferInfo.sType = vk::StructureType::eBufferCreateInfo;
                m_bufferInfo.size = byteSize;
                m_bufferInfo.usage = usageFlags;
                m_bufferInfo.sharingMode = allowConcurrentAccess ?
                    vk::SharingMode::eConcurrent :
                    vk::SharingMode::eExclusive;
            }

            Descriptor(Descriptor&&) noexcept = default;
            Descriptor& operator=(Descriptor&&) noexcept = default;

            Descriptor(const Descriptor&) noexcept = default;
            Descriptor& operator=(const Descriptor&) noexcept = default;

            ~Descriptor() = default;

            vk::BufferCreateInfo getBufferInfo() const { return m_bufferInfo; };
        };

    private:
        VkBuffer m_buffer = nullptr;
        vk::MemoryRequirements m_memRequirements;
        Descriptor m_descriptor;
        size_t m_capacity = 0;
    public:

        Buffer() {};
        Buffer(const Device& device, size_t byteSize,
            BufferUsage::Flags usageFlags, bool allowConcurrentAccess = false);

        Buffer(const Device& device, Descriptor descriptor);

        Buffer(Buffer&& other) noexcept {
            m_buffer = std::exchange(other.m_buffer, nullptr);
            m_memRequirements = std::exchange(other.m_memRequirements, vk::MemoryRequirements());
            m_descriptor = std::exchange(other.m_descriptor, {});
            m_capacity = std::exchange(other.m_capacity, 0);
        };

        //moving to an initialized device is undefined behavior, destroy before moving
        Buffer& operator=(Buffer&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(m_buffer == nullptr && "Buffer& Buffer::operator=(Buffer&& other) noexcept - Buffer already initialized");

            m_buffer = std::exchange(other.m_buffer, nullptr);
            m_memRequirements = std::exchange(other.m_memRequirements, vk::MemoryRequirements());
            m_descriptor = std::exchange(other.m_descriptor, {});
            m_capacity = std::exchange(other.m_capacity, 0);

            return *this;
        };

        Buffer(const Buffer&) noexcept = delete;
        Buffer& operator=(const Buffer&) noexcept = delete;

        ~Buffer() { assert(m_buffer == nullptr && "Buffer::~Buffer() - Buffer was not destroyed!"); };

        void destroy(const Device& device) {
            if (m_buffer == nullptr)
                return;

            device.getFunctionTable().execute<DeviceFunction::DestroyBuffer>(
                device.getHandle(), m_buffer, nullptr);

            m_buffer = nullptr;
            m_capacity = 0;
			m_memRequirements = vk::MemoryRequirements();
#ifdef _DEBUG
            std::cout << "void Buffer::destroy(const Device& device) - Destroyed buffer" << std::endl;
#endif
        }

        Descriptor getDescriptor() const { return m_descriptor; };

        const VkBuffer& getHandle() const { return m_buffer; };
        const VkMemoryRequirements& getMemoryRequirements() const { return m_memRequirements; };

        bool isInitialized() const { return m_buffer != nullptr; };

    };

}