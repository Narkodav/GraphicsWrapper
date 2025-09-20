#include "PhysicalDevice.h"

namespace Graphics
{
    Memory::Properties PhysicalDevice::getMemoryProperties(const InstanceFunctionTable& functions) const
    {
        Memory::Properties props;
        functions.execute<InstanceFunction::GetPhysicalDeviceMemoryProperties>(
            getHandle(), props.getUnderlyingPointer());
		return props;
    }

    std::vector<ExtensionProperties> PhysicalDevice::getExtensions(const InstanceFunctionTable& functions) const
    {
        uint32_t propertyCount = 0;
        auto result = functions.execute<InstanceFunction::EnumerateDeviceExtensionProperties>(
            getHandle(), nullptr, &propertyCount, nullptr);

        if (result != VK_SUCCESS) throw std::runtime_error("Failed to get physical device extension properties: " +
            s_resultMessages.at(result));

        std::vector<ExtensionProperties> properties(propertyCount);

        result = functions.execute<InstanceFunction::EnumerateDeviceExtensionProperties>(
            getHandle(), nullptr, &propertyCount, ExtensionProperties::underlyingCast(properties.data()));

        if (result != VK_SUCCESS) throw std::runtime_error("Failed to get physical device extension properties: " +
            s_resultMessages.at(result));
        return properties;
    }

    bool PhysicalDevice::getSurfaceSupport(const InstanceFunctionTable& functions, const SurfaceRef& surface, uint32_t familyIndex) const
    {
        VkBool32 supported;
        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceSupportKHR>(getHandle(),
            familyIndex, surface.getHandle(), &supported);
        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to get physical device surface support: " + s_resultMessages.at(result));
        return supported;
    }

    FormatProperties PhysicalDevice::getFormatProperties(const InstanceFunctionTable& functions,
        Format format) const
    {
        FormatProperties props;
        functions.execute<InstanceFunction::GetPhysicalDeviceFormatProperties>(
            getHandle(), convertCEnum(format), props.getUnderlyingPointer());
		return props;
    }

    Surface::Capabilities PhysicalDevice::getSurfaceCapabilities(const InstanceFunctionTable& functions,
        const SurfaceRef& surface) const
    {
        GRAPHICS_VERIFY(surface.isSet(), "Trying to get surface capabilities with an invalid surface");
        Surface::Capabilities capabilities;

        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceCapabilitiesKHR>(
            getHandle(), surface.getHandle(), 
            capabilities.getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to get physical device surface capabilities: " + s_resultMessages.at(result));

        return capabilities;        
    }

    std::vector<Surface::Format> PhysicalDevice::getSurfaceFormats(const InstanceFunctionTable& functions,
        const SurfaceRef& surface) const
    {
        GRAPHICS_VERIFY(surface.isSet(), "Trying to get surface formats with an invalid surface");
        uint32_t formatsCount = 0;

        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceFormatsKHR>(
            getHandle(), surface.getHandle(), &formatsCount, nullptr);

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to get physical device surface formats: " + s_resultMessages.at(result));

        std::vector<Surface::Format> formats(formatsCount);

        result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceFormatsKHR>(
            getHandle(), surface.getHandle(), &formatsCount,
            Surface::Format::underlyingCast(formats.data()));

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to get physical device surface formats: " + s_resultMessages.at(result));

        return formats;        
    }

    std::vector<PresentMode> PhysicalDevice::getSurfacePresentModes(const InstanceFunctionTable& functions,
        const SurfaceRef& surface) const
    {
        GRAPHICS_VERIFY(surface.isSet(), "Trying to get surface present modes with an invalid surface");
        uint32_t presentModesCount = 0;

        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfacePresentModesKHR>(
            getHandle(), surface.getHandle(), &presentModesCount, nullptr);

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to get physical device present modes: " + s_resultMessages.at(result));

        std::vector<PresentMode> presentModes(presentModesCount);

        result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfacePresentModesKHR>(
            getHandle(), surface.getHandle(), &presentModesCount,
            reinterpret_cast<VkPresentModeKHR*>(presentModes.data()));

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to get physical device present modes: " + s_resultMessages.at(result));

        return presentModes;
    }
}