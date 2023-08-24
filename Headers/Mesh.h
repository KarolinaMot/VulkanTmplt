#pragma once
#include "Renderer.h"
#include "VBO.h"

struct Material
{
	Texture* diffuse = nullptr;
	Texture* specular = nullptr;
};

class Mesh
{
public:
	Mesh(Renderer* vulkan, vector<Vertex> _vertices, vector<uint16_t> _indices, Material mat);
	~Mesh();

	void Draw(Renderer* vulkan);
	VBO* GetVBO() {return vbo;}
	Material GetMaterial() { return mat; };


private:
	vector<Vertex> vertices;
	vector<uint16_t> indices;

	Material mat;
	VBO* vbo;
};

