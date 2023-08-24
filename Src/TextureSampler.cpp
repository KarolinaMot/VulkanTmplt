#include "TextureSampler.h"

TextureSampler::TextureSampler(shared_ptr<VulkanDevice> device)
{
    associated_device = device;

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(device->physical(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.f;// Optional
    samplerInfo.maxLod = 16.f;
    samplerInfo.mipLodBias = 0.0f; // Optional

    CHECK_VK(vkCreateSampler(device->handle(), &samplerInfo, nullptr, &sampler));
}

TextureSampler::~TextureSampler()
{
	vkDestroySampler(associated_device->handle(), sampler, nullptr);
}
