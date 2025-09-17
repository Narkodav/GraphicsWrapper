#include "SwapChain.h"

namespace Graphics {

    SwapChain::SwapChain(const Instance& instance, const Device& device, const Surface& surface,
        const RenderPass& renderPass, const SwapChainFormat& format,
        uint32_t presentQueueIndex, uint32_t workerQueueIndex) {
        init(instance, device, surface, renderPass, format, presentQueueIndex, workerQueueIndex);
    }

    void SwapChain::init(const Instance& instance, const Device& device, const Surface& surface,
        const RenderPass& renderPass, const SwapChainFormat& format,
        uint32_t presentQueueIndex, uint32_t workerQueueIndex)
    {
        m_activeSwapChainFormat = format;
        auto supportDetails = surface.getSwapChainSupportDetails(instance, device.getPhysicalDevice());

        m_imageCount = supportDetails.capabilities.minImageCount + 1;
        auto d = supportDetails.capabilities.currentTransform;
        if (supportDetails.capabilities.maxImageCount > 0 && m_imageCount > supportDetails.capabilities.maxImageCount) {
            m_imageCount = supportDetails.capabilities.maxImageCount;
        }

        m_swapChain = createSwapChain(device, supportDetails, surface,
            m_activeSwapChainFormat, m_imageCount, presentQueueIndex, workerQueueIndex);
        m_swapChainImages = getSwapChainImages(device, m_swapChain, m_imageCount);
        m_swapChainImageViews = createImageViews(device, m_activeSwapChainFormat,
            std::span(m_swapChainImages, m_imageCount));

        if (m_activeSwapChainFormat.getDepthFormat() != vk::Format::eUndefined)
        {
            initDepthImage(instance, device);
            m_swapChainFrameBuffers = createFrameBuffers(device, renderPass,
                std::span(m_swapChainImageViews, m_imageCount), m_depthImageView, m_activeSwapChainFormat);
        }
        else m_swapChainFrameBuffers = createFrameBuffers(device, renderPass,
            std::span(m_swapChainImageViews, m_imageCount), m_activeSwapChainFormat);
    }

    void SwapChain::recreate(const Instance& instance, const Device& device, const Surface& surface,
        const RenderPass& renderPass, const SwapChainFormat& format, uint32_t presentQueueIndex, uint32_t workerQueueIndex)
    {
        destroy(device);
        init(instance, device, surface, renderPass, format, presentQueueIndex, workerQueueIndex);
    }

