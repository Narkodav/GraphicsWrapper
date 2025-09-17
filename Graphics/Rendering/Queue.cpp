#include "Queue.h"

namespace Graphics {

    Queue::Queue(const Device& device, uint32_t familyIndex, uint32_t queueIndex)
    {
        m_family = familyIndex;
        m_index = queueIndex;
        device.getFunctionTable().execute<DeviceFunction::GetDeviceQueue>(
			device.getHandle(), m_index, m_index, &m_queue);
    }

    void Queue::submit(const Device& device,
        const std::vector<PipelineStage::Flags>& waitStages,
        const std::vector<Semaphore>& waitSenaphores,
        const std::vector<CommandBuffer>& commandBuffers,
        const std::vector<Semaphore>& signalSemaphores,
        Fence& fence) const
    {
		vk::SubmitInfo submitInfo;
		submitInfo.waitSemaphoreCount = waitSenaphores.size();
		submitInfo.pWaitSemaphores = reinterpret_cast<const vk::Semaphore*>(waitSenaphores.data());
		submitInfo.pWaitDstStageMask = reinterpret_cast<const vk::PipelineStageFlags*>(waitStages.data());
		submitInfo.commandBufferCount = commandBuffers.size();
		submitInfo.pCommandBuffers = reinterpret_cast<const vk::CommandBuffer*>(commandBuffers.data());
		submitInfo.signalSemaphoreCount = signalSemaphores.size();
		submitInfo.pSignalSemaphores = reinterpret_cast<const vk::Semaphore*>(signalSemaphores.data());

        submit(device, &submitInfo, 1, fence);
    }

    void Queue::present(const Device& device,
        std::vector<Semaphore>& waitSemaphores,
        std::vector<SwapChain>& swapChains,
        std::vector<uint32_t>& imageIndices) const
    {
        std::vector<vk::SwapchainKHR> swapChainsRaw(swapChains.size());
        swapChainsRaw = convert<vk::SwapchainKHR>
            (swapChains, [](std::reference_wrapper<SwapChain> chain)
                { return chain.get().getSwapChain(); });

        vk::PresentInfoKHR presentInfo;
        presentInfo.swapchainCount = swapChains.size();
        presentInfo.pSwapchains = swapChainsRaw.data();
        presentInfo.pImageIndices = imageIndices.data();
        presentInfo.waitSemaphoreCount = waitSemaphores.size();
        presentInfo.pWaitSemaphores = reinterpret_cast<vk::Semaphore*>(waitSemaphores.data());

        present(device, presentInfo);
    }

    void Queue::submit(const Device& device,
        vk::SubmitInfo* submitInfo, size_t count,
        Fence& fence) const
    {
        auto result = device.getFunctionTable().execute<DeviceFunction::QueueSubmit>(
            m_queue, count, submitInfo, fence.getHandle());
        if (result != VK_SUCCESS)
            throw std::runtime_error("void Queue::submit(const Device& device, "
                "vk::SubmitInfo * submitInfo, size_t count, "
                "Fence & fence) const - Failed to submit to queue: " +
                s_resultMessages.at(result));
    }

    void Queue::present(const Device& device, vk::PresentInfoKHR& presentInfo) const
    {
        auto result = device.getFunctionTable().execute<DeviceFunction::QueuePresentKHR>(
            m_queue, &presentInfo);
        if (result != VK_SUCCESS)
            throw std::runtime_error("void Queue::present("
                "const Device & device, "
                "vk::PresentInfoKHR * presentInfo, "
                "size_t count) const - Failed to submit present to queue: " +
                s_resultMessages.at(result));
    }
}