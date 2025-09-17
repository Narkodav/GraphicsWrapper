#include "Instance.h"
#include "DebugUtilsMessenger.h"

namespace Graphics::DebugUtils
{
    void Messenger::create(const Graphics::InstanceRef& instance,
        const InstanceFunctionTable& functions,
        const CreateInfo& debugCreateInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create an already existing debug messenger");
        auto result = functions.execute<InstanceFunction::CreateDebugUtilsMessengerEXT>(
            instance,
            debugCreateInfo.getUnderlyingPointer(),
            nullptr,
            getUnderlyingPointer()
        );

        GRAPHICS_VERIFY(result == VK_SUCCESS,
            "Failed to create a debug messenger: " + s_resultMessages.at(result));
    };

    void Messenger::destroy(const Graphics::InstanceRef& instance,
        const InstanceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid debug messenger");
        GRAPHICS_VERIFY(instance.isSet(),
            "Trying to destroy a debug messenger with an invalid instance");
        functions.execute<InstanceFunction::DestroyDebugUtilsMessengerEXT>(
            instance,
            getHandle(),
            nullptr
        );
        reset();
    };
}