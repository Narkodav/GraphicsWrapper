#include "Graphics/Wrappers/InstanceWrapper.h"

namespace Graphics::Wrappers {

    Instance::Instance() {
        createSelf();
        updateSelf();
    }

	Instance::Instance(const AppInfo& appInfo,
        std::span<const char*> requiredExtensions /*= {}*/,
        std::span<const char*> requiredLayers /*= {}*/) {
        createSelf();
        updateSelf();
        createImpl(appInfo, requiredExtensions, requiredLayers);
    }

    GRAPHICS_API_ATTR Bool32 GRAPHICS_API_CALL Instance::static_debugCallback(
        Flags::DebugMessageSeverity::Bits messageSeverity,
        Flags::DebugMessageType messageType,
        const DebugUtils::CallbackData* pCallbackData,
        void* pUserData) {
        auto* self = *reinterpret_cast<Instance**>(pUserData);
        return self->debugCallback(messageSeverity, messageType, pCallbackData);
    }

    Bool32 Instance::debugCallback(
        Flags::DebugMessageSeverity::Bits messageSeverity,
        Flags::DebugMessageType messageType,
        const DebugUtils::CallbackData* pCallbackData) {

        switch (messageSeverity) {
        case Flags::DebugMessageSeverity::Bits::Verbose:
            // Verbose messages for debugging
            m_debugLogger.logMessage<Flags::DebugMessageSeverity::Bits::Verbose>(messageType, pCallbackData);
            break;
        case Flags::DebugMessageSeverity::Bits::Info:
            // Information messages for general use
            m_debugLogger.logMessage<Flags::DebugMessageSeverity::Bits::Info>(messageType, pCallbackData);
            break;
        case Flags::DebugMessageSeverity::Bits::Warning:
            // Warning messages for invalid behavior
            m_debugLogger.logMessage<Flags::DebugMessageSeverity::Bits::Warning>(messageType, pCallbackData);
            m_debugLogger.dumpMessagesToStream(std::cerr);
            break;
        case Flags::DebugMessageSeverity::Bits::Error:
            // Error messages for invalid behavior
            m_debugLogger.logMessage<Flags::DebugMessageSeverity::Bits::Error>(messageType, pCallbackData);
            m_debugLogger.dumpMessagesToStream(std::cerr);
            throw std::runtime_error("Validation layer error");
            break;
        default:
            m_debugLogger.dumpMessagesToStream(std::cerr);
            std::cerr << "Unknown message severity: " << static_cast<uint32_t>(messageSeverity) << std::endl;
            std::abort();
            break;
        }
        return 0;
    }

    void Instance::updateSelf() {
        *m_self = this;
    }

    void Instance::createSelf() {
        m_self = std::make_unique<Instance*>(this);
    }

    Instance::~Instance() {
        destroy();
    }

    void Instance::create(const AppInfo& appInfo,
        std::span<const char*> requiredExtensions /*= {}*/,
        std::span<const char*> requiredLayers /*= {}*/)
    {
        destroy();
        createImpl(appInfo, requiredExtensions, requiredLayers);
    }
    void Instance::destroy() {
        if (m_instance.isValid())
            m_instance.destroy(m_instanceFunctionTable);
    }

    void Instance::createImpl(const AppInfo& appInfo,
        std::span<const char*> requiredExtensions,
        std::span<const char*> requiredLayers)
    {
        m_debugMessengerCreateInfo.setCallback(static_debugCallback)
            .setMessageSeverity(Flags::DebugMessageSeverity::Bits::All)
            .setMessageType(Flags::DebugMessageType::Bits::AllNoAddressBinding)
            .setUserData(m_self.get());

        m_appInfo = appInfo;
        m_createInfo.setAppInfo(m_appInfo)
            .setNext(&m_debugMessengerCreateInfo);
        m_availibleExtensions = Graphics::Instance::getAvailableExtensions();
        m_requiredExtensions = Graphics::Instance::enumerateRequiredExtensions(m_availibleExtensions);
        m_requiredExtensions.insert(m_requiredExtensions.end(), requiredExtensions.begin(), requiredExtensions.end());
        m_requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        m_requiredLayers.insert(m_requiredLayers.end(), requiredLayers.begin(), requiredLayers.end());
        m_createInfo.setEnabledExtensions(m_requiredExtensions)
            .setEnabledLayers(m_requiredLayers);

        m_instance.create(m_createInfo);
        m_instanceFunctionTable = m_instance.getFunctionTable();
        m_debugMessenger.create(m_instanceFunctionTable, m_instance, m_debugMessengerCreateInfo);
    }

    InstanceRef Instance::getReference() const {
        return m_instance;
    }

    void Instance::dumpDebugInfo(std::ostream& stream) const {
        m_debugLogger.dumpMessagesToStream(stream);
    }

    void Instance::cachePhysicalDevices() {
        m_physicalDeviceCache = m_instance.cachePhysicalDevices(m_instanceFunctionTable);
    }
}