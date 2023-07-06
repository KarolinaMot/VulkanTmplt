#pragma once
#include "Transform.h"
#include "Model.h"
#include "UniformBuffer.h"
class Skybox
{
public:
	Skybox(Vulkan* vulkan, DescriptorPool* pool, Model* model, std::string texturePath);
	~Skybox();
	void Update(float deltaTime, float currentFrame);
	void Draw(Vulkan* vulkan);
private:
	Model* cube;
	Transform* transform;
	DescriptorSet** set;
	CubemapTexture* texture;

};

