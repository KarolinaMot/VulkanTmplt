#pragma once
#include "Renderer.h"
#include "VBO.h"

struct Material
{
	Material(Renderer* renderer, std::string diff, std::string spec, std::string normal);
	Material(Texture* diff, Texture* spec, Texture* normal);
	~Material() { delete diffuse; delete specular; delete normal; }
	Texture* diffuse = nullptr;
	Texture* specular = nullptr;
	Texture* normal = nullptr;
};

class Mesh
{
public:
	Mesh(Renderer* vulkan, vector<Vertex> _vertices, vector<uint16_t> _indices, Material* mat);
	Mesh(Renderer* vulkan, vector<Vertex> _vertices, vector<uint16_t> _indices);
	~Mesh();

	void Draw(Renderer* vulkan);
	VBO* GetVBO() {return vbo;}
	Material* GetMaterial() { return mat; };


private:
	vector<Vertex> vertices;
	vector<uint16_t> indices;

	Material* mat;
	VBO* vbo;
};

