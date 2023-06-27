#pragma once
#include "GLFWindow.h"
#include "Model.h"
#include "Vulkan.h"
#include "GameObject.h"


class Game
{
	public:
		Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool);
		~Game();

		void Update(float deltaTime, int currentFrame, VkExtent2D size);
		void Render(Vulkan* vulkan);
	private:
		Inputs* inputs;
		Model* model;
		GameObject* gato;

		DescriptorSet** cameraDescriptorSet;
		UniformBuffer* cameraBuffer;

};

