#pragma once
#include "VulkanDevice.h"

class DescriptorPool {
public:

    DescriptorPool(VkDescriptorPool pool_object, shared_ptr<VulkanDevice> device);
    ~DescriptorPool();

    VkDescriptorPool handle() { return descriptorPool; }

private:
    
    VkDescriptorPool descriptorPool = nullptr;
    shared_ptr<VulkanDevice> owning_device;
};

class DescriptorPoolBuilder {

public:

    DescriptorPoolBuilder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
    DescriptorPoolBuilder& WithMaxSets(uint max_sets);

    shared_ptr<DescriptorPool> Build(shared_ptr<VulkanDevice> device);

private:

    uint sets = 0;
    vector<VkDescriptorPoolSize> poolSizes;
};


