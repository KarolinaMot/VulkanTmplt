#include "Game.h"

Game::Game(Inputs* inputs, Renderer* vulkan, DescriptorPool* pool)
{
	this->inputs = inputs;
	camera = new Camera(
		vulkan, pool, glm::vec3(0.0f, 0.0f, 5.0f), vec2(0.1f, 60.f), 45.f, 5.f, 2.f,
		vulkan->GetSwapchain()->get_extent().width, vulkan->GetSwapchain()->get_extent().height);

	models.push_back(new Model("Assets/Models/Cube.fbx", vulkan));
	models.push_back(new Model("Assets/Models/Gato.obj", vulkan));
	//models.push_back(new Model("Assets/Models/box.fbx", vulkan));

	skybox = new Skybox(vulkan, camera, pool, models[0], "Assets/Images/Natural");

	CreateGameObjectsFromModel(models[1], "GATO1", glm::vec3(2.0f, -1.25f, 0.f), glm::vec3(0.f, glm::radians(180.f), 0.f), glm::vec3(0.05f, 0.05f, 0.05f), vulkan, pool);
	CreateGameObjectsFromModel(models[1], "GATO2", glm::vec3(-6.0f, -1.25f, 0.f),glm::vec3(0.f, glm::radians(180.f), 0.f), glm::vec3(0.05f, 0.05f, 0.05f), vulkan, pool);
	//CreateGameObjectsFromModel(models[2], "DIO", glm::vec3(-2.0f, -1.25f, 0.f), glm::vec3(0.f, glm::radians(180.f), 0.f), glm::vec3(1.f, 1.f, 1.f), vulkan, pool);

	light = new Light(vulkan, "LIGHT", pool, models[0]->GetMeshes()[0], glm::vec3(0.f, 5.f, 0.f), vec4(1.0, 0.5f, 0.5f, 1.f), glm::normalize(vec3(0.5f, 1.0f, 0.3f)), 20.f);
	sceneObjects.push_back(light);
}

Game::~Game()
{
	for (auto&& model : models) {
		delete model;
	}

	models.clear();
	
	delete camera;
	delete skybox;

	for (int i = 0; i < sceneObjects.size(); i++) {
		delete sceneObjects[i];
	}
}

void Game::Update(float deltaTime, int currentFrame, GUI* gui)
{
	camera->ControlInputs(inputs, deltaTime);
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->Update(deltaTime, currentFrame);
	}

	camera->Update(deltaTime, currentFrame, gui->GetViewportW(), gui->GetViewportH());
	skybox->Update(deltaTime, currentFrame);
}

void Game::Render(Renderer* vulkan)
{
	vulkan->GetBoxPipeline()->Bind(vulkan->GetCommandBuffer());
	skybox->Draw(vulkan);

	vulkan->GetViewportPipeline()->Bind(vulkan->GetCommandBuffer());
	camera->Bind(vulkan);
	light->Bind(vulkan);
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->Draw(vulkan);
	}
}

void Game::CreateGameObjectsFromModel(Model* model, string name, vec3 position, vec3 rotation, vec3 scale, Renderer* vulkan, DescriptorPool* pool)
{
	vector<Mesh*> meshes = model->GetMeshes();

	for (int i = 0; i < meshes.size(); i++) {
		string objName = name + to_string(i);
		sceneObjects.push_back(new GameObject(objName, vulkan, meshes[i], position, glm::quat(rotation), scale, pool));
	}
}
