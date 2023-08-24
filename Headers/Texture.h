#pragma once
#include "Image.h"
#include "BufferObject.h"



class Texture
{
public:
	Texture(Renderer* vulkan, string path, const VkDescriptorSetLayoutBinding& binding);
	~Texture();

	inline Image* GetImage() { return image; }
	inline VkImageView GetImageView() { return textureImageView; }
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	inline string GetPath() { return path; }
	VkDescriptorImageInfo GetImageInfo(Renderer* vulkan);
	void GenerateMipmaps(Renderer* vulkan, int32_t texWidth, int32_t texHeight);

private:
	string path;
	VkDevice device;
	Image* image;
	VkDescriptorSetLayoutBinding layoutBinding;
	VkImageView textureImageView;
	VkDescriptorImageInfo imageInfo;
};

class CubemapTexture {
public:
	CubemapTexture(Renderer* vulkan, string _path, const VkDescriptorSetLayoutBinding& binding);
	~CubemapTexture();

	inline Image* GetImage() { return image; }
	inline VkImageView GetImageView() { return textureImageView; }
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	inline string GetPath() { return path; }
	VkDescriptorImageInfo GetImageInfo(Renderer* vulkan) { return info; };
	void GenerateMipmaps(Renderer* vulkan, int32_t texWidth, int32_t texHeight, uint arrayLayers, uint layer);

private:
	string path;
	VkDevice device;
	Image* image;
	VkImageView textureImageView;
	VkDescriptorSetLayoutBinding layoutBinding;
	VkDescriptorImageInfo info;
};