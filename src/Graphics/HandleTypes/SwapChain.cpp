#include "Graphics/Graphics.h"

namespace Graphics {
    void SwapChain::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const SwapChainCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid swap chain");

        auto result = functions.execute<DeviceFunction::CreateSwapchainKHR>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create swap chain");
    }

    void SwapChain::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid swap chain");
        functions.execute<DeviceFunction::DestroySwapchainKHR>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }

    Result SwapChain::acquireNextImage(const DeviceFunctionTable& functions, const DeviceRef& device,
        const SemaphoreRef& semaphore, const FenceRef& fence, uint32_t& imageIndex,
        uint32_t timeout /*= std::numeric_limits<uint32_t>::max()*/)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to acquire next image from an invalid swap chain");
        return convertCEnum(functions.execute<DeviceFunction::AcquireNextImageKHR>(
            device.getHandle(), getHandle(), timeout, semaphore.getHandle(),
            fence.getHandle(), &imageIndex));
    }

    void SwapChain::recreate(const DeviceFunctionTable& functions, const DeviceRef& device,
        SwapChainCreateInfo& createInfo)
    {
		SwapChain newSwapChain;
        createInfo.setOldSwapChain(*this);
        newSwapChain.create(functions, device, createInfo);
        destroy(functions, device);
        *this = std::move(newSwapChain);
    }

    std::vector<ImageRef> SwapChainRef::getImages(const DeviceFunctionTable& functions, const DeviceRef& device) const {
        uint32_t imageCount = 0;

        auto result = functions.execute<DeviceFunction::GetSwapchainImagesKHR>(
            device.getHandle(), getHandle(), &imageCount, nullptr);

        GRAPHICS_VERIFY_RESULT(result, "Failed to get swap chain images");

        std::vector<ImageRef> swapChainImages(imageCount);
        result = functions.execute<DeviceFunction::GetSwapchainImagesKHR>(
            device.getHandle(), getHandle(), &imageCount, ImageRef::underlyingCast(swapChainImages.data()));

        GRAPHICS_VERIFY_RESULT(result, "Failed to get swap chain images");

        return swapChainImages;    
    }
}