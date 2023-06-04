#pragma once
#include "Vulkan.h"
class Image
{
public:
	Image(Vulkan* vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) :device(vulkan->GetDevice()) {
		CreateImage(vulkan, width, height, format, tiling, usage, properties);
	};
	~Image();

	inline VkImage& GetImage() { return image; }
	inline VkDeviceMemory& GetImageMemory() { return imageMemory; }

private:
	void CreateImage(Vulkan* vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

	VkDevice& device;
	VkImage image;
	VkDeviceMemory imageMemory;
};

