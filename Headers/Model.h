#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "Vulkan.h"
#include "Mesh.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "DescriptorSet.h"
#include "Common.h"

struct Transform {
	vec3 pos;
	quat rotation;
	vec3 scale;
};

class Model
{
public:
	Model(std::string path, Vulkan* vulkan, DescriptorPool* pool);
	~Model();

	std::vector<VBO*> GetVBOs();
	std::string GetDirectory() { return directory; }
	Texture* GetDiffuseTex() { return diffuseTex; };


	void Draw(Vulkan* vulkan);
	void UpdateModelMatrix(const Transform& transform, uint frame);

private:
	void LoadModel(std::string path, Vulkan* vulkan);
	void ProcessNode(Vulkan* vulkan, aiNode* node, const aiScene* scene, std::string objPath);
	Mesh* ProcessMesh(Vulkan* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath);
	void ProcessMaterials(Vulkan* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath);
	std::string FixPath(std::string path);

	std::vector<Mesh*> meshes;
	std::string directory;
	Texture* diffuseTex = nullptr;
	DescriptorSet** set;
	UniformBuffer* uniform;




};

