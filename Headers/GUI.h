#pragma once

#include "Common.h"
#include "GLFW_Window.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Light.h"

class GUI
{
public:
	
	GUI(
		shared_ptr<GLFW_Window> window,
		shared_ptr<RenderPass> ui_renderpass,
		ImGui_ImplVulkan_InitInfo* init_info,
		VkCommandBuffer loading_commands
	);

	~GUI();
	
	void CreateViewportDescriptors(vector<VkImageView>& viewport_views, shared_ptr<TextureSampler> sampler);
	void FreeViewportDescriptors();

	void UpdateDisplaySize(shared_ptr<GLFW_Window> window);

	void StartFrame(float deltaTime);
	void EndFrame();
	
	void ViewportWindow(uint current_frame_index);
	void SceneWindow(vector<GameObject*>& objects);
	void DetailsWindow(vector<GameObject*>& objects);
	void FPSWindow(float fps);

	float GetViewportW() { return viewportW; }
	float GetViewportH() { return viewportH; }

private:

	void ConfigureStyle(ImGuiStyle& style_ref);

	Inputs* inputs;

	int FULL_SCREEN_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;

	vector<VkDescriptorSet> m_Dset;

	uint selectedObject = 0;
	float viewportW = 0.f;
	float viewportH = 0.f;
};

