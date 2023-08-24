#pragma once
#include "VulkanDevice.h"


class DescriptorSetLayout
{
public:

    DescriptorSetLayout(shared_ptr<VulkanDevice> device, uint id);
    ~DescriptorSetLayout();

    VkDescriptorSetLayout handle() { return descriptorSetLayout; }
    inline vector<VkDescriptorSetLayoutBinding>& GetBindings() { return bindings; }
    inline VkDescriptorSetLayoutBinding GetBinding(int index) { return bindings[index]; }
    inline uint GetIndex() { return index; }

    void AddBindings(uint binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint count);
    void AddBindings(VkDescriptorSetLayoutBinding bind) { bindings.push_back(bind); };

    void CreateDescriptorSetLayout();
private:

    uint index = 0;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    vector<VkDescriptorSetLayoutBinding> bindings;
    shared_ptr<VulkanDevice> owning_device;
};

