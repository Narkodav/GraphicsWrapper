#pragma once
#include "Enums.h"

namespace Graphics {
    template<StructureType T>
    struct EnumToStructTraits { static_assert(false, "StructureType not implemented"); };

    template<typename T>
    struct StructToEnumTraits { static_assert(false, "StructureType not implemented"); };

    template<StructureType type>
    using EnumToStructTraits_t = EnumToStructTraits<type>::Type;

    template<StructureType type>
    using EnumToStructTraits_ct = EnumToStructTraits<type>::CType;

    template<typename T>
    constexpr auto StructToEnumTraits_v = StructToEnumTraits<T>::s_type;

    // PhysicalDeviceProperties2
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceProperties2> {
        using Type = vk::PhysicalDeviceProperties2;
        using CType = VkPhysicalDeviceProperties2;
        static constexpr auto s_name = "VkPhysicalDeviceProperties2";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceProperties2> {
        static constexpr auto s_type = StructureType::PhysicalDeviceProperties2;
        static constexpr auto s_name = "VkPhysicalDeviceProperties2";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceProperties2> {
        static constexpr auto s_type = StructureType::PhysicalDeviceProperties2;
        static constexpr auto s_name = "VkPhysicalDeviceProperties2";
    };

    // PhysicalDeviceFeatures2
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceFeatures2> {
        using Type = vk::PhysicalDeviceFeatures2;
        using CType = VkPhysicalDeviceFeatures2;
        static constexpr auto s_name = "VkPhysicalDeviceFeatures2";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceFeatures2> {
        static constexpr auto s_type = StructureType::PhysicalDeviceFeatures2;
        static constexpr auto s_name = "VkPhysicalDeviceFeatures2";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceFeatures2> {
        static constexpr auto s_type = StructureType::PhysicalDeviceFeatures2;
        static constexpr auto s_name = "VkPhysicalDeviceFeatures2";
    };

    // PhysicalDeviceMemoryProperties2
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMemoryProperties2> {
        using Type = vk::PhysicalDeviceMemoryProperties2;
        using CType = VkPhysicalDeviceMemoryProperties2;
        static constexpr auto s_name = "VkPhysicalDeviceMemoryProperties2";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMemoryProperties2> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMemoryProperties2;
        static constexpr auto s_name = "VkPhysicalDeviceMemoryProperties2";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMemoryProperties2> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMemoryProperties2;
        static constexpr auto s_name = "VkPhysicalDeviceMemoryProperties2";
    };

    // PhysicalDeviceVulkan11Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkan11Features> {
        using Type = vk::PhysicalDeviceVulkan11Features;
        using CType = VkPhysicalDeviceVulkan11Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan11Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkan11Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan11Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan11Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkan11Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan11Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan11Features";
    };

    // PhysicalDeviceVulkan11Properties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkan11Properties> {
        using Type = vk::PhysicalDeviceVulkan11Properties;
        using CType = VkPhysicalDeviceVulkan11Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan11Properties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkan11Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan11Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan11Properties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkan11Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan11Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan11Properties";
    };

    // PhysicalDeviceVulkan12Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkan12Features> {
        using Type = vk::PhysicalDeviceVulkan12Features;
        using CType = VkPhysicalDeviceVulkan12Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan12Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkan12Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan12Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan12Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkan12Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan12Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan12Features";
    };

    // PhysicalDeviceVulkan12Properties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkan12Properties> {
        using Type = vk::PhysicalDeviceVulkan12Properties;
        using CType = VkPhysicalDeviceVulkan12Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan12Properties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkan12Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan12Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan12Properties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkan12Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan12Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan12Properties";
    };

