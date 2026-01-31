#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    PhysicalDeviceMemoryProperties PhysicalDevice::getMemoryProperties(const InstanceFunctionTable& functions) const
    {
        PhysicalDeviceMemoryProperties props;
        functions.execute<InstanceFunction::GetPhysicalDeviceMemoryProperties>(*this, &props);
		return props;
    }

    std::vector<ExtensionProperties> PhysicalDevice::getExtensions(const InstanceFunctionTable& functions) const
    {
        uint32_t propertyCount = 0;
        auto result = functions.execute<InstanceFunction::EnumerateDeviceExtensionProperties>(
            *this, nullptr, &propertyCount, nullptr);

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device extension properties");

        std::vector<ExtensionProperties> properties(propertyCount);

        result = functions.execute<InstanceFunction::EnumerateDeviceExtensionProperties>(
            *this, nullptr, &propertyCount, properties.data());

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device extension properties");

        return properties;
    }

    bool PhysicalDevice::getSurfaceSupport(const InstanceFunctionTable& functions, const SurfaceRef& surface, uint32_t familyIndex) const
    {
        Bool32 supported;
        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceSupportKHR>(getHandle(),
            familyIndex, surface.getHandle(), &supported);
        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device surface support");
        return supported;
    }

    FormatProperties PhysicalDevice::getFormatProperties(const InstanceFunctionTable& functions,
        PixelFormat format) const
    {
        FormatProperties props;
        functions.execute<InstanceFunction::GetPhysicalDeviceFormatProperties>(*this, format, &props);
		return props;
    }

    SurfaceCapabilities PhysicalDevice::getSurfaceCapabilities(const InstanceFunctionTable& functions,
        const SurfaceRef& surface) const
    {
        GRAPHICS_VERIFY(surface.isSet(), "Trying to get surface capabilities with an invalid surface");
        SurfaceCapabilities capabilities;

        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceCapabilitiesKHR>(*this, surface, &capabilities);

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device surface capabilities");
        return capabilities;        
    }

    std::vector<SurfaceFormat> PhysicalDevice::getSurfaceFormats(const InstanceFunctionTable& functions,
        const SurfaceRef& surface) const
    {
        GRAPHICS_VERIFY(surface.isSet(), "Trying to get surface formats with an invalid surface");
        uint32_t formatsCount = 0;

        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceFormatsKHR>(*this, surface, &formatsCount, nullptr);

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device surface formats");

        std::vector<SurfaceFormat> formats(formatsCount);

        result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceFormatsKHR>(*this, surface, &formatsCount, formats.data());

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device surface formats");

        return formats;        
    }

    std::vector<PresentMode> PhysicalDevice::getSurfacePresentModes(const InstanceFunctionTable& functions,
        const SurfaceRef& surface) const
    {
        GRAPHICS_VERIFY(surface.isSet(), "Trying to get surface present modes with an invalid surface");
        uint32_t presentModesCount = 0;

        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfacePresentModesKHR>(*this, surface, &presentModesCount, nullptr);

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device present modes");

        std::vector<PresentMode> presentModes(presentModesCount);

        result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfacePresentModesKHR>(*this, surface, &presentModesCount, presentModes.data());

        GRAPHICS_VERIFY_RESULT(result, "Failed to get physical device present modes");

        return presentModes;
    }
}