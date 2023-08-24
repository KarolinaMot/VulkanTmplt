#include "../Headers/RenderPass.h"


RenderPassBuilder& RenderPassBuilder::DefineColorAttachment(VkAttachmentDescription desc, uint id)
{
	VkAttachmentReference colorRef;
	colorRef.attachment = id;
	colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments.push_back(desc);
	refs.push_back(colorRef);

	return *this;
}

RenderPassBuilder& RenderPassBuilder::DefineDepthAttachment(VkAttachmentDescription desc, uint id)
{
	VkAttachmentReference depthRef;
	depthRef.attachment = id;
	depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments.push_back(desc);
	refs.push_back(depthRef);

	return *this;
}

RenderPassBuilder& RenderPassBuilder::DefineColorResolveAttachment(VkAttachmentDescription desc, uint id)
{
	VkAttachmentReference colorResolveRef;
	colorResolveRef.attachment = id;
	colorResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments.push_back(desc);
	refs.push_back(colorResolveRef);

	return *this;
}

RenderPassBuilder& RenderPassBuilder::DefineSubpass(uint colorRefId, uint depthRefId, uint colorResolveId)
{
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &refs[colorRefId];
	subpass.pDepthStencilAttachment = &refs[depthRefId];
	subpass.pResolveAttachments = &refs[colorResolveId];
	subPasses.push_back(subpass);

	return *this;
}

RenderPassBuilder& RenderPassBuilder::DefineDependencies(VkSubpassDependency dependency)
{
	dependencies.push_back(dependency);

	return *this;
}

shared_ptr<RenderPass> RenderPassBuilder::Build(shared_ptr<VulkanDevice> device)
{
	VkRenderPass new_pass = nullptr;

	VkRenderPassCreateInfo renderPassInfo{};

	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = subPasses.size();
	renderPassInfo.pSubpasses = subPasses.data();
	renderPassInfo.dependencyCount = dependencies.size();
	renderPassInfo.pDependencies = dependencies.data();

	CHECK_VK(vkCreateRenderPass(device->handle(), &renderPassInfo, nullptr, &new_pass));

	return make_shared<RenderPass>(new_pass, device);
}

RenderPass::RenderPass(VkRenderPass pass_object, shared_ptr<VulkanDevice> device)
{
	pass = pass_object;
	associated_device = device;
}

RenderPass::~RenderPass()
{
	vkDestroyRenderPass(associated_device->handle(), pass, nullptr);
}
