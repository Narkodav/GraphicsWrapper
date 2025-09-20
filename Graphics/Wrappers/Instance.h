#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../InstanceFunctionTable.h"
#include "DebugUtilsMessenger.h"

namespace Graphics
{
    class PhysicalDeviceCache;

    class AppInfo : public StructBase<VkApplicationInfo, AppInfo>
    {
        using Base = StructBase<VkApplicationInfo, AppInfo>;
    public:
        using Base::Base;

        AppInfo(const std::string& engineName,
            const std::string& appName, Version engineVersion, Version appVersion) : Base() {
            this->pApplicationName = appName.data();
            this->applicationVersion = appVersion;
            this->pEngineName = engineName.data();
            engineVersion = engineVersion;
        }

        AppInfo& setEngineName(const std::string& engineName) {
            this->pEngineName = engineName.c_str();
            return *this;
        }

        AppInfo& setAppName(const std::string& appName) {
            this->pApplicationName = appName.c_str();
            return *this;
        }

        AppInfo& setAppVersion(Version version) {
            this->applicationVersion = version;
            return *this;
        }

        AppInfo& setEngineVersion(Version version) {
            this->engineVersion = version;
            return *this;
        }

        const std::string_view getEngineName() const { return this->pEngineName; };
        const std::string_view getAppName() const { return this->pApplicationName; };
        const Version& getEngineVersion() const { return this->engineVersion; };
        const Version& getAppVersion() const { return this->applicationVersion; };
    };

    class LayerProperties : public StructBase<VkLayerProperties, LayerProperties>
    {
        using Base = StructBase<VkLayerProperties, LayerProperties>;
    public:
        using Base::Base;
        const std::string_view getLayerName() const { return this->layerName; };
        const Version& getImplVersion() const { return this->implementationVersion; };
        const std::string_view getDescription() const { return this->description; };
    };

    class ExtensionProperties : public StructBase<VkExtensionProperties, ExtensionProperties>
    {
        using Base = StructBase<VkExtensionProperties, ExtensionProperties>;
    public:
        using Base::Base;
        const std::string_view getExtensionName() const { return this->extensionName; };
        const Version& getSpecVersion() const { return this->specVersion; };
    };

    class PhysicalDevice;

    class InstanceRef : public BaseComponent<VkInstance, InstanceRef> {
        using Base = BaseComponent<VkInstance, InstanceRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Instance";

        std::vector<PhysicalDevice> getPhysicalDevices(const InstanceFunctionTable& functions) const;
        PhysicalDeviceCache cachePhysicalDevices(const InstanceFunctionTable& functions) const;
        InstanceFunctionTable getFunctionTable() const;
    };

    class Instance : public VerificatorComponent<VkInstance, InstanceRef>
    {
        using Base = VerificatorComponent<VkInstance, InstanceRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkInstanceCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkInstanceCreateInfo, CreateInfo>;
        public:
            using Base::Base;

            CreateInfo(const AppInfo& appInfo,
                std::span<const char* const> requiredLayers,
                std::span<const char* const> requiredExtensions,
                const DebugUtils::Messenger::CreateInfo* debugCreateInfo = nullptr) : Base() {
                this->pApplicationInfo = AppInfo::underlyingCast(&appInfo);
                this->enabledLayerCount = requiredLayers.size();
                this->ppEnabledLayerNames = requiredLayers.data();
                this->enabledExtensionCount = requiredExtensions.size();
                this->ppEnabledExtensionNames = requiredExtensions.data();
                this->pNext = debugCreateInfo;
            }

            CreateInfo& setEnabledLayers(std::span<const char* const> requiredLayers) {
                this->enabledLayerCount = requiredLayers.size();
                this->ppEnabledLayerNames = requiredLayers.data();
                return *this;
            }

            CreateInfo& setEnabledExtensions(std::span<const char* const> requiredExtensions) {
                this->enabledExtensionCount = requiredExtensions.size();
                this->ppEnabledExtensionNames = requiredExtensions.data();
                return *this;
            }

            CreateInfo& setAppInfo(const AppInfo& appInfo) {
                this->pApplicationInfo = AppInfo::underlyingCast(&appInfo);
                return *this;
            }

            std::span<const char* const> getEnabledLayers() const {
                return std::span<const char* const>(this->ppEnabledLayerNames,
                    this->enabledLayerCount);
            };

            std::span<const char* const> getEnabledExtensions() const {
                return std::span<const char* const>(this->ppEnabledExtensionNames,
                    this->enabledExtensionCount);
            };

            const AppInfo& getAppInfo() const {
                return *AppInfo::underlyingCast(this->pApplicationInfo);
            };
        };

        static std::vector<LayerProperties> getAvailableValidationLayers();

        static bool validateLayers(
            std::span<const LayerProperties> availible,
            std::span<const char* const> required);

        static std::vector<const char*> getRequiredExtensions();

        static std::vector<ExtensionProperties> getAvailableExtensions();

        static bool validateExtensions(
            std::span<const ExtensionProperties> available,
            std::span<const char* const> required);

        void create(const std::string& engineName,
            const std::string& appName, Version& engineVersion, Version& appVersion,
            std::span<const char* const> requiredValidationLayers,
            std::span<const char* const> requiredExtensions,
            DebugUtils::Messenger::CreateInfo* debugCreateInfo = nullptr);

        void create(const CreateInfo& createInfo);

        void destroy(const InstanceFunctionTable& functions);        
    };

}