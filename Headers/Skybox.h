#pragma once
#include "Transform.h"
#include "Model.h"
#include "UniformBuffer.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(Renderer* vulkan, Camera* camera, DescriptorPool* pool, Model* model, string texturePath);
	~Skybox();
	void Update(float deltaTime, float currentFrame);
	void Draw(Renderer* vulkan);

private:

	Model* cube;
	vector<DescriptorSet*> sets;

	//DescriptorSet** cameraSet;
	Transform* transform;
	CubemapTexture* texture;
	UniformBuffer* cameraBuffer;
	Camera* cam;
};

