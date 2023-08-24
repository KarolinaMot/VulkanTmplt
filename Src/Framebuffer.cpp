#include "Framebuffer.h"

Framebuffer::Framebuffer(shared_ptr<VulkanDevice> device, shared_ptr<RenderPass> renderpass, vector<VkImageView>& attachments, VkExtent2D size)
{
    owning_device = device;

    VkFramebufferCreateInfo framebufferInfo{}
    ;
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderpass->handle();

    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();

    framebufferInfo.width = size.width;
    framebufferInfo.height = size.height;
    framebufferInfo.layers = 1;

    CHECK_VK(vkCreateFramebuffer(device->handle(), &framebufferInfo, nullptr, &framebuffer));

}

Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(owning_device->handle(), framebuffer, nullptr);
}
