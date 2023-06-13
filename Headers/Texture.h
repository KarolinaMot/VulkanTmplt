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
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	inline std::string GetPath() { return path; }
	VkDescriptorImageInfo GetImageInfo(Vulkan* vulkan);
	void CreateTexture(Vulkan* vulkan);



private:
	std::string path;
	VkDevice device;
	Image* image;
	VkDescriptorSetLayoutBinding layoutBinding;
	VkImageView textureImageView;
	VkDescriptorImageInfo imageInfo;

};

