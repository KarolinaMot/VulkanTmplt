#pragma once
#include "GLFW_Window.h"
#include "Model.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "RenderPipeline.h"
#include "GUI.h"
#include "Light.h"


class Game
{
	public:
		Game(Inputs* inputs, Renderer* vulkan, DescriptorPool* pool);
		~Game();

		void Update(float deltaTime, int currentFrame, GUI* gui);
		void Render(Renderer* vulkan);

		vector<GameObject*>& GetSceneObjects() { return sceneObjects; }

	private:

		Inputs* inputs;
		vector<GameObject*> sceneObjects;
		Camera* camera;
		vector<Model*> models;
		Skybox* skybox;
		uint w, h;
		Light* light;

		void CreateGameObjectsFromModel(Model* model, string name, vec3 position, vec3 rotation, vec3 scale, Renderer* vulkan, DescriptorPool* pool);

		//DescriptorSet** cameraDescriptorSet;
		//UniformBuffer* cameraBuffer;

};

