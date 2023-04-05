#pragma once
#include "GLFWindow.h"
class Game
{
	public:
		Game(Inputs* inputs);
		void Update(float);
		void Render();
	private:
		Inputs* inputs;
		
};

