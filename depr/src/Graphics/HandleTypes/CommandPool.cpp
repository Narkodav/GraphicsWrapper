#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void CommandPool::create(const DeviceFunctionTable& functions, const DeviceRef& device, const CommandPoolCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid command pool");

        auto result = functions.execute<DeviceFunction::CreateCommandPool>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a command pool");
    }

    void CommandPool::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid command pool");
        functions.execute<DeviceFunction::DestroyCommandPool>(
            device.getHandle(), getHandle(), nullptr);
        BaseComponent<VkCommandPool, CommandPoolRef>::reset();
    }

    CommandBuffer CommandPoolRef::allocateCommandBuffer(const DeviceFunctionTable& functions, const DeviceRef& device, CommandBufferLevel level /*= CommandBufferLevel::Primary*/)
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

    void CommandPoolRef::freeCommandBuffer(const DeviceFunctionTable& functions, const DeviceRef& device,
        CommandBuffer& buffer)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to free a command buffer from an invalid command pool");
        functions.execute<DeviceFunction::FreeCommandBuffers>(
            device.getHandle(), getHandle(), 1, CommandBuffer::underlyingCast(&buffer));
    }

    std::vector<CommandBuffer> CommandPoolRef::allocateCommandBuffers(const DeviceFunctionTable& functions,
        const DeviceRef& device, size_t count, CommandBufferLevel level /*= CommandBufferLevel::Primary*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to allocate command buffers from an invalid command pool");
        CommandBufferAllocateInfo allocInfo{};
        allocInfo.setCommandPool(getHandle())
            .setLevel(level)
            .setCommandBufferCount(count);

        return CommandPool::allocateCommandBuffers(functions, device, allocInfo);
    }

    void CommandPoolRef::freeCommandBuffers(const DeviceFunctionTable& functions, const DeviceRef& device,
        std::span<const CommandBuffer> buffer)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to free command buffers from an invalid command pool");
        functions.execute<DeviceFunction::FreeCommandBuffers>(
            device.getHandle(), getHandle(), 1, CommandBuffer::underlyingCast(buffer.data()));
    }

    void CommandPoolRef::reset(const DeviceFunctionTable& functions, const DeviceRef& device,
        Flags::CommandPoolReset flags /*= CommandPoolReset::Bits::ReleaseResources*/) {
        auto result = functions.execute<DeviceFunction::ResetCommandPool>(
            device.getHandle(), getHandle(), flags);
        GRAPHICS_VERIFY_RESULT(result, "Failed to reset a command pool");
    }

    static std::vector<CommandBuffer> allocateCommandBuffers(const DeviceFunctionTable& functions,
        const DeviceRef& device, const CommandBufferAllocateInfo& allocInfo)
    {
        std::vector<CommandBuffer> buffers(allocInfo.getCommandBufferCount());
        auto result = functions.execute<DeviceFunction::AllocateCommandBuffers>(device.getHandle(),
            allocInfo.getUnderlyingPointer(),
            CommandBuffer::underlyingCast(buffers.data()));
        GRAPHICS_VERIFY_RESULT(result, "Failed to allocate command buffers");
		return buffers;
    }
}