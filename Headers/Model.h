#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "Vulkan.h"
#include "Mesh.h"
#include "Texture.h"
#include "Common.h"

class Model
{
public:
	Model(std::string path, Vulkan* vulkan) {
		LoadModel(path, vulkan);
		printf("---------------------------------------------------\n");
		printf("---------------------------------------------------\n");
		printf("---------------------------------------------------\n\n");
	}
	std::vector<VBO*> GetVBOs();
	Texture* GetDiffuseTex() { return diffuseTex; };
	std::string GetDirectory() { return directory; }
	void Draw(Vulkan* vulkan);

private:
	void LoadModel(std::string path, Vulkan* vulkan);
	void ProcessNode(Vulkan* vulkan, aiNode* node, const aiScene* scene, std::string objPath);
	Mesh* ProcessMesh(Vulkan* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath);
	void ProcessMaterials(Vulkan* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath);
	std::string FixPath(std::string path);

	std::vector<Mesh*> meshes;
	std::string directory;
	Texture* diffuseTex = nullptr;

};

