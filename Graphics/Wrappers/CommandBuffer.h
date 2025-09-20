#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "Buffer.h"

namespace Graphics
{
	class CommandBuffer : public BaseComponent<VkCommandBuffer, CommandBuffer>
	{
		using Base = BaseComponent<VkCommandBuffer, CommandBuffer>;
	public:
		using Base::Base;

        class InheritanceInfo : public StructBase<VkCommandBufferInheritanceInfo, InheritanceInfo>
        {
            using Base = StructBase<VkCommandBufferInheritanceInfo, InheritanceInfo>;
        public:
            using Base::Base;

            InheritanceInfo(const RenderPassRef& renderPass,
                uint32_t subpass, const FrameBuffer& framebuffer,
                bool occlusionQueryEnable, QueryControl::Flags queryControl,
                QueryPipelineStatistic::Flags queryPipelineStatistic) : Base()
            {
                this->renderPass = renderPass.getHandle();
                this->subpass = subpass;
                this->framebuffer = framebuffer;
                this->occlusionQueryEnable = occlusionQueryEnable;
                this->queryFlags = queryControl;
                this->pipelineStatistics = queryPipelineStatistic;
            }

            InheritanceInfo& setRenderPass(const RenderPassRef& renderPass)
            {
                this->renderPass = renderPass.getHandle();
                return *this;
            }

            InheritanceInfo& setSubpass(uint32_t subpass)
            {
                this->subpass = subpass;
                return *this;
            }

            InheritanceInfo& setFrameBuffer(const FrameBuffer& framebuffer)
            {
                this->framebuffer = framebuffer;
                return *this;
            }

            InheritanceInfo& setOcclusionQueryEnable(bool occlusionQueryEnable)
            {
                this->occlusionQueryEnable = occlusionQueryEnable;
                return *this;
            }

            InheritanceInfo& setQueryFlags(QueryControl::Flags flags)
            {
                this->queryFlags = flags;
                return *this;
            }

            InheritanceInfo& setPipelineStatistic(QueryPipelineStatistic::Flags flags)
            {
                this->pipelineStatistics = flags;
                return *this;
            }
        };

        class BeginInfo : public StructBase<VkCommandBufferBeginInfo, BeginInfo>
        {
            using Base = StructBase<VkCommandBufferBeginInfo, BeginInfo>;
        public:
            using Base::Base;

            BeginInfo(CommandBufferUsage::Flags flags = CommandBufferUsage::Bits::None) : Base()
            {
                this->flags = flags;
            }

            BeginInfo(const InheritanceInfo& inheritanceInfo,
                CommandBufferUsage::Flags flags = CommandBufferUsage::Bits::None) : Base()
            {
                this->pInheritanceInfo = InheritanceInfo::underlyingCast(&inheritanceInfo);
                this->flags = flags;
            }

            BeginInfo& setFlags(CommandBufferUsage::Flags flags)
            {
                this->flags = flags;
                return *this;
            }

            BeginInfo& setInheritanceInfo(const InheritanceInfo& inheritanceInfo)
            {
                this->pInheritanceInfo = InheritanceInfo::underlyingCast(&inheritanceInfo);
                return *this;
            }
        };

        void record(const DeviceFunctionTable& functions, const BeginInfo& beginInfo);
        void beginRenderPass(const DeviceFunctionTable& functions, const RenderPass::BeginInfo& beginInfo,
            SubpassContents subpassContents = SubpassContents::Inline);

        void bindPipeline(const DeviceFunctionTable& functions,
            const PipelineRef& pipeline, PipelineBindPoint bindPoint);

        template<typename BufferType>
        void bindVertexBuffers(const DeviceFunctionTable& functions, uint32_t firstBinding,
            std::span<const BufferType> buffers, std::span<const DeviceSize_t> offsets) requires
            std::is_same_v<BufferType, Buffer> || std::is_same_v<BufferType, BufferRef>
        {
            GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
            GRAPHICS_VERIFY(buffers.size() == offsets.size(), "Buffers and offsets sizes must match");
            functions.execute<DeviceFunction::CmdBindVertexBuffers>(getHandle(),
                firstBinding, buffers.size(), BufferType::underlyingCast(buffers.data()), offsets.data());
        }

        template<typename T, typename BufferType>
        inline void bindIndexBuffer(const DeviceFunctionTable& functions,
            const BufferType& buffer, size_t offset) requires
            std::is_same_v<BufferType, Buffer> || std::is_same_v<BufferType, BufferRef>
        {
            bindIndexBuffer(functions, buffer, offset, IndexTypeTraits_v<T>);
        }

        template<typename BufferType>
        void bindIndexBuffer(const DeviceFunctionTable& functions,
            const BufferType& buffer, size_t offset, IndexType indexType) requires
            std::is_same_v<BufferType, Buffer> || std::is_same_v<BufferType, BufferRef>
        {
            GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
            functions.execute<DeviceFunction::CmdBindIndexBuffer>(getHandle(),
                buffer.getBuffer(), offset, convertCEnum(indexType));
        }

        void bindDescriptorSets(const DeviceFunctionTable& functions, PipelineBindPoint pipelineBindPoint,
            const Pipeline::LayoutRef& pipelineLayout, uint32_t firstSet, std::span<const DescriptorSet> descriptorSets,
            std::span<const uint32_t> dynamicOffsets = {});

        void setPipelineBarrier(const DeviceFunctionTable& functions, PipelineStage::Flags srcStage,
            PipelineStage::Flags dstStage, Dependency::Flags dependencyFlags,
            std::span<const Memory::Barrier> memoryBarriers,
            std::span<const Buffer::MemoryBarrier> bufferMemoryBarriers,
            std::span<const Image::MemoryBarrier> imageMemoryBarriers);

        void copyBuffer(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
            const BufferRef& dstBuffer, std::span<const CopyRegion> copyRegions);

        void copyBufferToImage(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
            const ImageRef& dstImage, ImageLayout dstLayout, 
            std::span<const Buffer::ImageCopy> imageCopies);

        void blitImage(const DeviceFunctionTable& functions,
            const Image& srcImage, const Image& dstImage,
            ImageLayout srcLayout, ImageLayout dstLayout,
            std::span<const Image::Blit> blit, Filter filter = Filter::Linear);

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

        void setViewports(const DeviceFunctionTable& functions, uint32_t firstViewport,
            std::span<const Viewport> viewports);
        void setScissors(const DeviceFunctionTable& functions, uint32_t firstScissor,
            std::span<const Scissor> scissors);

        void draw(const DeviceFunctionTable& functions,
            size_t vertexCount, size_t instanceCount,
            size_t firstVertex, size_t firstInstance);

        void drawIndexed(const DeviceFunctionTable& functions,
            size_t indexCount, size_t instanceCount, size_t firstIndex,
            size_t vertexOffset, size_t firstInstance);

        void drawIndirect(const DeviceFunctionTable& functions, const BufferRef& buffer,
            DeviceSize_t offset, uint32_t drawCount, uint32_t stride);

        void endRenderPass(const DeviceFunctionTable& functions);
        void stopRecord(const DeviceFunctionTable& functions);
        void reset(const DeviceFunctionTable& functions,
            CommandBufferReset::Flags flags = CommandBufferReset::Bits::None);

        void dispatch(const DeviceFunctionTable& functions,
            uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);


	};
}

