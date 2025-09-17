#pragma once
#pragma once
#include "../Common.h"
#include "../Rendering/Instance.h"
#include "../Rendering/Device.h"
#include "../PlatformManagement/Window.h"
#include "DescriptorSet.h"
#include "../Rendering/Pipeline.h"
#include "../Enums.h"

namespace Graphics {

    class DescriptorPool
    {
    public:
        class Size
        {
        private:
            vk::DescriptorPoolSize m_poolSize{};

        public:
            Size(size_t count, DescriptorType type)
            {
                m_poolSize.type = static_cast<vk::DescriptorType>(type);
                m_poolSize.descriptorCount = static_cast<uint32_t>(count);
            }

            operator vk::DescriptorPoolSize() const { return m_poolSize; };

        };

    private:
        vk::DescriptorPool m_pool;
        std::vector<vk::DescriptorPoolSize> m_sizes;

        bool m_initialized = false;
    public:
        DescriptorPool() : m_pool(nullptr), m_initialized(false) {};

        DescriptorPool(const Instance& instance, const Device& device,
            const std::vector<Size>& sizes, size_t maxSets,
            DescriptorPoolCreateFlags::Flags flags);

        DescriptorPool(DescriptorPool&& other) noexcept {
            m_pool = std::exchange(other.m_pool, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);
        };

        //moving to an initialized swap chain is undefined behavior, destroy before moving
        DescriptorPool& operator=(DescriptorPool&& other) noexcept
        {
            if (this == &other)
                return *this;

            assert(!m_initialized && "DescriptorPool::operator=() - DescriptorPool already initialized");

            m_pool = std::exchange(other.m_pool, nullptr);
            m_initialized = std::exchange(other.m_initialized, false);

            return *this;
        };

        DescriptorPool(const DescriptorPool&) noexcept = delete;
        DescriptorPool& operator=(const DescriptorPool&) noexcept = delete;

        ~DescriptorPool() {
            assert(!m_initialized && "DescriptorPool was not destroyed!");
            //assert(m_allocatedBuffers.size() == 0 && "DescriptorPool has allocated buffers!");
        };

        void destroy(const Instance& instance, const Device& device) {
            if (!m_initialized)
                return;
            //assert(m_allocatedBuffers.size() == 0 && "DescriptorPool has allocated buffers!");

            device.getDevice().destroyDescriptorPool(m_pool, nullptr, instance.getDispatchLoader());

#ifdef _DEBUG
            std::cout << "Destroyed DescriptorPool" << std::endl;
#endif
            m_initialized = false;
        };

        std::vector<DescriptorSet> allocateSets(
            const Instance& instance, const Device& device,
            const std::vector<const DescriptorSetLayout*>& layouts) {

            vk::DescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
            allocInfo.descriptorPool = m_pool;

            std::vector<vk::DescriptorSetLayout> layoutsRaw;
            for (auto& layout : layouts)
                layoutsRaw.push_back(layout->getLayout());

            allocInfo.descriptorSetCount = layoutsRaw.size();
            allocInfo.pSetLayouts = layoutsRaw.data();

            auto setsRaw = device.getDevice().allocateDescriptorSets(allocInfo, instance.getDispatchLoader());
            size_t i = 0;
            auto setsWrapped = convert<DescriptorSet>
                (setsRaw, [this, &layouts, &i](vk::DescriptorSet set)
                    {
                        DescriptorSet setWrapped = DescriptorSet(set, layouts[i]->getLayoutInfo());
                        i++;
                        return setWrapped;
                    });

            return setsWrapped;
        }

        std::vector<DescriptorSet> allocateSets(
            const Instance& instance, const Device& device,
            const std::vector<const DescriptorSetLayout*>& layouts,
            const std::vector<uint32_t>& variableDescriptorCounts) {

            vk::DescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = vk::StructureType::eDescriptorSetAllocateInfo;
            allocInfo.descriptorPool = m_pool;

            std::vector<vk::DescriptorSetLayout> layoutsRaw;
            for (auto& layout : layouts)
                layoutsRaw.push_back(layout->getLayout());

            allocInfo.descriptorSetCount = layoutsRaw.size();
            allocInfo.pSetLayouts = layoutsRaw.data();

            // Set up the variable descriptor count info
            vk::DescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo{};
            variableCountInfo.sType = vk::StructureType::eDescriptorSetVariableDescriptorCountAllocateInfo;
            variableCountInfo.descriptorSetCount = variableDescriptorCounts.size();
            variableCountInfo.pDescriptorCounts = variableDescriptorCounts.data();

            // Link the variable count info to the allocation info
            allocInfo.pNext = &variableCountInfo;

            auto setsRaw = device.getDevice().allocateDescriptorSets(allocInfo, instance.getDispatchLoader());
            size_t i = 0;
            auto setsWrapped = convert<DescriptorSet>
                (setsRaw, [this, &layouts, &i](vk::DescriptorSet set)
                    {
                        auto setWrapped = DescriptorSet(set, layouts[i]->getLayoutInfo());
                        i++;
                        return setWrapped;
                    });

            return setsWrapped;
        }

        const vk::DescriptorPool& getPool() const { return m_pool; };
    };

}