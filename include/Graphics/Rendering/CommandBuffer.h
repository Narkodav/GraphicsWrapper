#pragma once
#include "../Common.h"
#include "Instance.h"
#include "Device.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "ComputePipeline.h"
#include "RenderRegion.h"
#include "CommandPool.h"
#include "../Flags.h"
#include "../MemoryManagement/Buffer.h"
#include "../MemoryManagement/Image.h"
#include "../MemoryManagement/DescriptorSet.h"
#include "../MemoryManagement/DescriptorPool.h"
#include "../Enums.h"

namespace Graphics {

    class CommandPool;

    class CommandBuffer
    {
    private:
        VkCommandBuffer m_commandBuffer = nullptr;

    public:
		operator VkCommandBuffer() { return m_commandBuffer; };

        CommandBuffer() = default;

        CommandBuffer(const Instance& intsance, const Device& device, const CommandPool& pool,
            vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

        CommandBuffer(CommandBuffer&& other) noexcept {
            m_commandBuffer = std::exchange(other.m_commandBuffer, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);
        };

        //moving to an initialized swap chain is undefined behavior, destroy before moving
        CommandBuffer& operator=(CommandBuffer&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(!m_initialized && "CommandBuffer::operator=() - CommandBuffer already allocated");

            m_commandBuffer = std::exchange(other.m_commandBuffer, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);

            return *this;
        };

        CommandBuffer(const CommandBuffer&) noexcept = delete;
        CommandBuffer& operator=(const CommandBuffer&) noexcept = delete;

        ~CommandBuffer() { assert(!m_initialized && "CommandBuffer was not deallocated!"); };

        void destroy(const Instance& instance, const Device& device, const CommandPool& pool)
        {
            if (!m_initialized)
                return;

            device.freeCommandBuffer(instance, pool.getPool(), m_commandBuffer);
#ifdef _DEBUG
            std::cout << "Destroyed CommandBuffer" << std::endl;
#endif
            m_initialized = false;
        }

        void record(const Instance& instance, CommandBufferUsage::Flags flags = 0);
        void beginRenderPass(const Instance& instance, const RenderPass& renderPass,
            const SwapChain& swapChain, uint32_t imageIndex, Color clearColor);

        void beginRenderPass(const Instance& instance, const RenderPass& renderPass,
            const SwapChain& swapChain, uint32_t imageIndex, Color clearColor, float clearDepth);

        template<typename PipelineType>
        void bindPipeline(const Instance& instance, const PipelineType& pipeline)
        {
            try {
                m_commandBuffer.bindPipeline(pipeline.getBindPoint(), pipeline.getPipeline(), instance.getDispatchLoader());
            }
            catch (const vk::SystemError& e) {
                throw std::runtime_error("failed to bind graphics pipeline: " + std::string(e.what()));
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Unexpected error when binding graphics pipeline: " + std::string(e.what()));
            }
        }
        template<size_t bufferAmount>
        void bindVertexBuffers(const Instance& instance,
            std::array<const Buffer*, bufferAmount> buffers,
            std::array<size_t, bufferAmount> offsets,
            uint32_t firstBinding = 0)
        {
            auto rawBuffers = convert<vk::Buffer>
                (buffers, [](const Buffer* buffer)
                    { return buffer->getBuffer(); });

            m_commandBuffer.bindVertexBuffers(firstBinding, rawBuffers.size(), rawBuffers.data(),
                offsets.data(), instance.getDispatchLoader());
        }

        void bindIndexBuffer(const Instance& instance,
            const Buffer& buffer, size_t offset);

        template<typename PipelineType>
        void bindDescriptorSets(const Instance& instance,
            const PipelineType& pipeline, const std::vector<DescriptorSet*>& descriptorSets,
            const std::vector<uint32_t>& dynamicOffsets = {})
        {
            auto descriptorSetsRaw = convert<vk::DescriptorSet>
                (descriptorSets, [](DescriptorSet* set)
                    { return set->getSet(); });

            m_commandBuffer.bindDescriptorSets(pipeline.getBindPoint(),
                pipeline.getLayout(), 0, descriptorSetsRaw, dynamicOffsets,
                instance.getDispatchLoader());
        }

        void setPipelineBarrier(const Instance& instance,
            PipelineStage::Flags srcStage, PipelineStage::Flags dstStage,
            Image& image, ImageLayout oldLayout, ImageLayout newLayout,
            Access::Flags srcAccess, Access::Flags dstAccess,
            uint32_t baseMipLevel = 0, uint32_t levelCount = 1);

        void setPipelineBarrier(const Instance& instance,
            PipelineStage::Flags srcStage, PipelineStage::Flags dstStage,
            Buffer& buffer, Access::Flags srcAccess,
            Access::Flags dstAccess);

        void transferBufferData(const Instance& instance, const Buffer& srcBuffer,
            const Buffer& dstBuffer, const CopyRegion& copyRegion);

        void transferImageData(const Instance& instance, const Buffer& srcBuffer,
            Image& dstImage, ImageLayout dstLayout, Extent3D imageExtent, size_t offset /*= 0*/,
            Offset3D imageOffset /*= Offset3D()*/);

        void blitImage(const Instance& instance,
            const Image& srcImage, const Image& dstImage,
            ImageLayout srcLayout, ImageLayout dstLayout,
            Offset3D srcOffsetMin, Offset3D srcOffsetMax,
            Offset3D dstOffsetMin, Offset3D dstOffsetMax,
            uint32_t srcMipLevel = 1, uint32_t dstMipLevel = 1,
            ImageAspect::Flags srcAspect = ImageAspect::Bits::Color, 
            ImageAspect::Flags dstAspect = ImageAspect::Bits::Color,
            Filter filter = Filter::Linear);

        template<typename PipelineType>
        void pushConstants(const Instance& instance, const PipelineType& pipeline,
            ShaderStage::Flags stageFlags, size_t offset, size_t size, const void* values)
        {
            try {
                m_commandBuffer.pushConstants(pipeline.getLayout(), stageFlags,
                    offset, size, values, instance.getDispatchLoader());
            }
            catch (const vk::SystemError& e) {
                throw std::runtime_error("Failed to write push constants: " + std::string(e.what()));
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Unexpected error when writing push constants: " + std::string(e.what()));
            }
        };

        void setRenderView(const Instance& instance, const RenderRegion& canvas);
        void draw(const Instance& instance,
            size_t vertexCount, size_t instanceCount, size_t firstVertex, size_t firstInstance);
        void drawIndexed(const Instance& instance,
            size_t indexCount, size_t instanceCount, size_t firstIndex, size_t indexIncrement, size_t firstInstance);

        void endRenderPass(const Instance& instance);
        void stopRecord(const Instance& instance);
        void reset(const Instance& instance);

        void dispatch(const Instance& instance, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

        void drawIndirect(const Instance& instance, const Buffer& buffer,
            vk::DeviceSize offset, uint32_t drawCount, uint32_t stride);

        bool isValid() const {
            return m_initialized;
        }

        vk::CommandBuffer getCommandBuffer() const { return m_commandBuffer; };

        friend class CommandPool;
    };

}