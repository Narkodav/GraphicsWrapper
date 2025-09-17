#include "Surface.h"

namespace Graphics
{
    void Surface::create(const InstanceRef& instance,
        const Window& window, const InstanceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid surface");
        auto result = glfwCreateWindowSurface(instance.getHandle(), window.getHandle(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to create a surface: " + s_resultMessages.at(result));
    }

    void Surface::destroy(const InstanceRef& instance, const InstanceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid surface");
        functions.execute<InstanceFunction::DestroySurfaceKHR>(instance, getHandle(), nullptr);
        reset();
    }

    //SwapChainSupportDetails Surface::getSwapChainSupportDetails(
    //    const InstanceFunctionTable& functions, const PhysicalDevice& device) const
    //{
    //    GRAPHICS_VERIFY(isValid(), "Trying to get swap chain support details from an invalid surface");
    //    SwapChainSupportDetails supports;
    //    supports.capabilities = device.getSurfaceCapabilities(functions, *this);
    //    supports.formats = device.getSurfaceFormats(functions, *this);
    //    supports.presentModes = device.getPresentModes(functions, *this);
    //    return supports;
    //}
}