#pragma once
#include "GLFWindow.h"
#include "Model.h"
#include "Vulkan.h"


class Game
{
	public:
		Game(Inputs* inputs, Vulkan* vulkan);
		void Update(float);
		void Render(Vulkan* vulkan);
	private:
		Inputs* inputs;
		Model* model;
};

