#pragma once
#include "VulkanDevice.h"

// Note from Santiago:
// When doing Builder Patterns, it might be good to separate the builder and effective class into two different ones
// This makes it so that you will NEVER utilize an unbuilt renderpass
// Also separates initialization functionality from runtime use

class RenderPass {
public:

	RenderPass(VkRenderPass pass_object, shared_ptr<VulkanDevice> device);
	~RenderPass();

	VkRenderPass handle() { return pass; }

	//These functions are unused
	void Start() {};
	void End() {};

private:

	shared_ptr<VulkanDevice> associated_device;
	VkRenderPass pass = nullptr;

};

class RenderPassBuilder
{
public:

	RenderPassBuilder() {};
	
	RenderPassBuilder& DefineColorAttachment(VkAttachmentDescription desc, uint id);
	RenderPassBuilder& DefineDepthAttachment(VkAttachmentDescription desc, uint id);
	RenderPassBuilder& DefineColorResolveAttachment(VkAttachmentDescription desc, uint id);
	RenderPassBuilder& DefineSubpass(uint colorRefId, uint depthRefId, uint colorResolveId);
	RenderPassBuilder& DefineDependencies(VkSubpassDependency dependency);

	//Build returns a shared pointer but it is also
	//completely fine to return a RenderPass instance and wrap it in a shared_ptr

	shared_ptr<RenderPass> Build(shared_ptr<VulkanDevice> device);

private:

	vector<VkAttachmentDescription> attachments;
	vector<VkSubpassDescription> subPasses;
	vector<VkAttachmentReference> refs;
	vector<VkSubpassDependency> dependencies;

};
