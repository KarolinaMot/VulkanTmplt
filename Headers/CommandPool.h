#pragma once
#include "VulkanDevice.h"

class CommandPool
{
public:

	CommandPool(shared_ptr<VulkanDevice> device, uint allocate_buffers);
	~CommandPool();

	VkCommandPool handle() { return command_pool; }
	VkCommandBuffer get_buffer(uint index) { return command_buffers[index]; }

	// Consider creating a wrapper for vk command buffer if automatic cleanup or
	// if specific setup and functionality is necessary

	VkCommandBuffer allocate_one_time_buffer();
	void free_one_time_buffer(VkCommandBuffer to_free);

private:

	VkCommandPool command_pool;
	vector<VkCommandBuffer> command_buffers;

	shared_ptr<VulkanDevice> owning_device;
};

