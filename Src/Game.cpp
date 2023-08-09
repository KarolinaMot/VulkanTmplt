#include "../Headers/Game.h"

Game::Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool)
{
	this->inputs = inputs;
	camera = new Camera(vulkan, pool, glm::vec3(0.0f, 0.0f, 5.0f), vec2(0.1f, 60.f), 45.f, 5.f, 2.f, vulkan->GetSwapchainExtent().width, vulkan->GetSwapchainExtent().height);

	models.push_back(new Model("Assets/Models/Gato.obj", vulkan));
	models.push_back(new Model("Assets/Models/Cube.fbx", vulkan));
	sceneObjects.push_back(new GameObject("gato1", vulkan, models[0], glm::vec3(2.0f, -1.25f, 0.f), glm::quat(glm::vec3(0.f, glm::radians(180.f), 0.f)), glm::vec3(0.05f, 0.05f, 0.05f), pool));
	skybox = new Skybox(vulkan, camera, pool, models[1], "Assets/Images/Natural");

	sceneObjects.push_back(new GameObject("gato2", vulkan, models[0], glm::vec3(-2.0f, -1.25f, 0.f), glm::quat(glm::vec3(0.f, glm::radians(180.f), 0.f)), glm::vec3(0.05f, 0.05f, 0.05f), pool));
}

Game::~Game()
{
	delete inputs;
	models.clear();
	delete camera;
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

void Game::Render(Vulkan* vulkan)
{
	vulkan->GetBoxPipeline()->Bind(vulkan);
	skybox->Draw(vulkan);

	vulkan->GetViewportPipeline()->Bind(vulkan);
	camera->Bind(vulkan);
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->Draw(vulkan);
	}
}
