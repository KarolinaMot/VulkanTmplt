#pragma once
#include "Image.h"
#include "BufferObject.h"



class Texture
{
public:
	Texture(Vulkan* vulkan, std::string path, const VkDescriptorSetLayoutBinding& binding);
	~Texture();

	inline Image* GetImage() { return image; }
	inline VkImageView GetImageView() { return textureImageView; }
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	inline std::string GetPath() { return path; }
	VkDescriptorImageInfo GetImageInfo(Vulkan* vulkan);
	void GenerateMipmaps(Vulkan* vulkan, int32_t texWidth, int32_t texHeight);

private:
	std::string path;
	VkDevice device;
	Image* image;
	VkDescriptorSetLayoutBinding layoutBinding;
	VkImageView textureImageView;
	VkDescriptorImageInfo imageInfo;
};

class CubemapTexture {
public:
	CubemapTexture(Vulkan* vulkan, std::string _path, const VkDescriptorSetLayoutBinding& binding);
	~CubemapTexture();

	inline Image* GetImage() { return image; }
	inline VkImageView GetImageView() { return textureImageView; }
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	inline std::string GetPath() { return path; }
	VkDescriptorImageInfo GetImageInfo(Vulkan* vulkan) { return info; };
	void GenerateMipmaps(Vulkan* vulkan, int32_t texWidth, int32_t texHeight, uint arrayLayers, uint layer);

private:
	std::string path;
	VkDevice device;
	Image* image;
	VkImageView textureImageView;
	VkDescriptorSetLayoutBinding layoutBinding;
	VkDescriptorImageInfo info;
};