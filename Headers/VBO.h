#pragma once
#include "Vulkan.h"
#include "BufferObject.h"

class VBO
{
public:
	VBO(const Vulkan& vulkan, const std::vector<Vertex>& vertices, const std::vector<uint16_t> &indices);
	~VBO();

	void Draw(VkCommandBuffer commandBuffer);

private:
	void CreateVertexBuffer(const Vulkan& vulkan, const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(const Vulkan& vulkan, const std::vector<uint16_t>& indices);

	BufferObject* vertexBuffer;
	BufferObject* indexBuffer;

	int indicesSize = 0;
	int verticesSize = 0;
};