#include "Queue.h"
#include "CommandBuffer.h"
#include "CommandPool.h"

namespace Graphics {

    CommandPool::CommandPool(const Instance& instance, const Device& device, uint32_t queueFamilyIndex)
    {
        vk::CommandPoolCreateInfo poolInfo{};
        poolInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
        poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolInfo.queueFamilyIndex = queueFamilyIndex;

        try {
            m_pool = device.getDevice().createCommandPool(poolInfo, nullptr, instance.getDispatchLoader());
        }
        catch (const vk::SystemError& e) {
            throw std::runtime_error("Failed to create a CommandPool: " + std::string(e.what()));
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Unexpected error creating a CommandPool: " + std::string(e.what()));
        }

        m_initialized = true;
    }

    void CommandPool::makeOneTimeSubmit(const Instance& instance, const Device& device,
        const Queue& queue, std::function<void(CommandBuffer&)>&& func)
    {
        auto temporary = CommandBuffer(instance, device, *this);
        try {
            temporary.reset(instance);
            temporary.record(instance, CommandBufferUsage::Bits::OneTimeSubmit);

            func(temporary);

            temporary.stopRecord(instance);
            queue.submit(instance, std::array{ std::ref(temporary) });
            queue.waitIdle(instance);
            temporary.destroy(instance, device, *this);
        }
        catch (...) {
            temporary.destroy(instance, device, *this);
            throw;
        }
    }

    void CommandPool::makeOneTimeImageDataTransfer(const Instance& instance, const Device& device,
        const Queue& queue, MappedMemory& stagingMemory, const Buffer& stagingBuffer,
        Image& image, const PixelData& data, uint32_t srcOffset /*= 0*/,
        Offset3D dstImageOffset /*= Offset3D()*/,    
        ImageLayout startLayout /*= ImageLayout::Undefined*/,
        ImageLayout finalLayout /*= ImageLayout::ShaderReadOnlyOptimal*/,
        PipelineStage::Flags srcStage /*= PipelineStage::Bits::TopOfPipe*/,
        PipelineStage::Flags dstStage /*= PipelineStage::Bits::FragmentShader*/,
        Access::Flags srcAccess /*= Access::Bits::None*/,
        Access::Flags dstAccess /*= Access::Bits::ShaderRead*/)
    {
        makeOneTimeSubmit(instance, device, queue,
            [this, &instance, &stagingMemory, &stagingBuffer, &image,
            &data, &srcOffset, &dstImageOffset, &startLayout, &finalLayout,
            &srcStage, &dstStage, &srcAccess, &dstAccess]
            (CommandBuffer& temporary) {
                temporary.setPipelineBarrier(instance,
                    srcStage,
                    PipelineStage::Bits::Transfer,
                    image, startLayout,
                    ImageLayout::TransferDstOptimal,
                    srcAccess,
                    Access::Bits::TransferWrite
                    );
                auto pixelData = data.getPixelData();
                auto mappedMemory = stagingMemory.getMapping<uint8_t>(pixelData.size());
                std::copy(pixelData.begin(), pixelData.end(), mappedMemory.begin());
                temporary.transferImageData(instance, stagingBuffer, image, ImageLayout::TransferDstOptimal,
                    data.getExtent3D(), srcOffset, dstImageOffset);

                temporary.setPipelineBarrier(instance,
                    PipelineStage::Bits::Transfer,
                    dstStage,
                    image, ImageLayout::TransferDstOptimal,
                    finalLayout,
                    Access::Bits::TransferWrite,
                    dstAccess
                );
            });
    }

    void CommandPool::makeOneTimeImageMipMaps(const Instance& instance, 
        const Device& device, const Queue& queue, Image& image, 
        ImageLayout startLayout /*= ImageLayout::Undefined*/, 
        ImageLayout finalLayout /*= ImageLayout::ShaderReadOnlyOptimal*/,
        PipelineStage::Flags srcStage /*= PipelineStage::Bits::TopOfPipe*/,
        PipelineStage::Flags dstStage /*= PipelineStage::Bits::FragmentShader*/,
        Access::Flags srcAccess /*= Access::Bits::None*/,
        Access::Flags dstAccess /*= Access::Bits::ShaderRead*/)
    {
        makeOneTimeSubmit(instance, device, queue,
            [this, &instance, &image, &startLayout,
            &finalLayout, &srcStage, &dstStage,
            &srcAccess, &dstAccess]
            (CommandBuffer& temporary) {
                uint32_t mipWidth = image.getWidth();
                uint32_t mipHeight = image.getHeight();
                uint32_t mipLevelCount = image.getMipLevelCount();

                temporary.setPipelineBarrier(instance,
                    srcStage,
                    PipelineStage::Bits::Transfer,
                    image, startLayout,
                    ImageLayout::TransferDstOptimal,
                    srcAccess,
                    Access::Bits::TransferWrite, 0, mipLevelCount);

                for (uint32_t i = 0; i < mipLevelCount - 1; ++i) {

                    temporary.setPipelineBarrier(instance,
                        PipelineStage::Bits::Transfer,
                        PipelineStage::Bits::Transfer,
                        image, ImageLayout::TransferDstOptimal,
                        ImageLayout::TransferSrcOptimal,
                        Access::Bits::TransferWrite,
                        Access::Bits::TransferRead, i);

                    temporary.blitImage(instance, image, image,
                        ImageLayout::TransferSrcOptimal, ImageLayout::TransferDstOptimal,
                        { 0, 0, 0 }, { static_cast<int32_t>(mipWidth), static_cast<int32_t>(mipHeight), 1 },
                        { 0, 0, 0 }, { static_cast<int32_t>(mipWidth > 1u ? mipWidth / 2u : 1u),
                        static_cast<int32_t>(mipHeight > 1u ? mipHeight / 2u : 1u), 1u },
                        i, i + 1);

                    temporary.setPipelineBarrier(instance,
                        PipelineStage::Bits::Transfer, dstStage,
                        image, ImageLayout::TransferSrcOptimal,
                        finalLayout,
                        Access::Bits::TransferRead,
                        dstAccess, i);

                    if (mipWidth > 1u) mipWidth /= 2u;
                    if (mipHeight > 1u) mipHeight /= 2u;
                }

                temporary.setPipelineBarrier(instance,
                    PipelineStage::Bits::Transfer, dstStage,
                    image, ImageLayout::TransferDstOptimal,
                    finalLayout,
                    Access::Bits::TransferWrite,
                    dstAccess, mipLevelCount - 1);
            });
    }
}