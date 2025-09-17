#pragma once
#include "Instance.h"

namespace Graphics {

    Instance::Instance(const std::string& engineName, const std::string& appName,
        Version engineVersion, Version appVersion,
        DebugMessageSeverity::Flags debugMessageSeveritySettings /*= DebugMessageSeverity::Bits::ALL*/,
        DebugMessageType::Flags debugMessageTypeSettings /*= DebugMessageType::Bits::All*/)
#ifdef _DEBUG
        : m_debugMessageSeveritySettings(debugMessageSeveritySettings),
        m_debugMessageTypeSettings(debugMessageTypeSettings)
#endif
    {
        PlatformContext::instance();

        m_appName = appName;
        m_engineName = engineName;
        m_thisPointer = new Instance* { this };

#ifdef _DEBUG
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo =
            Wrappers::DebugUtilsMessenger::getDebugCreateInfo(static_debugCallback,
                debugMessageSeveritySettings, debugMessageTypeSettings, m_thisPointer);

        m_instance.create(engineName, appName,
            engineVersion, appVersion, s_validationLayers,
            s_extensions, &debugCreateInfo);
        m_functions = m_instance.getFunctionTable();

        m_debugMessenger.create(m_instance, m_functions, debugCreateInfo);
        std::cout << __FUNCTION__ << " - Created Instance" << std::endl;
#else
        (void)debugMessageSeveritySettings;
        (void)debugMessageTypeSettings;
        m_instance.create(engineName, appName,
            engineVersion, appVersion, s_validationLayers,
            s_extensions);
        m_functions = m_instance.getFunctionTable();
#endif
    }

    void Instance::destroy()
    {        
#ifdef _DEBUG
        m_debugMessenger.destroy(m_instance, m_functions);
#endif
        m_instance.destroy(m_functions);
        delete m_thisPointer;
#ifdef _DEBUG
        std::cout << __FUNCTION__ << " - Destroyed Instance" << std::endl;
#endif
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Instance::static_debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        Instance* instance = *static_cast<Instance**>(pUserData);
        return instance->debugCallback(messageSeverity, messageType,
            pCallbackData, pUserData);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
#ifdef _DEBUG
        if ((static_cast<uint32_t>(m_debugMessageSeveritySettings) &
            static_cast<uint32_t>(messageSeverity)) == 0)
            return VK_FALSE;
#endif
        std::cerr << "Engine [" << m_engineName << "], Application [" << m_appName << "] ";
        switch (messageSeverity) {
        case static_cast<uint32_t>(DebugMessageSeverity::Bits::Verbose):
            // Diagnostic messages
            std::cout << "VERBOSE: " << pCallbackData->pMessage << std::endl;
            break;

        case static_cast<uint32_t>(DebugMessageSeverity::Bits::Info):
            // Informational messages like resource creation
            std::cout << "INFO: " << pCallbackData->pMessage << std::endl;
            break;

        case static_cast<uint32_t>(DebugMessageSeverity::Bits::Warning):
            // Warning messages like use of deprecated functions
            std::cout << "WARNING: " << pCallbackData->pMessage << std::endl;
            break;

        case static_cast<uint32_t>(DebugMessageSeverity::Bits::Error):
            // Error messages for invalid behavior
            std::cerr << "ERROR: " << pCallbackData->pMessage << std::endl;
            throw std::runtime_error("Validation layer error");
            break;

        default:
            std::cout << "UNKNOWN SEVERITY: " << pCallbackData->pMessage << std::endl;
            std::abort();
            break;
        }
        return VK_FALSE;
    }

}