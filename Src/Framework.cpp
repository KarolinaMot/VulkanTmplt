#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());

	uniformBuffer = new UniformBuffer(vulkan, 0, 1, VK_SHADER_STAGE_VERTEX_BIT, vulkan->GetMaxFramesInFlight(), sizeof(MVPMatrix));
	texture = new Texture(vulkan, "Assets/Models/Textures/Gato2.png", 1);
	texture->CreateTexture(vulkan);

	pool = new DescriptorPool(vulkan, vulkan->GetMaxFramesInFlight());
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vulkan->GetMaxFramesInFlight());
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, vulkan->GetMaxFramesInFlight());
	pool->CreateDescriptorPool();

	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo info = uniformBuffer->GetBufferInfo(i);
		globalDescriptorSet.push_back(new DescriptorSet(vulkan, pool, vulkan->GetGlobalSetLayout()));
		globalDescriptorSet[i]->AllocateSet();
		globalDescriptorSet[i]->WriteBuffer(uniformBuffer->GetLayoutBinding().binding, &info);
		VkDescriptorImageInfo info2 = texture->GetImageInfo(vulkan);
		globalDescriptorSet[i]->WriteImage(texture->GetLayoutBinding().binding, &info2);
		globalDescriptorSet[i]->WriteSet();
	}

	time = new TimeManager();
	game = new Game(window->GetInputs(), vulkan);
}

Framework::~Framework()
{
	delete window;
	delete time;
	delete texture;
	delete uniformBuffer;
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
		MVPMatrix ubo{};
		ubo.model = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.f, -1.25f)) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.f), (float)vulkan->GetSwapchainExtent().width / vulkan->GetSwapchainExtent().height, 0.1f, 10.f);
		ubo.proj[1][1] *= -1;

		uniformBuffer->SetBufferData(vulkan->GetCurrentFrame(), &ubo, sizeof(ubo));

		vulkan->ResetFences(window);

		globalDescriptorSet[vulkan->GetCurrentFrame()]->Bind(vulkan);
		game->Render(vulkan);
		vulkan->EndDrawFrame(window);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
