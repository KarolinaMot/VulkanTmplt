#pragma once
#include "Vulkan.h"

class BufferObject
{
    public:
        BufferObject(const Vulkan& vulkan, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : device(vulkan.GetDevice()) {
            CreateBuffer(vulkan, size, usage, properties);
        };
        ~BufferObject();

    private:
        void CreateBuffer(const Vulkan& vulkan, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

        const VkDevice& device;
        VkBuffer buffer = 0;
        VkDeviceMemory bufferMemory = 0;
};

