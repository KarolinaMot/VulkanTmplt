#pragma once
#include "Common.h"
#include "TimeManager.h"
#include "Game.h"
#include "Vulkan.h"
#include "VBO.h"
#include "Model.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"
#include "GUI.h"

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
	DescriptorPool* pool;
	GUI* gui;

	int frame = 0;
	const int height = 900;
	const int width = 1600;

	void Update();
	void RenderGame();
	void RenderUI();

	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},  {0.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f},  {0.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f},  {0.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f},  {0.0f, 0.0f, 0.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f},  {0.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f},  {0.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f},  {0.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};
};

