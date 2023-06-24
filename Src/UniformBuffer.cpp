#include "../Headers/UniformBuffer.h"

UniformBuffer::UniformBuffer(Vulkan* vulkan, int binding, int descriptorCount, VkShaderStageFlags shaderStage, int framesInFlight, uint typeSize)
{
    dataTypeSize = typeSize;
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = descriptorCount;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.pImmutableSamplers = nullptr;
    layoutBinding.stageFlags = shaderStage;
    frames = framesInFlight;

    uniformBuffersMapped.resize(framesInFlight);
    for (int i = 0; i < frames; i++) {
        uniformBuffers.push_back(new BufferObject(vulkan, dataTypeSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
        vkMapMemory(vulkan->GetDevice(), uniformBuffers[i]->GetBufferMemory(), 0, dataTypeSize, 0, &uniformBuffersMapped[i]);
    }   
}

UniformBuffer::~UniformBuffer()
{
    for (int i = 0; i < frames; i++) {
        delete uniformBuffers[i];
        delete uniformBuffersMapped[i];
    }
}

void UniformBuffer::SetBufferData(int currentImage, void* data, uint dataSize)
{
    memcpy(uniformBuffersMapped[currentImage], data, dataSize);
}

VkDescriptorBufferInfo UniformBuffer::GetBufferInfo(int index)
{
    bufferInfo.buffer = uniformBuffers[index]->GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = dataTypeSize;
    return bufferInfo;
}
