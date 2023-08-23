#pragma once
#include "Vulkan.h"
#include "VBO.h"

struct Material
{
	Texture* diffuse = nullptr;
	Texture* specular = nullptr;
	Texture* normal = nullptr;
};

class Mesh
{
public:
	Mesh(Vulkan* vulkan, std::vector<Vertex> _vertices, std::vector<uint16_t> _indices, Material mat);
	~Mesh();

	void Draw(Vulkan* vulkan);
	VBO* GetVBO() {return vbo;}
	Material GetMaterial() { return mat; };


private:
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	Material mat;
	VBO* vbo;
};

