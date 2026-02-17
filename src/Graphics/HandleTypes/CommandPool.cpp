#include "Graphics/Graphics.h"

namespace Graphics
{
    void CommandPool::create(const DeviceFunctionTable& functions, DeviceRef device, const CommandPoolCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid command pool");

        auto result = functions.execute<DeviceFunction::CreateCommandPool>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a command pool");
    }

    void CommandPool::destroy(const DeviceFunctionTable& functions, DeviceRef device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid command pool");
        functions.execute<DeviceFunction::DestroyCommandPool>(
            device.getHandle(), getHandle(), nullptr);
        BaseComponent<VkCommandPool, CommandPoolRef>::reset();
    }

    CommandBuffer CommandPoolRef::allocateCommandBuffer(const DeviceFunctionTable& functions, 
        DeviceRef device, CommandBufferLevel level /*= CommandBufferLevel::Primary*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to allocate a command buffer from an invalid command pool");
        CommandBufferAllocateInfo allocInfo;
        allocInfo.setCommandPool(getHandle())
            .setLevel(level)
            .setCommandBufferCount(1);

        CommandBuffer buffer;
        auto result = functions.execute<DeviceFunction::AllocateCommandBuffers>(device.getHandle(),
            allocInfo.getUnderlyingPointer(), buffer.getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to allocate a command buffer");
        return buffer;
    }

    void CommandPoolRef::freeCommandBuffer(const DeviceFunctionTable& functions, DeviceRef device,
        CommandBuffer buffer)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to free a command buffer from an invalid command pool");
        functions.execute<DeviceFunction::FreeCommandBuffers>(
            device.getHandle(), getHandle(), 1, CommandBuffer::underlyingCast(&buffer));
    }

    std::vector<CommandBuffer> CommandPoolRef::allocateCommandBuffers(const DeviceFunctionTable& functions,
        DeviceRef device, size_t count, CommandBufferLevel level /*= CommandBufferLevel::Primary*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to allocate command buffers from an invalid command pool");
        CommandBufferAllocateInfo allocInfo{};
        allocInfo.setCommandPool(getHandle())
            .setLevel(level)
            .setCommandBufferCount(count);

        return CommandPool::allocateCommandBuffers(functions, device, allocInfo);
    }

    void CommandPoolRef::freeCommandBuffers(const DeviceFunctionTable& functions, DeviceRef device,
        std::span<const CommandBuffer> buffer)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to free command buffers from an invalid command pool");
        functions.execute<DeviceFunction::FreeCommandBuffers>(
            device.getHandle(), getHandle(), 1, CommandBuffer::underlyingCast(buffer.data()));
    }

    void CommandPoolRef::reset(const DeviceFunctionTable& functions, DeviceRef device,
        Flags::CommandPoolReset flags /*= CommandPoolReset::Bits::ReleaseResources*/) {
        auto result = functions.execute<DeviceFunction::ResetCommandPool>(
            device.getHandle(), getHandle(), flags);
        GRAPHICS_VERIFY_RESULT(result, "Failed to reset a command pool");
    }

    std::vector<CommandBuffer> CommandPool::allocateCommandBuffers(const DeviceFunctionTable& functions,
        DeviceRef device, const CommandBufferAllocateInfo& allocInfo)
    {
        std::vector<CommandBuffer> buffers(allocInfo.getCommandBufferCount());
        auto result = functions.execute<DeviceFunction::AllocateCommandBuffers>(device.getHandle(),
            allocInfo.getUnderlyingPointer(),
            CommandBuffer::underlyingCast(buffers.data()));
        GRAPHICS_VERIFY_RESULT(result, "Failed to allocate command buffers");
		return buffers;
    }

    Result CommandPoolRef::oneTimeDataTransfer(const DeviceFunctionTable& functions, DeviceRef device,
        Queue submitQueue, std::span<const DataTransferInfo> transferInfos, 
        BufferRef stagingBuffer, MemoryMapping& stagingMapping) {
        return this->oneTimeSubmit(functions, device, submitQueue,
        [&](Result result, CommandBuffer commandBuffer) {
            if(result != Result::Success) return result;

            size_t stagingOffset = 0;
            void* stagingPtr = stagingMapping.get();

            for(size_t i = 0; i < transferInfos.size(); ++i) {					
                std::memcpy(stagingPtr, static_cast<const std::byte*>(transferInfos[i].getSourceData()) 
                + transferInfos[i].getSourceOffset(), transferInfos[i].getByteSize());
                commandBuffer.copyBuffer(functions, stagingBuffer, transferInfos[i].getDestinationBuffer(),
                    { stagingOffset, transferInfos[i].getDestinationOffset(), transferInfos[i].getByteSize() });					
                stagingOffset += transferInfos[i].getByteSize();
                stagingPtr = static_cast<std::byte*>(stagingPtr) + transferInfos[i].getByteSize();
            }
            return Result::Success;
        });
    }

    Result CommandPoolRef::oneTimeDataTransfer(const DeviceFunctionTable& functions, DeviceRef device,
        Queue submitQueue, std::span<const DataTransferInfo> transferInfos, BufferRef stagingBuffer, 
        MemoryMapping& stagingMapping, size_t stagingSize) {
        size_t counter = 0;

        while (counter < transferInfos.size()) {
            size_t batchStart = counter;
            size_t transferSize = 0;

            if (transferInfos[counter].getByteSize() > stagingSize) {
                throw std::runtime_error("Staging buffer cannot fit one source");
            }

            while (counter < transferInfos.size() &&
                transferSize + transferInfos[counter].getByteSize() <= stagingSize) {
                transferSize += transferInfos[counter].getByteSize();
                ++counter;
            }

            size_t batchCount = counter - batchStart;

            auto result = this->oneTimeDataTransfer(functions, device, 
                submitQueue, {transferInfos.data() + batchStart, batchCount}, 
                stagingBuffer, stagingMapping);
            if(result != Result::Success) return result;
        }

        return Result::Success;
    }
}