#pragma once
#include "Graphics/Common.h"
#include "Graphics/Flags.h"
#include "Graphics/HandleTypes/Device.h"
#include "Graphics/HandleTypes/Instance.h"
#include "Graphics/HandleTypes/Memory.h"
#include "Graphics/HandleTypes/Buffer.h"
#include "Graphics/Utility//Utility.h"

#include "Memory/ExternalMetadataAllocators/BuddyAllocator.h"

#include <vector>
#include <list>

namespace Allocators = Memory::ExternalMetadataAllocators;

namespace Graphics::MemoryManagement
{
	class MemoryPool
	{
	public:
		struct MemoryRegion {
			uint32_t offset;
			uint32_t size;
		};

		struct Allocation {
			MemoryRegion region;
			uint32_t bufferIndex;

			struct Hash
			{
				size_t operator()(const Allocation& allocation) const {
					return std::hash<uint32_t>()(allocation.bufferIndex) ^ std::hash<uint32_t>()(allocation.region.offset);
				}
			};

			struct EqualTo
			{
				constexpr bool operator()(const Allocation& left, const Allocation& right) const {
					return left.region.offset == right.region.offset
					&& left.region.size == right.region.size
					&& left.bufferIndex == right.bufferIndex;
				}
			};

			static Allocation getEmptyAllocation() {
				return Allocation{ MemoryRegion{ 0, 0 }, 0 };
			};

			bool operator==(const Allocation& other) {
				return region.offset == other.region.offset
					&& region.size == other.region.size
					&& bufferIndex == other.bufferIndex;
			}
		};

		using AllocationSet = std::unordered_set<Allocation, typename Allocation::Hash, typename Allocation::EqualTo>;

	private:
		struct MemoryChunk {
			Buffer buffer;
			Memory memory;
			Allocators::BuddyAllocatorBase buddyAlloc;
			uint32_t allocatedSize;
		};

		std::vector<MemoryChunk> m_memoryChunks;

		uint32_t m_chunkCapacity = 0;
		BufferCreateInfo m_bufferInfo;
		PhysicalDeviceMemoryProperties m_deviceMemoryProps;
		std::function<void(Memory& memory, Buffer& buffer, size_t bufferIndex)> m_onBufferAlloc;

		// memory type is reasonably guaranteed to be identical for identical alloc infos,
		// doesnt depend on size or alignment
		uint32_t m_memoryTypeIndex; 
		uint32_t m_memoryTypeBits;
	public:

		MemoryPool() = default;

		MemoryPool(const DeviceFunctionTable& functions, DeviceRef device, PhysicalDeviceMemoryProperties deviceMemoryProps,
			uint32_t memoryChunkCapacity, Flags::BufferUsage usageFlags, Flags::MemoryProperty requiredProperties, 
			Flags::MemoryProperty forbiddenProperties, SharingMode sharingMode, std::function<void(Memory&, Buffer&, size_t)>&& onBufferAlloc =
			[](Memory&, Buffer&, size_t) {})
		{
			m_onBufferAlloc = std::move(onBufferAlloc);
			m_deviceMemoryProps = deviceMemoryProps;
			m_chunkCapacity = memoryChunkCapacity;
			m_memoryChunks.push_back({});
			auto& chunk = m_memoryChunks.back();

			m_bufferInfo = { m_chunkCapacity, usageFlags, sharingMode };

			chunk.buffer.create(functions, device, m_bufferInfo);
			auto memoryRequirements = chunk.buffer.getMemoryRequirements(functions, device);
			m_memoryTypeBits = memoryRequirements.getMemoryTypeBits();
			m_memoryTypeIndex = Utility::findMemoryTypeFirstFit(deviceMemoryProps,
					m_memoryTypeBits, requiredProperties, forbiddenProperties);

			chunk.memory.create(functions, device, { memoryRequirements.getSize(), m_memoryTypeIndex });
			chunk.memory.bindBuffer(functions, device, chunk.buffer);

			m_onBufferAlloc(chunk.memory, chunk.buffer, 0);

			chunk.buddyAlloc.assign(0, m_chunkCapacity);
			chunk.allocatedSize = 0;
		}

		MemoryPool(MemoryPool&& other) noexcept
		{
			m_memoryChunks = std::exchange(other.m_memoryChunks, {});
			m_chunkCapacity = std::exchange(other.m_chunkCapacity, 0);
			m_bufferInfo = std::exchange(other.m_bufferInfo, {});
			m_deviceMemoryProps = std::exchange(other.m_deviceMemoryProps, {});
			m_onBufferAlloc = std::exchange(other.m_onBufferAlloc, [](Memory&, Buffer&, size_t) {});
			m_memoryTypeIndex = std::exchange(other.m_memoryTypeIndex, 0);
		}

