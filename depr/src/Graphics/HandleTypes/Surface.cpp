#include "../../../include/Graphics/Graphics.h"

#ifdef _WIN32
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#else
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#endif

namespace Graphics
{ 
    void Surface::create(const InstanceFunctionTable& functions, const InstanceRef& instance,
        const Platform::Window& window)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid surface");
        
#ifdef _WIN32
        Win32::SurfaceCreateInfo createInfo;
        createInfo.sType = StructureType::SurfaceCreateInfoWin32;
        createInfo.hwnd = window.getHandle();
        createInfo.hinstance = Platform::Window::getInstanceHandle();
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        auto result = functions.execute<InstanceFunction::CreateWin32SurfaceKHR>(instance, &createInfo, nullptr, this);
#else
        Xlib::SurfaceCreateInfo createInfo{};
        createInfo.sType = StructureType::SurfaceCreateInfoXlib;
        createInfo.dpy = window.getDisplay();
        createInfo.window = window.getHandle();
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        auto result = functions.execute<InstanceFunction::CreateXlibSurfaceKHR>(instance, &createInfo, nullptr, this);
#endif

        GRAPHICS_VERIFY_RESULT(result, "Failed to create a surface");
    }

    void Surface::destroy(const InstanceFunctionTable& functions, const InstanceRef& instance)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid surface");
        functions.execute<InstanceFunction::DestroySurfaceKHR>(instance, getHandle(), nullptr);
        reset();
    }
}