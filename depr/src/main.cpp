#include "../include/Graphics/HandleTypes/Instance.h"

// #include "../include/Graphics/HandleTypes/DebugUtilsMessenger.h"
// #include "../include/Graphics/HandleTypes/Surface.h"
// #include "../include/Graphics/HandleTypes/PhysicalDevice.h"
// #include "../include/Graphics/HandleTypes/PhysicalDeviceCache.h"
// #include "../include/Graphics/HandleTypes/RenderPass.h"
// #include "../include/Graphics/HandleTypes/FrameBuffer.h"
// #include "../include/Graphics/HandleTypes/Image.h"
// #include "../include/Graphics/HandleTypes/SwapChain.h"
// #include "../include/Graphics/HandleTypes/CommandPool.h"
// #include "../include/Graphics/HandleTypes/DescriptorPool.h"
// #include "../include/Graphics/Wrappers/InstanceWrapper.h"
// #include "../include/Graphics/Wrappers/DeviceWrapper.h"

#include "Graphics/Structs.h"
#include "Graphics/Camera.h"
#include "Graphics/Utility/PixelData2D.h"

#include "Graphics/Utility/Utility.h"

#include "Graphics/PlatformManagement/Window.h"


#include "PlatformAbstractions/Console.h"

namespace Gfx = Graphics;

