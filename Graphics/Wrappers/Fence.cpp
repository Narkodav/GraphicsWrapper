#include "Fence.h"

namespace Graphics
{
    void Fence::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const CreateInfo& createInfo /*= CreateInfo()*/)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid fence");

        auto result = functions.execute<DeviceFunction::CreateSemaphore>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to create fence: " + s_resultMessages.at(result));
    }

    void Fence::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid fence");
        functions.execute<DeviceFunction::DestroyFence>(device.getHandle(), getHandle(), nullptr);
        Fence::BaseComponent<VkFence, FenceRef>::reset();
    }

    void FenceRef::wait(const DeviceRef& device, const DeviceFunctionTable& functions,
        size_t timeout /*= std::numeric_limits<size_t>::max()*/) const
    {
        wait(device, functions, this, 1, timeout, true);
    }

    void Fence::reset(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        reset(device, functions, this, 1);
    }

    void FenceRef::wait(const DeviceRef& device, const DeviceFunctionTable& functions,
        const std::vector<Fence>& fences, size_t timeout /*= std::numeric_limits<size_t>::max()*/,
        bool waitAll /*= true*/)
    {
        wait(device, functions, reinterpret_cast<const FenceRef*>(fences.data()), fences.size(), timeout, true);
    }

    void Fence::reset(const DeviceRef& device, const DeviceFunctionTable& functions,
        const std::vector<Fence>& fences)
    {
        reset(device, functions, reinterpret_cast<const FenceRef*>(fences.data()), fences.size());
    }

    void FenceRef::wait(const DeviceRef& device, const DeviceFunctionTable& functions,
        const std::vector<FenceRef>& fences, size_t timeout /*= std::numeric_limits<size_t>::max()*/,
        bool waitAll /*= true*/)
    {
        wait(device, functions, fences.data(), fences.size(), timeout, true);
    }

    void Fence::reset(const DeviceRef& device, const DeviceFunctionTable& functions,
        const std::vector<FenceRef>& fences)
    {
        reset(device, functions, fences.data(), fences.size());
    }

    void FenceRef::wait(const DeviceRef& device, const DeviceFunctionTable& functions,
        const FenceRef* fences, size_t count, size_t timeout /*= std::numeric_limits<size_t>::max()*/,
        bool waitAll /*= true*/) {
        for (size_t i = 0; i < count; ++i)
            GRAPHICS_VERIFY(fences[i].isSet(),
                "Trying to wait for an invalid fence at index " + std::to_string(i));

        auto result = functions.execute<DeviceFunction::WaitForFences>(
            device.getHandle(), count, FenceRef::underlyingCast(fences),
            waitAll, timeout);
        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to wait for fences: " + s_resultMessages.at(result));
    }

    void Fence::reset(const DeviceRef& device, const DeviceFunctionTable& functions,
        const FenceRef* fences, size_t count) {
        for (size_t i = 0; i < count; ++i)
            GRAPHICS_VERIFY(fences[i].isSet(),
                "Trying to reset an invalid fence at index " + std::to_string(i));
        auto result = functions.execute<DeviceFunction::ResetFences>(
            device.getHandle(), count, FenceRef::underlyingCast(fences));
        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to reset fences: " + s_resultMessages.at(result));
    }
}