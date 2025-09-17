#include "Graphics/Wrappers/Instance.h"
#include "Graphics/Wrappers/DebugUtilsMessenger.h"
#include "Graphics/Wrappers/Surface.h"
#include "Graphics/Wrappers/PhysicalDevice.h"
#include "Graphics/Wrappers/PhysicalDeviceCache.h"
#include "Graphics/Wrappers/RenderPass.h"
#include "Graphics/Wrappers/FrameBuffer.h"
#include "Graphics/Wrappers/Image.h"
#include "Graphics/Wrappers/SwapChain.h"

#include "Graphics/Wrappers/Utility/Utility.h"

#include "Graphics/PlatformManagement/Window.h"

#include "MultiThreading/Console.h"

namespace Gfx = Graphics;

GRAPHICS_API_ATTR Gfx::bool32_t GRAPHICS_API_CALL static_debugCallback(
    Gfx::DebugMessageSeverity::Bits messageSeverity,
    Gfx::DebugMessageType::Flags messageType,
    const Gfx::DebugUtils::CallbackData* pCallbackData,
    void* pUserData) {

    switch (messageSeverity) {
    case Gfx::DebugMessageSeverity::Bits::Verbose:
        // Diagnostic messages
        std::cout << "VERBOSE: " << pCallbackData->getMessage() << std::endl;
        break;

    case Gfx::DebugMessageSeverity::Bits::Info:
        // Informational messages like resource creation
        std::cout << "INFO: " << pCallbackData->getMessage() << std::endl;
        break;

    case Gfx::DebugMessageSeverity::Bits::Warning:
        // Warning messages like use of deprecated functions
        std::cout << "WARNING: " << pCallbackData->getMessage() << std::endl;
        break;

    case Gfx::DebugMessageSeverity::Bits::Error:
        // Error messages for invalid behavior
        std::cerr << "ERROR: " << pCallbackData->getMessage() << std::endl;
        throw std::runtime_error("Validation layer error");
        break;

    default:
        std::cout << "UNKNOWN SEVERITY: " << pCallbackData->getMessage() << std::endl;
        std::abort();
        break;
    }
    return 0;
}

