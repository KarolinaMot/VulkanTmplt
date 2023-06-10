#pragma once
#include "Vulkan.h"
#include "BufferObject.h"

class UniformBuffer
{
public:
	UniformBuffer(Vulkan* vulkan, int binding, int descriptorCount, VkShaderStageFlags shaderStage, int framesInFlight);
	~UniformBuffer();
	void UpdateBuffer(int currentImage, float width, float height);
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	VkDescriptorBufferInfo GetBufferInfo(int index);
	inline BufferObject* GetBuffer(int frame) { return uniformBuffers[frame]; }
private:
	VkDescriptorSetLayoutBinding layoutBinding;
	std::vector<BufferObject*> uniformBuffers;
	std::vector<void*> uniformBuffersMapped;
	VkDescriptorBufferInfo bufferInfo;
	int frames = 0;

};

