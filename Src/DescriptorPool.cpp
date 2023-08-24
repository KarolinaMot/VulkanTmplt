#include "DescriptorPool.h"

DescriptorPool::DescriptorPool(VkDescriptorPool pool_object, shared_ptr<VulkanDevice> device)
{
    descriptorPool = pool_object;
    associated_device = device;
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(associated_device->handle(), descriptorPool, nullptr);
}

DescriptorPoolBuilder& DescriptorPoolBuilder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
    poolSizes.push_back({ descriptorType, count });
    return *this;
}

DescriptorPoolBuilder& DescriptorPoolBuilder::WithMaxSets(uint max_sets)
{
    sets = max_sets;
    return *this;
}

shared_ptr<DescriptorPool> DescriptorPoolBuilder::Build(shared_ptr<VulkanDevice> device)
{
    VkDescriptorPool new_pool = nullptr;

    VkDescriptorPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.data();
    poolCreateInfo.maxSets = sets;

    CHECK_VK(vkCreateDescriptorPool(device->handle(), &poolCreateInfo, nullptr, &new_pool));

    return make_shared<DescriptorPool>(new_pool, device);
}
