#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "Image.h"
#include "Semaphore.h"
#include "Fence.h"

namespace Graphics {
    class SwapChainRef : public BaseComponent<VkSwapchainKHR, SwapChainRef> 
    {
        using Base = BaseComponent<VkSwapchainKHR, SwapChainRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "SwapChain";
    };


	class SwapChain : public VerificatorComponent<VkSwapchainKHR, SwapChainRef>
	{
		using Base = VerificatorComponent<VkSwapchainKHR, SwapChainRef>;
	public:
		using Base::Base;
    public:
        class Images : protected std::vector<VkImage> {
        private:
            friend class SwapChain;
			
            Images(std::vector<VkImage>&& images) : std::vector<VkImage>(std::move(images)) {};
        public:
            
            Images() = default;
            Images(const Images&) = delete;
            Images& operator=(const Images&) = delete;
            Images(Images&&) = default;
            Images& operator=(Images&&) = default;

            const Image& operator[](uint32_t index) const
            {
                return *reinterpret_cast<const Image*>(&(std::vector<VkImage>::operator[](index)));
            };

            size_t size() const { return std::vector<VkImage>::size(); };
            const ImageRef* data() const { return Image::underlyingCast(std::vector<VkImage>::data()); };

            const ImageRef* begin() const { return Image::underlyingCast(std::vector<VkImage>::data()); }
            const ImageRef* end() const { return Image::underlyingCast(std::vector<VkImage>::data()) + size(); }

            const ImageRef* cbegin() const { return Image::underlyingCast(std::vector<VkImage>::data()); }
            const ImageRef* cend() const { return Image::underlyingCast(std::vector<VkImage>::data()) + size(); }
        };

        class CreateInfo : public StructBase<VkSwapchainCreateInfoKHR, CreateInfo>
        {
			using Base = StructBase<VkSwapchainCreateInfoKHR, CreateInfo>;
            public:
            using Base::Base;
            CreateInfo(const Surface& surface,
                const Extent2D& imageExtent = { 1, 1 },
                uint32_t minImageCount = 2,
                uint32_t imageArrayLayers = 1,
                Format imageFormat = Format::B8G8R8A8Srgb,
				ColorSpace imageColorSpace = ColorSpace::SrgbNonlinear,
                ImageUsage::Flags imageUsage = ImageUsage::Bits::ColorAttachment,
                SharingMode imageSharingMode = SharingMode::Exclusive,
                std::span<const uint32_t> queueFamilyIndices = {},
                SurfaceTransform::Bits preTransform = SurfaceTransform::Bits::Identity,
                CompositeAlpha::Bits compositeAlpha = CompositeAlpha::Bits::Opaque,
                PresentMode presentMode = PresentMode::Fifo,
                bool clipped = true,
                const SwapChain& oldSwapChain = SwapChain()) : Base()
            {
                this->surface = surface.getHandle();
                this->minImageCount = minImageCount;
                this->imageFormat = convertCEnum(imageFormat);
                this->imageColorSpace = convertCEnum(imageColorSpace);
                this->imageExtent = imageExtent.getStruct();
                this->imageArrayLayers = imageArrayLayers;
                this->imageUsage = imageUsage;
                this->imageSharingMode = convertCEnum(imageSharingMode);
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                this->preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
                this->compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(compositeAlpha);
                this->presentMode = convertCEnum(presentMode);
                this->clipped = clipped;
                this->oldSwapchain = oldSwapChain.getHandle();
            }

            CreateInfo(const Surface& surface,                
                const Surface::Format& surfaceFormat,
                const Extent2D& imageExtent = { 1, 1 },
                uint32_t minImageCount = 2,
                uint32_t imageArrayLayers = 1,
                ImageUsage::Flags imageUsage = ImageUsage::Bits::ColorAttachment,
                SharingMode imageSharingMode = SharingMode::Exclusive,
                const std::vector<uint32_t>& queueFamilyIndices = {},
                SurfaceTransform::Bits preTransform = SurfaceTransform::Bits::Identity,
                CompositeAlpha::Bits compositeAlpha = CompositeAlpha::Bits::Opaque,
                PresentMode presentMode = PresentMode::Fifo,
                bool clipped = true,
                const SwapChain& oldSwapChain = SwapChain()) : Base()
            {
                this->surface = surface.getHandle();
                this->minImageCount = minImageCount;
                this->imageFormat = convertCEnum(surfaceFormat.getFormat());
                this->imageColorSpace = convertCEnum(surfaceFormat.getColorSpace());
                this->imageExtent = imageExtent.getStruct();
                this->imageArrayLayers = imageArrayLayers;
                this->imageUsage = imageUsage;
                this->imageSharingMode = convertCEnum(imageSharingMode);
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                this->preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
                this->compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(compositeAlpha);
                this->presentMode = convertCEnum(presentMode);
                this->clipped = clipped;
                this->oldSwapchain = oldSwapChain.getHandle();
            }

