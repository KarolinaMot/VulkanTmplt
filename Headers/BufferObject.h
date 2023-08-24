#pragma once
#include "VulkanDevice.h"

class BufferObject
{
    public:
        BufferObject() = default;

        BufferObject(shared_ptr<VulkanDevice> device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        ~BufferObject();

        VkDeviceMemory GetBufferMemory() { return bufferMemory;  };
        VkBuffer GetBuffer() const { return buffer; };
        
        void CopyFrom(const BufferObject& srcBuffer, VkDeviceSize size, VkCommandBuffer command_buffer);

    private:

        shared_ptr<VulkanDevice> associated_device;

        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
};

