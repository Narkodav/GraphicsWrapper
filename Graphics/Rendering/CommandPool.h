#pragma once
#include "../Common.h"
#include "Instance.h"
#include "Device.h"
#include "../PlatformManagement/Window.h"
#include "../MemoryManagement/MappedMemory.h"
#include "../MemoryManagement/PixelData.h"

namespace Graphics {

    class Queue;
    class CommandBuffer;

    class CommandPool
    {
    private:
        vk::CommandPool m_pool;

        bool m_initialized = false;
    public:
        CommandPool() : m_pool(nullptr), m_initialized(false) {};

        CommandPool(const Instance& instance, const Device& device, uint32_t queueFamilyIndex);

        CommandPool(CommandPool&& other) noexcept {
            m_pool = std::exchange(other.m_pool, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);
        };

        //moving to an initialized swap chain is undefined behavior, destroy before moving
        CommandPool& operator=(CommandPool&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(!m_initialized && "CommandPool::operator=() - CommandPool already initialized");

            m_pool = std::exchange(other.m_pool, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);

            return *this;
        };

        CommandPool(const CommandPool&) noexcept = delete;
        CommandPool& operator=(const CommandPool&) noexcept = delete;

        ~CommandPool() {
            assert(!m_initialized && "CommandPool was not destroyed!");
        };

        void destroy(const Instance& instance, const Device& device) {
            if (!m_initialized)
                return;

            device.getDevice().destroyCommandPool(m_pool, nullptr, instance.getDispatchLoader());
#ifdef _DEBUG
            std::cout << "Destroyed CommandPool" << std::endl;
#endif
            m_initialized = false;
        };

        // Reset the entire pool (faster than freeing individual buffers)
        void reset(const Instance& instance, const Device& device) {
            device.getDevice().resetCommandPool(m_pool, vk::CommandPoolResetFlags(), instance.getDispatchLoader());
        }

        void makeOneTimeSubmit(const Instance& instance, const Device& device,
            const Queue& queue, std::function<void(CommandBuffer&)>&& func);

        template <typename Type, typename Data>
        void makeOneTimeDataTransfer(const Instance& instance, const Device& device,
            const Queue& queue, MappedMemory& stagingMemory, const Buffer& stagingBuffer,
            const Buffer& dstBuffer, const Data& data, uint32_t dstOffset = 0)
        {
            if constexpr (std::is_same_v<Type, Data>)
            {
                makeOneTimeSubmit(instance, device, queue,
                    [this, &instance, &stagingMemory, &stagingBuffer, &dstBuffer, &dstOffset, &data]
                    (auto& temporary) {
                        auto mappedMemory = stagingMemory.getMapping<Type>(1);
                        mappedMemory[0] = data;
                        temporary.transferBufferData(instance, stagingBuffer,
                            dstBuffer, CopyRegion(0, dstOffset, sizeof(Type)));
                    });
            }
            else
            {
                makeOneTimeSubmit(instance, device, queue,
                    [this, &instance, &stagingMemory, &stagingBuffer, &dstBuffer, &dstOffset, &data]
                    (auto& temporary) {
                        auto mappedMemory = stagingMemory.getMapping<Type>(data.size());
                        std::copy(data.begin(), data.end(), mappedMemory.begin());
                        temporary.transferBufferData(instance, stagingBuffer,
                            dstBuffer, CopyRegion(0, dstOffset, data.size() * sizeof(Type)));
                    });
            }
        }

        void makeOneTimeImageDataTransfer(const Instance& instance, const Device& device,
            const Queue& queue, MappedMemory& stagingMemory, const Buffer& stagingBuffer,
            Image& image, const PixelData& data, uint32_t srcOffset = 0,
            Offset3D dstImageOffset = Offset3D(),
            ImageLayout startLayout = ImageLayout::Undefined,
            ImageLayout finalLayout = ImageLayout::ShaderReadOnlyOptimal,
            PipelineStage::Flags srcStage = PipelineStage::Bits::TopOfPipe,
            PipelineStage::Flags dstStage = PipelineStage::Bits::FragmentShader,
            Access::Flags srcAccess = Access::Bits::None,
            Access::Flags dstAccess = Access::Bits::ShaderRead);

        const vk::CommandPool& getPool() const { return m_pool; };

        void makeOneTimeImageMipMaps(const Instance& instance,
            const Device& device, const Queue& queue, Image& image,
            ImageLayout startLayout = ImageLayout::Undefined,
            ImageLayout finalLayout = ImageLayout::ShaderReadOnlyOptimal,
            PipelineStage::Flags srcStage = PipelineStage::Bits::TopOfPipe,
            PipelineStage::Flags dstStage = PipelineStage::Bits::FragmentShader,
            Access::Flags srcAccess = Access::Bits::None,
            Access::Flags dstAccess = Access::Bits::ShaderRead);
    };
}