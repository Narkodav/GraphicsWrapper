#include "SwapChain.h"

namespace Graphics {
    SwapChain::Images SwapChain::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid swap chain");

		size_t imageCount = 0;

        auto result = functions.execute<DeviceFunction::CreateSwapchainKHR>(
            device.getHandle(), &createInfo, nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create swap chain: " +
            s_resultMessages.at(result));

        auto result = functions.execute<DeviceFunction::GetSwapchainImagesKHR>(
            device.getHandle(), getHandle(), &imageCount, nullptr);
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to get swap chain images: " +
            s_resultMessages.at(result));

        std::vector<VkImage> swapChainImages(imageCount);
        auto result = functions.execute<DeviceFunction::GetSwapchainImagesKHR>(
            device.getHandle(), getHandle(), &imageCount, swapChainImages.data());
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to get swap chain images: " +
            s_resultMessages.at(result));

        return static_cast<Images>(std::move(swapChainImages));
    }

    void SwapChain::destroy(const DeviceRef& device, const DeviceFunctionTable& functions,
        Images& swapChainImages)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid swap chain");
        swapChainImages.clear();
        functions.execute<DeviceFunction::DestroySwapchainKHR>(
            device.getHandle(), getHandle(), nullptr);
    }

    bool SwapChain::acquireNextImage(const DeviceRef& device, const DeviceFunctionTable& functions,
        const SemaphoreRef& semaphore, const FenceRef& fence, uint32_t& imageIndex,
        uint32_t timeout /*= std::numeric_limits<uint32_t>::max()*/)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to acquire next image from an invalid swap chain");
        auto result = functions.execute<DeviceFunction::AcquireNextImageKHR>(
            device.getHandle(), getHandle(), timeout, semaphore.getHandle(),
            fence.getHandle(), &imageIndex);
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to acquire next image: " +
            s_resultMessages.at(result));

        if (result != VK_SUCCESS)
        {
            GRAPHICS_VERIFY(result == vk::Result::eErrorOutOfDateKHR ||
                result == vk::Result::eSuboptimalKHR, "Failed to acquire next image: " +
                s_resultMessages.at(result));
            return false;
        }

        return true;
    }

    void SwapChain::recreate(const DeviceRef& device, const DeviceFunctionTable& functions,
        CreateInfo& createInfo, Images& swapChainImages)
    {
		SwapChain newSwapChain;
        createInfo.setOldSwapChain(*this);
        Images newSwapChainImages = newSwapChain.create(device, functions, createInfo);
        destroy(device, functions, swapChainImages);
        *this = std::move(newSwapChain);
		swapChainImages = std::move(newSwapChainImages);
    }
}