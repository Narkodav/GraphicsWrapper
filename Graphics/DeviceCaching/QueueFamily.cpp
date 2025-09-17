#include "QueueFamily.h"

namespace Graphics {

    QueueFamily::QueueFamily(const VkQueueFamilyProperties2& properties, uint32_t familyIndex) :
        m_familyIndex(familyIndex)
    {
        VkQueueFamilyProperties baseProps = properties.queueFamilyProperties;
        VkQueueFamilyGlobalPriorityPropertiesKHR globalPriorityProps =
            *static_cast<VkQueueFamilyGlobalPriorityPropertiesKHR*>(properties.pNext);
        VkQueueFamilyVideoPropertiesKHR videoProps =
            *static_cast<VkQueueFamilyVideoPropertiesKHR*>(globalPriorityProps.pNext);
        VkQueueFamilyQueryResultStatusPropertiesKHR queryResultProps =
            *static_cast<VkQueueFamilyQueryResultStatusPropertiesKHR*>(videoProps.pNext);
        VkQueueFamilyCheckpointPropertiesNV checkpointProps =
            *static_cast<VkQueueFamilyCheckpointPropertiesNV*>(queryResultProps.pNext);
        VkQueueFamilyCheckpointProperties2NV checkpoint2Props =
            *static_cast<VkQueueFamilyCheckpointProperties2NV*>(checkpointProps.pNext);

        storeProperty(QueueProperty::QueueFlags,
            static_cast<QueueFlags::Flags>(baseProps.queueFlags));
        storeProperty(QueueProperty::QueueCount,
            static_cast<uint32_t>(baseProps.queueCount));
        storeProperty(QueueProperty::MinImageTransferGranularity,
            std::array<uint32_t, 3>({ baseProps.minImageTransferGranularity.width,
            baseProps.minImageTransferGranularity.height,
            baseProps.minImageTransferGranularity.depth }));
        storeProperty(QueueProperty::TimestampValidBits,
            static_cast<uint32_t>(baseProps.timestampValidBits));

    }

    bool QueueFamily::getSurfaceSupport(const VkSurfaceKHR& surface,
        const VkPhysicalDevice& physicalDevice, const InstanceFunctionTable& functions) const
    {
        VkBool32 supported;
        auto result = functions.execute<InstanceFunction::GetPhysicalDeviceSurfaceSupportKHR>(physicalDevice,
            m_familyIndex, surface, &supported);

        if (result != VK_SUCCESS) throw std::runtime_error("Failed to get physical device surface support: " + s_resultMessages.at(result));

        return supported;
    }

}