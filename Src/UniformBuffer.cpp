#include "../Headers/UniformBuffer.h"

UniformBuffer::UniformBuffer(shared_ptr<VulkanDevice> device, VkDescriptorSetLayoutBinding binding, int framesInFlight, uint typeSize)
{
    owning_device = device;

    dataTypeSize = typeSize;
    layoutBinding = binding;
    frames = framesInFlight;

    uniformBuffersMapped.resize(framesInFlight);
    for (int i = 0; i < frames; i++) {

        uniformBuffers.push_back(
            new BufferObject(device, dataTypeSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            )
        );

        vkMapMemory(device->handle(), uniformBuffers[i]->GetBufferMemory(), 0, dataTypeSize, 0, &uniformBuffersMapped[i]);
    }   
}

UniformBuffer::~UniformBuffer()
{
    for (auto&& buffer : uniformBuffers) {
        vkUnmapMemory(owning_device->handle(), buffer->GetBufferMemory());
        delete buffer;
    }
}

void UniformBuffer::SetBufferData(int currentImage, void* data, uint dataSize)
{
    memcpy(uniformBuffersMapped[currentImage], data, dataSize);
}

VkDescriptorBufferInfo UniformBuffer::GetBufferInfo(int index)
{
    VkDescriptorBufferInfo bufferInfo {};

    bufferInfo.buffer = uniformBuffers[index]->GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = dataTypeSize;

    return bufferInfo;
}
