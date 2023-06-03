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
	vbos.push_back(new VBO(vulkan, vertices, indices));
}

Framework::~Framework()
{
	delete window;
	delete time;
	for (size_t i = 0; i < vbos.size(); i++)
		delete vbos[i];
	delete vulkan;
	delete game;
}

void Framework::Loop()
{

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	while (!window->GetClosing()) { 
		//time->Update();
		window->Update();
		vulkan->DrawFrame(window, vbos);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
