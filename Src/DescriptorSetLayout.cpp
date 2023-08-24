#include "DescriptorSetLayout.h"

void DescriptorSetLayout::AddBindings(uint binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint count)
{
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings.push_back(layoutBinding);
}
void DescriptorSetLayout::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutCreateInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(owning_device->handle(), &layoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw runtime_error("failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::DescriptorSetLayout(shared_ptr<VulkanDevice> device, uint id)
{
    owning_device = device;
    index = id;
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    if (descriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(owning_device->handle(), descriptorSetLayout, nullptr);
        descriptorSetLayout = VK_NULL_HANDLE;
    }
}
