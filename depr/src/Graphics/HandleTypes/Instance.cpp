#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    std::vector<LayerProperties> Instance::getAvailableValidationLayers() {
        uint32_t layerCount;
        auto result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        GRAPHICS_VERIFY_RESULT(result, "Failed to enumerate instance layer properties");

        std::vector<LayerProperties> availableLayers(layerCount);
        result = vkEnumerateInstanceLayerProperties(&layerCount, 
            LayerProperties::underlyingCast(availableLayers.data()));

        GRAPHICS_VERIFY_RESULT(result, "Failed to enumerate instance layer properties");

        return availableLayers;
    };

    bool Instance::validateLayers(
        std::span<const LayerProperties> availible,
        std::span<const char* const> required)
    {
        std::vector<const char*> unavailible;

        for (auto& requiredLayer : required) {
            bool found = false;
            for (auto& availibleLayer : availible) {
                if (strcmp(requiredLayer, availibleLayer.getLayerName().data()) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                unavailible.push_back(requiredLayer);
            }
        }
        std::cout << std::endl << "Available layers:" << std::endl;
        for (auto layer : availible)
            std::cout << "\t" << layer.getLayerName() << std::endl;

        std::cout << std::endl << "Required layers:" << std::endl;
        for (auto layer : required)
            std::cout << "\t" << layer << std::endl;

        if (unavailible.size() > 0)
        {
            std::cout << std::endl << "Unavailable layers:" << std::endl;
            for (auto layer : unavailible)
                std::cout << "\t" << layer << std::endl;
            return false;
        }
        return true;
    };

    std::vector<const char*> Instance::enumerateRequiredExtensions(
        const std::vector<ExtensionProperties>& availibleExtensions)
    {
        std::vector<const char*> requiredExtensions;

        for (size_t i = 0; i < availibleExtensions.size(); i++)
        {
            if (availibleExtensions[i].getExtensionName() == "VK_KHR_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_KHR_win32_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_MVK_macos_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_MVK_metal_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_KHR_xlib_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_KHR_xcb_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_KHR_wayland_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
            else if (availibleExtensions[i].getExtensionName() == "VK_EXT_headless_surface")
            {
                requiredExtensions.push_back(availibleExtensions[i].getExtensionName().data());
            }
        }

        return requiredExtensions;
    };

    std::vector<ExtensionProperties> Instance::getAvailableExtensions()
    {
        uint32_t extensionCount = 0;
        auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        GRAPHICS_VERIFY_RESULT(result, "Failed to enumerate instance extension properties");

        std::vector<ExtensionProperties> extensions(extensionCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, 
            ExtensionProperties::underlyingCast(extensions.data()));

        GRAPHICS_VERIFY_RESULT(result, "Failed to enumerate instance extension properties");

        return extensions;
    };

    bool Instance::validateExtensions(
        std::span<const ExtensionProperties> available,
        std::span<const char* const> required)
    {
        std::vector<const char*> unavailible;

        for (auto& requiredExtension : required) {
            bool found = false;
            for (auto& availibleExtension : available) {
                if (std::strcmp(requiredExtension, availibleExtension.getExtensionName().data()) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                unavailible.push_back(requiredExtension);
            }
        }

        std::cout << std::endl << "Available extensions:" << std::endl;
        for (auto extension : available)
            std::cout << "\t" << extension.getExtensionName() << std::endl;

        std::cout << std::endl << "Required extensions:" << std::endl;
        for (auto extension : required)
            std::cout << "\t" << extension << std::endl;

        if (unavailible.size() > 0)
        {
            std::cout << std::endl << "Unavailable extensions:" << std::endl;
            for (auto extension : unavailible)
                std::cout << "\t" << extension << std::endl;
            return false;
        }
        return true;
    };

    void Instance::create(const std::string& engineName,
        const std::string& appName, Version& engineVersion, Version& appVersion,
        std::span<const char* const> requiredValidationLayers,
        std::span<const char* const> requiredExtensions,
        DebugUtils::MessengerCreateInfo* debugCreateInfo /*= nullptr*/)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create an already existing instance");
        AppInfo appInfo = AppInfo()
            .setEngineName(appName)
            .setAppName(engineName)
            .setEngineVersion(engineVersion)
            .setAppVersion(appVersion);

        std::vector<ExtensionProperties> availableExtensions = getAvailableExtensions();
        std::vector<LayerProperties> availableLayers = getAvailableValidationLayers();
        std::vector<const char*> requiredExtensionsFull = enumerateRequiredExtensions(availableExtensions);
        requiredExtensionsFull.insert(requiredExtensionsFull.end(),
            requiredExtensions.begin(), requiredExtensions.end());
        validateExtensions(availableExtensions, requiredExtensionsFull);
        validateLayers(availableLayers, requiredValidationLayers);
        InstanceCreateInfo createInfo = InstanceCreateInfo()
            .setAppInfo(appInfo)
            .setEnabledExtensions(requiredExtensionsFull)
            .setEnabledLayers(requiredValidationLayers)
            .setNext(debugCreateInfo);
        create(createInfo);
    };

    void Instance::create(const InstanceCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create an already existing instance");
        auto result = vkCreateInstance(createInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create an instance");
    }

    void Instance::destroy(const InstanceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid instance");
        functions.execute<InstanceFunction::DestroyInstance>(
            getHandle(),
            nullptr
        );
        reset();
    };

    InstanceFunctionTable InstanceRef::getFunctionTable() const
    {
        GRAPHICS_VERIFY(isSet(), "Trying to get function table from an invalid instance");
        InstanceFunctionTable table(vkGetInstanceProcAddr);
        table.loadAllFunctions(getHandle());
        return table;
    };

    std::vector<PhysicalDevice> InstanceRef::getPhysicalDevices(
        const InstanceFunctionTable& functions) const
    {
        GRAPHICS_VERIFY(isSet(), "Trying to get physical devices from an invalid instance");
        uint32_t deviceCount = 0;
        auto result = functions.execute<InstanceFunction::EnumeratePhysicalDevices>(
            *this,
            &deviceCount,
            nullptr
        );

        GRAPHICS_VERIFY_RESULT(result, "Failed to enumerate physical devices");

        std::vector<PhysicalDevice> devices(deviceCount);
        result = functions.execute<InstanceFunction::EnumeratePhysicalDevices>(
            *this,
            &deviceCount,
            devices.data()
        );

        GRAPHICS_VERIFY_RESULT(result, "Failed to enumerate physical devices");

        return devices;
    };

    PhysicalDeviceCache InstanceRef::cachePhysicalDevices(const InstanceFunctionTable& functions) const
    {
        return PhysicalDeviceCache(*this, functions);
    }
}