#include "CommandPool.h"

namespace Graphics
{
    void CommandPool::create(const DeviceRef& device, const DeviceFunctionTable& functions, const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid command pool");

        auto result = functions.execute<DeviceFunction::CreateCommandPool>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create a command pool: " +
            s_resultMessages.at(result));
    }

    void CommandPool::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid command pool");
        functions.execute<DeviceFunction::DestroyCommandPool>(
            device.getHandle(), getHandle(), nullptr);
        BaseComponent<VkCommandPool, CommandPoolRef>::reset();
    }

    CommandBuffer CommandPoolRef::allocateCommandBuffer(const DeviceRef& device, const DeviceFunctionTable& functions,
        CommandBufferLevel level /*= CommandBufferLevel::Primary*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to allocate a command buffer from an invalid command pool");
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.setCommandPool(getHandle())
            .setLevel(convertEnum(level))
            .setCommandBufferCount(1);

        CommandBuffer buffer;
        auto result = functions.execute<DeviceFunction::AllocateCommandBuffers>(device.getHandle(),
            reinterpret_cast<VkCommandBufferAllocateInfo*>(&allocInfo), CommandBuffer::underlyingCast(&buffer));

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to allocate command buffer: " +
            s_resultMessages.at(result));
        return buffer;
    }

    void CommandPoolRef::freeCommandBuffer(const DeviceRef& device, const DeviceFunctionTable& functions,
        CommandBuffer& buffer)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to free a command buffer from an invalid command pool");
        functions.execute<DeviceFunction::FreeCommandBuffers>(
            device.getHandle(), getHandle(), 1, CommandBuffer::underlyingCast(&buffer));
    }

    std::vector<CommandBuffer> CommandPoolRef::allocateCommandBuffers(const DeviceRef& device, const DeviceFunctionTable& functions,
        size_t count, CommandBufferLevel level /*= CommandBufferLevel::Primary*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to allocate command buffers from an invalid command pool");
        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.setCommandPool(getHandle())
            .setLevel(convertEnum(level))
            .setCommandBufferCount(count);

        std::vector<CommandBuffer> buffer(count);
        auto result = functions.execute<DeviceFunction::AllocateCommandBuffers>(device.getHandle(),
            reinterpret_cast<VkCommandBufferAllocateInfo*>(&allocInfo), CommandBuffer::underlyingCast(buffer.data()));

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to allocate command buffers: " +
            s_resultMessages.at(result));
        return buffer;
    }

    void CommandPoolRef::freeCommandBuffers(const DeviceRef& device, const DeviceFunctionTable& functions,
        std::span<const CommandBuffer> buffer)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to free command buffers from an invalid command pool");
        functions.execute<DeviceFunction::FreeCommandBuffers>(
            device.getHandle(), getHandle(), 1, CommandBuffer::underlyingCast(buffer.data()));
    }

    void CommandPoolRef::reset(const DeviceRef& device, const DeviceFunctionTable& functions,
        CommandPoolReset::Flags flags /*= CommandPoolReset::Bits::ReleaseResources*/) {
        auto result = functions.execute<DeviceFunction::ResetCommandPool>(
            device.getHandle(), getHandle(), flags);
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to reset a command pool: " +
            s_resultMessages.at(result));
    }
}