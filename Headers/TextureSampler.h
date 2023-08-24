#pragma once
#include "VulkanDevice.h"

class TextureSampler
{
public:

	TextureSampler(shared_ptr<VulkanDevice> device);
	~TextureSampler();

	VkSampler handle() { return sampler; }

private:

	VkSampler sampler;
	shared_ptr<VulkanDevice> associated_device;
};

