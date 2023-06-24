#pragma once
#include "Vulkan.h"
#include "BufferObject.h"

class UniformBuffer
{
public:
	UniformBuffer(Vulkan* vulkan, int binding, int descriptorCount, VkShaderStageFlags shaderStage, int framesInFlight, uint typeSize);
	~UniformBuffer();
	void SetBufferData(int currentImage, void* data, uint dataSize);
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	VkDescriptorBufferInfo GetBufferInfo(int index);
	inline BufferObject* GetBuffer(int frame) { return uniformBuffers[frame]; }
private:
	VkDescriptorSetLayoutBinding layoutBinding;
	std::vector<BufferObject*> uniformBuffers;
	std::vector<void*> uniformBuffersMapped;
	VkDescriptorBufferInfo bufferInfo;
	int frames = 0;
	uint dataTypeSize = 0;

};

