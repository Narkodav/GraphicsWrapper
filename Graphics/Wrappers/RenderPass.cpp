#include "RenderPass.h"

namespace Graphics
{
    void RenderPass::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid render pass");
        auto result = functions.execute<DeviceFunction::CreateRenderPass>(
            device, createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create render pass: " +
            s_resultMessages.at(result));
    }

    void RenderPass::destroy(const DeviceRef& device, const DeviceFunctionTable& functions) {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid render pass");
        functions.execute<DeviceFunction::DestroyRenderPass>(
            device, getHandle(), nullptr);
        reset();
    }
}