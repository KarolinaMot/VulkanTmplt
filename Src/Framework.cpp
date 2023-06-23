#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());

	uniformBuffer = new UniformBuffer(vulkan, 0, 1, VK_SHADER_STAGE_VERTEX_BIT, vulkan->GetMaxFramesInFlight());
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
		vulkan->StartDrawFrame(window, uniformBuffer, globalDescriptorSet[vulkan->GetCurrentFrame()]->GetHandle());
		game->Render(vulkan);
		vulkan->EndDrawFrame(window);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
