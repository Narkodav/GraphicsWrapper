#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../InstanceFunctionTable.h"

namespace Graphics {
    class InstanceRef;
};

namespace Graphics::DebugUtils
{
    class ObjectInfo : public StructBase<VkDebugUtilsObjectNameInfoEXT, ObjectInfo>
    {
        using Base = StructBase<VkDebugUtilsObjectNameInfoEXT, ObjectInfo>;
    public:
        using Base::Base;
        const std::string_view getName() const { return std::string_view(this->pObjectName); };
        ObjectType getType() const { this->objectType; };
        uint64_t getHandle() const { this->objectHandle; };
    };

    class Label : public StructBase<VkDebugUtilsLabelEXT, Label>
    {
        using Base = StructBase<VkDebugUtilsLabelEXT, Label>;
    public:
        using Base::Base;
        const Color& getColor() const { *reinterpret_cast<const Color*>(this->color); };
        const std::string_view& getName() const { this->pLabelName; };
    };

    class CallbackData : public StructBase<VkDebugUtilsMessengerCallbackDataEXT, CallbackData>
    {
        using Base = StructBase<VkDebugUtilsMessengerCallbackDataEXT, CallbackData>;
    public:
        using Base::Base;
        const std::string_view getMessageIdName() const { return this->pMessageIdName; };
        const std::string_view getMessage() const { return this->pMessage; };
        uint32_t getMessageIdNumber() const { return this->messageIdNumber; };

        std::span<const ObjectInfo> getObjectInfos() const {
            return std::span<const ObjectInfo>(
                ObjectInfo::underlyingCast(this->pObjects),
                this->objectCount);
        };

        std::span<const Label> getQueueLabels() const {
            return std::span<const Label>(
                Label::underlyingCast(this->pQueueLabels),
                this->queueLabelCount);
        };

        std::span<const Label> getCmdBufLabels() const {
            return std::span<const Label>(
                Label::underlyingCast(this->pCmdBufLabels),
                this->cmdBufLabelCount);
        };

        const DebugUtils::MessengerCallbackData::Flags& getFlags() const
        {
            return this->flags;
        };
    };

    typedef bool32_t(GRAPHICS_API_PTR* PFN_callback)(
        DebugMessageSeverity::Bits,
        DebugMessageType::Flags,
        const CallbackData*,
        void* pUserData
        );

    class MessengerRef : public BaseComponent<VkDebugUtilsMessengerEXT, MessengerRef> {
        using Base = BaseComponent<VkDebugUtilsMessengerEXT, MessengerRef>;
    public:        
		using Base::Base;
        static inline const std::string s_typeName = "Messenger";
    };

    class Messenger : public VerificatorComponent<VkDebugUtilsMessengerEXT, MessengerRef>
    {
        using Base = VerificatorComponent<VkDebugUtilsMessengerEXT, MessengerRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkDebugUtilsMessengerCreateInfoEXT, CreateInfo>
        {
            using Base = StructBase<VkDebugUtilsMessengerCreateInfoEXT, CreateInfo>;
        public:
            using Base::Base;

            CreateInfo(PFN_callback callback,
                DebugMessageSeverity::Flags messageSeverityFlags = DebugMessageSeverity::Bits::All,
                DebugMessageType::Flags messageTypeFlags = DebugMessageType::Bits::AllLegacy,
                void* pUserData = nullptr) : Base() {
                this->messageSeverity = messageSeverityFlags;
                this->messageType = messageTypeFlags;
                this->pfnUserCallback = reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(callback);
                this->pUserData = pUserData;
            };

            CreateInfo& setCallback(PFN_callback callback) {
                this->pfnUserCallback = reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(callback);
                return *this;
            }

            CreateInfo& setMessageSeverity(DebugMessageSeverity::Flags messageSeverityFlags) {
                this->messageSeverity = messageSeverityFlags;
                return *this;
            }

            CreateInfo& setMessageType(DebugMessageType::Flags messageTypeFlags) {
                this->messageType = messageTypeFlags;
                return *this;
            }

            CreateInfo& setUserData(void* pUserData) {
                this->pUserData = pUserData;
                return *this;
            }

            PFN_callback getCallback() const {
                return reinterpret_cast<PFN_callback>(this->pfnUserCallback);
            }

            DebugMessageSeverity::Flags getMessageSeverity() const {
                return this->messageSeverity;
            }

            DebugMessageType::Flags getMessageType() const {
                return this->messageType;
            }

            void* getUserData() const {
                return this->pUserData;
            }
        };

        void create(const InstanceRef& instance,
            const InstanceFunctionTable& functions,
            const CreateInfo& debugCreateInfo);

        void destroy(const InstanceRef& instance, const InstanceFunctionTable& functions);
    };

}