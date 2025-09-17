#include "Memory.h"
#include "Device.h"
#include "Buffer.h"
#include "Image.h"

namespace Graphics
{
    void Memory::create(const DeviceRef& device,
        const DeviceFunctionTable& functions,
        const AllocateInfo& allocInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid memory");

        auto result = functions.execute<DeviceFunction::AllocateMemory>(
            device.getHandle(), allocInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to allocate memory: " +
            s_resultMessages.at(result));
    }

    void Memory::bindBuffer(const DeviceRef& device, const DeviceFunctionTable& functions,
        const BufferRef& buffer, size_t offset /*= 0*/)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to bind to an invalid memory");
        auto result = functions.execute<DeviceFunction::BindBufferMemory>(
            device.getHandle(), buffer.getHandle(), getHandle(), offset);
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to bind buffer memory: " +
            s_resultMessages.at(result));
    }

    void Memory::bindImage(const DeviceRef& device, const DeviceFunctionTable& functions,
        const ImageRef& image, size_t offset /*= 0*/)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to bind to an invalid memory");
        auto result = functions.execute<DeviceFunction::BindImageMemory>(
            device.getHandle(), image.getHandle(), getHandle(), offset);
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to bind image memory: " +
            s_resultMessages.at(result));
    }

    void Memory::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to free an invalid memory");
        functions.execute<DeviceFunction::FreeMemory>(
            device.getHandle(), getHandle(), nullptr);
    }
}