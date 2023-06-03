#pragma once
#include "Vulkan.h"
#include "BufferObject.h"

class VBO
{
public:
	VBO(Vulkan* vulkan, const std::vector<Vertex>& vertices, const std::vector<uint16_t> &indices);
	~VBO();

	void Draw(VkCommandBuffer commandBuffer);

private:
	void CreateVertexBuffer(Vulkan* vulkan, const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(Vulkan* vulkan, const std::vector<uint16_t>& indices);

	BufferObject* vertexBuffer;
	BufferObject* indexBuffer;

	size_t indicesSize = 0;
	size_t verticesSize = 0;
};