#pragma once
#include "../Common.h"
#include "Instance.h"
#include "Device.h"
#include "../Enums.h"

namespace Graphics {

    class Sampler
    {
    public:

        class Descriptor
        {
        private:
            vk::SamplerCreateInfo m_samplerInfo;

        public:
            operator vk::SamplerCreateInfo() const { return m_samplerInfo; };

            Descriptor() {};
            Descriptor(const Instance& instance, const Device& device,
                Filter magFilter = Filter::Linear, Filter minFilter = Filter::Linear,
                SamplerMipmapMode mipmapMode = SamplerMipmapMode::Linear,
                SamplerAddressMode modeU = SamplerAddressMode::Repeat,
                SamplerAddressMode modeV = SamplerAddressMode::Repeat,
                SamplerAddressMode modeW = SamplerAddressMode::Repeat)
            {
                m_samplerInfo.sType = vk::StructureType::eSamplerCreateInfo;
                m_samplerInfo.magFilter = convertEnum(magFilter);
                m_samplerInfo.minFilter = convertEnum(minFilter);
                m_samplerInfo.addressModeU = convertEnum(modeU);
                m_samplerInfo.addressModeV = convertEnum(modeV);
                m_samplerInfo.addressModeW = convertEnum(modeW);

                m_samplerInfo.anisotropyEnable = VK_TRUE;
                m_samplerInfo.maxAnisotropy = 1.0f;

                vk::PhysicalDeviceProperties properties{};
                properties = device.getPhysicalDevice().getHandle().getProperties(instance.getDispatchLoader());
                m_samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

                m_samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
                m_samplerInfo.unnormalizedCoordinates = VK_FALSE;
                m_samplerInfo.compareEnable = VK_FALSE;
                m_samplerInfo.compareOp = vk::CompareOp::eAlways;
                
                m_samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
                m_samplerInfo.mipLodBias = 0.0f;
                m_samplerInfo.minLod = 0.0f;
                m_samplerInfo.maxLod = vk::LodClampNone;
            }

            Descriptor(Descriptor&&) noexcept = default;
            Descriptor& operator=(Descriptor&&) noexcept = default;

            Descriptor(const Descriptor&) noexcept = default;
            Descriptor& operator=(const Descriptor&) noexcept = default;

            ~Descriptor() = default;

            vk::SamplerCreateInfo getSamplerInfo() const { return m_samplerInfo; };
        };

    private:
        vk::Sampler m_sampler;
        Descriptor m_descriptor;

        bool m_initialized = false;
    public:

        Sampler() {};
        Sampler(const Instance& instance, const Device& device, const Descriptor& descriptor);

        Sampler(Sampler&& other) noexcept {

            m_sampler = std::exchange(other.m_sampler, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);

        };

        //moving to an initialized device is undefined behavior, destroy before moving
        Sampler& operator=(Sampler&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(!m_initialized && "Sampler::operator=() - Sampler already initialized");

            m_sampler = std::exchange(other.m_sampler, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);

            return *this;
        };

        Sampler(const Sampler&) noexcept = delete;
        Sampler& operator=(const Sampler&) noexcept = delete;

        ~Sampler() { assert(!m_initialized && "Sampler was not destroyed!"); };

        void destroy(const Instance& instance, const Device& device) {
            if (!m_initialized)
                return;

            device.getDevice().destroySampler(m_sampler, nullptr, instance.getDispatchLoader());
#ifdef _DEBUG
            std::cout << "Destroyed Sampler" << std::endl;
#endif
            m_initialized = false;
        }

        bool isInitialized() const { return m_initialized; };

        const vk::Sampler& getSampler() const { return m_sampler; };


    };

}