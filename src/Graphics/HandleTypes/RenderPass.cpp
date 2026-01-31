#include "Graphics/Graphics.h"

namespace Graphics
{
    void RenderPass::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const RenderPassCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid render pass");
        auto result = functions.execute<DeviceFunction::CreateRenderPass>(
            device, createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create render a pass");
    }

    void RenderPass::destroy(const DeviceFunctionTable& functions, const DeviceRef& device) {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid render pass");
        functions.execute<DeviceFunction::DestroyRenderPass>(
            device, getHandle(), nullptr);
        reset();
    }

    RenderPassBeginInfo::RenderPassBeginInfo(const RenderPassRef& renderPass, const FrameBufferRef& frameBuffer,
        std::span<const ClearValue> clearValues, const Rect2D& renderArea) : RenderPassBeginInfo::Base()
    {
        this->renderPass = renderPass;
        this->framebuffer = frameBuffer.getHandle();
        this->clearValueCount = clearValues.size();
        this->pClearValues = ClearValue::underlyingCast(clearValues.data());
        this->renderArea = renderArea;
    }

    RenderPassBeginInfo& RenderPassBeginInfo::setRenderPass(const RenderPassRef& renderPass) {
        this->renderPass = renderPass;
        return *this;
    }

    RenderPassBeginInfo& RenderPassBeginInfo::setFrameBuffer(const FrameBufferRef& frameBuffer) {
        this->framebuffer = frameBuffer.getHandle();
        return *this;
    }

    RenderPassBeginInfo& RenderPassBeginInfo::setClearValues(std::span<const ClearValue> clearValues) {
        this->clearValueCount = clearValues.size();
        this->pClearValues = ClearValue::underlyingCast(clearValues.data());
        return *this;
    }

    RenderPassBeginInfo& RenderPassBeginInfo::setRenderArea(const Rect2D& renderArea) {
        this->renderArea = renderArea;
        return *this;
    }
}