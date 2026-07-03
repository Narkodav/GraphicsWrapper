#include "Graphics/Graphics.h"

namespace Graphics
{
    void Image::create(const DeviceFunctionTable& functions, DeviceRef device,
        const ImageCreateInfo& createInfo)
    {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid image");
        GRAPHICS_VERIFY(createInfo.getStruct().mipLevels > 0, "Image Cannot have 0 mip levels");

        auto result = functions.execute<DeviceFunction::CreateImage>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());

        GRAPHICS_VERIFY_RESULT(result, "Failed to create an image");
    }

    void Image::destroy(const DeviceFunctionTable& functions, DeviceRef device)
    {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid image");
        functions.execute<DeviceFunction::DestroyImage>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }

    MemoryRequirements ImageRef::getMemoryRequirements(const DeviceFunctionTable& functions, DeviceRef device) const {
        GRAPHICS_VERIFY(isSet(), "Cannot get memory requirements for an invalid image");
        MemoryRequirements memRequirements;
        functions.execute<DeviceFunction::GetImageMemoryRequirements>(
            device.getHandle(), getHandle(), memRequirements.getUnderlyingPointer());
        return memRequirements;
    }

    SubresourceLayout ImageRef::getSubresourceLayout(const DeviceFunctionTable& functions, 
        DeviceRef device, const ImageSubresource& subresource) const {
        SubresourceLayout layout;
        functions.execute<DeviceFunction::GetImageSubresourceLayout>(
            device.getHandle(), getHandle(), subresource.getUnderlyingPointer(), layout.getUnderlyingPointer());
        return layout;
    }

    void ImageView::create(const DeviceFunctionTable& functions, DeviceRef device, const ImageViewCreateInfo& createInfo) {
        GRAPHICS_VERIFY(!isValid(), "Trying to create a valid image view");
        auto result = functions.execute<DeviceFunction::CreateImageView>(
            device.getHandle(), createInfo.getUnderlyingPointer(), nullptr, getUnderlyingPointer());
        GRAPHICS_VERIFY_RESULT(result, "Failed to create an image view");
    }

    void ImageView::destroy(const DeviceFunctionTable& functions, DeviceRef device) {
        GRAPHICS_VERIFY(isValid(), "Trying to destroy an invalid image view");
        functions.execute<DeviceFunction::DestroyImageView>(
            device.getHandle(), getHandle(), nullptr);
        reset();
    }
}