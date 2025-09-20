#include "Queue.h"

namespace Graphics
{
    void Queue::submit(const DeviceFunctionTable& functions,
        std::span<const SubmitInfo> submitInfos,
        const FenceRef& fence) const
    {
        GRAPHICS_VERIFY(!isSet(), "Cannot submit to an unset queue");
        auto result = functions.execute<DeviceFunction::QueueSubmit>(
            getHandle(), submitInfos.size(), SubmitInfo::underlyingCast(submitInfos.data()),
            fence.getHandle());
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to submit to queue: " +
            s_resultMessages.at(result));
    }

    void Queue::present(const DeviceFunctionTable& functions, const PresentInfo& presentInfo) const
    {
        GRAPHICS_VERIFY(!isSet(), "Cannot present to an unset queue");
        auto result = functions.execute<DeviceFunction::QueuePresentKHR>(
            getHandle(), presentInfo.getUnderlyingPointer());
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to present queue: " +
            s_resultMessages.at(result));
    }
}