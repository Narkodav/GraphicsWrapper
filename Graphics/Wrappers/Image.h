#pragma once
#include "../Common.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class SwapChain;

    class ComponentMapping : public StructBase<VkComponentMapping, ComponentMapping>
    {
		using Base = StructBase<VkComponentMapping, ComponentMapping>;
        public:
        using Base::Base;
        ComponentMapping(ComponentSwizzle r = ComponentSwizzle::Identity,
            ComponentSwizzle g = ComponentSwizzle::Identity,
            ComponentSwizzle b = ComponentSwizzle::Identity,
            ComponentSwizzle a = ComponentSwizzle::Identity) : Base() {
            this->r = convertCEnum(r);
            this->g = convertCEnum(g);
            this->b = convertCEnum(b);
            this->a = convertCEnum(a);
        }
        ComponentMapping& setR(ComponentSwizzle r) {
            this->r = convertCEnum(r);
            return *this;
        }
        ComponentMapping& setG(ComponentSwizzle g) {
            this->g = convertCEnum(g);
            return *this;
        }
        ComponentMapping& setB(ComponentSwizzle b) {
            this->b = convertCEnum(b);
            return *this;
        }
        ComponentMapping& setA(ComponentSwizzle a) {
            this->a = convertCEnum(a);
            return *this;
        }
	};

    class ImageRef : public BaseComponent<VkImage, ImageRef>
    {
        using Base = BaseComponent<VkImage, ImageRef>;
    public:
        using Base::Base;

        MemoryRequirements getMemoryRequirements(const DeviceRef& device,
            const DeviceFunctionTable& functions) const;
    };

    class Image : public VerificatorComponent<VkImage, ImageRef>
    {
        friend class SwapChain;
        using Base = VerificatorComponent<VkImage, ImageRef>;
    public:
        using Base::Base;

        class CreateInfo : public StructBase<VkImageCreateInfo, CreateInfo>
        {
            using Base = StructBase<VkImageCreateInfo, CreateInfo>;
        public:
            using Base::Base;

            CreateInfo(ImageType imageType = ImageType::T2D,
                Format format = Format::R8G8B8A8Srgb,
                const Extent3D& extent = { 1, 1, 1 },
                uint32_t mipLevels = 1,
                uint32_t arrayLayers = 1,
                SampleCount::Bits samples = SampleCount::Bits::SC1,
                ImageTiling tiling = ImageTiling::Optimal,
                ImageUsage::Flags usage = ImageUsage::Bits::TransferDst |
                ImageUsage::Bits::Sampled |
                ImageUsage::Bits::TransferSrc,
                SharingMode sharingMode = SharingMode::Exclusive,
                const std::vector<uint32_t>& queueFamilyIndices = {},
                ImageLayout initialLayout = ImageLayout::Undefined) : Base()
            {
                this->imageType = convertCEnum(imageType);
                this->format = convertCEnum(format);
                this->extent = extent.getStruct();
                this->mipLevels = mipLevels;
                this->arrayLayers = arrayLayers;
                this->samples = static_cast<VkSampleCountFlagBits>(samples);
                this->tiling = convertCEnum(tiling);
                this->usage = usage;
                this->sharingMode = convertCEnum(sharingMode);
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                this->initialLayout = convertCEnum(initialLayout);
            }
            CreateInfo& setImageType(ImageType imageType) {
                this->imageType = convertCEnum(imageType);
                return *this;
            }
            CreateInfo& setFormat(Format format) {
                this->format = convertCEnum(format);
                return *this;
            }
            CreateInfo& setExtent(const Extent3D& extent) {
                this->extent = extent.getStruct();
                return *this;
            }

            CreateInfo& setMipLevels(uint32_t mipLevels) {
                this->mipLevels = mipLevels;
                return *this;
            }

            CreateInfo& setArrayLayers(uint32_t arrayLayers) {
                this->arrayLayers = arrayLayers;
                return *this;
            }
            CreateInfo& setSamples(SampleCount::Bits samples) {
                this->samples = convertCEnum(samples);
                return *this;
            }
            CreateInfo& setTiling(ImageTiling tiling) {
                this->tiling = convertCEnum(tiling);
                return *this;
            }
            CreateInfo& setUsage(ImageUsage::Flags usage) {
                this->usage = usage;
                return *this;
            }
            CreateInfo& setSharingMode(SharingMode sharingMode) {
                this->sharingMode = convertCEnum(sharingMode);
                return *this;
            }
            CreateInfo& setQueueFamilyIndices(const std::vector<uint32_t>& queueFamilyIndices) {
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                return *this;
            }
            CreateInfo& setInitialLayout(ImageLayout initialLayout) {
                this->initialLayout = convertCEnum(initialLayout);
                return *this;
            }
        };

        class SubresourceRange : public StructBase<VkImageSubresourceRange, SubresourceRange>
        {
			using Base = StructBase<VkImageSubresourceRange, SubresourceRange>;
            public:
            using Base::Base;
            SubresourceRange(ImageAspect::Flags aspectMask = ImageAspect::Bits::Color,
                uint32_t baseMipLevel = 0, uint32_t levelCount = 1,
                uint32_t baseArrayLayer = 0, uint32_t layerCount = 1) : Base() {
                this->aspectMask = aspectMask;
                this->baseMipLevel = baseMipLevel;
                this->levelCount = levelCount;
                this->baseArrayLayer = baseArrayLayer;
                this->layerCount = layerCount;
            }
            SubresourceRange& setAspectMask(ImageAspect::Flags aspectMask) {
                this->aspectMask = aspectMask;
                return *this;
            }
            SubresourceRange& setBaseMipLevel(uint32_t baseMipLevel) {
                this->baseMipLevel = baseMipLevel;
                return *this;
            }
            SubresourceRange& setLevelCount(uint32_t levelCount) {
                this->levelCount = levelCount;
                return *this;
            }
            SubresourceRange& setBaseArrayLayer(uint32_t baseArrayLayer) {
                this->baseArrayLayer = baseArrayLayer;
                return *this;
            }
            SubresourceRange& setLayerCount(uint32_t layerCount) {
                this->layerCount = layerCount;
                return *this;
            }
		};

        class ViewRef : public BaseComponent<VkImageView, ViewRef> {
            using Base = BaseComponent<VkImageView, ViewRef>;
        public:
            using Base::Base;
        };

        class View : public VerificatorComponent<VkImageView, ViewRef>
        {
            using Base = VerificatorComponent<VkImageView, ViewRef>;
        public:
            using Base::Base;

            class CreateInfo : public StructBase<VkImageViewCreateInfo, CreateInfo>
            {
                using Base = StructBase<VkImageViewCreateInfo, CreateInfo>;
            public:
                using Base::Base;
                CreateInfo(ImageViewType viewType = ImageViewType::T2D,
                    Format format = Format::R8G8B8A8Srgb,
                    const ComponentMapping& components = ComponentMapping(),
                    const SubresourceRange& subresourceRange = SubresourceRange()) : Base() {
                    this->viewType = convertCEnum(viewType);
                    this->format = convertCEnum(format);
                    this->components = components.getStruct();
                    this->subresourceRange = subresourceRange.getStruct();
                }
                CreateInfo& setImage(const Image& image) {
                    this->image = image.getHandle();
                    return *this;
                }
                CreateInfo& setViewType(ImageViewType viewType) {
                    this->viewType = convertCEnum(viewType);
                    return *this;
                }
                CreateInfo& setFormat(Format format) {
                    this->format = convertCEnum(format);
                    return *this;
                }
                CreateInfo& setComponents(const ComponentMapping& components) {
                    this->components = components.getStruct();
                    return *this;
                }
                CreateInfo& setSubresourceRange(const SubresourceRange& range) {
                    this->subresourceRange = range.getStruct();
                    return *this;
                }

                ComponentMapping& getComponents() {
                    return *reinterpret_cast<ComponentMapping*>(&this->components);
				}

                SubresourceRange& getSubresourceRange() {
                    return *reinterpret_cast<SubresourceRange*>(&this->subresourceRange);
                }
            };
            
            void create(const DeviceRef& device, const DeviceFunctionTable& functions,
                const ImageRef& image, const CreateInfo& createInfo);
            void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
        };

        void create(const DeviceRef& device, const DeviceFunctionTable& functions,
            const CreateInfo& createInfo);      

        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions);
    };

}

