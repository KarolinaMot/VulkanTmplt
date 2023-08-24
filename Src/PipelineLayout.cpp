#include "PipelineLayout.h"

PipelineLayoutBuilder& PipelineLayoutBuilder::AddDescriptorSet(shared_ptr<DescriptorSetLayout> set)
{
    descriptor_layouts.push_back(set->handle());
    return *this;
}

PipelineLayoutBuilder& PipelineLayoutBuilder::ClearDescriptorSets()
{
    descriptor_layouts.clear();
    return *this;
}

shared_ptr<PipelineLayout> PipelineLayoutBuilder::Build(shared_ptr<VulkanDevice> device)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptor_layouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptor_layouts.data();

    VkPipelineLayout pipeline_layout;
    CHECK_VK(vkCreatePipelineLayout(device->handle(), &pipelineLayoutInfo, nullptr, &pipeline_layout));

    return make_shared<PipelineLayout>(pipeline_layout, device);
}

PipelineLayout::PipelineLayout(VkPipelineLayout layout_object, shared_ptr<VulkanDevice> device)
{
    layout = layout_object;
    owning_device = device;
}

PipelineLayout::~PipelineLayout()
{
    //Cleanup
    vkDestroyPipelineLayout(owning_device->handle(), layout, nullptr);
}
