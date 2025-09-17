// Example usage of the centralized allocator

#include "Device.h"
#include "VulkanAllocator.h"

void exampleUsage() {
    // Create device with centralized allocator
    Device device(instance, physicalDevice, requirements, 
                  queueFamilyIndices, queueCounts, queuePriorities, 
                  true); // Enable centralized allocator
    
    auto& allocator = device.getAllocator();
    
    // Create objects through allocator - they're automatically tracked
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = 1024;
    bufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    auto buffer = allocator.createBuffer(bufferInfo);
    
    vk::MemoryAllocateInfo memInfo{};
    memInfo.allocationSize = 1024;
    auto memory = allocator.allocateMemory(memInfo);
    
    vk::ImageCreateInfo imageInfo{};
    imageInfo.imageType = vk::ImageType::e2D;
    imageInfo.extent = {512, 512, 1};
    imageInfo.format = vk::Format::eR8G8B8A8Unorm;
    auto image = allocator.createImage(imageInfo);
    
    // When device.destroy() is called, all objects are destroyed in correct order:
    // 1. Images, buffers (child objects)
    // 2. Memory allocations  
    // 3. Command pools, pipelines, etc.
    // 4. Finally the device itself
    device.destroy(instance);
}

void manualUsage() {
    // Create device without centralized allocator
    Device device(instance, physicalDevice, requirements,
                  queueFamilyIndices, queueCounts, queuePriorities,
                  false); // Disable centralized allocator
    
    auto& allocator = device.getAllocator(); // Returns NullVulkanAllocator
    
    // Objects created normally, no automatic tracking
    auto buffer = allocator.createBuffer(bufferInfo);
    
    // Manual cleanup required in correct order
    device.getDevice().destroyBuffer(buffer);
    device.destroy(instance);
}