#include "../Headers/Game.h"

Game::Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool)
{
	this->inputs = inputs;
	model = new Model("Assets/Models/Gato.obj", vulkan);
	gato = new GameObject(vulkan, model, glm::vec3(0.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f), pool);
	gato2 = new GameObject(vulkan, model, glm::vec3(2.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f), pool);
	gato3 = new GameObject(vulkan, model, glm::vec3(-2.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f), pool);
	camera = new Camera(vulkan, pool, glm::vec3(0.0f, 5.0f, 0.0f), vec2(0.1f, 30.f), 45.f, 5.f, 2.f, vulkan->GetSwapchainExtent().width, vulkan->GetSwapchainExtent().height);
}

Game::~Game()
{
	delete inputs;
	delete model;
	delete camera;
	delete gato;
	delete gato2;
	delete gato3;
}

void Game::Update(float deltaTime, int currentFrame)
{
	camera->ControlInputs(inputs, deltaTime);
	gato->Update(deltaTime, currentFrame);
	gato2->Update(deltaTime, currentFrame);
	gato3->Update(deltaTime, currentFrame);
	camera->Update(deltaTime, currentFrame);
}

void Game::Render(Vulkan* vulkan)
{
	camera->Bind(vulkan);
	gato->Draw(vulkan);
	gato2->Draw(vulkan);
	gato3->Draw(vulkan);
}
