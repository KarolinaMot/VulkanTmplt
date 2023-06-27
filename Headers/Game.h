#pragma once
#include "GLFWindow.h"
#include "Model.h"
#include "Vulkan.h"


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

		DescriptorSet** cameraDescriptorSet;
		UniformBuffer* cameraBuffer;

};

