#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());
	time = new TimeManager();
	game = new Game(window->GetInputs(), vulkan);
}

Framework::~Framework()
{
	delete window;
	delete time;
	delete vulkan;
	delete game;
}

void Framework::Loop()
{
	while (!window->GetClosing()) {
		//time->Update();
		window->Update();
		vulkan->StartDrawFrame(window);
		game->Render(vulkan);
		vulkan->EndDrawFrame(window);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
