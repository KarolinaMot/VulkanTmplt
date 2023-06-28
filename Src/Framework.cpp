#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());

	pool = new DescriptorPool(vulkan, 8);
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4 * vulkan->GetMaxFramesInFlight());
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3* vulkan->GetMaxFramesInFlight());
	pool->CreateDescriptorPool();


	time = new TimeManager();
	game = new Game(window->GetInputs(), vulkan, pool);
}

Framework::~Framework()
{
	delete window;
	delete time;
	delete pool;
	delete vulkan;
	delete game;
}

void Framework::Loop()
{
	while (!window->GetClosing()) {

		vulkan->WaitForFences(window);
		time->Update();
		window->Update();
		game->Update(time->GetDeltaTime(), vulkan->GetCurrentFrame());
		vulkan->ResetFences(window);

		game->Render(vulkan);
		vulkan->EndDrawFrame(window);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
