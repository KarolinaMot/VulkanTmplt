#include "../Headers/Game.h"

Game::Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool)
{
	this->inputs = inputs;
	model = new Model("Assets/Models/Gato.obj", vulkan);
	sceneObjects.push_back(new GameObject("gato1", vulkan, model, glm::vec3(0.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f), pool));
	sceneObjects.push_back(new GameObject("gato2", vulkan, model, glm::vec3(2.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f), pool));
	sceneObjects.push_back(new GameObject("gato3", vulkan, model, glm::vec3(-2.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f), pool));
	camera = new Camera(vulkan, pool, glm::vec3(0.0f, 5.0f, 0.0f), vec2(0.1f, 30.f), 45.f, 5.f, 2.f, vulkan->GetSwapchainExtent().width, vulkan->GetSwapchainExtent().height);
}

Game::~Game()
{
	delete inputs;
	delete model;
	delete camera;
	for (int i = 0; i < sceneObjects.size(); i++) {
		delete sceneObjects[i];
	}
}

void Game::Update(float deltaTime, int currentFrame)
{
	camera->ControlInputs(inputs, deltaTime);
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->Update(deltaTime, currentFrame);
	}
	camera->Update(deltaTime, currentFrame);
}

void Game::Render(Vulkan* vulkan)
{
	camera->Bind(vulkan);
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->Draw(vulkan);
	}
}
