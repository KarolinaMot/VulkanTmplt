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
	private:
		Inputs* inputs;
		Model* model;
		std::vector<GameObject*> sceneObjects;
		Camera* camera;

		//DescriptorSet** cameraDescriptorSet;
		//UniformBuffer* cameraBuffer;

};

