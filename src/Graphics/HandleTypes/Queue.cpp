#include "Graphics/Graphics.h"

namespace Graphics
{
    Result Queue::submit(const DeviceFunctionTable& functions,
        const QueueSubmitInfo& submitInfo,
        const FenceRef& fence) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot submit to an unset queue");
        return convertCEnum(functions.execute<DeviceFunction::QueueSubmit>(
            getHandle(), 1, QueueSubmitInfo::underlyingCast(&submitInfo),
            fence.getHandle()));
    }

    Result Queue::submit(const DeviceFunctionTable& functions,
        std::span<const QueueSubmitInfo> submitInfos,
        const FenceRef& fence) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot submit to an unset queue");
        return convertCEnum(functions.execute<DeviceFunction::QueueSubmit>(
            getHandle(), submitInfos.size(), QueueSubmitInfo::underlyingCast(submitInfos.data()),
            fence.getHandle()));
    }

    Result Queue::present(const DeviceFunctionTable& functions, const QueuePresentInfo& presentInfo) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot present to an unset queue");
        return convertCEnum(functions.execute<DeviceFunction::QueuePresentKHR>(
            getHandle(), presentInfo.getUnderlyingPointer()));
    }

    Result Queue::waitIdle(const DeviceFunctionTable& functions) const {
        GRAPHICS_VERIFY(isSet(), "Cannot wait for queue idle for an unset queue");
        return convertCEnum(functions.execute<DeviceFunction::QueueWaitIdle>(getHandle()));
    };
}