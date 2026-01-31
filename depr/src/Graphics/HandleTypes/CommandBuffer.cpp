#include "../../../include/Graphics/Graphics.h"

namespace Graphics {

	void CommandBuffer::beginRenderPass(const DeviceFunctionTable& functions,
		const RenderPassBeginInfo& beginInfo, SubpassContents subpassContents
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
		const PipelineLayoutRef& pipelineLayout, uint32_t firstSet, std::span<const DescriptorSet> descriptorSets,
		std::span<const uint32_t> dynamicOffsets /*= {}*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdBindDescriptorSets>(getHandle(),
			convertCEnum(pipelineBindPoint), pipelineLayout.getHandle(), firstSet,
			descriptorSets.size(), DescriptorSet::underlyingCast(descriptorSets.data()),
			dynamicOffsets.size(), dynamicOffsets.data());
	}

	void CommandBuffer::setPipelineBarrier(const DeviceFunctionTable& functions, Flags::PipelineStage srcStage,
		Flags::PipelineStage dstStage, Flags::Dependency dependencyFlags,
		std::span<const MemoryBarrier> memoryBarriers,
		std::span<const BufferMemoryBarrier> bufferMemoryBarriers,
		std::span<const ImageMemoryBarrier> imageMemoryBarriers)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdPipelineBarrier>(getHandle(), srcStage, dstStage, dependencyFlags,
			memoryBarriers.size(), MemoryBarrier::underlyingCast(memoryBarriers.data()), 
			bufferMemoryBarriers.size(), BufferMemoryBarrier::underlyingCast(bufferMemoryBarriers.data()),
			imageMemoryBarriers.size(), ImageMemoryBarrier::underlyingCast(imageMemoryBarriers.data()));
	}

	void CommandBuffer::copyBuffer(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
		const BufferRef& dstBuffer, std::span<const BufferCopy> copyRegions)
	{ 
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdCopyBuffer>(getHandle(), srcBuffer.getHandle(),
			dstBuffer.getHandle(), copyRegions.size(), BufferCopy::underlyingCast(copyRegions.data()));
	}

	void CommandBuffer::copyBuffer(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
		const BufferRef& dstBuffer, const BufferCopy& copyRegion)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdCopyBuffer>(getHandle(), srcBuffer.getHandle(),
			dstBuffer.getHandle(), 1, BufferCopy::underlyingCast(&copyRegion));
	}

	void CommandBuffer::copyBufferToImage(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
		const ImageRef& dstImage, ImageLayout dstLayout, 
		std::span<const BufferImageCopy> imageCopies)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdCopyBufferToImage>(getHandle(), srcBuffer.getHandle(),
			dstImage.getHandle(), convertCEnum(dstLayout), imageCopies.size(),
			BufferImageCopy::underlyingCast(imageCopies.data()));
	}

	void CommandBuffer::copyBufferToImage(const DeviceFunctionTable& functions, const BufferRef& srcBuffer,
		const ImageRef& dstImage, ImageLayout dstLayout, const BufferImageCopy& imageCopy)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdCopyBufferToImage>(getHandle(), srcBuffer.getHandle(),
			dstImage.getHandle(), convertCEnum(dstLayout), 1, BufferImageCopy::underlyingCast(&imageCopy));
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
		uint32_t vertexCount, uint32_t instanceCount,
		uint32_t firstVertex, uint32_t firstInstance)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdDraw>(getHandle(), 
			vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::drawIndexed(const DeviceFunctionTable& functions,
		uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
		uint32_t vertexOffset, uint32_t firstInstance)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdDrawIndexed>(getHandle(),
			indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void CommandBuffer::drawIndirect(const DeviceFunctionTable& functions, const BufferRef& buffer,
		DeviceSize offset, uint32_t drawCount, uint32_t stride)
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

	Result CommandBuffer::stopRecord(const DeviceFunctionTable& functions)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		return convertCEnum(functions.execute<DeviceFunction::EndCommandBuffer>(getHandle()));
	}

	Result CommandBuffer::reset(const DeviceFunctionTable& functions,
		Flags::CommandBufferReset flags /*= CommandBufferReset::Bits::None*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		return convertCEnum(functions.execute<DeviceFunction::ResetCommandBuffer>(getHandle(), flags));
	}

	void CommandBuffer::blitImage(const DeviceFunctionTable& functions,
		const Image& srcImage, const Image& dstImage,
		ImageLayout srcLayout, ImageLayout dstLayout,
		std::span<const ImageBlit> blit, Filter filter /*= Filter::Linear*/)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdBlitImage>(getHandle(), srcImage,
			convertCEnum(srcLayout), dstImage.getHandle(), convertCEnum(dstLayout), blit.size(),
			ImageBlit::underlyingCast(blit.data()), convertCEnum(filter));
	}

	void CommandBuffer::pushConstants(const DeviceFunctionTable& functions, const PipelineLayoutRef& pipelineLayout,
		Flags::ShaderStage stageFlags, uint32_t offset, uint32_t size, const void* values)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdPushConstants>(getHandle(), pipelineLayout,
			stageFlags, offset, size, values);
	}

	Result CommandBuffer::begin(const DeviceFunctionTable& functions, const CommandBufferBeginInfo& beginInfo)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		return convertCEnum(functions.execute<DeviceFunction::BeginCommandBuffer>(getHandle(),
			beginInfo.getUnderlyingPointer()));
	}

	Result CommandBuffer::end(const DeviceFunctionTable& functions)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		return convertCEnum(functions.execute<DeviceFunction::EndCommandBuffer>(getHandle()));
	}

	void CommandBuffer::pipelineBarrier(const DeviceFunctionTable& functions,
		Flags::PipelineStage srcStage, Flags::PipelineStage dstStage,
		Flags::Dependency dependencyFlags, std::span<const MemoryBarrier> memoryBarriers,
		std::span<const BufferMemoryBarrier> bufferMemoryBarriers,
		std::span<const ImageMemoryBarrier> imageMemoryBarriers)
	{
		GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
		functions.execute<DeviceFunction::CmdPipelineBarrier>(getHandle(), srcStage, dstStage, dependencyFlags,
			memoryBarriers.size(), MemoryBarrier::underlyingCast(memoryBarriers.data()),
			bufferMemoryBarriers.size(), BufferMemoryBarrier::underlyingCast(bufferMemoryBarriers.data()),
			imageMemoryBarriers.size(), ImageMemoryBarrier::underlyingCast(imageMemoryBarriers.data()));
	}
}