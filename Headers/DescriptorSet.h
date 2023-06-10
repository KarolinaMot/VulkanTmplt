#pragma once
#include "Vulkan.h"

class DescriptorSetLayout
{
public:
    DescriptorSetLayout(Vulkan* vulkan) : device(vulkan->GetDevice()) {
    }
    ~DescriptorSetLayout();

    inline VkDescriptorSetLayout GetHandle() const {return descriptorSetLayout;}
    inline std::vector<VkDescriptorSetLayoutBinding>& GetBindings() { return bindings; }
    inline VkDescriptorSetLayoutBinding GetBinding(int index) { return bindings[index]; }

    void AddBindings(uint binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint count = 1);
    void AddBindings(VkDescriptorSetLayoutBinding bind) {bindings.push_back(bind);};

    void CreateDescriptorSetLayout();
private:
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    VkDevice device;
};

class DescriptorPool {
public:
    DescriptorPool(Vulkan* vulkan, int maxSets) : device(vulkan->GetDevice()), maxSets(maxSets){};
    ~DescriptorPool();
    inline VkDescriptorPool GetHandle() { return descriptorPool; }

    void CreateDescriptorPool();
    void AddPoolSize(VkDescriptorType descriptorType, uint32_t count);

    void AllocateDescriptor(DescriptorSetLayout* layout, DescriptorSet* set, int count);


private:
    std::vector<VkDescriptorPoolSize> poolSizes;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkDevice device;
    int maxSets;
};

class DescriptorSet {
public:
    DescriptorSet(Vulkan* vulkan, DescriptorPool* descPool, DescriptorSetLayout* layout) : device(vulkan->GetDevice()), pool(descPool), layout(layout) {
    };
    ~DescriptorSet();
    void AllocateSet();
    void WriteSet();
    void WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
    void WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

    inline VkDescriptorSet GetHandle() { return set; }
private:
    VkDevice device = VK_NULL_HANDLE;
    DescriptorPool* pool = nullptr;
    DescriptorSetLayout* layout = nullptr;

    VkDescriptorSet set = VK_NULL_HANDLE;
    std::vector<VkWriteDescriptorSet> descriptorWrites;
};