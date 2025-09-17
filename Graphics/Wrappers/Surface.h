#pragma once
#include "../Common.h"
#include "../InstanceFunctionTable.h"
#include "Instance.h"
#include "../PlatformManagement/Window.h"

namespace Graphics
{
    class SurfaceRef : public BaseComponent<VkSurfaceKHR, SurfaceRef>
    {
        using Base = BaseComponent<VkSurfaceKHR, SurfaceRef>;
    public:
        using Base::Base;
    };

    class Surface : public VerificatorComponent<VkSurfaceKHR, SurfaceRef>
    {
        using Base = VerificatorComponent<VkSurfaceKHR, SurfaceRef>;
    public:
        using Base::Base;

        class Format : public StructBase<VkSurfaceFormatKHR, Format> {
            using Base = StructBase<VkSurfaceFormatKHR, Format>;
        public:
            using Base::Base;

            ColorSpace getColorSpace() const {
                return convertEnum(this->colorSpace);
            }

            Graphics::Format getFormat() const {
                return convertEnum(this->format);
            }
        };

        class Capabilities : public StructBase<VkSurfaceCapabilitiesKHR, Capabilities> {
            using Base = StructBase<VkSurfaceCapabilitiesKHR, Capabilities>;
        public:
            using Base::Base;

            Extent2D getMinImageExtent() const {
                return this->minImageExtent;
            }

            Extent2D getMaxImageExtent() const {
                return this->maxImageExtent;
            }

            Extent2D getCurrentExtent() const {
                return this->currentExtent;
            }

            SurfaceTransform::Flags getCurrentTransform() const {
                return SurfaceTransform::Flags(this->currentTransform);
            }

            uint32_t getMaxImageArrayLayers() const {
                return this->maxImageArrayLayers;
            }

            uint32_t getMaxImageCount() const {
                return this->maxImageCount;
            }

            uint32_t getMinImageCount() const {
                return this->minImageCount;
            }
        };

        void create(const InstanceRef& instance,
            const Window& window, const InstanceFunctionTable& functions);

        void destroy(const InstanceRef& instance, const InstanceFunctionTable& functions);
    };
}

