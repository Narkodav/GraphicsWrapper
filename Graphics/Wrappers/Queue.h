#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Fence.h"
#include "Semaphore.h"
#include "CommandBuffer.h"
#include "SwapChain.h"

namespace Graphics
{
    class Queue : public BaseComponent<VkQueue, Queue>
    {
        friend class DeviceRef;
        friend class Device;
        using Base = BaseComponent<VkQueue, Queue>;
    public:
        using Base::Base;

        class SubmitInfo : public StructBase<VkSubmitInfo, SubmitInfo>
        {
            using Base = StructBase<VkSubmitInfo, SubmitInfo>;
        public:
            using Base::Base;

            template<typename SemaphoreT>
            SubmitInfo(std::span<const CommandBuffer> commandBuffers,
                std::span<const PipelineStage::Flags> waitStages = {},
                std::span<const SemaphoreT> waitSenaphores = {},
                std::span<const SemaphoreT> signalSemaphores = {}) requires
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

            template<typename SemaphoreType>
            SubmitInfo& setWaitStages(std::span<const PipelineStage::Flags> waitStages,
                std::span<const SemaphoreType> waitSenaphores) requires
                std::is_convertible_v<SemaphoreType, Semaphore> ||
                std::is_convertible_v<SemaphoreType, SemaphoreRef> {
                GRAPHICS_VERIFY(waitStages.size() == waitSenaphores.size(), "Wait stages and wait semaphores must have the same size");
                this->waitSemaphoreCount = waitSenaphores.size();
                this->pWaitSemaphores = Semaphore::underlyingCast(waitSenaphores.data());
                this->pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>(waitStages.data());
                return *this;
            }

            template<typename SemaphoreType>
            SubmitInfo& setSignalSemaphores(std::span<const SemaphoreType> signalSemaphores) requires
                std::is_convertible_v<SemaphoreType, Semaphore> ||
                std::is_convertible_v<SemaphoreType, SemaphoreRef> {
                this->signalSemaphoreCount = signalSemaphores.size();
                Semaphore::underlyingCast(signalSemaphores.data());
                return *this;
            }

            SubmitInfo& setCommandBuffers(std::span<const CommandBuffer> commandBuffers) {
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
            PresentInfo(std::span<const Semaphore> waitSemaphores,
                std::span<const SwapChain> swapChains,
                std::span<const uint32_t> imageIndices) requires
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
            PresentInfo& setWaitSemaphores(std::span<const SemaphoreT> waitSemaphores) requires
                std::is_convertible_v<SemaphoreT, Semaphore> ||
                std::is_convertible_v<SemaphoreT, SemaphoreRef> {
                this->waitSemaphoreCount = waitSemaphores.size();
                this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
                return *this;
            }

            template<typename SwapChainT>
            PresentInfo& setSwapChains(std::span<const SwapChainT> swapChains,
                std::span<const uint32_t> imageIndices) requires
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
            std::span<const SubmitInfo> submitInfos,
            const FenceRef& fence) const;

        void present(const DeviceFunctionTable& functions, const PresentInfo& presentInfo) const;
    };
}