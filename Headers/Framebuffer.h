#pragma once
#include "RenderPass.h"

class Framebuffer
{
private:

	VkFramebuffer framebuffer;
	shared_ptr<VulkanDevice> owning_device;

public:

	Framebuffer(
		shared_ptr<VulkanDevice> device,
		shared_ptr<RenderPass> renderpass,
		vector<VkImageView>& attachments,
		VkExtent2D size
	);

	~Framebuffer();

	VkFramebuffer handle() { return framebuffer; }
};

