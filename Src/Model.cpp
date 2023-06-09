#include "../Headers/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

Model::Model(std::string path, Vulkan* vulkan)
{
	LoadModel(path, vulkan);

	printf("---------------------------------------------------\n");
	printf("---------------------------------------------------\n");
	printf("\n\n\n");
}

Model::~Model()
{
	delete diffuseTex;
}

std::vector<VBO*> Model::GetVBOs()
{
	std::vector<VBO*> vbos;
	for (int i = 0; i < meshes.size(); i++) {
		vbos.push_back(meshes[i]->GetVBO());
	}
	return vbos;
}

void Model::Draw(Vulkan* vulkan)
{
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->Draw(vulkan);
	}
}

void Model::LoadModel(std::string path, Vulkan* vulkan)
{
	Assimp::Importer importer;
	//we must use triangles, aiProcess_Triangulate tells ASSIMP to split poligons into triangles
	//aiProcess_GenSmoothNormals is for lighting
	//aiProcess_FlipUVs flips texture coordinates along the y axis of the texture
	//aiProcess_JoinIdenticalVertices joins identical vertices, saves some memory
	const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	printf("---------------------------------------------------\n");
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("Error parsing '%s': '%s'\n", path.c_str(), importer.GetErrorString());
	}
	printf("Model loaded'%s': '%s'\n", path.c_str(), importer.GetErrorString());
	directory = path.substr(0, path.find_last_of('/'));


	printf("Processing model... \n");
	ProcessNode(vulkan, scene->mRootNode, scene, path);
	printf("Done processing model! \n");
}

void Model::ProcessNode(Vulkan* vulkan, aiNode* node, const aiScene* scene, std::string objPath)
{
	//process all the node's meshes (if any)
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		printf("---------------------------------------------------\n");
		printf("Processing mesh %i out of %i... \n", node->mMeshes[i], scene->mNumMeshes);
		meshes.push_back(ProcessMesh(vulkan, mesh, scene, objPath));

	}

	//this is good for creating parent-children relations, in case I need this in the future
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(vulkan, node->mChildren[i], scene, objPath);
	}
}

Mesh* Model::ProcessMesh(Vulkan* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath)
{
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.pos = Common::assimpToGlm(mesh->mVertices[i]);
		vertex.norm = Common::assimpToGlm(mesh->mNormals[i]);
		//vertex.norm.y = Common::assimpToGlm(mesh->mNormals[i]).z;
		//vertex.norm.z = Common::assimpToGlm(mesh->mNormals[i]).y;

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord = Common::assimpToGlm(mesh->mTextureCoords[0][i]);
		}
		else
			vertex.texCoord.x = vertex.texCoord.y = 0;

		vertices.push_back(vertex);

	}
	printf("Processed %i vertices\n", mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	printf("Processed %i indices \n", indices.size());
	printf("Done processing mesh! \n");

	//Material material = ProcessMaterial(mesh, scene, objPath);
	ProcessMaterials(vulkan, mesh, scene, objPath);
	return new Mesh(vulkan, vertices, indices);
}

void Model::ProcessMaterials(Vulkan* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath)
{
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString texPath;
		aiString texPath2;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
			std::string texName = texPath.data;
			texName = FixPath(texName);
			std::string fullTexPath = directory + "/Textures/" + texName;

			diffuseTex = new Texture(vulkan, fullTexPath, vulkan->GetModelSetLayout()->GetBinding(1));
		}
	}
}

std::string Model::FixPath(std::string path)
{
	char ch = '\\';
	std::size_t pos = path.find(ch);

	while (pos != std::string::npos) {
		path = path.substr(pos + 1);
		pos = path.find(ch);
	}

	return path;
}
