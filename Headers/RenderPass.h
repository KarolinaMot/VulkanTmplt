#pragma once
#include "Vulkan.h"

class RenderPass
{
public:
	RenderPass(Vulkan* vulkan);
	void Start();
	void End();
	void Build();

	VkRenderPass& GetHandle() { return pass; }
	void DefineColorAttachment(VkAttachmentDescription desc, uint id);
	void DefineDepthAttachment(VkAttachmentDescription desc, uint id);
	void DefineColorResolveAttachment(VkAttachmentDescription desc, uint id);
	void DefineSubpass(uint colorRefId, uint depthRefId, uint colorResolveId);
	void DefineDependencies(VkSubpassDependency dependency);
private:
	Vulkan* vulkanInstance;
	VkRenderPass pass;
	std::vector<VkAttachmentDescription> attachments;
	std::vector<VkSubpassDescription> subPasses;
	std::vector<VkAttachmentReference> refs;
	std::vector<VkSubpassDependency> dependencies;


};
