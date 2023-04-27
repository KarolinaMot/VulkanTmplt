#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());
	time = new TimeManager();
	game = new Game(window->GetInputs());
}

Framework::~Framework()
{
	delete window;
	delete time;
}

void Framework::Loop()
{

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	while (!window->GetClosing()) { 
		//time->Update();
		window->Update();
		vulkan->DrawFrame(window);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
