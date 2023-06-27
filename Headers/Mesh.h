#pragma once
#include "Vulkan.h"
#include "VBO.h"


class Mesh
{
public:
	Mesh(Vulkan* vulkan, std::vector<Vertex> _vertices, std::vector<uint16_t> _indices);
	~Mesh();

	void Draw(Vulkan* vulkan);
	VBO* GetVBO() {return vbo;}

private:
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	VBO* vbo;
};

