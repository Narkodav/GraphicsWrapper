#pragma once
#include "../Common.h"
#include "Instance.h"
#include "Device.h"
#include "SwapChainFormat.h"
#include "Semaphore.h"
#include "Fence.h"
#include "CommandBuffer.h"
#include "CommandPool.h"

namespace Graphics {
    class Queue
    {
    private:
        VkQueue m_queue = nullptr;
        uint32_t m_family = 0;
        uint32_t m_index = 0;
    public:

        Queue() {};
        Queue(const Device& device, uint32_t familyIndex, uint32_t queueIndex);

        Queue(Queue&&) noexcept = default;
        Queue& operator=(Queue&&) noexcept = default;

        Queue(const Queue&) noexcept = default;
        Queue& operator=(const Queue&) noexcept = default;

        uint32_t getFamily() const { return m_family; }
        uint32_t getIndex() const { return m_index; }

        const VkQueue& getHandle() const { return m_queue; };

        void waitIdle(const Device& device) const {
            auto result = device.getFunctionTable().execute<DeviceFunction::QueueWaitIdle>(m_queue);
            if (result != VK_SUCCESS)
                throw std::runtime_error("Failed to wait for queue idle: " +
                    s_resultMessages.at(result));
        };

        void submit(const Device& device,
            const std::vector<PipelineStage::Flags>& waitStages,
            const std::vector<Semaphore>& waitSenaphores,
            const std::vector<CommandBuffer>& commandBuffers,
            const std::vector<Semaphore>& signalSemaphores,
            Fence& fence) const;

        void present(
            const Device& device,
            std::vector<Semaphore>& waitSemaphores,
            std::vector<SwapChain>& swapChains,
            std::vector<uint32_t>& imageIndices) const;

        void submit(const Device& device,
            vk::SubmitInfo* submitInfo, size_t count,
            Fence& fence) const;

        void present(const Device& device, vk::PresentInfoKHR& presentInfo) const;
    };

}