#pragma once
#include "Vulkan.h"
#include "BufferObject.h"

class UniformBuffer
{
public:
	UniformBuffer(Vulkan* vulkan, int binding, int descriptorCount, VkShaderStageFlags shaderStage);
	~UniformBuffer();
	void UpdateBuffer(glm::mat4 model, glm::mat4 view, glm::mat4 proj);
private:
	VkDescriptorSetLayoutBinding layoutBinding;
	BufferObject* uniformBuffer = nullptr;
	void* uniformBuffersMapped;
};

