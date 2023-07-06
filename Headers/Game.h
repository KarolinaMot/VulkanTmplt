#pragma once
#include "GLFWindow.h"
#include "Model.h"
#include "Vulkan.h"
#include "GameObject.h"
#include "Camera.h"


class Game
{
	public:
		Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool);
		~Game();

		void Update(float deltaTime, int currentFrame);
		void Render(Vulkan* vulkan);

		std::vector<GameObject*>& GetSceneObjects() { return sceneObjects; }
	private:
		Inputs* inputs;
		std::vector<GameObject*> sceneObjects;
		Camera* camera;
		std::vector<Model*> models;
		CubemapTexture* cubemap;

		//DescriptorSet** cameraDescriptorSet;
		//UniformBuffer* cameraBuffer;

};

