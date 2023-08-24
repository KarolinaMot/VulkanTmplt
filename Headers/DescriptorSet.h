#pragma once
#include "Renderer.h"


class DescriptorSet {
public:

    DescriptorSet() = default;

    DescriptorSet(Renderer* vulkan, DescriptorPool* descPool, shared_ptr<DescriptorSetLayout> layout) : device(vulkan->GetDevice()->handle()), pool(descPool), layout(layout.get()) {
    };

    ~DescriptorSet();
    void AllocateSet();
    void WriteSet();
    void WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
    void WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

    void Bind(Renderer* vulkan, VkPipelineLayout pipelineLayout);

    inline VkDescriptorSet handle() { return set; }

private:

    VkDevice device = VK_NULL_HANDLE;

    DescriptorPool* pool = nullptr;
    DescriptorSetLayout* layout = nullptr;

    VkDescriptorSet set = VK_NULL_HANDLE;
    vector<VkWriteDescriptorSet> descriptorWrites;
};