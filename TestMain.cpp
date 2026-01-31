#include "Graphics/HandleTypes/Instance.h"
#include "Graphics/HandleTypes/DebugUtilsMessenger.h"
#include "Graphics/HandleTypes/Surface.h"
#include "Graphics/HandleTypes/PhysicalDevice.h"
#include "Graphics/HandleTypes/PhysicalDeviceCache.h"
#include "Graphics/HandleTypes/RenderPass.h"
#include "Graphics/HandleTypes/FrameBuffer.h"
#include "Graphics/HandleTypes/Image.h"
#include "Graphics/HandleTypes/SwapChain.h"
#include "Graphics/HandleTypes/CommandPool.h"
#include "Graphics/HandleTypes/DescriptorPool.h"
#include "Graphics/Structs.h"
#include "Graphics/Camera.h"
#include "Graphics/Utility/PixelData2D.h"

#include "Graphics/Utility/Utility.h"

#include "Graphics/PlatformManagement/Window.h"

#include "PlatformAbstractions/Console.h"

namespace Gfx = Graphics;

GRAPHICS_API_ATTR Gfx::Bool32 GRAPHICS_API_CALL static_debugCallback(
    Gfx::Flags::DebugMessageSeverity::Bits messageSeverity,
    Gfx::Flags::DebugMessageType messageType,
    const Gfx::DebugUtils::CallbackData* pCallbackData,
    void* pUserData) {

    switch (messageSeverity) {
    case Gfx::Flags::DebugMessageSeverity::Bits::Verbose:
        // Diagnostic messages
        std::cout << "VERBOSE: " << pCallbackData->getMessage() << std::endl;
        break;

    case Gfx::Flags::DebugMessageSeverity::Bits::Info:
        // Informational messages like resource creation
        std::cout << "INFO: " << pCallbackData->getMessage() << std::endl;
        break;

    case Gfx::Flags::DebugMessageSeverity::Bits::Warning:
        // Warning messages like use of deprecated functions
        std::cout << "WARNING: " << pCallbackData->getMessage() << std::endl;
        break;

    case Gfx::Flags::DebugMessageSeverity::Bits::Error:
        // Error messages for invalid behavior
        std::cerr << "ERROR: " << pCallbackData->getMessage() << std::endl;
        throw std::runtime_error("Validation layer error");
        break;

    default:
        std::cout << "UNKNOWN SEVERITY: " << pCallbackData->getMessage() << std::endl;
        std::abort();
        break;
    }
    return 0;
}

void foo(Graphics::Flags::MemoryMap f) {
}

int main()
{
    Gfx::Instance instance;

    Gfx::InstanceFunctionTable table1;
    Gfx::DeviceFunctionTable table2;

    Gfx::AllocationCallbacks c;

    auto f = table1.get<Gfx::InstanceFunction::DestroyInstance>();
    f(instance, &c);

    auto t = table1.execute<Gfx::InstanceFunction::DestroyInstance>(
        instance, &c
    );
}