#pragma once
#include "Transform.h"
#include "Model.h"
#include "UniformBuffer.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(Vulkan* vulkan, Camera* camera, DescriptorPool* pool, Model* model, std::string texturePath);
	~Skybox();
	void Update(float deltaTime, float currentFrame);
	void Draw(Vulkan* vulkan);
private:
	Model* cube;
	DescriptorSet** set;
	CubemapTexture* texture;
	UniformBuffer* buffer;
	Camera* cam;
};

