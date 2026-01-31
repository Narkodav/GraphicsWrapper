#pragma once
#include "../Common.h"
#include "../PlatformManagement/Window.h"
#include "Device.h"
#include "Surface.h"
#include "../Wrappers.h"

namespace Graphics {
    
    class SwapChainFormat
    {
    private:
        Wrappers::SwapChainFormat m_format;

    public:

        SwapChainFormat(
            VkSurfaceFormatKHR format = {},
            PresentMode mode = PresentMode::Immediate,
            Extent2D extent = { 0, 0 }
        ) : m_format(format, mode, extent) {
        };

        SwapChainFormat(const Instance& instance,
            const Device& device,
            VkSurfaceFormatKHR format = {},
            PresentMode mode = PresentMode::Immediate,
            Extent2D extent = { 0, 0 }
        ) : m_format(device.getPhysicalDevice().getHandle(), instance.getFunctionTable(),
            format, mode, extent) {
        };

        SwapChainFormat(const SwapChainFormat&) = default;
        SwapChainFormat& operator=(const SwapChainFormat&) = default;

        SwapChainFormat(SwapChainFormat&&) = default;
        SwapChainFormat& operator=(SwapChainFormat&&) = default;

        void setSurfaceFormat(VkSurfaceFormatKHR surfaceFormat) { m_format.setSurfaceFormat(surfaceFormat); };
        void setPresentMode(PresentMode presentMode) { m_format.setPresentMode(presentMode); };
        void setSwapChainExtent(Extent2D swapChainExtent) { m_format.setSwapChainExtent(swapChainExtent); };

        const auto& getSurfaceFormat() const { return m_format.getSurfaceFormat(); };
        const auto& getPresentMode() const { return m_format.getPresentMode(); };
        const auto& getSwapChainExtent() const { return m_format.getSwapChainExtent(); };
        const auto& getDepthFormat() const { return m_format.getDepthFormat(); };

        static VkSurfaceFormatKHR chooseSurfaceFormat(const SwapChainSupportDetails& supportDetails) {
            return Wrappers::SwapChainFormat::chooseSurfaceFormat(supportDetails);
        };

        static PresentMode choosePresentMode(const SwapChainSupportDetails& supportDetails) {
            return Wrappers::SwapChainFormat::choosePresentMode(supportDetails);
        };

        static Extent2D chooseExtent(const SwapChainSupportDetails& supportDetails, Extent2D extent) {
            return Wrappers::SwapChainFormat::chooseExtent(supportDetails, extent);
        };

        static SwapChainFormat create(
            const Instance& instance,
            const Device& device,
            const Surface& surface,
            const Extent2D& extent
        ) {
            SwapChainFormat format;
            format.m_format = Wrappers::SwapChainFormat::create(device.getPhysicalDevice().getHandle(),
                instance.getFunctionTable(), surface.getHandle(), extent);
            return format;
        };

        bool isValid() const {
            return m_format.isValid();
        }
    };

}