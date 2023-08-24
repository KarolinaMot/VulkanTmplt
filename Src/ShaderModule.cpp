#include "ShaderModule.h"

ShaderModule::ShaderModule(FilePath spirv_source, shared_ptr<VulkanDevice> device)
{
    associated_device = device;
    ifstream file(spirv_source, ios::ate | ios::binary);

    if (!file.is_open()) {
        throw runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    vector<char> source(fileSize);

    file.seekg(0);
    file.read(source.data(), fileSize);

    file.close();

    VkShaderModuleCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

    CHECK_VK(vkCreateShaderModule(device->handle(), &createInfo, nullptr, &shader));
}

ShaderModule::~ShaderModule()
{
	vkDestroyShaderModule(associated_device->handle(), shader, nullptr);
}
