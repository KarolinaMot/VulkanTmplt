#include "../Headers/Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

Model::Model(std::string path, Renderer* vulkan)
{
	LoadModel(path, vulkan);

	std::printf("---------------------------------------------------\n");
	std::printf("---------------------------------------------------\n");
	std::printf("\n\n\n");
}

Model::~Model()
{
	for (auto&& mesh : meshes) {
		delete mesh;
	}
	meshes.clear();
}

std::vector<VBO*> Model::GetVBOs()
{
	std::vector<VBO*> vbos;
	for (int i = 0; i < meshes.size(); i++) {
		vbos.push_back(meshes[i]->GetVBO());
	}
	return vbos;
}

void Model::Draw(Renderer* vulkan)
{
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->Draw(vulkan);
	}
}

void Model::LoadModel(std::string path, Renderer* vulkan)
{
	Assimp::Importer importer;
	//we must use triangles, aiProcess_Triangulate tells ASSIMP to split poligons into triangles
	//aiProcess_GenSmoothNormals is for lighting
	//aiProcess_FlipUVs flips texture coordinates along the y axis of the texture
	//aiProcess_JoinIdenticalVertices joins identical vertices, saves some memory
	const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices | 
		aiProcess_CalcTangentSpace);

	std::printf("---------------------------------------------------\n");
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::printf("Error parsing '%s': '%s'\n", path.c_str(), importer.GetErrorString());
	}
	std::printf("Model loaded'%s': '%s'\n", path.c_str(), importer.GetErrorString());
	directory = path.substr(0, path.find_last_of('/'));


	std::printf("Processing model... \n");
	ProcessNode(vulkan, scene->mRootNode, scene, path);
	std::printf("Done processing model! \n");
}

void Model::ProcessNode(Renderer* vulkan, aiNode* node, const aiScene* scene, std::string objPath)
{
	//process all the node's meshes (if any)
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::printf("---------------------------------------------------\n");
		std::printf("Processing mesh %i out of %i... \n", node->mMeshes[i] + 1, scene->mNumMeshes);
		meshes.push_back(ProcessMesh(vulkan, mesh, scene, objPath));

	}

	//this is good for creating parent-children relations, in case I need this in the future
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(vulkan, node->mChildren[i], scene, objPath);
	}
}

Mesh* Model::ProcessMesh(Renderer* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath)
{
	vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);

	//We are assuming that all faces have the same number of vertices
	vector<uint16_t> indices;
	indices.reserve(mesh->mNumFaces * mesh->mFaces->mNumIndices);

	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.pos = Common::assimpToGlm(mesh->mVertices[i]);
		vertex.norm = Common::assimpToGlm(mesh->mNormals[i]);
		vertex.tangent = Common::assimpToGlm(mesh->mTangents[i]);

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord = Common::assimpToGlm(mesh->mTextureCoords[0][i]);
		}
		else
			vertex.texCoord.x = vertex.texCoord.y = 0;

		vertices.push_back(vertex);

	}
	std::printf("Processed %i vertices\n", mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	std::printf("Processed %i indices \n", indices.size());
	std::printf("Done processing mesh! \n");

	//Material material = ProcessMaterial(mesh, scene, objPath);
	return new Mesh(vulkan, vertices, indices, ProcessMaterials(vulkan, mesh, scene, objPath));
}

Material* Model::ProcessMaterials(Renderer* vulkan, aiMesh* mesh, const aiScene* scene, std::string objPath)
{
	Texture* diff = nullptr;
	Texture* spec = nullptr;
	Texture* norm = nullptr;

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString texPath;
		aiString texPath2;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
			std::string texName = texPath.data;
			texName = FixPath(texName);
			std::string fullTexPath = directory + "/Textures/" + texName;

			diff = new Texture(vulkan, fullTexPath, vulkan->GetModelSetLayout()->GetBinding(1));
		}
		if (material->GetTexture(aiTextureType_METALNESS, 0, &texPath) == AI_SUCCESS) {
			std::string texName = texPath.data;
			texName = FixPath(texName);
			std::string fullTexPath = directory + "/Textures/" + texName;

			spec = new Texture(vulkan, fullTexPath, vulkan->GetModelSetLayout()->GetBinding(1));
		}
		if (material->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS) {
			std::string texName = texPath.data;
			texName = FixPath(texName);
			std::string fullTexPath = directory + "/Textures/" + texName;

			norm = new Texture(vulkan, fullTexPath, vulkan->GetModelSetLayout()->GetBinding(1));

		}
	}

	if(diff == nullptr)
		diff = new Texture(vulkan, "Assets/Models/Textures/untitled.png", vulkan->GetModelSetLayout()->GetBinding(1));

	if (spec == nullptr)
		spec = new Texture(vulkan, "Assets/Models/Textures/DefaultSpecular.png", vulkan->GetModelSetLayout()->GetBinding(2));

	if (norm == nullptr)
		norm = new Texture(vulkan, "Assets/Models/Textures/DefaultNormal.png", vulkan->GetModelSetLayout()->GetBinding(3));

	return new Material(diff, spec, norm);
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
