#pragma once
#include "VulkanDevice.h"

class ShaderModule
{
private:

    VkShaderModule shader;
    shared_ptr<VulkanDevice> owning_device;

public:

    ShaderModule(FilePath spirv_source, shared_ptr<VulkanDevice> device);
    ~ShaderModule();

    VkShaderModule handle() { return shader; }
};
