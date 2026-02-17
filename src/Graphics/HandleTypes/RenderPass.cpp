#include "Graphics/Graphics.h"

namespace Graphics
{
    void RenderPass::create(const DeviceFunctionTable& functions, DeviceRef device,
        const RenderPassCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid render pass");
        auto result = functions.execute<DeviceFunction::CreateRenderPass>(
            device.getHandle(), createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create render a pass");
    }

    void RenderPass::destroy(const DeviceFunctionTable& functions, DeviceRef device) {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid render pass");
        functions.execute<DeviceFunction::DestroyRenderPass>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }
}