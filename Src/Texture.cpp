#include "../Headers/Texture.h"
#include <stb_image.h>


Texture::Texture(Vulkan* vulkan, std::string path, int binding)
{
    device = vulkan->GetDevice();
    layoutBinding.binding = binding;
    layoutBinding.descriptorCount = 1;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBinding.pImmutableSamplers = nullptr;
    layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("Images/popCat.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    BufferObject stagingBuffer(vulkan, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(vulkan->GetDevice(), stagingBuffer.GetBufferMemory(), 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(vulkan->GetDevice(), stagingBuffer.GetBufferMemory());

    stbi_image_free(pixels);

    image = new Image(vulkan, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    image->TransitionImageLayout(vulkan, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    image->CopyBufferToImage(vulkan, stagingBuffer.GetBuffer(), texWidth, texHeight);
    image->TransitionImageLayout(vulkan, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    textureImageView = Image::CreateImageView(vulkan, image->GetImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

Texture::~Texture()
{
    delete image;
    vkDestroyImageView(device, textureImageView, nullptr);
}
