#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());

	cameraBuffer = new UniformBuffer(vulkan, 0, 1, VK_SHADER_STAGE_VERTEX_BIT, vulkan->GetMaxFramesInFlight(), sizeof(VPMatrix));
	modelBuffer = new UniformBuffer(vulkan, 0, 1, VK_SHADER_STAGE_VERTEX_BIT, vulkan->GetMaxFramesInFlight(), sizeof(ModelMatrix));

	texture = new Texture(vulkan, "Assets/Models/Textures/Gato2.png", 1);
	texture->CreateTexture(vulkan);

	pool = new DescriptorPool(vulkan, 4);
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2* vulkan->GetMaxFramesInFlight());
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1* vulkan->GetMaxFramesInFlight());
	pool->CreateDescriptorPool();

	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo info = cameraBuffer->GetBufferInfo(i);
		cameraDescriptorSet.push_back(new DescriptorSet(vulkan, pool, vulkan->GetCameraSetLayout()));
		cameraDescriptorSet[i]->AllocateSet();
		cameraDescriptorSet[i]->WriteBuffer(cameraBuffer->GetLayoutBinding().binding, &info);
		cameraDescriptorSet[i]->WriteSet();
	}

	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = modelBuffer->GetBufferInfo(i);
		VkDescriptorImageInfo textureInfo = texture->GetImageInfo(vulkan);
		modelDescriptorSet.push_back(new DescriptorSet(vulkan, pool, vulkan->GetModelSetLayout()));
		modelDescriptorSet[i]->AllocateSet();
		modelDescriptorSet[i]->WriteBuffer(0, &bufferInfo);
		modelDescriptorSet[i]->WriteImage(1, &textureInfo);
		modelDescriptorSet[i]->WriteSet();
	}

	time = new TimeManager();
	game = new Game(window->GetInputs(), vulkan);
}

Framework::~Framework()
{
	delete window;
	delete time;
	delete texture;
	delete cameraBuffer;
	delete pool;
	delete vulkan;
	delete game;
}

void Framework::Loop()
{
	while (!window->GetClosing()) {
		//time->Update();
		window->Update();
		//vulkan->StartDrawFrame(window, uniformBuffer, globalDescriptorSet[vulkan->GetCurrentFrame()]->GetHandle());
		vulkan->WaitForFences(window);
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		ModelMatrix modelMatrix;
		modelMatrix.model = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.f, -1.25f)) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));;
		

		VPMatrix cameraMatrix{};
		cameraMatrix.view = glm::lookAt(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		cameraMatrix.proj = glm::perspective(glm::radians(45.f), (float)vulkan->GetSwapchainExtent().width / vulkan->GetSwapchainExtent().height, 0.1f, 10.f);
		cameraMatrix.proj[1][1] *= -1;

		cameraBuffer->SetBufferData(vulkan->GetCurrentFrame(), &cameraMatrix, sizeof(cameraMatrix));
		modelBuffer->SetBufferData(vulkan->GetCurrentFrame(), &modelMatrix, sizeof(modelMatrix));

		vulkan->ResetFences(window);

		cameraDescriptorSet[vulkan->GetCurrentFrame()]->Bind(vulkan);
		modelDescriptorSet[vulkan->GetCurrentFrame()]->Bind(vulkan);

		game->Render(vulkan);
		vulkan->EndDrawFrame(window);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