    VkSwapchainKHR SwapChain::createSwapChain(const Device& device, const SwapChainSupportDetails& swapChainSupportDetails,
        const Surface& surface, const SwapChainFormat& activeSwapChainFormat, uint32_t imageCount,
        uint32_t presentQueueIndex, uint32_t workerQueueIndex)
    {
        vk::SwapchainCreateInfoKHR createInfo;
        createInfo.surface = surface.getHandle();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = activeSwapChainFormat.getSurfaceFormat().format;
        createInfo.imageColorSpace = activeSwapChainFormat.getSurfaceFormat().colorSpace;
        createInfo.imageExtent = activeSwapChainFormat.getSwapChainExtent();
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        std::array<uint32_t, 2> queueFamilyIndices = { presentQueueIndex, workerQueueIndex };
        if (presentQueueIndex != workerQueueIndex) {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        createInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = activeSwapChainFormat.getPresentMode();
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkSwapchainKHR swapChain;

        auto result = device.getFunctionTable().execute<DeviceFunction::CreateSwapchainKHR>(
			device.getHandle(), &createInfo, nullptr, &swapChain);

        if (result != VK_SUCCESS) throw std::runtime_error("VkSwapchainKHR SwapChain::createSwapChain("
            "const Device & device, const SwapChainSupportDetails & swapChainSupportDetails, "
            "const Surface & surface, const SwapChainFormat & activeSwapChainFormat, uint32_t imageCount, "
            "uint32_t presentQueueIndex, uint32_t workerQueueIndex) - failed to create swap chain: " +
            s_resultMessages.at(result));

        return swapChain;
    }

    VkImage* SwapChain::getSwapChainImages(const Device& device, const VkSwapchainKHR& swapChain, uint32_t& imageCount)
    {
        VkImage* swapChainImages;

        auto result = device.getFunctionTable().execute<DeviceFunction::GetSwapchainImagesKHR>(
            device.getHandle(), swapChain, &imageCount, swapChainImages);

        if (result != VK_SUCCESS) throw std::runtime_error("VkImage* SwapChain::getSwapChainImages("
            "const Device & device, const VkSwapchainKHR & swapChain, uint32_t & imageCount) - failed to get swap chain images: " +
            s_resultMessages.at(result));

        return swapChainImages;
    }

    VkImageView* SwapChain::createImageViews(const Device& device, 
    const SwapChainFormat& activeSwapChainFormat,const std::span<VkImage>& swapChainImages)
    {
		VkImageView* imageViews = new VkImageView[swapChainImages.size()];

        for (int i = 0; i < swapChainImages.size(); i++)
        {
            vk::ImageViewCreateInfo createInfo;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = vk::ImageViewType::e2D;
            createInfo.format = activeSwapChainFormat.getSurfaceFormat().format;
            createInfo.components.r = vk::ComponentSwizzle::eIdentity;
            createInfo.components.g = vk::ComponentSwizzle::eIdentity;
            createInfo.components.b = vk::ComponentSwizzle::eIdentity;
            createInfo.components.a = vk::ComponentSwizzle::eIdentity;
            createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            auto result = device.getFunctionTable().execute<DeviceFunction::CreateImageView>(
                device.getHandle(), &createInfo, nullptr, imageViews + i);
            if (result != VK_SUCCESS) throw std::runtime_error(
                "VkImageView* SwapChain::createImageViews("
                "const Device & device, const SwapChainFormat & activeSwapChainFormat, "
                "const std::span<VkImage>&swapChainImages) - failed to create image view: " +
                s_resultMessages.at(result));
        }

        return imageViews;
    }

    VkFramebuffer* SwapChain::createFrameBuffers(const Device& device, const RenderPass& renderPass,
        const std::span<VkImageView>& imageViews, const SwapChainFormat& format)
    {
        VkFramebuffer* framebuffers = new VkFramebuffer[imageViews.size()];

        for (size_t i = 0; i < imageViews.size(); i++) {
            vk::FramebufferCreateInfo createInfo;
            createInfo.renderPass = renderPass.getRenderPass();
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = reinterpret_cast<vk::ImageView*>(&imageViews[i]);
            createInfo.width = format.getSwapChainExtent().width;
            createInfo.height = format.getSwapChainExtent().height;
            createInfo.layers = 1;

            auto result = device.getFunctionTable().execute<DeviceFunction::CreateFramebuffer>(
                device.getHandle(), &createInfo, nullptr, framebuffers + i);

            if (result != VK_SUCCESS) throw std::runtime_error(
                "VkFramebuffer* SwapChain::createFrameBuffers("
                "const Device & device, const RenderPass & renderPass, "
                "const std::span<VkImageView>&imageViews, const SwapChainFormat & format) - failed to create image view: " +
                s_resultMessages.at(result));
        }
        return framebuffers;
    }

    VkFramebuffer* SwapChain::createFrameBuffers(const Device& device, const RenderPass& renderPass,
        const std::span<VkImageView>& imageViews, VkImageView depthImageView,
        const SwapChainFormat& format)
    {
        VkFramebuffer* framebuffers = new VkFramebuffer[imageViews.size()];

        for (size_t i = 0; i < imageViews.size(); i++) {
            vk::ImageView attachments[] = {
                imageViews[i], depthImageView
            };

            vk::FramebufferCreateInfo createInfo{};
            createInfo.sType = vk::StructureType::eFramebufferCreateInfo;
            createInfo.renderPass = renderPass.getRenderPass();
            createInfo.attachmentCount = 2;
            createInfo.pAttachments = attachments;
            createInfo.width = format.getSwapChainExtent().width;
            createInfo.height = format.getSwapChainExtent().height;
            createInfo.layers = 1;

            auto result = device.getFunctionTable().execute<DeviceFunction::CreateFramebuffer>(
                device.getHandle(), &createInfo, nullptr, framebuffers + i);

            if (result != VK_SUCCESS) throw std::runtime_error(
                "VkFramebuffer* SwapChain::createFrameBuffers("
                "const Device & device, const RenderPass & renderPass, "
                "const std::span<VkImageView>&imageViews, VkImageView depthImageView, "
                "const SwapChainFormat & format) - failed to create image view: " +
                s_resultMessages.at(result));
        }
        return framebuffers;
    }

    void SwapChain::initDepthImage(const Instance& instance, const Device& device)
    {
        vk::ImageCreateInfo imageInfo{};
        imageInfo.sType = vk::StructureType::eImageCreateInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = m_activeSwapChainFormat.getSwapChainExtent().width;
        imageInfo.extent.height = m_activeSwapChainFormat.getSwapChainExtent().height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = m_activeSwapChainFormat.getDepthFormat();
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.flags = vk::ImageCreateFlags(); // Optional

        auto result = device.getFunctionTable().execute<DeviceFunction::CreateImage>(
            device.getHandle(), &imageInfo, nullptr, &m_depthImage);
        if (result != VK_SUCCESS) throw std::runtime_error(
            "void SwapChain::initDepthImage(const Instance& instance, const Device& device) - failed to create image: " +
            s_resultMessages.at(result));

        vk::MemoryRequirements imageMemoryRequirements;

        device.getFunctionTable().execute<DeviceFunction::GetImageMemoryRequirements>(
            device.getHandle(), m_depthImage, &imageMemoryRequirements);

        m_depthImageMemory = Memory(instance, device, imageMemoryRequirements,
            MemoryProperty::Bits::DeviceLocal, imageMemoryRequirements.size);

        auto result = device.getFunctionTable().execute<DeviceFunction::BindImageMemory>(
            device.getHandle(), m_depthImage, m_depthImageMemory.getHandle());
        if (result != VK_SUCCESS) throw std::runtime_error(
            "void SwapChain::initDepthImage(const Instance& instance, const Device& device) - failed to bind image memory: " +
            s_resultMessages.at(result));

        vk::ImageViewCreateInfo createInfo{};
        createInfo.sType = vk::StructureType::eImageViewCreateInfo;
        createInfo.image = m_depthImage;
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = m_activeSwapChainFormat.getDepthFormat();
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        if (m_activeSwapChainFormat.getDepthFormat() == vk::Format::eD32SfloatS8Uint
            || m_activeSwapChainFormat.getDepthFormat() == vk::Format::eD24UnormS8Uint)
            createInfo.subresourceRange.aspectMask =
            vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
        else createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        auto result = device.getFunctionTable().execute<DeviceFunction::CreateImageView>(
            device.getHandle(), &createInfo, nullptr, m_depthImage);
        if (result != VK_SUCCESS) throw std::runtime_error(
            "void SwapChain::initDepthImage(const Instance& instance, const Device& device) - failed to create image view: " +
            s_resultMessages.at(result));
    }

}