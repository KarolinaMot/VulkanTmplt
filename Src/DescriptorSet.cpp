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

void DescriptorPool::CreateDescriptorPool()
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

void DescriptorPool::AllocateDescriptor(DescriptorSetLayout* layout, DescriptorSet* set, int count)
{
    VkDescriptorSetLayout setLayout = layout->GetHandle();
    VkDescriptorSet descSet = set->GetHandle();

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &setLayout;
    allocInfo.descriptorSetCount = count;

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

void DescriptorSet::AllocateSet()
{
    VkDescriptorSetLayout lay = layout->GetHandle();
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool->GetHandle();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &lay; 
    
    if (vkAllocateDescriptorSets(device, &allocInfo, &set) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

void DescriptorSet::WriteSet()
{
    vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void DescriptorSet::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
{

    if (layout->GetBindings().size() < 1) {
        throw std::runtime_error("Layout does not contain specified binding");
    }

    if (layout == nullptr)
        throw std::runtime_error("Layout unavailable.");

    auto bindingDescription = layout->GetBinding(binding);

    if (bindingDescription.descriptorCount != 1) {
        throw std::runtime_error("Binding single descriptor info, but binding expects multiple");
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
        throw std::runtime_error("Layout does not contain specified binding");

    auto bindingDescription = layout->GetBinding(binding);

    if (bindingDescription.descriptorCount != 1)
        throw std::runtime_error("Binding single descriptor info, but binding expects multiple");


    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.dstSet = set;
    write.descriptorCount = 1;

    descriptorWrites.push_back(write);
}

void DescriptorSet::Bind(Vulkan* vulkan)
{
    vkCmdBindDescriptorSets(vulkan->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->GetViewportPipelineLayout(), layout->GetIndex(), 1, &set, 0, nullptr);
}
