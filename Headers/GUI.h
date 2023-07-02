#pragma once
#include "Common.h"
#include "GLFWindow.h"
#include "Vulkan.h"
class GUI
{
public:
	GUI(Vulkan* vulkan, DescriptorPool* pool, GLFWindow* window, Inputs* _inputs, uint scrW, uint scrH) { Init(vulkan, pool, window, _inputs, scrW, scrH); }
	~GUI() { ImGui::DestroyContext(); }
	void Init(Vulkan* vulkan, DescriptorPool* pool, GLFWindow* window, Inputs* _inputs, uint scrW, uint scrH);
	void StartFrame(float deltaTime);
	void EndFrame();
	void SimpleWindow();
	void StartViewportWindow();
	void EndViewportWindow();

private:
	Inputs* inputs;
	int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;

};

