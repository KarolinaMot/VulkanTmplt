#pragma once
#include "TimeManager.h"
#include "Game.h"
#include "Vulkan.h"
#include "VBO.h"

class Framework
{
public:
	Framework();
	~Framework();
	void Loop();

private:
	GLFWindow* window;
	Vulkan* vulkan;
	std::vector<VBO*> vbos;
	TimeManager* time;
	Game* game;
	int frame = 0;
	const int height = 900;
	const int width = 1600;

	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};
};

