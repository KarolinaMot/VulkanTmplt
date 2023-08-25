#pragma once
#include "Renderer.h"
class Image
{
public:
	Image(Renderer* vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint _mipLevels, VkSampleCountFlagBits numSamples, uint arrayLayers, VkImageCreateFlags flag = 0) :device(vulkan->GetDevice()->handle()), mipLevels(_mipLevels), format(format) {
		CreateImage(vulkan, width, height, format, tiling, usage, properties, _mipLevels, numSamples, arrayLayers, flag);
	}
	~Image();

	inline VkImage& GetImage() { return image; }
	inline VkDeviceMemory& GetImageMemory() { return imageMemory; }
	inline uint32_t GetMipLevels() { return mipLevels; };
	inline VkFormat GetFormat() { return format; }

	void CopyBufferToImage(Renderer* vulkan, VkBuffer buffer, uint32_t width, uint32_t height, uint arrayLayers, uint layer);
	void TransitionImageLayout(Renderer* vulkan, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint totalLayers, uint layer);
	static VkImageView CreateImageView(Renderer* vulkan, VkImage image, VkImageViewType type, VkFormat format, VkImageAspectFlags aspectFlags, uint mipLevels, uint layerCount, uint layer);

private:
	void CreateImage(Renderer* vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint _mipLevels, VkSampleCountFlagBits numSamples, uint arrayLayers, VkImageCreateFlags flag);
	bool HasStencilComponent(VkFormat format);

	VkDevice device;
	VkImage image;
	VkDeviceMemory imageMemory;
	uint32_t mipLevels;
	VkFormat format;

};

