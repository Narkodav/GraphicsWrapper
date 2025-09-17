#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Fence.h"
#include "Semaphore.h"
#include "CommandBuffer.h"
#include "SwapChain.h"

namespace Graphics
{
    class Queue : public BaseComponent<VkQueue, Queue>
    {
        using Base = BaseComponent<VkQueue, Queue>;
    public:
        using Base::Base;

        class SubmitInfo : public StructBase<VkSubmitInfo, SubmitInfo>
        {
            using Base = StructBase<VkSubmitInfo, SubmitInfo>;
        public:
            using Base::Base;

            template<typename SemaphoreT>
            SubmitInfo(const std::vector<CommandBuffer>& commandBuffers,
                const std::vector<PipelineStage::Flags>& waitStages = {},
                const std::vector<SemaphoreT>& waitSenaphores = {},
                const std::vector<SemaphoreT>& signalSemaphores = {}) requires
                std::is_convertible_v<SemaphoreT, Semaphore> ||
                std::is_convertible_v<SemaphoreT, SemaphoreRef> : Base() {
                this->commandBufferCount = commandBuffers.size();
                this->pCommandBuffers = CommandBuffer::underlyingCast(commandBuffers.data());
                this->signalSemaphoreCount = signalSemaphores.size();
                this->pSignalSemaphores = Semaphore::underlyingCast(signalSemaphores.data());
                this->waitSemaphoreCount = waitSenaphores.size();
                this->pWaitSemaphores = Semaphore::underlyingCast(waitSenaphores.data());
                this->pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>(waitStages.data());
            }

            template<typename SemaphoreT>
            SubmitInfo& setWaitStages(const std::vector<PipelineStage::Flags>& waitStages,
                const std::vector<SemaphoreT>& waitSenaphores) requires
                std::is_convertible_v<SemaphoreT, Semaphore> ||
                std::is_convertible_v<SemaphoreT, SemaphoreRef> {
                GRAPHICS_VERIFY(waitStages.size() == waitSenaphores.size(), "Wait stages and wait semaphores must have the same size");
                this->waitSemaphoreCount = waitSenaphores.size();
                this->pWaitSemaphores = Semaphore::underlyingCast(waitSenaphores.data());
                this->pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>(waitStages.data());
                return *this;
            }

            template<typename SemaphoreT>
            SubmitInfo& setSignalSemaphores(const std::vector<SemaphoreT>& signalSemaphores) requires
                std::is_convertible_v<SemaphoreT, Semaphore> ||
                std::is_convertible_v<SemaphoreT, SemaphoreRef> {
                this->signalSemaphoreCount = signalSemaphores.size();
                Semaphore::underlyingCast(signalSemaphores.data());
                return *this;
            }

            SubmitInfo& setCommandBuffers(const std::vector<CommandBuffer>& commandBuffers) {
                this->commandBufferCount = commandBuffers.size();
                this->pCommandBuffers = CommandBuffer::underlyingCast(commandBuffers.data());
                return *this;
            }
        };

        class PresentInfo : public StructBase<VkPresentInfoKHR, PresentInfo>
        {
            using Base = StructBase<VkPresentInfoKHR, PresentInfo>;
        public:
            using Base::Base;

            template<typename SemaphoreT, typename SwapChainT>
            PresentInfo(const std::vector<Semaphore>& waitSemaphores,
                const std::vector<SwapChain>& swapChains,
                const std::vector<uint32_t>& imageIndices) requires
                std::is_convertible_v<SemaphoreT, Semaphore> ||
                std::is_convertible_v<SemaphoreT, SemaphoreRef> &&
                std::is_convertible_v<SwapChainT, SwapChain> ||
                std::is_convertible_v<SwapChainT, SwapChainRef> : Base() {
                this->swapchainCount = swapChains.size();
                this->pSwapchains = SwapChain::underlyingCast(swapChains.data());
                this->pImageIndices = imageIndices.data();
                this->waitSemaphoreCount = waitSemaphores.size();
                this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
            }

            template<typename SemaphoreT>
            PresentInfo& setWaitSemaphores(const std::vector<SemaphoreT>& waitSemaphores) requires
                std::is_convertible_v<SemaphoreT, Semaphore> ||
                std::is_convertible_v<SemaphoreT, SemaphoreRef> {
                this->waitSemaphoreCount = waitSemaphores.size();
                this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
                return *this;
            }

            template<typename SwapChainT>
            PresentInfo& setSwapChains(const std::vector<SwapChainT>& swapChains,
                const std::vector<uint32_t>& imageIndices) requires
                std::is_convertible_v<SwapChainT, SwapChain> ||
                std::is_convertible_v<SwapChainT, SwapChainRef> {
                GRAPHICS_VERIFY(swapChains.size() == imageIndices.size(), "swap chains and image indices must have the same size");
                this->swapchainCount = swapChains.size();
                this->pSwapchains = SwapChain::underlyingCast(swapChains.data());
                this->pImageIndices = imageIndices.data();
                return *this;
            }
        };

        void waitIdle(const DeviceFunctionTable& functions) const {
            GRAPHICS_VERIFY(!isSet(), "Cannot wait for queue idle for an unset queue");
            auto result = functions.execute<DeviceFunction::QueueWaitIdle>(getHandle());
            GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to wait for queue idle: " +
                s_resultMessages.at(result));
        };

        void submit(const DeviceFunctionTable& functions,
            const std::vector<SubmitInfo>& submitInfos,
            const FenceRef& fence) const;

        void present(const DeviceFunctionTable& functions, const PresentInfo& presentInfo) const;
    };
}