#include "Graphics/Graphics.h"

namespace Graphics
{
    void Memory::create(const DeviceFunctionTable& functions,
        const DeviceRef& device, const MemoryAllocateInfo& allocInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid memory");

        auto result = functions.execute<DeviceFunction::AllocateMemory>(
            device.getHandle(), allocInfo.getUnderlyingPointer(),
            nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to allocate memory");
    }

    void MemoryRef::bindBuffer(const DeviceFunctionTable& functions, const DeviceRef& device,
        const BufferRef& buffer, size_t offset /*= 0*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to bind to an invalid memory");
        auto result = functions.execute<DeviceFunction::BindBufferMemory>(
            device.getHandle(), buffer.getHandle(), getHandle(), offset);
        GRAPHICS_VERIFY_RESULT(result, "Failed to bind buffer memory");
    }

    void MemoryRef::bindImage(const DeviceFunctionTable& functions, const DeviceRef& device,
        const ImageRef& image, size_t offset /*= 0*/)
    {
        GRAPHICS_VERIFY(isSet(), "Trying to bind to an invalid memory");
        auto result = functions.execute<DeviceFunction::BindImageMemory>(
            device.getHandle(), image.getHandle(), getHandle(), offset);
        GRAPHICS_VERIFY_RESULT(result, "Failed to bind image memory");
    }

    void Memory::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to free an invalid memory");
        functions.execute<DeviceFunction::FreeMemory>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }

    MemoryMapping Memory::map(const DeviceFunctionTable& functions, const DeviceRef& device,
        DeviceSize offset /*= 0*/, DeviceSize size /*= MemoryMapping::s_wholeSize*/,
        Flags::MemoryMap flags /*= Flags::MemoryMap::Bits::None*/)
    {
        GRAPHICS_VERIFY(isValid(), "Memory was not created before being mapped");
        void* mapping = nullptr;
        auto result = functions.execute<DeviceFunction::MapMemory>(device, getHandle(),
            offset, size, flags, &mapping);
        GRAPHICS_VERIFY_RESULT(result, "Failed to map memory");
        return MemoryMapping(mapping);
    }

    void Memory::unmap(const DeviceFunctionTable& functions, const DeviceRef& device,
        MemoryMapping& mapping) {
        GRAPHICS_VERIFY(isValid(), "Memory was not created before being unmapped");
        functions.execute<DeviceFunction::UnmapMemory>(device, getHandle());
        mapping.invalidate();
    }
}