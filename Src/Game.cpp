#include "../Headers/Game.h"

Game::Game(Inputs* inputs, Vulkan* vulkan, DescriptorPool* pool)
{
	this->inputs = inputs;
	model = new Model("Assets/Models/Gato.obj", vulkan, pool);
	cameraBuffer = new UniformBuffer(vulkan, 0, 1, VK_SHADER_STAGE_VERTEX_BIT, vulkan->GetMaxFramesInFlight(), sizeof(VPMatrix));

	cameraDescriptorSet = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo info = cameraBuffer->GetBufferInfo(i);
		cameraDescriptorSet[i] = new DescriptorSet(vulkan, pool, vulkan->GetCameraSetLayout());
		cameraDescriptorSet[i]->AllocateSet();
		cameraDescriptorSet[i]->WriteBuffer(cameraBuffer->GetLayoutBinding().binding, &info);
		cameraDescriptorSet[i]->WriteSet();
	}

	
}

Game::~Game()
{
	delete inputs;
	delete model;
	delete cameraBuffer;
	delete[] cameraDescriptorSet;
}

void Game::Update(float deltaTime, int currentFrame, VkExtent2D size)
{
	//static auto startTime = std::chrono::high_resolution_clock::now();

	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	//mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//rotation = glm::rotate(rotation, time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//ModelMatrix modelMatrix;
	//modelMatrix.model = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.f, -1.25f)) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
	model->UpdateModelMatrix({ glm::vec3(0.0f, 0.f, -1.25f), glm::quat(glm::vec3(glm::radians(90.f), 0.f, glm::radians(180.f))), glm::vec3(0.05f, 0.05f, 0.05f)}, currentFrame);

	VPMatrix cameraMatrix{};
	cameraMatrix.view = glm::lookAt(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	cameraMatrix.proj = glm::perspective(glm::radians(45.f), (float)size.width / size.height, 0.1f, 10.f);
	cameraMatrix.proj[1][1] *= -1;

	cameraBuffer->SetBufferData(currentFrame, &cameraMatrix, sizeof(cameraMatrix));
}

void Game::Render(Vulkan* vulkan)
{
	cameraDescriptorSet[vulkan->GetCurrentFrame()]->Bind(vulkan);
	model->Draw(vulkan);
}
