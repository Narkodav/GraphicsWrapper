#include "CommandBuffer.h"

namespace Graphics {

	CommandBuffer::CommandBuffer(const Instance& intsance, const Device& device, const CommandPool& pool,
		vk::CommandBufferLevel level /*= vk::CommandBufferLevel::ePrimary*/) {
		m_commandBuffer = device.allocateCommandBuffer(intsance, pool.getPool(), level);
		m_initialized = true;
	};

	void CommandBuffer::record(const Instance& instance, CommandBufferUsage::Flags flags)
	{
		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
		beginInfo.flags = flags; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		try {
			m_commandBuffer.begin(beginInfo, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to begin recording command buffer: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when beginning recording command buffer: " + std::string(e.what()));
		}
	}

	void CommandBuffer::beginRenderPass(const Instance& instance,
		const RenderPass& renderPass, const SwapChain& swapChain,
		uint32_t imageIndex, Color clearColor)
	{
		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;
		renderPassInfo.renderPass = renderPass.getRenderPass();
		renderPassInfo.framebuffer = swapChain.getFrameBuffers()[imageIndex];
		renderPassInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.getActiveSwapChainFormat().getSwapChainExtent();

		vk::ClearValue clear = clearColor;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clear;
		try {
			m_commandBuffer.beginRenderPass(
				renderPassInfo, vk::SubpassContents::eInline, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to begin render pass: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when beginning render pass: " + std::string(e.what()));
		}
	}

	void CommandBuffer::beginRenderPass(const Instance& instance,
		const RenderPass& renderPass, const SwapChain& swapChain,
		uint32_t imageIndex, Color clearColor, float clearDepth)
	{
		vk::RenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = vk::StructureType::eRenderPassBeginInfo;
		renderPassInfo.renderPass = renderPass.getRenderPass();
		renderPassInfo.framebuffer = swapChain.getFrameBuffers()[imageIndex];
		renderPassInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
		renderPassInfo.renderArea.extent = swapChain.getActiveSwapChainFormat().getSwapChainExtent();

		vk::ClearValue clears[] = { clearColor, {clearDepth} };

		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clears;
		try {
			m_commandBuffer.beginRenderPass(
				renderPassInfo, vk::SubpassContents::eInline, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to begin render pass: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when beginning render pass: " + std::string(e.what()));
		}
	}

	void CommandBuffer::bindIndexBuffer(const Instance& instance,
		const Buffer& buffer, size_t offset)
	{
		try {
			m_commandBuffer.bindIndexBuffer(buffer.getBuffer(), offset, vk::IndexType::eUint32, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to bind index buffer: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when binding index buffer: " + std::string(e.what()));
		}
	}

	void CommandBuffer::setPipelineBarrier(const Instance& instance,
		PipelineStage::Flags srcStage, PipelineStage::Flags dstStage,
		Image& image, ImageLayout oldLayout, ImageLayout newLayout,
		Access::Flags srcAccess, Access::Flags dstAccess,
		uint32_t baseMipLevel /*= 0*/,
		uint32_t levelCount /*= 1*/)
	{
		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = convertEnum(oldLayout);
		barrier.newLayout = static_cast<vk::ImageLayout>(newLayout);
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image.getImage();
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = baseMipLevel;
		barrier.subresourceRange.levelCount = levelCount;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;

		m_commandBuffer.pipelineBarrier(srcStage, dstStage,
			{}, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	void CommandBuffer::transferImageData(const Instance& instance, const Buffer& srcBuffer,
		Image& dstImage, ImageLayout dstLayout, Extent3D imageExtent, size_t offset /*= 0*/,
		Offset3D imageOffset /*= Offset3D()*/)
	{
		vk::BufferImageCopy region{};
		region.bufferOffset = offset;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = imageOffset;
		region.imageExtent = imageExtent;

		try {
			m_commandBuffer.copyBufferToImage(srcBuffer.getBuffer(), dstImage.getImage(),
				convertEnum(dstLayout), 1, &region, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to transfer buffer data: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when transferring buffer data: " + std::string(e.what()));
		}
	}

	void CommandBuffer::setPipelineBarrier(const Instance& instance,
		PipelineStage::Flags srcStage, PipelineStage::Flags dstStage,
		Buffer& buffer, Access::Flags srcAccess,
		Access::Flags dstAccess)
	{
		vk::BufferMemoryBarrier barrier{};
		barrier.sType = vk::StructureType::eBufferMemoryBarrier;
		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.buffer = buffer.getBuffer();
		barrier.offset = 0;
		barrier.size = VK_WHOLE_SIZE;

		m_commandBuffer.pipelineBarrier(srcStage, dstStage,
			{}, 0, nullptr, 1, &barrier, 0, nullptr,
			instance.getDispatchLoader());
	}

	void CommandBuffer::transferBufferData(const Instance& instance, const Buffer& srcBuffer,
		const Buffer& dstBuffer, const CopyRegion& copyRegion)
	{
		try {
			m_commandBuffer.copyBuffer(srcBuffer.getBuffer(), dstBuffer.getBuffer(),
				static_cast<vk::BufferCopy>(copyRegion), instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to transfer buffer data: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when transferring buffer data: " + std::string(e.what()));
		}
	}
		
	void CommandBuffer::setRenderView(const Instance& instance, const RenderRegion& canvas)
	{

		try {
			m_commandBuffer.setViewport(0, canvas.viewport, instance.getDispatchLoader());
			m_commandBuffer.setScissor(0, canvas.scissor, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to set render view: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error when setting render view: " + std::string(e.what()));
		}
	}

	void CommandBuffer::draw(const Instance& instance,
		size_t vertexCount, size_t instanceCount, 
		size_t firstVertex, size_t firstInstance)
	{
		try {
			m_commandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to record draw command: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error recording draw command: " + std::string(e.what()));
		}
	}

	void CommandBuffer::drawIndexed(const Instance& instance,
		size_t indexCount, size_t instanceCount, size_t firstIndex, 
		size_t indexIncrement, size_t firstInstance)
	{
		try {
			m_commandBuffer.drawIndexed(
				indexCount, instanceCount, firstIndex, indexIncrement, firstInstance, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to record draw indexed command: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error recording draw indexed command: " + std::string(e.what()));
		}
	}

	void CommandBuffer::drawIndirect(const Instance& instance, const Buffer& buffer,
		size_t offset, uint32_t drawCount, uint32_t stride)
	{
		try {
			m_commandBuffer.drawIndirect(buffer.getBuffer(), offset, drawCount,
				stride, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to record draw indirect command: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error recording draw indirect command: " + std::string(e.what()));
		}
	}

	void CommandBuffer::dispatch(const Instance& instance, uint32_t groupCountX,
		uint32_t groupCountY, uint32_t groupCountZ)
	{
		try {
			m_commandBuffer.dispatch(groupCountX, groupCountY, groupCountZ, instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to record draw indexed command: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error recording draw indexed command: " + std::string(e.what()));
		}
	}

	void CommandBuffer::endRenderPass(const Instance& instance)
	{
		try {
			m_commandBuffer.endRenderPass(instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to end render pass: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error ending render pass: " + std::string(e.what()));
		}
	}

	void CommandBuffer::stopRecord(const Instance& instance)
	{
		try {
			m_commandBuffer.end(instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to end command buffer record: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error ending command buffer record: " + std::string(e.what()));
		}
	}

	void CommandBuffer::reset(const Instance& instance)
	{
		try {
			m_commandBuffer.reset(vk::CommandBufferResetFlagBits(), instance.getDispatchLoader());
		}
		catch (const vk::SystemError& e) {
			throw std::runtime_error("failed to end command buffer record: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unexpected error ending command buffer record: " + std::string(e.what()));
		}
	}

	void CommandBuffer::blitImage(const Instance& instance,
		const Image& srcImage, const Image& dstImage,
		ImageLayout srcLayout, ImageLayout dstLayout,
		Offset3D srcOffsetMin, Offset3D srcOffsetMax,
		Offset3D dstOffsetMin, Offset3D dstOffsetMax,
		uint32_t srcMipLevel/* = 1*/, uint32_t dstMipLevel/* = 1*/,
		ImageAspect::Flags srcAspect/* = ImageAspect::Bits::Color*/,
		ImageAspect::Flags dstAspect/* = ImageAspect::Bits::Color*/,
		Filter filter/* = Filter::Linear*/)
	{
		vk::ImageBlit blit;
		blit.srcOffsets[0] = srcOffsetMin;
		blit.srcOffsets[1] = srcOffsetMax;
		blit.srcSubresource.aspectMask = srcAspect;
		blit.srcSubresource.mipLevel = srcMipLevel;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = dstOffsetMin;
		blit.dstOffsets[1] = dstOffsetMax;
		blit.dstSubresource.aspectMask = dstAspect;
		blit.dstSubresource.mipLevel = dstMipLevel;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		m_commandBuffer.blitImage(srcImage.getImage(), convertEnum(srcLayout),
			dstImage.getImage(), convertEnum(dstLayout), blit, convertEnum(filter), instance.getDispatchLoader());
	}
}