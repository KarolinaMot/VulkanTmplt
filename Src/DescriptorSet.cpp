#include "DescriptorSet.h"



DescriptorSet::DescriptorSet(VkDescriptorSet descriptor_set, uint layout_index, shared_ptr<VulkanDevice> device, shared_ptr<DescriptorPool> pool)
{
    owning_device = device;
    owning_pool = pool;
    set = descriptor_set;
    index = layout_index;
}

DescriptorSet::~DescriptorSet()
{
    vkFreeDescriptorSets(owning_device->handle(), owning_pool->handle(), 1, &set);
}

void DescriptorSet::Bind(VkCommandBuffer command_buffer, VkPipelineLayout pipelineLayout)
{
    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, index, 1, &set, 0, nullptr);
}

DescriptorSetBuilder& DescriptorSetBuilder::WriteImage(uint binding_index, VkDescriptorImageInfo* image_info)
{
    if (binding_index >= layout->GetBindings().size())
        throw runtime_error("Layout does not contain specified binding");
    
    VkDescriptorSetLayoutBinding binding_description = layout->GetBinding(binding_index);

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;

    write.descriptorType = binding_description.descriptorType;
    write.dstBinding = binding_index;

    write.dstArrayElement = 0;
    write.pImageInfo = image_info;

    //write.dstSet = set; set when building the descriptor set
    write.descriptorCount = binding_description.descriptorCount;

    descriptor_writes.push_back(write);

    return *this;
}

DescriptorSetBuilder& DescriptorSetBuilder::WriteBuffer(uint binding_index, VkDescriptorBufferInfo* buffer_info)
{
    if (binding_index >= layout->GetBindings().size())
        throw runtime_error("Layout does not contain specified binding");

    VkDescriptorSetLayoutBinding binding_description = layout->GetBinding(binding_index);

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;

    write.descriptorType = binding_description.descriptorType;
    write.dstBinding = binding_index;

    write.pBufferInfo = buffer_info;
 
    write.descriptorCount = 1;

    descriptor_writes.push_back(write);

    return *this;
}

unique_ptr<DescriptorSet> DescriptorSetBuilder::Build(shared_ptr<VulkanDevice> device, shared_ptr<DescriptorPool> pool)
{
    VkDescriptorSetAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;

    allocInfo.descriptorPool = pool->handle();

    VkDescriptorSetLayout set_layout = layout->handle();

    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &set_layout;

    //Allocate Set
    VkDescriptorSet descriptor_set;
    CHECK_VK(vkAllocateDescriptorSets(device->handle(), &allocInfo, &descriptor_set));

    for (auto& write : descriptor_writes) {
        write.dstSet = descriptor_set;
    }

    //Update Writes
    vkUpdateDescriptorSets(device->handle(), static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);

    return make_unique<DescriptorSet>(descriptor_set, layout->GetIndex(), device, pool);
}
