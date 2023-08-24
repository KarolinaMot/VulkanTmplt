#pragma once

#include "VulkanDevice.h"
#include "GLFW_Window.h"

class Swapchain
{
private:

	VkSwapchainKHR swapchain = nullptr;

	VkSurfaceFormatKHR surface_format;
	VkExtent2D extent;

	vector<VkImage> swapchain_images;
	vector<VkImageView> swapchain_views;

	shared_ptr<VulkanDevice> owning_device;

public:

	Swapchain(shared_ptr<VulkanDevice> device, shared_ptr<Surface> surface, shared_ptr<GLFW_Window> window);
	~Swapchain();

	VkSwapchainKHR handle() { return swapchain; }
	VkImageView get_view(size_t index) { return swapchain_views[index]; }

	VkSurfaceFormatKHR get_format() const { return surface_format; }
	VkExtent2D get_extent() const { return extent; }
	size_t get_image_count() const { return swapchain_images.size(); }

private:

	VkSurfaceFormatKHR select_format(const SwapchainSupportInfo& support_info);
	VkExtent2D select_extent(const SwapchainSupportInfo& support_info, GLFWwindow* window_handle);
};
