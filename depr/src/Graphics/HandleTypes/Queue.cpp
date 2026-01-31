#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void Queue::submit(const DeviceFunctionTable& functions,
        const QueueSubmitInfo& submitInfo,
        const FenceRef& fence) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot submit to an unset queue");
        auto result = functions.execute<DeviceFunction::QueueSubmit>(
            getHandle(), 1, QueueSubmitInfo::underlyingCast(&submitInfo),
            fence.getHandle());
        GRAPHICS_VERIFY_RESULT(result, "Failed to submit to queues");
    }

    void Queue::submit(const DeviceFunctionTable& functions,
        std::span<const QueueSubmitInfo> submitInfos,
        const FenceRef& fence) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot submit to an unset queue");
        auto result = functions.execute<DeviceFunction::QueueSubmit>(
            getHandle(), submitInfos.size(), QueueSubmitInfo::underlyingCast(submitInfos.data()),
            fence.getHandle());
        GRAPHICS_VERIFY_RESULT(result, "Failed to submit to queues");
    }

    Result Queue::present(const DeviceFunctionTable& functions, const QueuePresentInfo& presentInfo) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot present to an unset queue");
        return convertCEnum(functions.execute<DeviceFunction::QueuePresentKHR>(
            getHandle(), presentInfo.getUnderlyingPointer()));
    }

    void Queue::waitIdle(const DeviceFunctionTable& functions) const {
        GRAPHICS_VERIFY(isSet(), "Cannot wait for queue idle for an unset queue");
        auto result = functions.execute<DeviceFunction::QueueWaitIdle>(getHandle());
        GRAPHICS_VERIFY_RESULT(result, "Failed to wait for queue idle");
    };
}