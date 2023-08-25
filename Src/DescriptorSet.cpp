#include "DescriptorSet.h"



DescriptorSet::~DescriptorSet()
{
}

void DescriptorSet::AllocateSet()
{
    VkDescriptorSetLayout lay = layout->handle();
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool->handle();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &lay; 
    
    if (vkAllocateDescriptorSets(device, &allocInfo, &set) != VK_SUCCESS) {
        throw runtime_error("failed to allocate descriptor sets!");
    }
}

void DescriptorSet::WriteSet()
{
    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void DescriptorSet::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
{
    if (layout->GetBindings().size() < 1) {
        throw runtime_error("Layout does not contain specified binding");
    }

    if (layout == nullptr)
        throw runtime_error("Layout unavailable.");

    auto bindingDescription = layout->GetBinding(binding);

    if (bindingDescription.descriptorCount != 1) {
        throw runtime_error("Binding single descriptor info, but binding expects multiple");
    }

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.dstSet = set;
    write.descriptorCount = 1;

    descriptorWrites.push_back(write);
}

void DescriptorSet::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
{
    if (layout->GetBindings().size() < 1)
        throw runtime_error("Layout does not contain specified binding");

    auto bindingDescription = layout->GetBinding(binding);

    //if (bindingDescription.descriptorCount != 1)
    //    throw runtime_error("Binding single descriptor info, but binding expects multiple");


    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.dstArrayElement = 0; 
    write.pImageInfo = imageInfo;
    write.dstSet = set;
    write.descriptorCount = bindingDescription.descriptorCount;

    descriptorWrites.push_back(write);
}

void DescriptorSet::Bind(Renderer* vulkan, VkPipelineLayout pipelineLayout)
{
    vkCmdBindDescriptorSets(vulkan->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, layout->GetIndex(), 1, &set, 0, nullptr);
}