		MemoryPool& operator=(MemoryPool&& other)
		{
			if (this == &other)
				return *this;

			m_memoryChunks = std::exchange(other.m_memoryChunks, {});
			m_chunkCapacity = std::exchange(other.m_chunkCapacity, 0);
			m_bufferInfo = std::exchange(other.m_bufferInfo, {});
			m_deviceMemoryProps = std::exchange(other.m_deviceMemoryProps, {});
			m_onBufferAlloc = std::exchange(other.m_onBufferAlloc, [](Memory&, Buffer&, size_t) {});
			m_memoryTypeIndex = std::exchange(other.m_memoryTypeIndex, 0);

			return *this;
		}

		MemoryPool(const MemoryPool&) noexcept = delete;
		MemoryPool& operator=(const MemoryPool&) noexcept = delete;

		~MemoryPool() { GRAPHICS_VERIFY(m_memoryChunks.empty(), "MemoryPool was dont destroyed"); };

		Allocation allocate( const DeviceFunctionTable& functions, const Device& device, size_t size)
		{
			if (size > m_chunkCapacity)
				throw std::runtime_error("Allocation size exceeds chunk capacity");

			for (size_t i = 0; i < m_memoryChunks.size(); ++i)
			{
				auto ptr = m_memoryChunks[i].buddyAlloc.allocate(size);
				if(ptr != std::numeric_limits<uintptr_t>::max())
					return Allocation{static_cast<uint32_t>(ptr), static_cast<uint32_t>(size), static_cast<uint32_t>(i) };
			}
			addBuffer(functions, device);
			auto ptr = m_memoryChunks.back().buddyAlloc.allocate(size);
			return Allocation{static_cast<uint32_t>(ptr), static_cast<uint32_t>(size), 
				static_cast<uint32_t>(m_memoryChunks.size() - 1) };
		}

		void free(Allocation& allocation)
		{
			m_memoryChunks[allocation.bufferIndex].buddyAlloc.deallocate(allocation.region.offset);
		}

		auto& getBuffer(size_t chunkIndex) { return m_memoryChunks[chunkIndex].buffer; };
		auto& getMemory(size_t chunkIndex) { return m_memoryChunks[chunkIndex].memory; };

		const auto& getBuffer(size_t chunkIndex) const { return m_memoryChunks[chunkIndex].buffer; };
		const auto& getMemory(size_t chunkIndex) const { return m_memoryChunks[chunkIndex].memory; };

		// const auto& getFreeRegions(size_t chunkIndex) const { return m_freeRegions[chunkIndex];  };
		// const auto& getAllocations(size_t chunkIndex) const { return m_allocations[chunkIndex]; };
		// const auto& getAllFreeRegions() const { return m_freeRegions; };
		// const auto& getAllAllocations() const { return m_allocations; };

		size_t getChunkSize() const { return m_chunkCapacity; };
		size_t getChunkAllocatedSize(size_t chunkIndex) const { return m_memoryChunks[chunkIndex].allocatedSize; };
		size_t getChunkFreeSize(size_t chunkIndex) const { return m_chunkCapacity - m_memoryChunks[chunkIndex].allocatedSize; };

		void destroy(const DeviceFunctionTable& functions, const Device& device) {
			if (m_memoryChunks.empty())
				return;

			for (auto& chunk : m_memoryChunks)
			{
				chunk.buffer.destroy(functions, device);
				chunk.memory.destroy(functions, device);
			}
			m_memoryChunks.clear();
		}

	private:

		void addBuffer(const DeviceFunctionTable& functions, const Device& device)
		{
			m_memoryChunks.push_back({});
			auto& chunk = m_memoryChunks.back();

			chunk.buffer.create(functions, device, m_bufferInfo);
			auto memoryRequirements = chunk.buffer.getMemoryRequirements(functions, device);
			GRAPHICS_VERIFY(m_memoryTypeBits & (static_cast<size_t>(1) << m_memoryTypeIndex), 
			"Required memory type doesnt match cached memory type");

			chunk.memory.create(functions, device, { memoryRequirements.getSize(), m_memoryTypeIndex });
			chunk.memory.bindBuffer(functions, device, chunk.buffer);
			chunk.buddyAlloc.assign(0, m_chunkCapacity);
			chunk.allocatedSize = 0;
			m_onBufferAlloc(chunk.memory, chunk.buffer, m_memoryChunks.size() - 1);
		}
	};
}
