#include "Mesh.h"
#include "Texture.h"

Mesh::Mesh(Renderer* vulkan, vector<Vertex> _vertices, vector<uint16_t> _indices, Material* _mat)
{
	vertices = _vertices;
	indices = _indices;
	vbo = new VBO(vulkan, vertices, indices);
	mat = _mat;
}

Mesh::Mesh(Renderer* vulkan, vector<Vertex> _vertices, vector<uint16_t> _indices)
{
	vertices = _vertices;
	indices = _indices;
	vbo = new VBO(vulkan, vertices, indices);
}

Mesh::~Mesh()
{
	delete vbo;
	delete mat;
}

void Mesh::Draw(Renderer* vulkan)
{
	vbo->Draw(vulkan->GetCommandBuffer());
}

Material::Material(Renderer* renderer, std::string diff, std::string spec, std::string norm)
{
	diffuse = new Texture(renderer, diff, renderer->GetModelSetLayout()->GetBinding(1));
	specular = new Texture(renderer, spec, renderer->GetModelSetLayout()->GetBinding(2));
	normal = new Texture(renderer, norm, renderer->GetModelSetLayout()->GetBinding(3));
}

Material::Material(Texture* diff, Texture* spec, Texture* norm)
{
	diffuse = diff;
	specular = spec;
	normal = norm;
}