            CreateInfo& setSurface(const SurfaceRef& surface) {
                this->surface = surface.getHandle();
                return *this;
            }
            CreateInfo& setMinImageCount(uint32_t minImageCount) {
                this->minImageCount = minImageCount;
                return *this;
            }
            CreateInfo& setImageFormat(const Surface::Format& imageFormat) {
                this->imageFormat = convertCEnum(imageFormat.getFormat());
                this->imageColorSpace = convertCEnum(imageFormat.getColorSpace());
                return *this;
            }
            CreateInfo& setImageFormat(Format imageFormat) {
                this->imageFormat = convertCEnum(imageFormat);
                return *this;
            }
            CreateInfo& setImageColorSpace(ColorSpace imageColorSpace) {
                this->imageColorSpace = convertCEnum(imageColorSpace);
                return *this;
            }
            CreateInfo& setImageExtent(const Extent2D& imageExtent) {
                this->imageExtent = imageExtent.getStruct();
                return *this;
            }
            CreateInfo& setImageUsage(ImageUsage::Flags imageUsage) {
				this->imageUsage = imageUsage;
                return *this;
            }
            CreateInfo& setImageSharingMode(SharingMode imageSharingMode) {
                this->imageSharingMode = convertCEnum(imageSharingMode);
                return *this;
            }
            CreateInfo& setQueueFamilyIndices(std::span<const uint32_t> queueFamilyIndices) {
                this->queueFamilyIndexCount = queueFamilyIndices.size();
                this->pQueueFamilyIndices = queueFamilyIndices.data();
                return *this;
            }
            CreateInfo& setPreTransform(SurfaceTransform::Bits preTransform) {
                this->preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(preTransform);
                return *this;
            }
            CreateInfo& setCompositeAlpha(CompositeAlpha::Bits compositeAlpha) {
                this->compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(compositeAlpha);
                return *this;
            }
            CreateInfo& setPresentMode(PresentMode presentMode) {
                this->presentMode = convertCEnum(presentMode);
                return *this;
            }
            CreateInfo& setClipped(bool clipped) {
                this->clipped = clipped;
                return *this;
            }
            CreateInfo& setOldSwapChain(const SwapChain& oldSwapChain) {
                this->oldSwapchain = oldSwapChain.getHandle();
                return *this;
            }
            CreateInfo& setImageArrayLayers(uint32_t imageArrayLayers) {
                this->imageArrayLayers = imageArrayLayers;
                return *this;
            }

            Format getImageFormat() const {
                return convertCEnum(this->imageFormat);
			};
            ColorSpace getImageColorSpace() const {
                return convertCEnum(this->imageColorSpace);
            };
            Extent2D getImageExtent() const {
                return Extent2D(this->imageExtent);
			};
            ImageUsage::Flags getImageUsage() const {
                return this->imageUsage;
			};
            SharingMode getImageSharingMode() const {
                return convertCEnum(this->imageSharingMode);
            };
            SurfaceTransform::Bits getPreTransform() const {
                return static_cast<SurfaceTransform::Bits>(this->preTransform);
            };
            CompositeAlpha::Bits getCompositeAlpha() const {
                return static_cast<CompositeAlpha::Bits>(this->compositeAlpha);
            };
            PresentMode getPresentMode() const {
                return convertCEnum(this->presentMode);
            };
            bool isClipped() const {
                return this->clipped;
			};
            const SwapChainRef& getOldSwapChain() const {
                return SwapChainRef(this->oldSwapchain);
            };
            const SurfaceRef& getSurface() const {
                return SurfaceRef(this->surface);
            };
            std::span<const uint32_t> getQueueFamilyIndices() const {
                return std::span<const uint32_t>(this->pQueueFamilyIndices, this->queueFamilyIndexCount);
            };
            uint32_t getImageArrayLayers() const {
                return this->imageArrayLayers;
            }
		};

        Images create(const DeviceRef& device, const DeviceFunctionTable& functions,
            const CreateInfo& createInfo);
        void destroy(const DeviceRef& device, const DeviceFunctionTable& functions,
            Images& swapChainImages);

		//implicitly sets the old swap chain parameter in create info
        void recreate(const DeviceRef& device, const DeviceFunctionTable& functions,
            CreateInfo& createInfo, Images& swapChainImages);

        bool acquireNextImage(const DeviceRef& device, const DeviceFunctionTable& functions,
            const SemaphoreRef& semaphore, const FenceRef& fence, uint32_t& imageIndex,
            uint32_t timeout = std::numeric_limits<uint32_t>::max());
	};
}

