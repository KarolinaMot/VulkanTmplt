#pragma once

#include "VulkanInstance.h"
#include "Surface.h"

struct QueueFamilyIndices {

	optional<uint32_t> graphicsFamily;
	optional<uint32_t> presentFamily;

	bool is_complete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapchainSupportInfo {

	VkSurfaceCapabilitiesKHR capabilities;

	vector<VkSurfaceFormatKHR> formats;
	vector<VkPresentModeKHR> presentModes;
};

class VulkanDevice
{
private:

	VkDevice device = nullptr;
	VkPhysicalDevice physical_device = nullptr;

	VkSampleCountFlagBits multisampling_flags;

	SwapchainSupportInfo swapchain_support;
	QueueFamilyIndices family_indices;

	VkFormat depth_format;

	VkQueue graphics_queue;
	VkQueue present_queue;

public:

	VulkanDevice(shared_ptr<VulkanInstance> instance, shared_ptr<Surface> surface);
	~VulkanDevice();

	VkDevice handle() { return device; }
	VkPhysicalDevice physical() { return physical_device; }

	void Flush() { vkDeviceWaitIdle(device); }

	VkQueue get_graphics_queue() { return graphics_queue; }
	VkQueue get_present_queue() { return present_queue; }

	SwapchainSupportInfo get_swapchain_support_info() const { return swapchain_support; }
	QueueFamilyIndices get_queue_family_indices() const { return family_indices; }
	VkFormat get_depth_format() const { return depth_format; }
	VkSampleCountFlagBits get_multisampling_flags() const { return multisampling_flags; }

	uint find_memory_index(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:

	//Almost every operation in Vulkan, anything from drawing to uploading textures, requires commands to be submitted to a queue.
	//There are different types of queues that originate from different queue families and each family of queues allows only a
	//subset of commands.
	//This checks which queue families are supported by the device and which one of these supports the commands that we want to use.


	QueueFamilyIndices find_queue_families(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
	SwapchainSupportInfo query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

	VkPhysicalDevice select_physical_device(VkInstance instance, VkSurfaceKHR surface, const vector<const char*>& requested_extensions);
	bool is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const vector<const char*>& requested_extensions);
	bool check_extension_support(VkPhysicalDevice physical_device, const vector<const char*>& requested_extensions);

	VkFormat find_depth_format(VkPhysicalDevice physical_device, const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkSampleCountFlagBits usable_sample_count(VkPhysicalDevice physical_device);

	//Config

	static inline const vector<const char*> device_extensions {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	static inline const float HIGH_PRIORITY = 1.0f;
};



