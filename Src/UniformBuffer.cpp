#include "../Headers/UniformBuffer.h"

UniformBuffer::UniformBuffer(Vulkan* vulkan, int binding, int descriptorCount, VkShaderStageFlags shaderStage)
{
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = descriptorCount;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.pImmutableSamplers = nullptr;
    layoutBinding.stageFlags = shaderStage;

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffer = new BufferObject(vulkan, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkMapMemory(vulkan->GetDevice(), uniformBuffer->GetBufferMemory(), 0, bufferSize, 0, &uniformBuffersMapped);
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::UpdateBuffer(glm::mat4 model, glm::mat4 view, glm::mat4 proj)
{
    UniformBufferObject ubo{};
    ubo.model = model;
    ubo.view = view;
    ubo.proj = proj;
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped, &ubo, sizeof(ubo));
}
