#include "Sampler.h"

namespace Graphics
{
    void Sampler::create(const DeviceRef& device, const DeviceFunctionTable& functions, const CreateInfo& createInfo)
    {
		GRAPHICS_VERIFY(!isValid(), "Trying to create a valid sampler");        
        auto result = functions.execute<DeviceFunction::CreateSampler>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
		GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create sampler: " + s_resultMessages.at(result));
    }

    void Sampler::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid sampler");
		functions.execute<DeviceFunction::DestroySampler>(device.getHandle(), getHandle(), nullptr);
        reset();
    }
}