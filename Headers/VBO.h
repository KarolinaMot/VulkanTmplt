#pragma once
#include "Renderer.h"
#include "BufferObject.h"

class VBO
{
public:
	VBO(Renderer* vulkan, const vector<Vertex>& vertices, const vector<uint16_t> &indices);
	~VBO();

	void Draw(VkCommandBuffer commandBuffer);

private:
	void CreateVertexBuffer(Renderer* vulkan, const vector<Vertex>& vertices);
	void CreateIndexBuffer(Renderer* vulkan, const vector<uint16_t>& indices);

	BufferObject* vertexBuffer;
	BufferObject* indexBuffer;

	size_t indicesSize = 0;
	size_t verticesSize = 0;
};