#include "Image.h"
//#include "Swapchain.h"

namespace Graphics
{
    void Image::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid image");
        GRAPHICS_VERIFY(createInfo.getStruct().mipLevels > 0, "Image Cannot have 0 mip levels");

        auto result = functions.execute<DeviceFunction::CreateImage>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create image: " +
            s_resultMessages.at(result));
    }

    void Image::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid image");
        functions.execute<DeviceFunction::DestroyImage>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }

    MemoryRequirements ImageRef::getMemoryRequirements(const DeviceRef& device,
        const DeviceFunctionTable& functions) const
    {
        GRAPHICS_VERIFY(isSet(), "Cannot get memory requirements for an invalid image");
        MemoryRequirements memRequirements;
        functions.execute<DeviceFunction::GetImageMemoryRequirements>(
            device.getHandle(), getHandle(), memRequirements.getUnderlyingPointer());
        return memRequirements;
    }

    void Image::View::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const ImageRef& image, const CreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid image view");
        auto result = functions.execute<DeviceFunction::CreateImageView>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY(result == VK_SUCCESS, "Failed to create image view: " +
            s_resultMessages.at(result));
    }

    void Image::View::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid image view");
        functions.execute<DeviceFunction::DestroyImageView>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }
}