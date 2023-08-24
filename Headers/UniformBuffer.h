#pragma once
#include "BufferObject.h"

class UniformBuffer
{
public:

	UniformBuffer(shared_ptr<VulkanDevice> device, VkDescriptorSetLayoutBinding binding, int framesInFlight, uint typeSize);
	~UniformBuffer();

	void SetBufferData(int currentImage, void* data, uint dataSize);

	VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	VkDescriptorBufferInfo GetBufferInfo(int index);
	BufferObject* GetBuffer(int frame) { return uniformBuffers[frame]; }

private:

	shared_ptr<VulkanDevice> owning_device;

	VkDescriptorSetLayoutBinding layoutBinding;

	vector<BufferObject*> uniformBuffers;
	vector<void*> uniformBuffersMapped;

	int frames = 0;
	uint dataTypeSize = 0;

};