    // PhysicalDeviceVulkan13Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkan13Features> {
        using Type = vk::PhysicalDeviceVulkan13Features;
        using CType = VkPhysicalDeviceVulkan13Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan13Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkan13Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan13Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan13Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkan13Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan13Features;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan13Features";
    };

    // PhysicalDeviceVulkan13Properties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkan13Properties> {
        using Type = vk::PhysicalDeviceVulkan13Properties;
        using CType = VkPhysicalDeviceVulkan13Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan13Properties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkan13Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan13Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan13Properties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkan13Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkan13Properties;
        static constexpr auto s_name = "VkPhysicalDeviceVulkan13Properties";
    };

    // PhysicalDevice16BitStorageFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDevice16BitStorageFeatures> {
        using Type = vk::PhysicalDevice16BitStorageFeatures;
        using CType = VkPhysicalDevice16BitStorageFeatures;
        static constexpr auto s_name = "VkPhysicalDevice16BitStorageFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDevice16BitStorageFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevice16BitStorageFeatures;
        static constexpr auto s_name = "VkPhysicalDevice16BitStorageFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDevice16BitStorageFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevice16BitStorageFeatures;
        static constexpr auto s_name = "VkPhysicalDevice16BitStorageFeatures";
    };

    // PhysicalDeviceMultiviewFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMultiviewFeatures> {
        using Type = vk::PhysicalDeviceMultiviewFeatures;
        using CType = VkPhysicalDeviceMultiviewFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceMultiviewFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMultiviewFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMultiviewFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceMultiviewFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMultiviewFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMultiviewFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceMultiviewFeatures";
    };

    // PhysicalDeviceDescriptorIndexingFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceDescriptorIndexingFeatures> {
        using Type = vk::PhysicalDeviceDescriptorIndexingFeatures;
        using CType = VkPhysicalDeviceDescriptorIndexingFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceDescriptorIndexingFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceDescriptorIndexingFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceDescriptorIndexingFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceDescriptorIndexingFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceDescriptorIndexingFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceDescriptorIndexingFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceDescriptorIndexingFeatures";
    };

    // PhysicalDeviceDescriptorIndexingProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceDescriptorIndexingProperties> {
        using Type = vk::PhysicalDeviceDescriptorIndexingProperties;
        using CType = VkPhysicalDeviceDescriptorIndexingProperties;
        static constexpr auto s_name = "VkPhysicalDeviceDescriptorIndexingProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceDescriptorIndexingProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceDescriptorIndexingProperties;
        static constexpr auto s_name = "VkPhysicalDeviceDescriptorIndexingProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceDescriptorIndexingProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceDescriptorIndexingProperties;
        static constexpr auto s_name = "VkPhysicalDeviceDescriptorIndexingProperties";
    };

    // PhysicalDeviceScalarBlockLayoutFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceScalarBlockLayoutFeatures> {
        using Type = vk::PhysicalDeviceScalarBlockLayoutFeatures;
        using CType = VkPhysicalDeviceScalarBlockLayoutFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceScalarBlockLayoutFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceScalarBlockLayoutFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceScalarBlockLayoutFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceScalarBlockLayoutFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceScalarBlockLayoutFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceScalarBlockLayoutFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceScalarBlockLayoutFeatures";
    };

    // PhysicalDeviceVulkanMemoryModelFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVulkanMemoryModelFeatures> {
        using Type = vk::PhysicalDeviceVulkanMemoryModelFeatures;
        using CType = VkPhysicalDeviceVulkanMemoryModelFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceVulkanMemoryModelFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVulkanMemoryModelFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkanMemoryModelFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceVulkanMemoryModelFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVulkanMemoryModelFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVulkanMemoryModelFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceVulkanMemoryModelFeatures";
    };

    // PhysicalDeviceImagelessFramebufferFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceImagelessFramebufferFeatures> {
        using Type = vk::PhysicalDeviceImagelessFramebufferFeatures;
        using CType = VkPhysicalDeviceImagelessFramebufferFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceImagelessFramebufferFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceImagelessFramebufferFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceImagelessFramebufferFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceImagelessFramebufferFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceImagelessFramebufferFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceImagelessFramebufferFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceImagelessFramebufferFeatures";
    };

    // PhysicalDeviceUniformBufferStandardLayoutFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceUniformBufferStandardLayoutFeatures> {
        using Type = vk::PhysicalDeviceUniformBufferStandardLayoutFeatures;
        using CType = VkPhysicalDeviceUniformBufferStandardLayoutFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceUniformBufferStandardLayoutFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceUniformBufferStandardLayoutFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceUniformBufferStandardLayoutFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceUniformBufferStandardLayoutFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceUniformBufferStandardLayoutFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceUniformBufferStandardLayoutFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceUniformBufferStandardLayoutFeatures";
    };

    // PhysicalDeviceShaderSubgroupExtendedTypesFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderSubgroupExtendedTypesFeatures> {
        using Type = vk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures;
        using CType = VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderSubgroupExtendedTypesFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderSubgroupExtendedTypesFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderSubgroupExtendedTypesFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures";
    };

    // PhysicalDeviceSeparateDepthStencilLayoutsFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceSeparateDepthStencilLayoutsFeatures> {
        using Type = vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures;
        using CType = VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSeparateDepthStencilLayoutsFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSeparateDepthStencilLayoutsFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures";
    };

    // PhysicalDeviceHostQueryResetFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceHostQueryResetFeatures> {
        using Type = vk::PhysicalDeviceHostQueryResetFeatures;
        using CType = VkPhysicalDeviceHostQueryResetFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceHostQueryResetFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceHostQueryResetFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceHostQueryResetFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceHostQueryResetFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceHostQueryResetFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceHostQueryResetFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceHostQueryResetFeatures";
    };

    // PhysicalDeviceTimelineSemaphoreFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceTimelineSemaphoreFeatures> {
        using Type = vk::PhysicalDeviceTimelineSemaphoreFeatures;
        using CType = VkPhysicalDeviceTimelineSemaphoreFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceTimelineSemaphoreFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceTimelineSemaphoreFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceTimelineSemaphoreFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceTimelineSemaphoreFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceTimelineSemaphoreFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceTimelineSemaphoreFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceTimelineSemaphoreFeatures";
    };

    // PhysicalDeviceTimelineSemaphoreProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceTimelineSemaphoreProperties> {
        using Type = vk::PhysicalDeviceTimelineSemaphoreProperties;
        using CType = VkPhysicalDeviceTimelineSemaphoreProperties;
        static constexpr auto s_name = "VkPhysicalDeviceTimelineSemaphoreProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceTimelineSemaphoreProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceTimelineSemaphoreProperties;
        static constexpr auto s_name = "VkPhysicalDeviceTimelineSemaphoreProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceTimelineSemaphoreProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceTimelineSemaphoreProperties;
        static constexpr auto s_name = "VkPhysicalDeviceTimelineSemaphoreProperties";
    };

    // PhysicalDeviceBufferDeviceAddressFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceBufferDeviceAddressFeatures> {
        using Type = vk::PhysicalDeviceBufferDeviceAddressFeatures;
        using CType = VkPhysicalDeviceBufferDeviceAddressFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceBufferDeviceAddressFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceBufferDeviceAddressFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceBufferDeviceAddressFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceBufferDeviceAddressFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceBufferDeviceAddressFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceBufferDeviceAddressFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceBufferDeviceAddressFeatures";
    };

    // PhysicalDeviceBufferDeviceAddressFeaturesEXT
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceBufferDeviceAddressFeaturesEXT> {
        using Type = vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT;
        using CType = VkPhysicalDeviceBufferDeviceAddressFeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceBufferDeviceAddressFeaturesEXT";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceBufferDeviceAddressFeaturesEXT> {
        static constexpr auto s_type = StructureType::PhysicalDeviceBufferDeviceAddressFeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceBufferDeviceAddressFeaturesEXT";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceBufferDeviceAddressFeaturesEXT> {
        static constexpr auto s_type = StructureType::PhysicalDeviceBufferDeviceAddressFeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceBufferDeviceAddressFeaturesEXT";
    };

    // PhysicalDeviceImageRobustnessFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceImageRobustnessFeatures> {
        using Type = vk::PhysicalDeviceImageRobustnessFeatures;
        using CType = VkPhysicalDeviceImageRobustnessFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceImageRobustnessFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceImageRobustnessFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceImageRobustnessFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceImageRobustnessFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceImageRobustnessFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceImageRobustnessFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceImageRobustnessFeatures";
    };

    // PhysicalDeviceInlineUniformBlockFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceInlineUniformBlockFeatures> {
        using Type = vk::PhysicalDeviceInlineUniformBlockFeatures;
        using CType = VkPhysicalDeviceInlineUniformBlockFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceInlineUniformBlockFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceInlineUniformBlockFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceInlineUniformBlockFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceInlineUniformBlockFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceInlineUniformBlockFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceInlineUniformBlockFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceInlineUniformBlockFeatures";
    };

    // PhysicalDeviceInlineUniformBlockProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceInlineUniformBlockProperties> {
        using Type = vk::PhysicalDeviceInlineUniformBlockProperties;
        using CType = VkPhysicalDeviceInlineUniformBlockProperties;
        static constexpr auto s_name = "VkPhysicalDeviceInlineUniformBlockProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceInlineUniformBlockProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceInlineUniformBlockProperties;
        static constexpr auto s_name = "VkPhysicalDeviceInlineUniformBlockProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceInlineUniformBlockProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceInlineUniformBlockProperties;
        static constexpr auto s_name = "VkPhysicalDeviceInlineUniformBlockProperties";
    };

    // PhysicalDevicePipelineCreationCacheControlFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDevicePipelineCreationCacheControlFeatures> {
        using Type = vk::PhysicalDevicePipelineCreationCacheControlFeatures;
        using CType = VkPhysicalDevicePipelineCreationCacheControlFeatures;
        static constexpr auto s_name = "VkPhysicalDevicePipelineCreationCacheControlFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDevicePipelineCreationCacheControlFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevicePipelineCreationCacheControlFeatures;
        static constexpr auto s_name = "VkPhysicalDevicePipelineCreationCacheControlFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDevicePipelineCreationCacheControlFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevicePipelineCreationCacheControlFeatures;
        static constexpr auto s_name = "VkPhysicalDevicePipelineCreationCacheControlFeatures";
    };

    // PhysicalDevicePrivateDataFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDevicePrivateDataFeatures> {
        using Type = vk::PhysicalDevicePrivateDataFeatures;
        using CType = VkPhysicalDevicePrivateDataFeatures;
        static constexpr auto s_name = "VkPhysicalDevicePrivateDataFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDevicePrivateDataFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevicePrivateDataFeatures;
        static constexpr auto s_name = "VkPhysicalDevicePrivateDataFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDevicePrivateDataFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevicePrivateDataFeatures;
        static constexpr auto s_name = "VkPhysicalDevicePrivateDataFeatures";
    };

    // PhysicalDeviceShaderDemoteToHelperInvocationFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderDemoteToHelperInvocationFeatures> {
        using Type = vk::PhysicalDeviceShaderDemoteToHelperInvocationFeatures;
        using CType = VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderDemoteToHelperInvocationFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderDemoteToHelperInvocationFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderDemoteToHelperInvocationFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures";
    };

    // PhysicalDeviceShaderTerminateInvocationFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderTerminateInvocationFeatures> {
        using Type = vk::PhysicalDeviceShaderTerminateInvocationFeatures;
        using CType = VkPhysicalDeviceShaderTerminateInvocationFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderTerminateInvocationFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderTerminateInvocationFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderTerminateInvocationFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderTerminateInvocationFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderTerminateInvocationFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderTerminateInvocationFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderTerminateInvocationFeatures";
    };

    // PhysicalDeviceSubgroupSizeControlFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceSubgroupSizeControlFeatures> {
        using Type = vk::PhysicalDeviceSubgroupSizeControlFeatures;
        using CType = VkPhysicalDeviceSubgroupSizeControlFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSubgroupSizeControlFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceSubgroupSizeControlFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSubgroupSizeControlFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSubgroupSizeControlFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceSubgroupSizeControlFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSubgroupSizeControlFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSubgroupSizeControlFeatures";
    };

    // PhysicalDeviceSubgroupSizeControlProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceSubgroupSizeControlProperties> {
        using Type = vk::PhysicalDeviceSubgroupSizeControlProperties;
        using CType = VkPhysicalDeviceSubgroupSizeControlProperties;
        static constexpr auto s_name = "VkPhysicalDeviceSubgroupSizeControlProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceSubgroupSizeControlProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSubgroupSizeControlProperties;
        static constexpr auto s_name = "VkPhysicalDeviceSubgroupSizeControlProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceSubgroupSizeControlProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSubgroupSizeControlProperties;
        static constexpr auto s_name = "VkPhysicalDeviceSubgroupSizeControlProperties";
    };

    // PhysicalDeviceSynchronization2Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceSynchronization2Features> {
        using Type = vk::PhysicalDeviceSynchronization2Features;
        using CType = VkPhysicalDeviceSynchronization2Features;
        static constexpr auto s_name = "VkPhysicalDeviceSynchronization2Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceSynchronization2Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSynchronization2Features;
        static constexpr auto s_name = "VkPhysicalDeviceSynchronization2Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceSynchronization2Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSynchronization2Features;
        static constexpr auto s_name = "VkPhysicalDeviceSynchronization2Features";
    };

    // PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures> {
        using Type = vk::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures;
        using CType = VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceZeroInitializeWorkgroupMemoryFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures";
    };

    // PhysicalDeviceDynamicRenderingFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceDynamicRenderingFeatures> {
        using Type = vk::PhysicalDeviceDynamicRenderingFeatures;
        using CType = VkPhysicalDeviceDynamicRenderingFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceDynamicRenderingFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceDynamicRenderingFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceDynamicRenderingFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceDynamicRenderingFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceDynamicRenderingFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceDynamicRenderingFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceDynamicRenderingFeatures";
    };

    // PhysicalDeviceShaderIntegerDotProductFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderIntegerDotProductFeatures> {
        using Type = vk::PhysicalDeviceShaderIntegerDotProductFeatures;
        using CType = VkPhysicalDeviceShaderIntegerDotProductFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderIntegerDotProductFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderIntegerDotProductFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderIntegerDotProductFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderIntegerDotProductFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderIntegerDotProductFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderIntegerDotProductFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderIntegerDotProductFeatures";
    };

    // PhysicalDeviceShaderIntegerDotProductProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderIntegerDotProductProperties> {
        using Type = vk::PhysicalDeviceShaderIntegerDotProductProperties;
        using CType = VkPhysicalDeviceShaderIntegerDotProductProperties;
        static constexpr auto s_name = "VkPhysicalDeviceShaderIntegerDotProductProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderIntegerDotProductProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderIntegerDotProductProperties;
        static constexpr auto s_name = "VkPhysicalDeviceShaderIntegerDotProductProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderIntegerDotProductProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderIntegerDotProductProperties;
        static constexpr auto s_name = "VkPhysicalDeviceShaderIntegerDotProductProperties";
    };

    // PhysicalDeviceMaintenance4Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMaintenance4Features> {
        using Type = vk::PhysicalDeviceMaintenance4Features;
        using CType = VkPhysicalDeviceMaintenance4Features;
        static constexpr auto s_name = "VkPhysicalDeviceMaintenance4Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMaintenance4Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMaintenance4Features;
        static constexpr auto s_name = "VkPhysicalDeviceMaintenance4Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMaintenance4Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMaintenance4Features;
        static constexpr auto s_name = "VkPhysicalDeviceMaintenance4Features";
    };

    // PhysicalDeviceMaintenance4Properties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMaintenance4Properties> {
        using Type = vk::PhysicalDeviceMaintenance4Properties;
        using CType = VkPhysicalDeviceMaintenance4Properties;
        static constexpr auto s_name = "VkPhysicalDeviceMaintenance4Properties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMaintenance4Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMaintenance4Properties;
        static constexpr auto s_name = "VkPhysicalDeviceMaintenance4Properties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMaintenance4Properties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMaintenance4Properties;
        static constexpr auto s_name = "VkPhysicalDeviceMaintenance4Properties";
    };

    // PhysicalDeviceMeshShaderFeaturesEXT
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMeshShaderFeaturesEXT> {
        using Type = vk::PhysicalDeviceMeshShaderFeaturesEXT;
        using CType = VkPhysicalDeviceMeshShaderFeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceMeshShaderFeaturesEXT";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMeshShaderFeaturesEXT> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMeshShaderFeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceMeshShaderFeaturesEXT";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMeshShaderFeaturesEXT> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMeshShaderFeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceMeshShaderFeaturesEXT";
    };

    // PhysicalDeviceMeshShaderFeaturesNV
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMeshShaderFeaturesNV> {
        using Type = vk::PhysicalDeviceMeshShaderFeaturesNV;
        using CType = VkPhysicalDeviceMeshShaderFeaturesNV;
        static constexpr auto s_name = "VkPhysicalDeviceMeshShaderFeaturesNV";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMeshShaderFeaturesNV> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMeshShaderFeaturesNV;
        static constexpr auto s_name = "VkPhysicalDeviceMeshShaderFeaturesNV";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMeshShaderFeaturesNV> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMeshShaderFeaturesNV;
        static constexpr auto s_name = "VkPhysicalDeviceMeshShaderFeaturesNV";
    };

    // PhysicalDeviceRobustness2FeaturesEXT
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceRobustness2FeaturesEXT> {
        using Type = vk::PhysicalDeviceRobustness2FeaturesEXT;
        using CType = VkPhysicalDeviceRobustness2FeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceRobustness2FeaturesEXT";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceRobustness2FeaturesEXT> {
        static constexpr auto s_type = StructureType::PhysicalDeviceRobustness2FeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceRobustness2FeaturesEXT";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceRobustness2FeaturesEXT> {
        static constexpr auto s_type = StructureType::PhysicalDeviceRobustness2FeaturesEXT;
        static constexpr auto s_name = "VkPhysicalDeviceRobustness2FeaturesEXT";
    };

    // QueueFamilyProperties2
    template<>
    struct EnumToStructTraits<StructureType::QueueFamilyProperties2> {
        using Type = vk::QueueFamilyProperties2;
        using CType = VkQueueFamilyProperties2;
        static constexpr auto s_name = "VkQueueFamilyProperties2";
    };

    template<>
    struct StructToEnumTraits<vk::QueueFamilyProperties2> {
        static constexpr auto s_type = StructureType::QueueFamilyProperties2;
        static constexpr auto s_name = "VkQueueFamilyProperties2";
    };

    template<>
    struct StructToEnumTraits<VkQueueFamilyProperties2> {
        static constexpr auto s_type = StructureType::QueueFamilyProperties2;
        static constexpr auto s_name = "VkQueueFamilyProperties2";
    };

    // QueueFamilyGlobalPriorityPropertiesKHR
    template<>
    struct EnumToStructTraits<StructureType::QueueFamilyGlobalPriorityPropertiesKHR> {
        using Type = vk::QueueFamilyGlobalPriorityPropertiesKHR;
        using CType = VkQueueFamilyGlobalPriorityPropertiesKHR;
        static constexpr auto s_name = "VkQueueFamilyGlobalPriorityPropertiesKHR";
    };

    template<>
    struct StructToEnumTraits<vk::QueueFamilyGlobalPriorityPropertiesKHR> {
        static constexpr auto s_type = StructureType::QueueFamilyGlobalPriorityPropertiesKHR;
        static constexpr auto s_name = "VkQueueFamilyGlobalPriorityPropertiesKHR";
    };

    template<>
    struct StructToEnumTraits<VkQueueFamilyGlobalPriorityPropertiesKHR> {
        static constexpr auto s_type = StructureType::QueueFamilyGlobalPriorityPropertiesKHR;
        static constexpr auto s_name = "VkQueueFamilyGlobalPriorityPropertiesKHR";
    };

    // QueueFamilyCheckpointPropertiesNV
    template<>
    struct EnumToStructTraits<StructureType::QueueFamilyCheckpointPropertiesNV> {
        using Type = vk::QueueFamilyCheckpointPropertiesNV;
        using CType = VkQueueFamilyCheckpointPropertiesNV;
        static constexpr auto s_name = "VkQueueFamilyCheckpointPropertiesNV";
    };

    template<>
    struct StructToEnumTraits<vk::QueueFamilyCheckpointPropertiesNV> {
        static constexpr auto s_type = StructureType::QueueFamilyCheckpointPropertiesNV;
        static constexpr auto s_name = "VkQueueFamilyCheckpointPropertiesNV";
    };

    template<>
    struct StructToEnumTraits<VkQueueFamilyCheckpointPropertiesNV> {
        static constexpr auto s_type = StructureType::QueueFamilyCheckpointPropertiesNV;
        static constexpr auto s_name = "VkQueueFamilyCheckpointPropertiesNV";
    };

    // QueueFamilyCheckpointProperties2NV
    template<>
    struct EnumToStructTraits<StructureType::QueueFamilyCheckpointProperties2NV> {
        using Type = vk::QueueFamilyCheckpointProperties2NV;
        using CType = VkQueueFamilyCheckpointProperties2NV;
        static constexpr auto s_name = "VkQueueFamilyCheckpointProperties2NV";
    };

    template<>
    struct StructToEnumTraits<vk::QueueFamilyCheckpointProperties2NV> {
        static constexpr auto s_type = StructureType::QueueFamilyCheckpointProperties2NV;
        static constexpr auto s_name = "VkQueueFamilyCheckpointProperties2NV";
    };

    template<>
    struct StructToEnumTraits<VkQueueFamilyCheckpointProperties2NV> {
        static constexpr auto s_type = StructureType::QueueFamilyCheckpointProperties2NV;
        static constexpr auto s_name = "VkQueueFamilyCheckpointProperties2NV";
    };

    // PhysicalDeviceMultiviewProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceMultiviewProperties> {
        using Type = vk::PhysicalDeviceMultiviewProperties;
        using CType = VkPhysicalDeviceMultiviewProperties;
        static constexpr auto s_name = "VkPhysicalDeviceMultiviewProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceMultiviewProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMultiviewProperties;
        static constexpr auto s_name = "VkPhysicalDeviceMultiviewProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceMultiviewProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceMultiviewProperties;
        static constexpr auto s_name = "VkPhysicalDeviceMultiviewProperties";
    };

    // PhysicalDeviceVariablePointersFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceVariablePointersFeatures> {
        using Type = vk::PhysicalDeviceVariablePointersFeatures;
        using CType = VkPhysicalDeviceVariablePointersFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceVariablePointersFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceVariablePointersFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVariablePointersFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceVariablePointersFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceVariablePointersFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceVariablePointersFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceVariablePointersFeatures";
    };

    // PhysicalDeviceProtectedMemoryFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceProtectedMemoryFeatures> {
        using Type = vk::PhysicalDeviceProtectedMemoryFeatures;
        using CType = VkPhysicalDeviceProtectedMemoryFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceProtectedMemoryFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceProtectedMemoryFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceProtectedMemoryFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceProtectedMemoryFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceProtectedMemoryFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceProtectedMemoryFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceProtectedMemoryFeatures";
    };

    // PhysicalDeviceProtectedMemoryProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceProtectedMemoryProperties> {
        using Type = vk::PhysicalDeviceProtectedMemoryProperties;
        using CType = VkPhysicalDeviceProtectedMemoryProperties;
        static constexpr auto s_name = "VkPhysicalDeviceProtectedMemoryProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceProtectedMemoryProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceProtectedMemoryProperties;
        static constexpr auto s_name = "VkPhysicalDeviceProtectedMemoryProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceProtectedMemoryProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceProtectedMemoryProperties;
        static constexpr auto s_name = "VkPhysicalDeviceProtectedMemoryProperties";
    };

    // PhysicalDeviceSamplerYcbcrConversionFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceSamplerYcbcrConversionFeatures> {
        using Type = vk::PhysicalDeviceSamplerYcbcrConversionFeatures;
        using CType = VkPhysicalDeviceSamplerYcbcrConversionFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSamplerYcbcrConversionFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceSamplerYcbcrConversionFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSamplerYcbcrConversionFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSamplerYcbcrConversionFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceSamplerYcbcrConversionFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceSamplerYcbcrConversionFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceSamplerYcbcrConversionFeatures";
    };

    // PhysicalDeviceShaderDrawParametersFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderDrawParametersFeatures> {
        using Type = vk::PhysicalDeviceShaderDrawParametersFeatures;
        using CType = VkPhysicalDeviceShaderDrawParametersFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderDrawParametersFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderDrawParametersFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderDrawParametersFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderDrawParametersFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderDrawParametersFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderDrawParametersFeatures;
        static constexpr auto s_name = "VkPhysicalDeviceShaderDrawParametersFeatures";
    };

    // PhysicalDevice8BitStorageFeatures
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDevice8BitStorageFeatures> {
        using Type = vk::PhysicalDevice8BitStorageFeatures;
        using CType = VkPhysicalDevice8BitStorageFeatures;
        static constexpr auto s_name = "VkPhysicalDevice8BitStorageFeatures";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDevice8BitStorageFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevice8BitStorageFeatures;
        static constexpr auto s_name = "VkPhysicalDevice8BitStorageFeatures";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDevice8BitStorageFeatures> {
        static constexpr auto s_type = StructureType::PhysicalDevice8BitStorageFeatures;
        static constexpr auto s_name = "VkPhysicalDevice8BitStorageFeatures";
    };

    // PhysicalDeviceShaderAtomicInt64Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderAtomicInt64Features> {
        using Type = vk::PhysicalDeviceShaderAtomicInt64Features;
        using CType = VkPhysicalDeviceShaderAtomicInt64Features;
        static constexpr auto s_name = "VkPhysicalDeviceShaderAtomicInt64Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderAtomicInt64Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderAtomicInt64Features;
        static constexpr auto s_name = "VkPhysicalDeviceShaderAtomicInt64Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderAtomicInt64Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderAtomicInt64Features;
        static constexpr auto s_name = "VkPhysicalDeviceShaderAtomicInt64Features";
    };

    // PhysicalDeviceShaderFloat16Int8Features
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceShaderFloat16Int8Features> {
        using Type = vk::PhysicalDeviceShaderFloat16Int8Features;
        using CType = VkPhysicalDeviceShaderFloat16Int8Features;
        static constexpr auto s_name = "VkPhysicalDeviceShaderFloat16Int8Features";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceShaderFloat16Int8Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderFloat16Int8Features;
        static constexpr auto s_name = "VkPhysicalDeviceShaderFloat16Int8Features";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceShaderFloat16Int8Features> {
        static constexpr auto s_type = StructureType::PhysicalDeviceShaderFloat16Int8Features;
        static constexpr auto s_name = "VkPhysicalDeviceShaderFloat16Int8Features";
    };

    // PhysicalDeviceFloatControlsProperties
    template<>
    struct EnumToStructTraits<StructureType::PhysicalDeviceFloatControlsProperties> {
        using Type = vk::PhysicalDeviceFloatControlsProperties;
        using CType = VkPhysicalDeviceFloatControlsProperties;
        static constexpr auto s_name = "VkPhysicalDeviceFloatControlsProperties";
    };

    template<>
    struct StructToEnumTraits<vk::PhysicalDeviceFloatControlsProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceFloatControlsProperties;
        static constexpr auto s_name = "VkPhysicalDeviceFloatControlsProperties";
    };

    template<>
    struct StructToEnumTraits<VkPhysicalDeviceFloatControlsProperties> {
        static constexpr auto s_type = StructureType::PhysicalDeviceFloatControlsProperties;
        static constexpr auto s_name = "VkPhysicalDeviceFloatControlsProperties";
    };
}
