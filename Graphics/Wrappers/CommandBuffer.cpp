#include "CommandBuffer.h"

namespace Graphics {

	void CommandBuffer::record(const DeviceFunctionTable& functions, const BeginInfo& beginInfo)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::BeginCommandBuffer>(getHandle(),
			beginInfo.getUnderlyingPointer());
	}

	void CommandBuffer::beginRenderPass(const DeviceFunctionTable& functions,
		const RenderPass::BeginInfo& beginInfo, SubpassContents subpassContents
	/*= SubpassContents::Inline*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdBeginRenderPass>(getHandle(),
			beginInfo.getUnderlyingPointer(), convertCEnum(subpassContents));
	}

	void CommandBuffer::bindPipeline(const DeviceFunctionTable& functions, const PipelineRef& pipeline,
		PipelineBindPoint bindPoint)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdBindPipeline>(getHandle(),
			convertCEnum(bindPoint), pipeline.getHandle());
	}

	void CommandBuffer::bindDescriptorSets(const DeviceFunctionTable& functions, PipelineBindPoint pipelineBindPoint,
		const Pipeline::LayoutRef& pipelineLayout, uint32_t firstSet, std::span<const DescriptorSet> descriptorSets,
		std::span<const uint32_t> dynamicOffsets /*= {}*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdBindDescriptorSets>(getHandle(),
			pipelineBindPoint, pipelineLayout.getHandle(), firstSet, descriptorSets.size(),
			DescriptorSet::underlyingCast(descriptorSets.data()),
			dynamicOffsets.size(), dynamicOffsets.data());
	}

	void CommandBuffer::setPipelineBarrier(const DeviceFunctionTable& functions, PipelineStage::Flags srcStage,
		PipelineStage::Flags dstStage, Dependency::Flags dependencyFlags,
		std::span<const Memory::Barrier> memoryBarriers,
		std::span<const Buffer::MemoryBarrier> bufferMemoryBarriers,
		std::span<const Image::MemoryBarrier> imageMemoryBarriers)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdPipelineBarrier>(srcStage, dstStage, dependencyFlags,
			memoryBarriers.size(), memoryBarriers.data(), bufferMemoryBarriers.size(),
			bufferMemoryBarriers.data(), imageMemoryBarriers.size(), imageMemoryBarriers.data()); 
	}

	void CommandBuffer::copyBuffer(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
		const BufferRef& dstBuffer, std::span<const CopyRegion> copyRegions)
	{ 
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdCopyBuffer>(getHandle(), srcBuffer.getHandle(),
			dstBuffer.getHandle(), copyRegions.size(), CopyRegion::underlyingCast(copyRegions.data()));
	} 

	void CommandBuffer::copyBufferToImage(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
		const ImageRef& dstImage, ImageLayout dstLayout, 
		std::span<const Buffer::ImageCopy> imageCopies)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdCopyBufferToImage>(getHandle(), srcBuffer.getHandle(),
			dstImage.getHandle(), convertCEnum(dstLayout), imageCopies.size(),
			Buffer::ImageCopy::underlyingCast(imageCopies.data()));
	}

	void CommandBuffer::setViewports(const DeviceFunctionTable& functions, uint32_t firstViewport,
		std::span<const Viewport> viewports)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdSetViewport>(getHandle(), firstViewport,
			viewports.size(), Viewport::underlyingCast(viewports.data()));
	}

	void CommandBuffer::setScissors(const DeviceFunctionTable& functions, uint32_t firstScissor,
		std::span<const Scissor> scissors)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdSetScissor>(getHandle(), firstScissor,
			scissors.size(), Scissor::underlyingCast(scissors.data()));
	}

	void CommandBuffer::draw(const DeviceFunctionTable& functions,
		size_t vertexCount, size_t instanceCount,
		size_t firstVertex, size_t firstInstance)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdDraw>(getHandle(),
			vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::drawIndexed(const DeviceFunctionTable& functions,
		size_t indexCount, size_t instanceCount, size_t firstIndex,
		size_t vertexOffset, size_t firstInstance)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdDrawIndexed>(getHandle(),
			indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CommandBuffer::drawIndirect(const DeviceFunctionTable& functions, const BufferRef& buffer,
		DeviceSize_t offset, uint32_t drawCount, uint32_t stride)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdDrawIndirect>(getHandle(), buffer,
			offset, drawCount, stride);
	}

	void CommandBuffer::dispatch(const DeviceFunctionTable& functions,
		uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdDispatch>(getHandle(), groupCountX,
			groupCountY, groupCountZ);
	}

	void CommandBuffer::endRenderPass(const DeviceFunctionTable& functions)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdEndRenderPass>(getHandle());
	}

	void CommandBuffer::stopRecord(const DeviceFunctionTable& functions)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::EndCommandBuffer>(getHandle());
	}

	void CommandBuffer::reset(const DeviceFunctionTable& functions,
		CommandBufferReset::Flags flags /*= CommandBufferReset::Bits::None*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::ResetCommandBuffer>(getHandle(), flags);
	}

	void CommandBuffer::blitImage(const DeviceFunctionTable& functions,
		const Image& srcImage, const Image& dstImage,
		ImageLayout srcLayout, ImageLayout dstLayout,
		std::span<const Image::Blit> blit, Filter filter /*= Filter::Linear*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdBlitImage>(getHandle(), srcImage,
			convertCEnum(srcLayout), dstImage.getHandle(), convertCEnum(dstLayout), blit.size(),
			Image::Blit::underlyingCast(blit.data()), convertCEnum(filter));
	}
}