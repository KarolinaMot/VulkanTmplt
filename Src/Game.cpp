#include "../Headers/Game.h"

Game::Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool)
{
	this->inputs = inputs;
	model = new Model("Assets/Models/Gato.obj", vulkan, pool);
	gato = new GameObject(vulkan, model, glm::vec3(0.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f));
	camera = new Camera(vulkan, pool, glm::vec3(0.0f, 5.0f, 0.0f), vec2(0.1f, 30.f), 45.f, 5.f, 2.f, vulkan->GetSwapchainExtent().width, vulkan->GetSwapchainExtent().height);
}

Game::~Game()
{
	delete inputs;
	delete model;
	delete camera;
}

void Game::Update(float deltaTime, int currentFrame)
{
	//static auto startTime = std::chrono::high_resolution_clock::now();

	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	//mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//rotation = glm::rotate(rotation, time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//ModelMatrix modelMatrix;
	//modelMatrix.model = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.f, -1.25f)) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	//model->UpdateModelMatrix({ glm::vec3(0.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f)}, currentFrame);
	camera->ControlInputs(inputs, deltaTime);
	gato->Update(deltaTime, currentFrame);
	camera->Update(deltaTime, currentFrame);

	//VPMatrix cameraMatrix{};
	//cameraMatrix.view = glm::lookAt(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//cameraMatrix.proj = glm::perspective(glm::radians(45.f), (float)size.width / size.height, 0.1f, 10.f);
	//cameraMatrix.proj[1][1] *= -1;

}

void Game::Render(Vulkan* vulkan)
{
	camera->Bind(vulkan);
	gato->Draw(vulkan);
}
