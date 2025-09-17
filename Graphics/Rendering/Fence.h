#pragma once
#include "../Common.h"
#include "Instance.h"
#include "Device.h"

namespace Graphics {

    class Fence
    {
    private:
        VkFence m_fence = nullptr;
    public:

        operator VkFence() { return m_fence; };

        Fence() {};
        Fence(const Device& device, bool createSignaled = false);

        Fence(Fence&& other) noexcept {

            m_fence = std::exchange(other.m_fence, nullptr);

        };

        //moving to an initialized device is undefined behavior, destroy before moving
        Fence& operator=(Fence&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(m_fence == nullptr && "Fence::operator=() - Fence already initialized");

            m_fence = std::exchange(other.m_fence, nullptr);

            return *this;
        };

        Fence(const Fence&) noexcept = delete;
        Fence& operator=(const Fence&) noexcept = delete;

        ~Fence() { assert(m_fence == nullptr && "Fence::~Fence() - Fence was not destroyed!"); };

        void destroy(const Device& device) {
            if (m_fence == nullptr)
                return;
            device.getFunctionTable().execute<DeviceFunction::DestroyFence>(device.getHandle(), m_fence, nullptr);
            m_fence = nullptr;
#ifdef _DEBUG
            std::cout << "void Fence::destroy(const Device& device) - Destroyed fence" << std::endl;
#endif
        }

        bool isInitialized() const { return m_fence != nullptr; };

        void wait(const Device& device, size_t timeout = std::numeric_limits<size_t>::max());
        void reset(const Device& device);

        VkFence getHandle() const { return m_fence; };

        static void wait(const Device& device, std::vector<Fence>& fences, size_t timeout = std::numeric_limits<size_t>::max());
        static void reset(const Device& device, std::vector<Fence>& fences);


    };

}