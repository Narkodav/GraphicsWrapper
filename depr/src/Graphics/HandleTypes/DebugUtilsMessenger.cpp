#include "../../../include/Graphics/Graphics.h"

namespace Graphics::DebugUtils
{
    void Messenger::create(const InstanceFunctionTable& functions,
        const Graphics::InstanceRef& instance,
        const MessengerCreateInfo& debugCreateInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create an already existing debug messenger");
        auto result = functions.execute<InstanceFunction::CreateDebugUtilsMessengerEXT>(
            instance,
            &debugCreateInfo,
            nullptr,
            this
        );
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a debug messenger");
    };

    void Messenger::destroy(const InstanceFunctionTable& functions,
        const Graphics::InstanceRef& instance)
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