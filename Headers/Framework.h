#pragma once
#include "TimeManager.h"
#include "Game.h"
#include "Vulkan.h"

class Framework
{
public:
	Framework();
	~Framework();
	void Loop();

private:
	GLFWindow* window;
	Vulkan* vulkan;
	TimeManager* time;
	Game* game;
	int frame = 0;
	const int height = 900;
	const int width = 1600;
};

