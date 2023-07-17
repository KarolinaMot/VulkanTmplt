#include "../Headers/RenderPass.h"

RenderPass::RenderPass(Vulkan* vulkan)
{
	vulkanInstance = vulkan;
}

void RenderPass::Start()
{
}

void RenderPass::End()
{
}

void RenderPass::DefineColorAttachment(VkAttachmentDescription desc, uint id)
{
	VkAttachmentReference colorRef;
	colorRef.attachment = id;
	colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments.push_back(desc);
	refs.push_back(colorRef);
}

void RenderPass::DefineDepthAttachment(VkAttachmentDescription desc, uint id)
{
	VkAttachmentReference depthRef;
	depthRef.attachment = id;
	depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments.push_back(desc);
	refs.push_back(depthRef);
}

void RenderPass::DefineColorResolveAttachment(VkAttachmentDescription desc, uint id)
{
	VkAttachmentReference colorResolveRef;
	colorResolveRef.attachment = id;
	colorResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments.push_back(desc);
	refs.push_back(colorResolveRef);
}

void RenderPass::DefineSubpass(uint colorRefId, uint depthRefId, uint colorResolveId)
{
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &refs[colorRefId];
	subpass.pDepthStencilAttachment = &refs[depthRefId];
	subpass.pResolveAttachments = &refs[colorResolveId];
	subPasses.push_back(subpass);

}

void RenderPass::DefineDependencies(VkSubpassDependency dependency)
{
	dependencies.push_back(dependency);
}

void RenderPass::Build()
{
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = subPasses.size();
	renderPassInfo.pSubpasses = subPasses.data();
	renderPassInfo.dependencyCount = dependencies.size();
	renderPassInfo.pDependencies = dependencies.data();

	if (vkCreateRenderPass(vulkanInstance->GetDevice(), &renderPassInfo, nullptr, &pass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
}

