#include "Semaphore.h"

namespace Graphics
{
    void Semaphore::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const CreateInfo& createInfo /*= CreateInfo()*/)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid Semaphore");

        auto result = functions.execute<DeviceFunction::CreateSemaphore>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to create semaphore: " + s_resultMessages.at(result));
    }

    void Semaphore::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid Semaphore");
        functions.execute<DeviceFunction::DestroySemaphore>
            (device.getHandle(), getHandle(), nullptr);
        reset();
    }
}