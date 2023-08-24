#include "../Headers/BufferObject.h"


BufferObject::BufferObject(shared_ptr<VulkanDevice> device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    associated_device = device;

    //Create Buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device->handle(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw runtime_error("failed to create buffer!");
    }

    //Allocate memory for the buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device->handle(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->find_memory_index(memRequirements.memoryTypeBits, properties);

    CHECK_VK(vkAllocateMemory(device->handle(), &allocInfo, nullptr, &bufferMemory));
    vkBindBufferMemory(device->handle(), buffer, bufferMemory, 0);
}

BufferObject::~BufferObject()
{
    // Destroy the buffer (vkDestroyBuffer)
    vkDestroyBuffer(associated_device->handle(), buffer, nullptr);

    // Free the associated memory (vkFreeMemory)
    vkFreeMemory(associated_device->handle(), bufferMemory, nullptr);
}

void BufferObject::CopyFrom(const BufferObject& srcBuffer, VkDeviceSize size, VkCommandBuffer command_buffer)
{
    VkBufferCopy copyRegion{};
    copyRegion.size = size;

    vkCmdCopyBuffer(command_buffer, srcBuffer.GetBuffer(), buffer, 1, &copyRegion);

}
