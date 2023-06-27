#pragma once
#include "Vulkan.h"
class Image
{
public:
	Image(Vulkan* vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint _mipLevels) :device(vulkan->GetDevice()), mipLevels(_mipLevels), format(format) {
		CreateImage(vulkan, width, height, format, tiling, usage, properties, _mipLevels);
	};
	~Image();

	inline VkImage& GetImage() { return image; }
	inline VkDeviceMemory& GetImageMemory() { return imageMemory; }
	inline uint32_t GetMipLevels() { return mipLevels; };
	inline VkFormat GetFormat() { return format; }

	void CopyBufferToImage(Vulkan* vulkan, VkBuffer buffer, uint32_t width, uint32_t height);
	void TransitionImageLayout(Vulkan* vulkan, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	static VkImageView CreateImageView(Vulkan* vulkan, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint mipLevels);

private:
	void CreateImage(Vulkan* vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint mipLevels);
	bool HasStencilComponent(VkFormat format);

	VkDevice& device;
	VkImage image;
	VkDeviceMemory imageMemory;
	uint32_t mipLevels;
	VkFormat format;

};

