#include "Graphics/Graphics.h"

namespace Graphics
{
    void Semaphore::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const SemaphoreCreateInfo& createInfo /*= CreateInfo()*/)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid Semaphore");
        auto result = functions.execute<DeviceFunction::CreateSemaphore>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a semaphore");
    }

    void Semaphore::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid Semaphore");
        functions.execute<DeviceFunction::DestroySemaphore>
            (device.getHandle(), getHandle(), nullptr);
        reset();
    }
}