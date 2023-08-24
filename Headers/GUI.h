#pragma once

#include "Common.h"
#include "GLFW_Window.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Light.h"

class GUI
{
public:
	
	GUI(Renderer* vulkan, DescriptorPool* pool, GLFW_Window* window, Inputs* _inputs, uint scrW, uint scrH);
	~GUI();
	

	void StartFrame(float deltaTime);
	void EndFrame();
	void ViewportWindow();
	void SceneWindow(vector<GameObject*>& objects);
	void DetailsWindow(vector<GameObject*>& objects);
	void FPSWindow(float fps);
	float GetViewportW() { return viewportW; }
	float GetViewportH() { return viewportH; }

private:

	Inputs* inputs;
	int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;

	Renderer* vulkanInstance;
	vector<VkDescriptorSet> m_Dset;

	uint selectedObject = 0;
	float viewportW = 0.f;
	float viewportH = 0.f;
};

