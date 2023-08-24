#include "../Headers/VBO.h"

VBO::VBO(Renderer* vulkan, const vector<Vertex>& vertices, const vector<uint16_t>& indices)
{
    indicesSize = indices.size();
    verticesSize = vertices.size();
    CreateVertexBuffer(vulkan, vertices);
    CreateIndexBuffer(vulkan, indices);
}

VBO::~VBO()
{
    delete vertexBuffer;
    delete indexBuffer;
}

void VBO::Draw(VkCommandBuffer commandBuffer)
{
    VkBuffer vertexBuffers[] = { vertexBuffer->GetBuffer()};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indicesSize), 1, 0, 0, 0);
}

void VBO::CreateVertexBuffer(Renderer* vulkan, const vector<Vertex>& vertices)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * verticesSize;
    BufferObject stagingBuffer(vulkan->GetDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(vulkan->GetDevice()->handle(), stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(vulkan->GetDevice()->handle(), stagingBuffer.GetBufferMemory());

    vertexBuffer = new BufferObject(vulkan->GetDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    VkCommandBuffer commands = vulkan->BeginSingleTimeCommands();

    vertexBuffer->CopyFrom(stagingBuffer, bufferSize, commands);

    vulkan->EndSingleTimeCommands(commands);
}

void VBO::CreateIndexBuffer(Renderer* vulkan, const vector<uint16_t>& indices)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indicesSize;
    BufferObject stagingBuffer(vulkan->GetDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(vulkan->GetDevice()->handle(), stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(vulkan->GetDevice()->handle(), stagingBuffer.GetBufferMemory());

    indexBuffer = new BufferObject(vulkan->GetDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    VkCommandBuffer commands = vulkan->BeginSingleTimeCommands();

    indexBuffer->CopyFrom(stagingBuffer, bufferSize, commands);

    vulkan->EndSingleTimeCommands(commands);
}
