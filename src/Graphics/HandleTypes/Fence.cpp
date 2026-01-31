#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void Fence::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const FenceCreateInfo& createInfo /*= CreateInfo()*/)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid fence");

        auto result = functions.execute<DeviceFunction::CreateFence>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a fence");
    }

    void Fence::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid fence");
        functions.execute<DeviceFunction::DestroyFence>(device.getHandle(), getHandle(), nullptr);
        Fence::BaseComponent<VkFence, FenceRef>::reset();
    }

    void FenceRef::wait(const DeviceFunctionTable& functions, const DeviceRef& device,
        size_t timeout /*= std::numeric_limits<size_t>::max()*/) const
    {
        Fence::wait(functions, device, this, 1, timeout, true);
    }

    void Fence::reset(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        reset(functions, device, this, 1);
    }

    void Fence::wait(const DeviceFunctionTable& functions, const DeviceRef& device,
        const FenceRef* fences, size_t count, size_t timeout /*= std::numeric_limits<size_t>::max()*/,
        bool waitAll /*= true*/) {
        for (size_t i = 0; i < count; ++i)
            GRAPHICS_VERIFY(fences[i].isSet(),
                "Trying to wait for an invalid fence at index " + std::to_string(i));

        auto result = functions.execute<DeviceFunction::WaitForFences>(
            device.getHandle(), count, FenceRef::underlyingCast(fences),
            waitAll, timeout);
        GRAPHICS_VERIFY_RESULT(result, "Failed to wait for fences");
    }

    void Fence::reset(const DeviceFunctionTable& functions, const DeviceRef& device,
        const FenceRef* fences, size_t count) {
        for (size_t i = 0; i < count; ++i)
            GRAPHICS_VERIFY(fences[i].isSet(),
                "Trying to reset an invalid fence at index " + std::to_string(i));
        auto result = functions.execute<DeviceFunction::ResetFences>(
            device.getHandle(), count, FenceRef::underlyingCast(fences));
        GRAPHICS_VERIFY_RESULT(result, "Failed to reset fences");
    }
}