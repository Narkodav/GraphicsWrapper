#pragma once
#include "../Common.h"
#include "Instance.h"
#include "Device.h"

namespace Graphics {

    class Semaphore
    {
    private:
        VkSemaphore m_semaphore = nullptr;
    public:

		operator VkSemaphore() { return m_semaphore; };

        Semaphore() {};
        Semaphore(const Device& device);

        Semaphore(Semaphore&& other) noexcept {

            m_semaphore = std::exchange(other.m_semaphore, nullptr);

        };

        //moving to an initialized device is undefined behavior, destroy before moving
        Semaphore& operator=(Semaphore&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(m_semaphore == nullptr && "Semaphore& Semaphore::operator=(Semaphore&& other) - Semaphore already initialized");

            m_semaphore = std::exchange(other.m_semaphore, nullptr);

            return *this;
        };

        Semaphore(const Semaphore&) noexcept = delete;
        Semaphore& operator=(const Semaphore&) noexcept = delete;

        ~Semaphore() { assert(m_semaphore == nullptr && "Semaphore::~Semaphore() - Semaphore was not destroyed!"); };

        void destroy(const Device& device) {
            if (m_semaphore == nullptr)
                return;

            device.getFunctionTable().execute<DeviceFunction::DestroySemaphore>
                (device.getHandle(), m_semaphore, nullptr);

            m_semaphore = nullptr;

#ifdef _DEBUG
            std::cout << "void Semaphore::destroy(const Device& device) - Destroyed semaphore" << std::endl;
#endif            
        }

        bool isInitialized() const { return m_semaphore != nullptr; };

        const vk::Semaphore& getSemaphore() const { return m_semaphore; };

    };

}