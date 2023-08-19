#pragma once
#include "GLFWindow.h"
#include "Model.h"
#include "Vulkan.h"
#include "GameObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "RenderPipeline.h"
#include "GUI.h"


class Game
{
	public:
		Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool);
		~Game();

		void Update(float deltaTime, int currentFrame, GUI* gui);
		void Render(Vulkan* vulkan);

		std::vector<GameObject*>& GetSceneObjects() { return sceneObjects; }
	private:
		Inputs* inputs;
		std::vector<GameObject*> sceneObjects;
		Camera* camera;
		std::vector<Model*> models;
		Skybox* skybox;
		uint w, h;

		void CreateGameObjectsFromModel(Model* model, std::string name, vec3 position, vec3 rotation, vec3 scale, Vulkan* vulkan, DescriptorPool* pool);

		//DescriptorSet** cameraDescriptorSet;
		//UniformBuffer* cameraBuffer;

};