int main()
{
    Platform::Window window = Platform::Window(Platform::Extent{ 800, 600 }, L"app", Platform::WindowAttributes::defaultAtr());

    Gfx::AppInfo appInfo;
    appInfo.setAppName("app")
        .setAppVersion({ 1, 0, 0 })
        .setEngineName("engine")
        .setEngineVersion({ 1, 0, 0 });

    Gfx::Wrappers::Instance instance(appInfo);
    instance.cachePhysicalDevices();
    const auto& instanceFunctions = instance.getFunctionTable();

    Gfx::Surface surface;
    surface.create(instanceFunctions, instance, window);

    //console.writeLine("Physical devices count: {}", physicalDevices.size());

    Gfx::DeviceRequirements requirements;
    requirements.extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME 
    };

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
        {Gfx::DeviceFeature::MeshShader, true},
        {Gfx::DeviceFeature::TaskShader, true},
        {Gfx::DeviceFeature::ShaderDrawParameters, true},
        {Gfx::DeviceFeature::MultiDrawIndirect, true},
        {Gfx::DeviceFeature::DescriptorBindingStorageBufferUpdateAfterBind, true},
        {Gfx::DeviceFeature::DescriptorIndexing, true},
        {Gfx::DeviceFeature::NullDescriptor, true}, };

    requirements.queueProperties.push_back(Gfx::RequiredQueueProperties());
    requirements.queueProperties.back().queueProperties
        .insert({ Gfx::QueueProperty::QueueFlags, 
        Gfx::Flags::Queue::Bits::Graphics | Gfx::Flags::Queue::Bits::Transfer });
    requirements.queueProperties.back().shouldSupportPresent = false;

    requirements.queueProperties.push_back(Gfx::RequiredQueueProperties());
    requirements.queueProperties.back().shouldSupportPresent = true;

    auto result = instance.getFittingDevice(surface, requirements);
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

    Gfx::DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.setQueueCreateInfos(queueCreateInfos)
        .setEnabledExtensions(requirements.extensions)
        .setEnabledFeatures(result.enabledFeatures);

    Gfx::Wrappers::Device device;
    device.create(instance, result.device, deviceCreateInfo);
    const auto& deviceFunctions = device.getFunctionTable();

    Gfx::Queue graphicsQueue = device.getQueue(graphicsIndex, 0);
    Gfx::Queue presentQueue = device.getQueue(presentIndex, 0);
    Gfx::Queue transferQueue = device.getQueue(graphicsIndex, 1);

    Gfx::RenderRegion canvas = Gfx::RenderRegion::createFullWindow(window.getWindowExtent());

    auto capabilities = physicalDevice.getSurfaceCapabilities(instanceFunctions, surface);
	auto formats = physicalDevice.getSurfaceFormats(instanceFunctions, surface);
	auto presentModes = physicalDevice.getSurfacePresentModes(instanceFunctions, surface);

    Gfx::PixelFormat depthFormat = Gfx::Utility::findDepthFormat(instanceFunctions, physicalDevice);

    if (depthFormat == Gfx::PixelFormat::Undefined) {
        throw std::runtime_error("Failed to find supported depth format");
    }

    auto renderPassData = Gfx::Utility::createColorDepthRenderPass(deviceFunctions, device,
        capabilities, formats, presentModes, depthFormat);    

    auto swapChainData = Gfx::Utility::createBasicSwapChain(instanceFunctions,
        deviceFunctions, physicalDevice, device, surface, renderPassData.renderPass,
        Gfx::Utility::chooseExtent(capabilities, window.getFrameBufferExtent()),
        renderPassData.surfaceFormat.getFormat(), depthFormat, renderPassData.surfaceFormat.getColorSpace(),
        Gfx::Flags::ImageUsage::Bits::ColorAttachment, renderPassData.presentMode, capabilities.getMinImageCount());

    std::vector<Gfx::DescriptorSetLayout> descriptorLayouts;
    std::vector<Gfx::DescriptorSetLayoutCreateInfo> descriptorLayoutCreateInfos;
    std::vector<Gfx::DescriptorSetLayoutBinding> bindings;

    bindings.push_back(Gfx::DescriptorSetLayoutBinding(0, Gfx::DescriptorType::UniformBuffer, 1, Gfx::Flags::ShaderStage::Bits::Vertex));
    bindings.push_back(Gfx::DescriptorSetLayoutBinding(0, Gfx::DescriptorType::CombinedImageSampler, 1024, Gfx::Flags::ShaderStage::Bits::Fragment));

    descriptorLayoutCreateInfos.push_back(Gfx::DescriptorSetLayoutCreateInfo(std::span(bindings.begin(), bindings.begin() + 1)));
    descriptorLayoutCreateInfos.push_back(Gfx::DescriptorSetLayoutCreateInfo(std::span(bindings.begin() + 1, bindings.end())));

    descriptorLayouts.resize(2);
    descriptorLayouts[0].create(deviceFunctions, device, descriptorLayoutCreateInfos[0]);
    descriptorLayouts[1].create(deviceFunctions, device, descriptorLayoutCreateInfos[1]);

    Gfx::Utility::GraphicsPipelineData pipelineData;

    pipelineData.pipelineLayoutCreateInfo.setSetLayouts(descriptorLayouts);
    pipelineData.pipelineLayout.create(deviceFunctions, device, pipelineData.pipelineLayoutCreateInfo);

    Gfx::Utility::ShaderModuleData shaderModuleData = Gfx::Utility::createShaderModules(
        deviceFunctions, device, { "Shaders/basic.vert.spv", "Shaders/basic.frag.spv" }
    );

    pipelineData.shaderStages = Gfx::Utility::createShaderStageInfos(shaderModuleData);
    pipelineData.vertexBindings.push_back(Gfx::VertexInputBindingDescription(
        0, sizeof(glm::vec4), Gfx::VertexInputRate::Vertex));
    pipelineData.vertexBindings.push_back(Gfx::VertexInputBindingDescription(
        1, sizeof(uint32_t), Gfx::VertexInputRate::Vertex));
    pipelineData.vertexAttributes.push_back(Gfx::VertexInputAttributeDescription(
        0, 0, Gfx::PixelFormat::R32G32B32A32Sfloat, 0));
    pipelineData.vertexAttributes.push_back(Gfx::VertexInputAttributeDescription(
        1, 1, Gfx::PixelFormat::R32Uint, 0));
    pipelineData.dynamicStates = { Gfx::DynamicState::Viewport, Gfx::DynamicState::Scissor };
    pipelineData.viewports = { canvas.getViewport()};
    pipelineData.scissors = { canvas.getScissor() };

    Gfx::Utility::createBasicGraphicsPipeline(pipelineData,
    deviceFunctions, device, renderPassData.renderPass, 0, Gfx::PrimitiveTopology::TriangleList,
        Gfx::PolygonMode::Fill, Gfx::Flags::CullMode::Bits::Back, Gfx::FrontFace::CounterClockwise,
        Gfx::CompareOp::Always, true, true);

    Gfx::CommandPoolCreateInfo commandPoolInfo;
    commandPoolInfo.setQueueFamilyIndex(graphicsIndex)
        .setFlags(Gfx::Flags::CommandPoolCreate::Bits::ResetCommandBuffer);
    Gfx::CommandPool graphicsCommandPool, temporaryBufferPool;
    graphicsCommandPool.create(deviceFunctions, device, commandPoolInfo);
    temporaryBufferPool.create(deviceFunctions, device, commandPoolInfo);

    std::vector<Gfx::DescriptorPoolSize> poolSizes = {
        Gfx::DescriptorPoolSize(1024, Gfx::DescriptorType::CombinedImageSampler),
        Gfx::DescriptorPoolSize(1, Gfx::DescriptorType::UniformBuffer)
    };

    Gfx::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.setPoolSizes(poolSizes).setMaxSets(2);
    Gfx::DescriptorPool descriptorPool;
    descriptorPool.create(deviceFunctions, device, descriptorPoolCreateInfo);

    Gfx::DescriptorSetAllocateInfo allocInfo;
    allocInfo.setDescriptorPool(descriptorPool)
        .setDescriptorSetLayouts(descriptorLayouts);

    std::vector<Gfx::DescriptorSet> descriptorSets =
        descriptorPool.allocateSets(deviceFunctions, device, allocInfo);

    Gfx::CommandBuffer commandBuffer = graphicsCommandPool.allocateCommandBuffer(deviceFunctions, device);

    Gfx::Buffer uniformBuffer;
    uniformBuffer.create(deviceFunctions, device,
        Gfx::BufferCreateInfo().setSize(sizeof(glm::mat4) * 2)
        .setSharingMode(Gfx::SharingMode::Exclusive)
        .setUsage(Gfx::Flags::BufferUsage::Bits::Uniform));
    auto uniformMemoryRequirements = uniformBuffer.getMemoryRequirements(deviceFunctions, device);

    Gfx::Memory uniformMemory;
    uniformMemory.create(deviceFunctions, device,
        Gfx::MemoryAllocateInfo().setAllocationSize(uniformMemoryRequirements.getSize()).setMemoryTypeIndex(
            Gfx::Utility::findMemoryType(instanceFunctions, physicalDevice, uniformMemoryRequirements.getMemoryTypeBits(),
            Gfx::Flags::MemoryProperty::Bits::HostVisibleCoherent)));
    uniformMemory.bindBuffer(deviceFunctions, device, uniformBuffer, 0);

    auto uniformMapping = uniformMemory.map(deviceFunctions, device, 0,
        uniformMemoryRequirements.getSize(), Gfx::Flags::MemoryMap::Bits::None);

    Gfx::Memory indexMemory;
    Gfx::Buffer indexBuffer;

    std::vector<Gfx::Buffer> vertexBuffers(2);
    std::vector<Gfx::Memory> vertexMemories(2);

    vertexBuffers[0].create(deviceFunctions, device, Gfx::BufferCreateInfo()
        .setSize(sizeof(glm::vec4) * 4)
        .setSharingMode(Gfx::SharingMode::Exclusive)
        .setUsage(Gfx::Flags::BufferUsage::Bits::Vertex));

    vertexBuffers[1].create(deviceFunctions, device, Gfx::BufferCreateInfo()
        .setSize(sizeof(uint32_t) * 4)
        .setSharingMode(Gfx::SharingMode::Exclusive)
        .setUsage(Gfx::Flags::BufferUsage::Bits::Vertex));

    indexBuffer.create(deviceFunctions, device, Gfx::BufferCreateInfo()
        .setSize(sizeof(uint32_t) * 6)
        .setSharingMode(Gfx::SharingMode::Exclusive)
        .setUsage(Gfx::Flags::BufferUsage::Bits::Index));

    auto vertexMemoryRequirements = vertexBuffers[0].getMemoryRequirements(deviceFunctions, device);
    auto textureIdMemoryRequirements = vertexBuffers[1].getMemoryRequirements(deviceFunctions, device);
    auto indexMemoryRequirements = indexBuffer.getMemoryRequirements(deviceFunctions, device);

    vertexMemories[0].create(deviceFunctions, device, Gfx::MemoryAllocateInfo()
        .setAllocationSize(vertexMemoryRequirements.getSize())
        .setMemoryTypeIndex(Gfx::Utility::findMemoryType(instanceFunctions, physicalDevice,
            vertexMemoryRequirements.getMemoryTypeBits(),
            Gfx::Flags::MemoryProperty::Bits::HostVisibleCoherent)));
    vertexMemories[1].create(deviceFunctions, device, Gfx::MemoryAllocateInfo()
        .setAllocationSize(textureIdMemoryRequirements.getSize())
        .setMemoryTypeIndex(Gfx::Utility::findMemoryType(instanceFunctions, physicalDevice,
            textureIdMemoryRequirements.getMemoryTypeBits(),
            Gfx::Flags::MemoryProperty::Bits::HostVisibleCoherent)));
    indexMemory.create(deviceFunctions, device, Gfx::MemoryAllocateInfo()
        .setAllocationSize(indexMemoryRequirements.getSize())
        .setMemoryTypeIndex(Gfx::Utility::findMemoryType(instanceFunctions, physicalDevice,
            indexMemoryRequirements.getMemoryTypeBits(),
            Gfx::Flags::MemoryProperty::Bits::HostVisibleCoherent)));

    vertexMemories[0].bindBuffer(deviceFunctions, device, vertexBuffers[0], 0);
    vertexMemories[1].bindBuffer(deviceFunctions, device, vertexBuffers[1], 0);
    indexMemory.bindBuffer(deviceFunctions, device, indexBuffer, 0);

    auto vertexMapping = vertexMemories[0].map(deviceFunctions, device, 0,
        vertexMemoryRequirements.getSize(), Gfx::Flags::MemoryMap::Bits::None);
    auto textureIdMapping = vertexMemories[1].map(deviceFunctions, device, 0,
        textureIdMemoryRequirements.getSize(), Gfx::Flags::MemoryMap::Bits::None);
    auto indexMapping = indexMemory.map(deviceFunctions, device, 0,
        indexMemoryRequirements.getSize(), Gfx::Flags::MemoryMap::Bits::None);

    Gfx::Memory stagingMemory;
    Gfx::Buffer stagingBuffer;

    stagingBuffer.create(deviceFunctions, device, Gfx::BufferCreateInfo()
        .setSize(1024 * 1024)
        .setSharingMode(Gfx::SharingMode::Exclusive)
        .setUsage(Gfx::Flags::BufferUsage::Bits::TransferSrc));
    auto stagingMemoryRequirements = stagingBuffer.getMemoryRequirements(deviceFunctions, device);
    stagingMemory.create(deviceFunctions, device, Gfx::MemoryAllocateInfo()
        .setAllocationSize(stagingMemoryRequirements.getSize())
        .setMemoryTypeIndex(Gfx::Utility::findMemoryType(instanceFunctions, physicalDevice,
            stagingMemoryRequirements.getMemoryTypeBits(),
            Gfx::Flags::MemoryProperty::Bits::HostVisibleCoherent)));
            stagingMemory.bindBuffer(deviceFunctions, device, stagingBuffer, 0);

    auto stagingMapping = stagingMemory.map(deviceFunctions, device, 0,
        stagingMemoryRequirements.getSize(), Gfx::Flags::MemoryMap::Bits::None);

    Gfx::Utility::PixelData2D textureData("res/dirt.png");
    Gfx::Memory textureMemory;
    Gfx::Image textureImage;

    textureImage.create(deviceFunctions, device, Gfx::ImageCreateInfo()
        .setImageType(Gfx::ImageType::T2D)
        .setFormat(Gfx::PixelFormat::R32G32B32A32Sfloat)
        .setExtent(textureData.getExtent3D())
        .setUsage(Gfx::Flags::ImageUsage::Bits::TransferDst | Gfx::Flags::ImageUsage::Bits::Sampled)
        .setSharingMode(Gfx::SharingMode::Exclusive)
        .setTiling(Gfx::ImageTiling::Optimal)
        .setInitialLayout(Gfx::ImageLayout::Undefined)
        .setMipLevels(1)
        .setArrayLayers(1));

    auto textureMemoryRequirements = textureImage.getMemoryRequirements(device, deviceFunctions);
    textureMemory.create(deviceFunctions, device, Gfx::MemoryAllocateInfo()
        .setAllocationSize(textureMemoryRequirements.getSize())
        .setMemoryTypeIndex(Gfx::Utility::findMemoryType(instanceFunctions, physicalDevice,
            textureMemoryRequirements.getMemoryTypeBits(),
            Gfx::Flags::MemoryProperty::Bits::DeviceLocal)));
    textureMemory.bindImage(deviceFunctions, device, textureImage, 0);

    {
        auto temporary = temporaryBufferPool.allocateCommandBuffer(deviceFunctions, device);
        auto data = stagingMapping.get<uint8_t>(0, textureData.getCapacity());
        auto pixelData = textureData.getPixelData();
        std::copy(pixelData.begin(), pixelData.end(), data.begin());

        temporary.begin(deviceFunctions, Gfx::CommandBufferBeginInfo()
            .setFlags(Gfx::Flags::CommandBufferUsage::Bits::OneTimeSubmit));
        
        // Transition: Undefined -> TransferDstOptimal
        Gfx::ImageMemoryBarrier barrier1;
        barrier1.setOldLayout(Gfx::ImageLayout::Undefined)
            .setNewLayout(Gfx::ImageLayout::TransferDstOptimal)
            .setSrcAccessMask(Gfx::Flags::Access::Bits::None)
            .setDstAccessMask(Gfx::Flags::Access::Bits::TransferWrite)
            .setImage(textureImage)
            .setSubresourceRange(Gfx::ImageSubresourceRange()
                .setAspectMask(Gfx::Flags::ImageAspect::Bits::Color)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1));

        temporary.pipelineBarrier(deviceFunctions,
            Gfx::Flags::PipelineStage::Bits::TopOfPipe,
            Gfx::Flags::PipelineStage::Bits::Transfer,
            Gfx::Flags::Dependency::Bits::None,
            {}, {}, std::span<Gfx::ImageMemoryBarrier>(&barrier1, 1));

        // Copy buffer to image
        Gfx::BufferImageCopy copyRegion;
        copyRegion.setBufferOffset(0)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource(Gfx::ImageSubresourceLayers()
                .setAspectMask(Gfx::Flags::ImageAspect::Bits::Color)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1))
            .setImageOffset({ 0, 0, 0 })
            .setImageExtent(textureData.getExtent3D());

        temporary.copyBufferToImage(deviceFunctions,
            stagingBuffer, textureImage,
            Gfx::ImageLayout::TransferDstOptimal,
            std::span<Gfx::BufferImageCopy>(&copyRegion, 1));

        // Transition: TransferDstOptimal -> ShaderReadOnlyOptimal
        Gfx::ImageMemoryBarrier barrier2;
        barrier2.setOldLayout(Gfx::ImageLayout::TransferDstOptimal)
            .setNewLayout(Gfx::ImageLayout::ShaderReadOnlyOptimal)
            .setSrcAccessMask(Gfx::Flags::Access::Bits::TransferWrite)
            .setDstAccessMask(Gfx::Flags::Access::Bits::ShaderRead)
            .setImage(textureImage)
            .setSubresourceRange(Gfx::ImageSubresourceRange()
                .setAspectMask(Gfx::Flags::ImageAspect::Bits::Color)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1));

        temporary.pipelineBarrier(deviceFunctions,
            Gfx::Flags::PipelineStage::Bits::Transfer,
            Gfx::Flags::PipelineStage::Bits::FragmentShader,
            Gfx::Flags::Dependency::Bits::None,
            {}, {}, std::span<Gfx::ImageMemoryBarrier>(&barrier2, 1));

        temporary.end(deviceFunctions);

        // Submit and wait
        Gfx::QueueSubmitInfo submitInfo;
        submitInfo.setCommandBuffers(std::span<Gfx::CommandBuffer>(&temporary, 1));
        transferQueue.submit(deviceFunctions, std::span<Gfx::QueueSubmitInfo>(&submitInfo, 1), nullptr);
        transferQueue.waitIdle(deviceFunctions);
        temporaryBufferPool.freeCommandBuffer(deviceFunctions, device, temporary);
    }

    Gfx::ImageView imageView;
    imageView.create(device, deviceFunctions, Gfx::ImageViewCreateInfo()
        .setImage(textureImage)
        .setViewType(Gfx::ImageViewType::T2D)
        .setFormat(Gfx::PixelFormat::R32G32B32A32Sfloat)
        .setComponents(Gfx::ComponentMapping())
        .setSubresourceRange(Gfx::ImageSubresourceRange()
            .setAspectMask(Gfx::Flags::ImageAspect::Bits::Color)
            .setBaseMipLevel(0)
            .setLevelCount(1)
            .setBaseArrayLayer(0)
            .setLayerCount(1)));

    Gfx::Sampler sampler;
    sampler.create(deviceFunctions, device, Gfx::SamplerCreateInfo()
        .setMagFilter(Gfx::Filter::Linear)
        .setMinFilter(Gfx::Filter::Linear)
        .setAddressModeU(Gfx::SamplerAddressMode::Repeat)
        .setAddressModeV(Gfx::SamplerAddressMode::Repeat)
        .setMipmapMode(Gfx::SamplerMipmapMode::Linear));

    Gfx::DescriptorBufferInfo bufferInfo;
    bufferInfo.setBuffer(uniformBuffer)
        .setOffset(0)
        .setRange(sizeof(glm::mat4) * 2);
    Gfx::DescriptorImageInfo imageInfo;
    imageInfo.setImageLayout(Gfx::ImageLayout::ShaderReadOnlyOptimal)
        .setImageView(imageView)
        .setSampler(sampler);

    std::vector<Gfx::DescriptorSetWrite> writeSets(2);
    writeSets[0].setBufferInfo(std::span<Gfx::DescriptorBufferInfo>(&bufferInfo, 1))
        .setDstBinding(0)
        .setDstSet(descriptorSets[0])
        .setDescriptorType(Gfx::DescriptorType::UniformBuffer)
        .setDstArrayElement(0);
    writeSets[1].setImageInfo(std::span<Gfx::DescriptorImageInfo>(&imageInfo, 1))
        .setDstBinding(0)
        .setDstSet(descriptorSets[1])
        .setDescriptorType(Gfx::DescriptorType::CombinedImageSampler)
        .setDstArrayElement(0);

    Gfx::DescriptorSet::update(deviceFunctions, device, writeSets);

    Gfx::CameraPerspective camera({ 0, 1, 0 }, { 1, 1, 1 }, 0, 0);
    Gfx::Semaphore imageAvailableSemaphore, renderFinishedSemaphore;
    imageAvailableSemaphore.create(deviceFunctions, device);
    renderFinishedSemaphore.create(deviceFunctions, device);
    Gfx::Fence inFlightFence;
    inFlightFence.create(deviceFunctions, device, Gfx::FenceCreateInfo());
    std::array<Gfx::ClearValue,2> clearValues;
    clearValues[0].setClearValue(Gfx::ClearColorValue()
        .setClearValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 1.0f })));
    clearValues[1].setClearValue(Gfx::ClearDepthStencilValue()
        .setDepth(1.0f)
        .setStencil(0));

    std::vector<Gfx::DeviceSize> offsets(2, 0);
    Gfx::Flags::PipelineStage waitStage = Gfx::Flags::PipelineStage::Bits::ColorAttachmentOutput;

    struct Uniform {
        glm::mat4 view;
        glm::mat4 projection;
    };

    auto uniform = uniformMapping.get<Uniform>(0, 1)[0];
    uniform.view = camera.getView();
    uniform.projection = camera.getProjection();

    // Add after mapping:
    std::vector<glm::vec4> vertices = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f}
    };
    std::memcpy(vertexMapping.get<void>(0), vertices.data(), sizeof(glm::vec4) * 4);

    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    std::memcpy(indexMapping.get<void>(0), indices.data(), sizeof(uint32_t) * 6);

    // Also fill texture IDs
    std::vector<uint32_t> texIds = { 0, 0, 0, 0 };
    std::memcpy(textureIdMapping.get<void>(0), texIds.data(), sizeof(uint32_t) * 4);

    // Render loop
    while (!window.shouldClose()) {
        window.pollEvents();

        inFlightFence.reset(deviceFunctions, device);
        
        uint32_t imageIndex;
        auto result = swapChainData.swapChain.acquireNextImage(deviceFunctions, device,
            imageAvailableSemaphore, nullptr, imageIndex);
        if (result != Gfx::Result::Success)
            throw std::runtime_error("Failed to acquire next image");

        commandBuffer.reset(deviceFunctions);
        commandBuffer.begin(deviceFunctions, Gfx::CommandBufferBeginInfo());
        
        Gfx::RenderPassBeginInfo renderPassBegin;
        renderPassBegin.setRenderPass(renderPassData.renderPass)
            .setFrameBuffer(swapChainData.swapChainFrameBuffers[imageIndex])
            .setRenderArea({ { 0, 0 }, swapChainData.swapChainInfo.getImageExtent() })
            .setClearValues(clearValues);

        commandBuffer.beginRenderPass(deviceFunctions, renderPassBegin, Gfx::SubpassContents::Inline);
        commandBuffer.bindPipeline(deviceFunctions,
            pipelineData.graphicsPipeline, Gfx::PipelineBindPoint::Graphics);
        commandBuffer.bindVertexBuffers(deviceFunctions, 0,
            vertexBuffers, offsets);
        commandBuffer.bindIndexBuffer(deviceFunctions, indexBuffer, 0, Gfx::IndexType::Uint32);
        commandBuffer.bindDescriptorSets(deviceFunctions, Gfx::PipelineBindPoint::Graphics, 
            pipelineData.pipelineLayout, 0, descriptorSets, {});
        commandBuffer.setViewports(deviceFunctions, 0, pipelineData.viewports);
        commandBuffer.setScissors(deviceFunctions, 0, pipelineData.scissors);
        commandBuffer.drawIndexed(deviceFunctions, 6, 1, 0, 0, 0);
        commandBuffer.endRenderPass(deviceFunctions);
        commandBuffer.end(deviceFunctions);

        Gfx::QueueSubmitInfo submitInfo;
        submitInfo.setWaitStages(std::span(&waitStage, 1), std::span(&imageAvailableSemaphore, 1))
            .setCommandBuffers(std::span(&commandBuffer, 1))
            .setSignalSemaphores(std::span(&renderFinishedSemaphore, 1));

        graphicsQueue.submit(deviceFunctions, std::span(&submitInfo, 1), inFlightFence);

        Gfx::QueuePresentInfo presentInfo;
        presentInfo.setWaitSemaphores(std::span(&renderFinishedSemaphore, 1))
            .setSwapChains(std::span(&swapChainData.swapChain, 1),
                std::span(&imageIndex, 1));

        result = presentQueue.present(deviceFunctions, presentInfo);
        if (result != Gfx::Result::Success)
            throw std::runtime_error("Failed to present queue");
        
        Gfx::Fence::wait(deviceFunctions, device, std::span(&inFlightFence, 1), std::numeric_limits<uint64_t>::max());
    }

    device.waitIdle();
    graphicsCommandPool.reset(deviceFunctions, device);

    inFlightFence.destroy(deviceFunctions, device);
    imageAvailableSemaphore.destroy(deviceFunctions, device);
    renderFinishedSemaphore.destroy(deviceFunctions, device);
    imageView.destroy(device, deviceFunctions);
    stagingMapping.unmap(deviceFunctions, device, stagingMemory);
    stagingBuffer.destroy(deviceFunctions, device);
    stagingMemory.destroy(deviceFunctions, device);
    sampler.destroy(deviceFunctions, device);
    textureImage.destroy(deviceFunctions, device);
    textureMemory.destroy(deviceFunctions, device);
    indexMapping.unmap(deviceFunctions, device, indexMemory);
    indexBuffer.destroy(deviceFunctions, device);
    indexMemory.destroy(deviceFunctions, device);

    vertexMapping.unmap(deviceFunctions, device, vertexMemories[0]);
    textureIdMapping.unmap(deviceFunctions, device, vertexMemories[1]);

    for(size_t i = 0; i < vertexBuffers.size(); i++)
    {
        vertexMemories[i].destroy(deviceFunctions, device);
        vertexBuffers[i].destroy(deviceFunctions, device);
    }

    uniformMapping.unmap(deviceFunctions, device, uniformMemory);
    uniformBuffer.destroy(deviceFunctions, device);
    uniformMemory.destroy(deviceFunctions, device);
    descriptorPool.reset(deviceFunctions, device);
    descriptorPool.destroy(deviceFunctions, device);
    graphicsCommandPool.destroy(deviceFunctions, device);
    temporaryBufferPool.destroy(deviceFunctions, device);
    pipelineData.graphicsPipeline.destroy(deviceFunctions, device);
    pipelineData.pipelineLayout.destroy(deviceFunctions, device);
    Gfx::Utility::destroySwapChainDaTa(deviceFunctions, device, swapChainData);
    renderPassData.renderPass.destroy(deviceFunctions, device);
    for (auto& shader : shaderModuleData.shaderModules)
        shader.destroy(deviceFunctions, device);

    for (auto& descriptorLayout : descriptorLayouts)
        descriptorLayout.destroy(deviceFunctions, device);

    device.destroy();
    surface.destroy(instanceFunctions, instance);
    instance.destroy();
    window.destroy();
    return 0;
}