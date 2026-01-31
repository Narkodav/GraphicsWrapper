#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void Device::create(const InstanceFunctionTable& functions,
        const PhysicalDevice& physicalDevice,        
        const DeviceCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid device");       

        auto result = functions.execute<InstanceFunction::CreateDevice>(
            physicalDevice, &createInfo, nullptr, this);
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a logical device");
    };

    void Device::destroy(const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid device");
        functions.execute<DeviceFunction::DestroyDevice>(getHandle(), nullptr);
        reset();
    }

    Queue DeviceRef::getQueue(uint32_t familyIndex, uint32_t queueIndex, const DeviceFunctionTable& functions) const
    {
        GRAPHICS_VERIFY(isSet(), "Trying to get queue from an invalid device");
        Queue queue;
        functions.execute<DeviceFunction::GetDeviceQueue>(
            getHandle(), familyIndex, queueIndex, queue.getUnderlyingPointer());
        return queue;
    }

    DeviceFunctionTable DeviceRef::getFunctionTable(const InstanceFunctionTable& functions) const
    {
        GRAPHICS_VERIFY(isSet(), "Trying to get function table from an invalid device");
        DeviceFunctionTable table(functions.get<InstanceFunction::GetDeviceProcAddr>());
        table.loadAllFunctions(getHandle());
        return table;
    };

    void DeviceRef::waitIdle(const DeviceFunctionTable& functions) const {
        GRAPHICS_VERIFY(isSet(), "Trying to wait on an invalid device");
        auto result = functions.execute<DeviceFunction::DeviceWaitIdle>(getHandle());
        GRAPHICS_VERIFY_RESULT(result, "Failed to wait for device idle");
    };
}