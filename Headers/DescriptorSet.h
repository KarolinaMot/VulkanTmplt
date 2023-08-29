#pragma once
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"


class DescriptorSet {
public:

    DescriptorSet(VkDescriptorSet descriptor_set, uint layout_index, shared_ptr<VulkanDevice> device, shared_ptr<DescriptorPool> pool);
    ~DescriptorSet();

    VkDescriptorSet handle() { return set; }
    void Bind(VkCommandBuffer command_buffer, VkPipelineLayout pipelineLayout);

private:

    VkDescriptorSet set;
    uint index;

    shared_ptr<VulkanDevice> owning_device;
    shared_ptr<DescriptorPool> owning_pool;

};

class DescriptorSetBuilder 
{
public:

    DescriptorSetBuilder(shared_ptr<DescriptorSetLayout> descriptor_layout) : layout(descriptor_layout) {}

    DescriptorSetBuilder& WriteImage(uint binding_index, VkDescriptorImageInfo* image_info);
    DescriptorSetBuilder& WriteBuffer(uint binding_index, VkDescriptorBufferInfo* buffer_info);

    unique_ptr<DescriptorSet> Build(shared_ptr<VulkanDevice> device, shared_ptr<DescriptorPool> pool);

private:

    shared_ptr<DescriptorSetLayout> layout;
    vector<VkWriteDescriptorSet> descriptor_writes;
};