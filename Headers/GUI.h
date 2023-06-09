#pragma once
#include "Common.h"
#include "GLFWindow.h"
#include "Vulkan.h"
#include "GameObject.h"
class GUI
{
public:
	GUI(Vulkan* vulkan, DescriptorPool* pool, GLFWindow* window, Inputs* _inputs, uint scrW, uint scrH) { Init(vulkan, pool, window, _inputs, scrW, scrH); }
	~GUI() { ImGui::DestroyContext(); }
	void Init(Vulkan* vulkan, DescriptorPool* pool, GLFWindow* window, Inputs* _inputs, uint scrW, uint scrH);
	void StartFrame(float deltaTime);
	void EndFrame();
	void ViewportWindow();
	void SceneWindow(std::vector<GameObject*>& objects);
	void DetailsWindow(std::vector<GameObject*>& objects);
	float GetViewportW() { return viewportW; }
	float GetViewportH() { return viewportH; }

private:
	Inputs* inputs;
	int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;
	Vulkan* vulkanInstance;
	std::vector<VkDescriptorSet> m_Dset;
	uint selectedObject = 0;
	float viewportW = 0.f;
	float viewportH = 0.f;
};

