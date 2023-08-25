#pragma once
#include "Common.h"
#include "Random.h"
#include "TimeManager.h"
#include "Game.h"
#include "Renderer.h"
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

	void Run();

private:

	shared_ptr<GLFW_Window> window;
	shared_ptr<Renderer> render_engine;

	TimeManager* time;
	Game* game;

	shared_ptr<DescriptorPool> descriptor_pool;
	shared_ptr<GUI> gui;

	int frame = 0;

	const int height = 900;
	const int width = 1600;

	void Update();
	void RenderGame();
	void RenderUI();

};

