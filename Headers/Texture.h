#pragma once
#include "Image.h"
#include "BufferObject.h"

class Texture
{
public:
	Texture(Vulkan* vulkan, std::string path, int binding);
	~Texture();

private:
	Image* image;
	VkDescriptorSetLayoutBinding layoutBinding;

};

