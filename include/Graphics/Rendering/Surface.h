#pragma once
#include "../Common.h"
#include "Instance.h"
#include "../DeviceCaching/PhysicalDevice.h"
#include "../PlatformManagement/Window.h"
#include "../Wrappers.h"

namespace Graphics {

    class Surface
    {
    private:
        Wrappers::Surface m_surface;
    public:

        Surface() {};
        Surface(const Instance& instance, const Window& window) {
            m_surface.create(instance.getHandle(), window, instance.getFunctionTable());
#ifdef _DEBUG
            std::cout << __FUNCTION__ << " - Created Surface" << std::endl;
#endif
        }

        Surface(Surface&&) noexcept = default;
        Surface& operator=(Surface&&) noexcept = default;

        Surface(const Surface&) noexcept = delete;
        Surface& operator=(const Surface&) noexcept = delete;

        ~Surface() = default;

        void destroy(const Instance& instance) {
            m_surface.destroy(instance.getHandle(), instance.getFunctionTable());
#ifdef _DEBUG
            std::cout << __FUNCTION__ << " - Destroyed Surface" << std::endl;
#endif
        }

        const auto& getHandle() const { return m_surface; };

        SwapChainSupportDetails getSwapChainSupportDetails(
            const Instance& instance, const PhysicalDevice& device) const
        {
            return m_surface.getSwapChainSupportDetails(instance.getFunctionTable(), device.getHandle());
        }

    };

}