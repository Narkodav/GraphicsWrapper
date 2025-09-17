#pragma once
#include "../Common.h"
#include "../InstanceFunctionTable.h"
#include "../DeviceFunctionTable.h"
#include "Instance.h"
#include "PhysicalDevice.h"

namespace Graphics
{
    class Queue;

    class QueueCreateInfo : public StructBase<VkDeviceQueueCreateInfo, QueueCreateInfo>
    {
        using Base = StructBase<VkDeviceQueueCreateInfo, QueueCreateInfo>;
    public:
        using Base::Base;

        QueueCreateInfo(uint32_t familyIndex, const std::vector<float>& queuePriorities) : Base()
        {
            this->pQueuePriorities = queuePriorities.data();
            this->queueCount = queuePriorities.size();
            this->queueFamilyIndex = familyIndex;
        }

        QueueCreateInfo& setQueueFamilyIndex(uint32_t index) {
            this->queueFamilyIndex = index;
            return *this;
        }

        QueueCreateInfo& setQueuePriorities(const std::vector<float>& queuePriorities) {
            this->pQueuePriorities = queuePriorities.data();
            this->queueCount = queuePriorities.size();
            return *this;
        }

        uint32_t getFamilyIndex() const {
            return this->queueFamilyIndex;
        }

        std::span<const float> getQueuePriorities() const {
            return std::span<const float>(this->pQueuePriorities, this->queueCount);
        }
    };

    class DeviceRef : public BaseComponent<VkDevice, DeviceRef>
    {
        using Base = BaseComponent<VkDevice, DeviceRef>;
    public:
        using Base::Base;

        DeviceFunctionTable getFunctionTable(const InstanceFunctionTable& functions) const
        {
            GRAPHICS_VERIFY(isSet(), "Trying to get function table from an invalid device");
            DeviceFunctionTable table(functions.get<InstanceFunction::GetDeviceProcAddr>());
            table.loadAllFunctions(getHandle());
            return table;
        };

        Queue getQueue(uint32_t familyIndex, uint32_t queueIndex,
            const DeviceFunctionTable& functions) const;

        void waitIdle(const DeviceFunctionTable& functions) const {
            GRAPHICS_VERIFY(isSet(), "Trying to wait on an invalid device");
            auto result = functions.execute<DeviceFunction::DeviceWaitIdle>(getHandle());
            GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to wait for device idle: "
                + s_resultMessages.at(result));
        };
    };

    class Device : public VerificatorComponent<VkDevice, DeviceRef>
    {
        using Base = VerificatorComponent<VkDevice, DeviceRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkDeviceCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkDeviceCreateInfo, CreateInfo>;
        public:
            using Base::Base;

            template<typename RequiredFeatures>
            CreateInfo(
                const RequiredFeatures& features,
                const std::vector<const char*>& extensions,
                const std::vector<QueueCreateInfo>& queueCreateInfos,
                const std::vector<const char*>& enabledLayers) : Base()
            {
                this->enabledExtensionCount = extensions.size();
                this->ppEnabledExtensionNames = extensions.data();
                this->enabledLayerCount = enabledLayers.size();
                this->ppEnabledLayerNames = enabledLayers.data();
                this->pQueueCreateInfos = QueueCreateInfo::underlyingCast(queueCreateInfos.data());
                this->queueCreateInfoCount = queueCreateInfos.size();
                this->pNext = features.getHead();
            }

            CreateInfo& setEnabledExtensions(const std::vector<const char*>& extensions) {
                this->enabledExtensionCount = extensions.size();
                this->ppEnabledExtensionNames = extensions.data();
                return *this;
            }

            CreateInfo& setEnabledLayers(const std::vector<const char*>& enabledLayers) {
                this->enabledLayerCount = enabledLayers.size();
                this->ppEnabledLayerNames = enabledLayers.data();
                return *this;
            }

            CreateInfo& setQueueCreateInfos(const std::vector<QueueCreateInfo>& queueCreateInfos) {
                this->pQueueCreateInfos = QueueCreateInfo::underlyingCast(queueCreateInfos.data());
                this->queueCreateInfoCount = queueCreateInfos.size();
                return *this;
            }

            template<typename RequiredFeatures>
            CreateInfo& setEnabledFeatures(const RequiredFeatures& features) {
                this->pNext = features.getHead();
                return *this;
            }
        };

        void create(const InstanceRef& instance,
            const PhysicalDevice& physicalDevice,
            const InstanceFunctionTable& functions,
            const CreateInfo& createInfo);

        void destroy(const DeviceFunctionTable& functions);        
    };
}

