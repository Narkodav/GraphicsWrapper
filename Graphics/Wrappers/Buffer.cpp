#include "Buffer.h"

namespace Graphics {
    void Buffer::create(const DeviceRef& device,
        const DeviceFunctionTable& functions,
        const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid buffer");

        auto result = functions.execute<DeviceFunction::CreateBuffer>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create buffer: " +
            s_resultMessages.at(result));
    }

    MemoryRequirements BufferRef::getMemoryRequirements(const DeviceRef& device,
        const DeviceFunctionTable& functions) const
    {
        GRAPHICS_VERIFY(!isSet(), "Cannot get memory requirements for an invalid buffer");
        MemoryRequirements memRequirements;
        functions.execute<DeviceFunction::GetBufferMemoryRequirements>(
            device.getHandle(), getHandle(), memRequirements.getUnderlyingPointer());
        return memRequirements;
    }

    void Buffer::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid buffer");
        functions.execute<DeviceFunction::DestroyBuffer>(
            device.getHandle(), getHandle(), nullptr);
    }
}