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
	inline VkImageView GetImageView() { return textureImageView[0]; }
	inline VkDescriptorSetLayoutBinding GetLayoutBinding() { return layoutBinding; }
	inline std::string GetPath() { return path; }
	std::vector<VkDescriptorImageInfo> GetImageInfos(Vulkan* vulkan);
	void GenerateMipmaps(Vulkan* vulkan, int32_t texWidth, int32_t texHeight, uint arrayLayers, uint layer);

private:
	std::string path;
	VkDevice device;
	Image* image;
	VkImageView textureImageView[6];
	VkDescriptorSetLayoutBinding layoutBinding;
	std::vector<VkDescriptorImageInfo> infos;
};