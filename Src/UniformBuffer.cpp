#include "../Headers/UniformBuffer.h"

UniformBuffer::UniformBuffer(Vulkan* vulkan, int binding, int descriptorCount, VkShaderStageFlags shaderStage, int framesInFlight)
{
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = descriptorCount;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBinding.pImmutableSamplers = nullptr;
    layoutBinding.stageFlags = shaderStage;
    frames = framesInFlight;

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffersMapped.resize(framesInFlight);
    for (int i = 0; i < frames; i++) {
        uniformBuffers.push_back(new BufferObject(vulkan, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
        vkMapMemory(vulkan->GetDevice(), uniformBuffers[i]->GetBufferMemory(), 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }   
}

UniformBuffer::~UniformBuffer()
{
    for (int i = 0; i < frames; i++) {
        delete uniformBuffers[i];
        delete uniformBuffersMapped[i];
    }
}

void UniformBuffer::UpdateBuffer(int currentImage, float width, float height)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

VkDescriptorBufferInfo UniformBuffer::GetBufferInfo(int index)
{
    bufferInfo.buffer = uniformBuffers[index]->GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);
    return bufferInfo;
}
