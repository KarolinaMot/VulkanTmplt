#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "DescriptorSet.h"
#include "Common.h"

//struct Transform {
//	vec3 pos;
//	quat rotation;
//	vec3 scale;
//};



class Model
{
public:
	Model(string path, Renderer* vulkan);
	~Model();

	vector<VBO*> GetVBOs();
	string GetDirectory() { return directory; }
	vector<Mesh*> GetMeshes() { return meshes; }

	void Draw(Renderer* vulkan);

private:

	void LoadModel(string path, Renderer* vulkan);
	void ProcessNode(Renderer* vulkan, aiNode* node, const aiScene* scene, string objPath);
	Mesh* ProcessMesh(Renderer* vulkan, aiMesh* mesh, const aiScene* scene, string objPath);
	Material ProcessMaterials(Renderer* vulkan, aiMesh* mesh, const aiScene* scene, string objPath);
	string FixPath(string path);

	vector<Mesh*> meshes;
	string directory;
};

