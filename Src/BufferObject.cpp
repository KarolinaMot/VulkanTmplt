#include "../Headers/BufferObject.h"

void BufferObject::CreateBuffer(Vulkan* vulkan, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    //Create Buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    //Allocate memory for the buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vulkan->FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void BufferObject::CopyBuffer(Vulkan* vulkan, const BufferObject &srcBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = vulkan->BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer.GetBuffer(), buffer, 1, &copyRegion);

    vulkan->EndSingleTimeCommands(commandBuffer);
}

BufferObject::~BufferObject()
{
    // Destroy the buffer (vkDestroyBuffer)
    vkDestroyBuffer(device, buffer, nullptr);

    // Free the associated memory (vkFreeMemory)
    vkFreeMemory(device, bufferMemory, nullptr);
}
