#include "../Headers/Mesh.h"

Mesh::Mesh(Vulkan* vulkan, std::vector<Vertex> _vertices, std::vector<uint16_t> _indices, Material _mat)
{
	vertices = _vertices;
	indices = _indices;
	vbo = new VBO(vulkan, vertices, indices);
	mat = _mat;
}

Mesh::~Mesh()
{
	delete vbo;

}

void Mesh::Draw(Vulkan* vulkan)
{
	vbo->Draw(vulkan->GetCommandBuffer());
}
