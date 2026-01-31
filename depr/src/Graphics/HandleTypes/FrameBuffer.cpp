#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void FrameBuffer::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const FrameBufferCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid frame buffer");
        auto result = functions.execute<DeviceFunction::CreateFramebuffer>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a frame buffer");
    }

    void FrameBuffer::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid frame buffer");
        functions.execute<DeviceFunction::DestroyFramebuffer>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }
}