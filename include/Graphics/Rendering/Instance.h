#pragma once
#include "../Common.h"
#include "../Enums.h"
#include "../Flags.h"
#include "../PlatformManagement/PlatformContext.h"
#include "../PlatformManagement/Window.h"

#include "../Wrappers.h"


namespace Graphics
{
    class Instance
    {
    private:
        std::string m_engineName;
        std::string m_appName;
        Wrappers::Instance m_instance;
        InstanceFunctionTable m_functions;
		Instance** m_thisPointer = nullptr;
        
        //DebugRelated
#ifdef _DEBUG
        DebugMessageSeverity::Flags m_debugMessageSeveritySettings = DebugMessageSeverity::Bits::None;
        DebugMessageType::Flags m_debugMessageTypeSettings = DebugMessageType::Bits::None;
        Wrappers::DebugUtilsMessenger m_debugMessenger;
#endif

    public:
        static inline const std::vector<const char*> s_validationLayers = {
    #ifdef _DEBUG
        "VK_LAYER_KHRONOS_validation"
    #endif
        };

        static inline const std::vector<const char*> s_extensions = {
    #ifdef _DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    #endif
        };

#ifdef _DEBUG
        static inline const bool enabledValidationLayers = true;
#else
        static inline const bool enabledValidationLayers = false;
#endif

    public:

        Instance() : m_instance(nullptr)
#ifdef _DEBUG
            , m_debugMessenger(nullptr)
#endif
        {
        };

        Instance(const std::string& engineName, const std::string& appName,
            Version engineVersion, Version appVersion,
            DebugMessageSeverity::Flags debugMessageSeveritySettings = DebugMessageSeverity::Bits::All,
            DebugMessageType::Flags debugMessageTypeSettings = DebugMessageType::Bits::AllLegacy);

        Instance(Instance&& other) noexcept
        {
            m_engineName = std::exchange(other.m_engineName, std::string());
            m_appName = std::exchange(other.m_appName, std::string());
            m_instance = std::move(other.m_instance);
            m_functions = std::exchange(other.m_functions, InstanceFunctionTable());
            m_thisPointer = std::exchange(other.m_thisPointer, nullptr);
            *m_thisPointer = this;
#ifdef _DEBUG
            m_debugMessenger = std::move(other.m_debugMessenger);
            m_debugMessageSeveritySettings = std::exchange(other.m_debugMessageSeveritySettings, DebugMessageSeverity::Bits::All);
            m_debugMessageTypeSettings = std::exchange(other.m_debugMessageTypeSettings, DebugMessageType::Bits::AllLegacy);
#endif
        };

        Instance& operator=(Instance&& other) noexcept
        {
            if (&other == this) {
                return *this;
            }

            m_engineName = std::exchange(other.m_engineName, std::string());
            m_appName = std::exchange(other.m_appName, std::string());
            m_instance = std::move(other.m_instance);
            m_functions = std::exchange(other.m_functions, InstanceFunctionTable());
            m_thisPointer = std::exchange(other.m_thisPointer, nullptr);
            *m_thisPointer = this;
#ifdef _DEBUG
            m_debugMessenger = std::move(other.m_debugMessenger);
            m_debugMessageSeveritySettings = std::exchange(other.m_debugMessageSeveritySettings, DebugMessageSeverity::Bits::All);
            m_debugMessageTypeSettings = std::exchange(other.m_debugMessageTypeSettings, DebugMessageType::Bits::AllLegacy);
#endif
            return *this;
        };

        Instance(const Instance&) noexcept = delete;
        Instance& operator=(const Instance&) noexcept = delete;

        ~Instance() = default;

        const InstanceFunctionTable& getFunctionTable() const { return m_functions; };

        const std::string& getEngineName() const { return m_engineName; };
        const std::string& getAppName() const { return m_appName; };
        const VkDebugUtilsMessengerEXT& getDebugManager() const {
#ifdef _DEBUG
            return m_debugMessenger;
#else
            return vk::DebugUtilsMessengerEXT();
#endif
        };

        const auto& getHandle() const { return m_instance; };

        static VKAPI_ATTR VkBool32 VKAPI_CALL static_debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        void destroy();
    };
}

