#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "Image.h"

namespace Graphics
{
    class BufferRef : public BaseComponent<VkBuffer, BufferRef>
    {
        using Base = BaseComponent<VkBuffer, BufferRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Buffer";

        MemoryRequirements getMemoryRequirements(const DeviceRef& device,
            const DeviceFunctionTable& functions) const;
    };

    class Buffer : public VerificatorComponent<VkBuffer, BufferRef>
    {
        using Base = VerificatorComponent<VkBuffer, BufferRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkBufferCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkBufferCreateInfo, CreateInfo>;
        public:
            using Base::Base;
            CreateInfo(size_t byteSize, BufferUsage::Flags usageFlags,
                SharingMode sharingMode = SharingMode::Exclusive) : Base()
            {
                this->size = static_cast<VkDeviceSize>(byteSize);
                this->usage = usageFlags;
                this->sharingMode = static_cast<VkSharingMode>(sharingMode);
                this->flags = 0;
                this->queueFamilyIndexCount = 0;
                this->pQueueFamilyIndices = nullptr;
            }
            CreateInfo& setSize(size_t byteSize) {
                this->size = static_cast<VkDeviceSize>(byteSize);
                return *this;
            }
            CreateInfo& setUsage(BufferUsage::Flags usageFlags) {
                this->usage = usageFlags;
                return *this;
            }
            CreateInfo& setSharingMode(SharingMode sharingMode) {
                this->sharingMode = static_cast<VkSharingMode>(sharingMode);
                return *this;
            }
            CreateInfo& setQueueFamilyIndices(std::span<const uint32_t> queueFamilyIndices) {
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                return *this;
            }

            size_t getSize() const { return static_cast<size_t>(this->size); }
            BufferUsage::Flags getUsage() const { return this->usage; }
            SharingMode getSharingMode() const { return static_cast<SharingMode>(this->sharingMode); }
            std::span<const uint32_t> getQueueFamilyIndices() const {
                return std::span<const uint32_t>(this->pQueueFamilyIndices, this->queueFamilyIndexCount);
            }
        };

        class MemoryBarrier : public StructBase<VkBufferMemoryBarrier, MemoryBarrier>
        {
            using Base = StructBase<VkBufferMemoryBarrier, MemoryBarrier>;
        public:
            using Base::Base;
            MemoryBarrier(const BufferRef& buffer, Access::Flags srcAccessMask, Access::Flags dstAccessMask,
                uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex,
                size_t offset, size_t size) : Base()
            {
                this->srcAccessMask = srcAccessMask;
                this->dstAccessMask = dstAccessMask;
                this->srcQueueFamilyIndex = srcQueueFamilyIndex;
                this->dstQueueFamilyIndex = dstQueueFamilyIndex;
                this->buffer = buffer;
                this->offset = offset;
                this->size = size;
            }
            MemoryBarrier& setSrcAccessMask(Access::Flags srcAccessMask) {
                this->srcAccessMask = srcAccessMask;
                return *this;
            }
            MemoryBarrier& setDstAccessMask(Access::Flags dstAccessMask) {
                this->dstAccessMask = dstAccessMask;
                return *this;
            }
            MemoryBarrier& setSrcQueueFamilyIndex(uint32_t srcQueueFamilyIndex) {
                this->srcQueueFamilyIndex = srcQueueFamilyIndex;
                return *this;
            }
            MemoryBarrier& setDstQueueFamilyIndex(uint32_t dstQueueFamilyIndex) {
                this->dstQueueFamilyIndex = dstQueueFamilyIndex;
                return *this;
            }
            MemoryBarrier& setBuffer(const BufferRef& buffer) {
                this->buffer = buffer;
                return *this;
            }
            MemoryBarrier& setOffset(size_t offset) {
                this->offset = offset;
                return *this;
            }
            MemoryBarrier& setSize(size_t size) {
                this->size = size;
                return *this;
            }
        };

        class ImageCopy : public StructBase<VkBufferImageCopy, ImageCopy>
        {
            using Base = StructBase<VkBufferImageCopy, ImageCopy>;
        public:
            using Base::Base;
            ImageCopy(size_t bufferOffset, size_t bufferRowLength, size_t bufferImageHeight,
                Image::Subresourcelayers imageSubresource, Offset3D imageOffset, Extent3D imageExtent) : Base()
            {
                this->bufferOffset = bufferOffset;
                this->bufferRowLength = bufferRowLength;
                this->bufferImageHeight = bufferImageHeight;
                this->imageSubresource = imageSubresource;
                this->imageOffset = imageOffset;
                this->imageExtent = imageExtent;
            }
            ImageCopy& setBufferOffset(size_t bufferOffset) {
                this->bufferOffset = bufferOffset;
                return *this;
            }
            ImageCopy& setBufferRowLength(size_t bufferRowLength) {
                this->bufferRowLength = bufferRowLength;
                return *this;
            }
            ImageCopy& setBufferImageHeight(size_t bufferImageHeight) {
                this->bufferImageHeight = bufferImageHeight;
                return *this;
            }
            ImageCopy& setImageSubresource(const Image::Subresourcelayers& imageSubresource) {
                this->imageSubresource = imageSubresource;
                return *this;
            }
            ImageCopy& setImageOffset(Offset3D imageOffset) {
                this->imageOffset = imageOffset;
                return *this;
            }
            ImageCopy& setImageExtent(Extent3D imageExtent) {
                this->imageExtent = imageExtent;
                return *this;
            }
            Image::Subresourcelayers& getImageSubresource() { 
                return *reinterpret_cast<Image::Subresourcelayers*>(&this->imageSubresource);
            }
        };

        class ViewRef : public BaseComponent<VkBufferView, ViewRef>
        {
            using Base = BaseComponent<VkBufferView, ViewRef>;
        public:
            using Base::Base;
            static inline const std::string s_typeName = "Buffer::View";
        };

        class View : public VerificatorComponent<VkBufferView, ViewRef>
        {
            using Base = VerificatorComponent<VkBufferView, ViewRef>;
        public:
            using Base::Base;
            
            class CreateInfo : public StructBase<VkBufferViewCreateInfo, CreateInfo>
            {
                using Base = StructBase<VkBufferViewCreateInfo, CreateInfo>;
            public:
                using Base::Base;
                CreateInfo(const BufferRef& buffer, Format format, size_t offset, size_t range,
                    BufferViewCreate::Flags flags = BufferViewCreate::Bits::None) : Base()
                {
                    this->buffer = buffer;
                    this->format = convertCEnum(format);
                    this->offset = offset;
                    this->range = range;
                    this->flags = flags;
                }
                CreateInfo& setBuffer(const BufferRef& buffer) {
                    this->buffer = buffer;
                    return *this;
                }
                CreateInfo& setFormat(Format format) {
                    this->format = convertCEnum(format);
                    return *this;
                }
                CreateInfo& setOffset(DeviceSize_t offset) {
                    this->offset = offset;
                    return *this;
                }
                CreateInfo& setRange(DeviceSize_t range) {
                    this->range = range;
                    return *this;
                }
                CreateInfo& setFlags(BufferViewCreate::Flags flags) {
                    this->flags = flags;
                    return *this;
                }
            };

            void create(const DeviceRef& device,
                const DeviceFunctionTable& functions,
                const CreateInfo& createInfo);

            void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
        };

        void create(const DeviceRef& device,
            const DeviceFunctionTable& functions,
            const CreateInfo& createInfo);

        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };
}

