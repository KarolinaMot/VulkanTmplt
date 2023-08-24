#pragma once
#include "DescriptorSetLayout.h"

class PipelineLayout
{
public:

	PipelineLayout(VkPipelineLayout layout_object, shared_ptr<VulkanDevice> device);
	~PipelineLayout();

	VkPipelineLayout handle() { return layout; }

private:

	VkPipelineLayout layout;
	shared_ptr<VulkanDevice> associated_device;
};

class PipelineLayoutBuilder {
public:

	PipelineLayoutBuilder& AddDescriptorSet(shared_ptr<DescriptorSetLayout> set);
	PipelineLayoutBuilder& ClearDescriptorSets();

	shared_ptr<PipelineLayout> Build(shared_ptr<VulkanDevice> device);

private:

	vector<VkDescriptorSetLayout> descriptor_layouts;

};

