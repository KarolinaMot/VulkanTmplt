#pragma once
#include "Vulkan.h"

class BufferObject
{
    public:
        BufferObject() = default;
        BufferObject(const Vulkan& vulkan, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : device(vulkan.GetDevice()) {
            CreateBuffer(vulkan, size, usage, properties);
        };
        ~BufferObject();

        VkDeviceMemory GetBufferMemory() { return bufferMemory;  };
        const VkBuffer& GetBuffer() const { return buffer; };
        void CopyBuffer(const Vulkan& vulkan, const BufferObject& srcBuffer, VkDeviceSize size);


    private:
        void CreateBuffer(const Vulkan& vulkan, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

        const VkDevice& device;
        VkBuffer buffer;
        VkDeviceMemory bufferMemory = 0;
};

