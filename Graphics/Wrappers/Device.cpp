#include "Device.h"
#include "Queue.h"

namespace Graphics
{
    void Device::create(const InstanceRef& instance,
        const PhysicalDevice& physicalDevice,
        const InstanceFunctionTable& functions,
        const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid device");       

        auto result = functions.execute<InstanceFunction::CreateDevice>(
            physicalDevice, createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to create a logical device: " + s_resultMessages.at(result));
    };

    void Device::destroy(const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid device");
        functions.execute<DeviceFunction::DestroyDevice>(getHandle(), nullptr);
        reset();
    }

    Queue DeviceRef::getQueue(uint32_t familyIndex,
        uint32_t queueIndex, const DeviceFunctionTable& functions) const
    {
        GRAPHICS_VERIFY(isSet(), "Trying to get queue from an invalid device");
        Queue queue;
        functions.execute<DeviceFunction::GetDeviceQueue>(
            getHandle(), familyIndex, queueIndex, queue.getUnderlyingPointer());
        return queue;
    }
}