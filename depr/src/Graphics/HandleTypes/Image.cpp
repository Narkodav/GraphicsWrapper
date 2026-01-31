#include "../../../include/Graphics/Graphics.h"

namespace Graphics
{
    void Image::create(const DeviceFunctionTable& functions, const DeviceRef& device,
        const ImageCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid image");
        GRAPHICS_VERIFY(createInfo.getStruct().mipLevels > 0, "Image Cannot have 0 mip levels");

        auto result = functions.execute<DeviceFunction::CreateImage>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create an image");
    }

    void Image::destroy(const DeviceFunctionTable& functions, const DeviceRef& device)
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

    void ImageView::create(const DeviceRef& device, const DeviceFunctionTable& functions,
        const ImageViewCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid image view");
        auto result = functions.execute<DeviceFunction::CreateImageView>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create an image view");
    }

    void ImageView::destroy(const DeviceRef& device, const DeviceFunctionTable& functions)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid image view");
        functions.execute<DeviceFunction::DestroyImageView>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }
}