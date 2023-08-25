#include "Mesh.h"
#include "Texture.h"

Mesh::Mesh(Renderer* vulkan, vector<Vertex> _vertices, vector<uint16_t> _indices, Material _mat)
{
	vertices = _vertices;
	indices = _indices;
	vbo = new VBO(vulkan, vertices, indices);
	mat = _mat;
}

Mesh::~Mesh()
{
	delete vbo;
	delete mat.diffuse;
	delete mat.specular;
	delete mat.normal;
}

void Mesh::Draw(Renderer* vulkan)
{
	vbo->Draw(vulkan->GetCommandBuffer());
}
