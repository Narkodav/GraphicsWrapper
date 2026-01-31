#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void Sampler::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const SamplerCreateInfo& createInfo)
    {
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid sampler");        
        auto result = functions.execute<DeviceFunction::CreateSampler>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create a sampler");
    }

    void Sampler::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid sampler");
		functions.execute<DeviceFunction::DestroySampler>(device.getHandle(), getHandle(), nullptr);
        reset();
    }
}