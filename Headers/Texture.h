#pragma once
#include "Image.h"
#include "BufferObject.h"

class Texture
{
public:
	Texture(Vulkan* vulkan, std::string path, int binding);
	~Texture();
	inline Image* GetImage() { return image; }
	inline VkImageView GetImageView() { return textureImageView; }

private:
	VkDevice device;
	Image* image;
	VkDescriptorSetLayoutBinding layoutBinding;
	VkImageView textureImageView;
};

