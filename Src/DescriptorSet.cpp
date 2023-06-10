#include "../Headers/DescriptorSet.h"
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

    if (vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    if (descriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        descriptorSetLayout = VK_NULL_HANDLE;
    }
}

void DescriptorPool::CreateDescriptorPool(const std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t maxSets)
{
    VkDescriptorPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.data();
    poolCreateInfo.maxSets = maxSets;

    if (vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void DescriptorPool::AddPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
    poolSizes.push_back({ descriptorType, count });
}

void DescriptorPool::AllocateDescriptor(DescriptorSetLayout* layout, DescriptorSet* set)
{
    VkDescriptorSetLayout setLayout = layout->GetHandle();
    VkDescriptorSet descSet = set->GetHandle();

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &setLayout;
    allocInfo.descriptorSetCount = 1;

    if (vkAllocateDescriptorSets(device, &allocInfo, &descSet) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate descriptor set.");
    }
}

DescriptorPool::~DescriptorPool()
{
    if (descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        descriptorPool = VK_NULL_HANDLE;
    }
}

DescriptorSet::~DescriptorSet()
{
}

void DescriptorSet::BuildSet()
{
    pool->AllocateDescriptor(layout, this);
    OverwriteSet();
}

void DescriptorSet::OverwriteSet()
{
    for (auto& write : descriptorWrites) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

void DescriptorSet::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
{

    if (layout->GetBindings().size() < 1) {
        throw std::runtime_error("Layout does not contain specified binding");
    }

    auto bindingDescription = layout->GetBinding(binding);

    if (bindingDescription.descriptorCount != 1) {
        throw std::runtime_error("Binding single descriptor info, but binding expects multiple");
    }

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    descriptorWrites.push_back(write);
}

void DescriptorSet::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
{
    if (layout->GetBindings().size() < 1)
        throw std::runtime_error("Layout does not contain specified binding");

    auto bindingDescription = layout->GetBinding(binding);

    if (bindingDescription.descriptorCount != 1)
        throw std::runtime_error("Binding single descriptor info, but binding expects multiple");


    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    descriptorWrites.push_back(write);
}