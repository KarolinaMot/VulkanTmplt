#include "../Headers/Texture.h"
#include <stb_image.h>


Texture::Texture(Renderer* vulkan, string _path, const VkDescriptorSetLayoutBinding& binding)
{
    device = vulkan->GetDevice()->handle();
    path = _path;
    layoutBinding = binding;

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw runtime_error("failed to load texture image!");
    }

    BufferObject stagingBuffer(vulkan->GetDevice(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(vulkan->GetDevice()->handle(), stagingBuffer.GetBufferMemory(), 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(vulkan->GetDevice()->handle(), stagingBuffer.GetBufferMemory());

    stbi_image_free(pixels);

    uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    image = new Image(vulkan, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mipLevels, VK_SAMPLE_COUNT_1_BIT, 1);

    image->TransitionImageLayout(vulkan, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 0);
    image->CopyBufferToImage(vulkan, stagingBuffer.GetBuffer(), texWidth, texHeight, 1, 0);
    textureImageView = Image::CreateImageView(vulkan, image->GetImage(), VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels,1, 0);

    GenerateMipmaps(vulkan, texWidth, texHeight);
}

Texture::~Texture()
{
    delete image;
    vkDestroyImageView(device, textureImageView, nullptr);
}

VkDescriptorImageInfo Texture::GetImageInfo(Renderer* vulkan)
{
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = textureImageView;
    imageInfo.sampler = vulkan->GetTextureSampler()->handle();

    return imageInfo;
}

void Texture::GenerateMipmaps(Renderer* vulkan, int32_t texWidth, int32_t texHeight)
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties = vulkan->GetFormatProperties(image->GetFormat());

    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = vulkan->BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image->GetImage();
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < image->GetMipLevels(); i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(commandBuffer,
            image->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = image->GetMipLevels() - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    vulkan->EndSingleTimeCommands(commandBuffer);
}

CubemapTexture::CubemapTexture(Renderer* vulkan, string _path, const VkDescriptorSetLayoutBinding& binding)
{
    device = vulkan->GetDevice()->handle();
    path = _path;
    layoutBinding = binding;

    vector<string> facePaths = {
    path + "/right.png",
    path + "/left.png",
    path + "/top.png",
    path + "/bottom.png",
    path + "/front.png",
    path + "/back.png"
    };

    vector<stbi_uc*> facePixels(6);
    int texWidth = 0, texHeight = 0, texChannels = 0;

    for (int i = 0; i < facePaths.size(); i++) {
        stbi_uc* pixels = stbi_load(facePaths[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (!pixels) {
            throw runtime_error("failed to load cubemap face: " + facePaths[i]);
        }

        facePixels[i] = pixels;
        //stbi_image_free(pixels);
    }

    uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    uint32_t arrayLayers = 6;
    image = new Image(vulkan, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mipLevels, VK_SAMPLE_COUNT_1_BIT, arrayLayers, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    textureImageView = Image::CreateImageView(vulkan, image->GetImage(), VK_IMAGE_VIEW_TYPE_CUBE, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, 6, 0);
    BufferObject stagingBuffer(vulkan->GetDevice(), imageSize * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    for (int i = 0; i < 6; i++) {
        void* writeLocation = vulkan->MapMemory(stagingBuffer.GetBufferMemory(), imageSize * i, imageSize);
        memcpy(writeLocation, facePixels[i], imageSize);
        vulkan->UnmapMemory(stagingBuffer.GetBufferMemory());
    }

    image->TransitionImageLayout(vulkan, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, 0);
    image->CopyBufferToImage(vulkan, stagingBuffer.GetBuffer(), texWidth, texHeight, 6, 0);
    //image->TransitionImageLayout(vulkan, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6, 0);
    GenerateMipmaps(vulkan, texWidth, texHeight, 6, 0);


    info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    info.imageView = textureImageView;
    info.sampler = vulkan->GetTextureSampler()->handle();

    for (int i = 0; i < 6; i++) {
        stbi_image_free(facePixels[i]);
    }

}

CubemapTexture::~CubemapTexture()
{
    delete image;
    vkDestroyImageView(device, textureImageView, nullptr);
}

void CubemapTexture::GenerateMipmaps(Renderer* vulkan, int32_t texWidth, int32_t texHeight, uint arrayLayers, uint layer)
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties = vulkan->GetFormatProperties(image->GetFormat());

    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = vulkan->BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image->GetImage();
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = layer;
    barrier.subresourceRange.layerCount = arrayLayers;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < image->GetMipLevels(); i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = layer;
        blit.srcSubresource.layerCount = arrayLayers;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = layer;
        blit.dstSubresource.layerCount = arrayLayers;

        vkCmdBlitImage(commandBuffer,
            image->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = image->GetMipLevels() - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    vulkan->EndSingleTimeCommands(commandBuffer);

}
