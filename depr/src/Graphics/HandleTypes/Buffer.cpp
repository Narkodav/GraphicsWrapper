#include "../../../include/Graphics/Graphics.h"

namespace Graphics {
    void Buffer::create(const DeviceFunctionTable& functions, const DeviceRef& device, const BufferCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid buffer");
        auto result = functions.execute<DeviceFunction::CreateBuffer>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create buffer");
    }

    MemoryRequirements BufferRef::getMemoryRequirements(const DeviceFunctionTable& functions, const DeviceRef& device) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot get memory requirements for an invalid buffer");
        MemoryRequirements memRequirements;
        functions.execute<DeviceFunction::GetBufferMemoryRequirements>(
            device.getHandle(), getHandle(), memRequirements.getUnderlyingPointer());
        return memRequirements;
    }

    void Buffer::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid buffer");
        functions.execute<DeviceFunction::DestroyBuffer>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }

    void BufferView::create(const DeviceFunctionTable& functions, const DeviceRef& device, const BufferViewCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid buffer view");
        auto result = functions.execute<DeviceFunction::CreateBufferView>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create buffer view");
    }

    void BufferView::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid buffer view");
        functions.execute<DeviceFunction::DestroyBufferView>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }
}