int main()
{

    MT::Console& console = MT::Console::getInstance();

    std::vector<const char*> requiredLayers = { "VK_LAYER_KHRONOS_validation"};
    std::vector<const char*> requiredExtensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    
    Window window = Window({ 800, 600 }, "app", Window::Attributes::defaultAtr());
    Gfx::Instance instance;
    Gfx::Instance::CreateInfo createInfo;
    Gfx::DebugUtils::Messenger::CreateInfo debugCreateInfo;
    Gfx::AppInfo appInfo;
    debugCreateInfo.setCallback(static_debugCallback)
        .setMessageSeverity(Gfx::DebugMessageSeverity::Bits::All)
        .setMessageType(Gfx::DebugMessageType::Bits::AllLegacy);

    appInfo.setAppName("app")
        .setAppVersion({ 1, 0, 0 })
        .setEngineName("engine")
        .setEngineVersion({ 1, 0, 0 });
    createInfo.setAppInfo(appInfo)
        .setNext(&debugCreateInfo);
    auto required = instance.getRequiredExtensions();

    requiredExtensions.insert(requiredExtensions.end(),
        required.begin(), required.end());

    createInfo.setEnabledExtensions(requiredExtensions)
        .setEnabledLayers(requiredLayers);

    instance.create(createInfo);
    auto instanceFunctions = instance.getFunctionTable();

    Gfx::DebugUtils::Messenger debugMessenger;
    debugMessenger.create(instance, instanceFunctions, debugCreateInfo);    

    Gfx::Surface surface;
    surface.create(instance, window, instanceFunctions);

    std::vector<Gfx::PhysicalDevice> physicalDevices = instance.getPhysicalDevices(instanceFunctions);

    console.writeLine("Physical devices count: {}", physicalDevices.size());    
    Gfx::PhysicalDeviceCache deviceCache = instance.cachePhysicalDevices(instanceFunctions);

    Gfx::DeviceRequirements requirements;
    requirements.extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        /*VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME*/ };
    requirements.properties = {
        {Gfx::DeviceProperty::PhysicalDeviceType, Gfx::PhysicalDeviceType::DiscreteGpu} };
    requirements.features = {
        {Gfx::DeviceFeature::GeometryShader, true},
        {Gfx::DeviceFeature::SamplerAnisotropy, true},
        {Gfx::DeviceFeature::DescriptorBindingPartiallyBound, true},
        {Gfx::DeviceFeature::RuntimeDescriptorArray, true},
        {Gfx::DeviceFeature::ShaderSampledImageArrayNonUniformIndexing, true},
        {Gfx::DeviceFeature::DescriptorBindingSampledImageUpdateAfterBind, true},
        {Gfx::DeviceFeature::DescriptorBindingVariableDescriptorCount, true},
        {Gfx::DeviceFeature::MeshShaderEXT, true},
        {Gfx::DeviceFeature::TaskShaderEXT, true},
        {Gfx::DeviceFeature::ShaderDrawParameters, true},
        {Gfx::DeviceFeature::MultiDrawIndirect, true},
        {Gfx::DeviceFeature::DescriptorBindingStorageBufferUpdateAfterBind, true},
        {Gfx::DeviceFeature::NullDescriptor, true}, };

    requirements.queueProperties.push_back(Gfx::RequiredQueueProperties());
    requirements.queueProperties.back().queueProperties
        .insert({ Gfx::QueueProperty::QueueFlags, 
        Gfx::QueueFlags::Bits::Graphics | Gfx::QueueFlags::Bits::Transfer });
    requirements.queueProperties.back().shouldSupportPresent = false;

    requirements.queueProperties.push_back(Gfx::RequiredQueueProperties());
    requirements.queueProperties.back().shouldSupportPresent = true;

    auto result = deviceCache.getFittingDevice(instanceFunctions, surface, requirements);
    if (!result.isSuitable())
        std::cout << "No suitable device found" << std::endl;
    else std::cout << "Suitable device found" << std::endl;

    Gfx::PhysicalDevice& physicalDevice = result.device;

    //search for discrete graphics, present and transfer families
    uint32_t graphicsIndex, presentIndex;

    graphicsIndex = result.queueFamilyIndices[0].front();
    size_t i;

    for (i = 0; i < result.queueFamilyIndices[1].size(); i++)
        if (graphicsIndex != result.queueFamilyIndices[1][i])
        {
            presentIndex = result.queueFamilyIndices[1][i];
            break;
        }
    if (i == result.queueFamilyIndices[1].size()) {
        presentIndex = result.queueFamilyIndices[1].front();
    }

    std::vector<std::vector<float>> queuePriorities = { {1.0f, 1.0f}, {1.0f} };
    std::vector<Gfx::QueueCreateInfo> queueCreateInfos(2);
    queueCreateInfos[0].setQueueFamilyIndex(graphicsIndex)
        .setQueuePriorities(queuePriorities[0]);
    queueCreateInfos[1].setQueueFamilyIndex(presentIndex)
        .setQueuePriorities(queuePriorities[1]);

    Gfx::Device::CreateInfo deviceCreateInfo;
    deviceCreateInfo.setQueueCreateInfos(queueCreateInfos)
        .setEnabledExtensions(requirements.extensions)
        .setEnabledFeatures(result.enabledFeatures);

    Gfx::Device device;
    device.create(instance, result.device, instanceFunctions, deviceCreateInfo);
    auto deviceFunctions = device.getFunctionTable(instanceFunctions);

    Gfx::Queue graphicsQueue = device.getQueue(graphicsIndex, 0, deviceFunctions);
    Gfx::Queue presentQueue = device.getQueue(presentIndex, 0, deviceFunctions);
    Gfx::Queue transferQueue = device.getQueue(graphicsIndex, 1, deviceFunctions);

    Gfx::RenderRegion canvas = Gfx::RenderRegion::createFullWindow(window.getWindowExtent());

    auto capabilities = physicalDevice.getSurfaceCapabilities(instanceFunctions, surface);
	auto formats = physicalDevice.getSurfaceFormats(instanceFunctions, surface);
	auto presentModes = physicalDevice.getSurfacePresentModes(instanceFunctions, surface);
    
	size_t formatIndex = 0;
    size_t presentModeIndex = 0;

    for (size_t i = 0; i < formats.size(); i++) {
        if (formats[i].getFormat() == Gfx::Format::B8G8R8A8Srgb &&
            formats[i].getColorSpace() == Gfx::ColorSpace::SrgbNonlinear) {
            formatIndex = i;
        }
    }

    for (size_t i = 0; i < presentModes.size(); i++) {
        if (presentModes[i] == Gfx::PresentMode::Mailbox) {
            presentModeIndex = i;
        }
    }

    std::vector<Gfx::Attachment> attachments;
    attachments.reserve(2);
	attachments.push_back(Gfx::Attachment{});

    attachments[0].setFormat(formats[formatIndex].getFormat())
        .setSamples(Gfx::SampleCount::Bits::SC1)
        .setLoadOp(Gfx::AttachmentLoadOp::Clear)
        .setStoreOp(Gfx::AttachmentStoreOp::Store)
        .setStencilLoadOp(Gfx::AttachmentLoadOp::DontCare)
        .setStencilStoreOp(Gfx::AttachmentStoreOp::DontCare)
        .setInitialLayout(Gfx::ImageLayout::Undefined)
        .setFinalLayout(Gfx::ImageLayout::PresentSrcKHR);

	std::vector<Gfx::Attachment::Reference> colorAattachmentRefs(1);
    colorAattachmentRefs[0].setAttachment(0)
		.setLayout(Gfx::ImageLayout::ColorAttachmentOptimal);

    std::vector<Gfx::Subpass> subpasses(1);
    subpasses[0].setBindPoint(Gfx::PipelineBindPoint::Graphics)
        .setColorAttachments(colorAattachmentRefs);

	std::vector<Gfx::Subpass::Dependency> dependencies(1);
    dependencies[0].setSrcSubpass(Gfx::Subpass::s_externalSubpass)
		.setDstSubpass(0)
		.setSrcStageMask(Gfx::PipelineStage::Bits::ColorAttachmentOutput)
		.setSrcAccessMask(Gfx::Access::Bits::None)
		.setDstStageMask(Gfx::PipelineStage::Bits::ColorAttachmentOutput)
		.setDstAccessMask(Gfx::Access::Bits::ColorAttachmentWrite);

    std::vector<Gfx::Attachment::Reference> depthAttachmentRefs{};

    Gfx::Format depthFormat = Gfx::Utility::findDepthFormat(instanceFunctions, physicalDevice);

    if (depthFormat != Gfx::Format::Undefined)
    {
        attachments.push_back(Gfx::Attachment{});
        attachments[1].setFormat(depthFormat)
            .setSamples(Gfx::SampleCount::Bits::SC1)
            .setLoadOp(Gfx::AttachmentLoadOp::Clear)
            .setStoreOp(Gfx::AttachmentStoreOp::DontCare)
            .setStencilLoadOp(Gfx::AttachmentLoadOp::DontCare)
            .setStencilStoreOp(Gfx::AttachmentStoreOp::DontCare)
            .setInitialLayout(Gfx::ImageLayout::Undefined)
            .setFinalLayout(Gfx::ImageLayout::DepthStencilAttachmentOptimal);

        depthAttachmentRefs[0].setAttachment(1)
            .setLayout(Gfx::ImageLayout::DepthStencilAttachmentOptimal);

        subpasses[0].setDepthStencilAttachment(&depthAttachmentRefs[0]);

        dependencies[0].setSrcStageMask(Gfx::PipelineStage::Bits::EarlyFragmentTests)
            .setSrcAccessMask(Gfx::Access::Bits::None)
            .setDstStageMask(Gfx::PipelineStage::Bits::EarlyFragmentTests)
            .setDstAccessMask(Gfx::Access::Bits::DepthStencilAttachmentWrite);
    }
	else throw std::runtime_error("Failed to find supported depth format");

    Gfx::RenderPass::CreateInfo renderPassInfo;
    renderPassInfo.setAttachments(attachments)
        .setSubpasses(subpasses)
        .setDependencies(dependencies);

	Gfx::RenderPass renderPass;
	renderPass.create(device, deviceFunctions, renderPassInfo);

    auto swapChainData = Gfx::Utility::createBasicSwapChain(instanceFunctions, deviceFunctions,
        device, surface, Gfx::Utility::chooseExtent(capabilities, window.getFrameBufferExtent()),
        formats[formatIndex].getFormat(),
        depthFormat,
        formats[formatIndex].getColorSpace(),
        Gfx::ImageUsage::Bits::ColorAttachment,
        presentModes[presentModeIndex], capabilities.getMinImageCount());





    Gfx::Utility::destroySwapChainDaTa(swapChainData, device, deviceFunctions);
	renderPass.destroy(device, deviceFunctions);
    device.destroy(deviceFunctions);
    surface.destroy(instance, instanceFunctions);
    debugMessenger.destroy(instance, instanceFunctions);
    instance.destroy(instanceFunctions);
    window.destroy();
    return 0;
}