#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());

	pool = new DescriptorPool(vulkan, 30);
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 * vulkan->GetMaxFramesInFlight());
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10* vulkan->GetMaxFramesInFlight());
	pool->CreateDescriptorPool();
	gui = new GUI(vulkan, pool, window, window->GetInputs(), window->GetWidth(), window->GetHeight());

	time = new TimeManager();
	game = new Game(window->GetInputs(), vulkan, pool);
}

Framework::~Framework()
{
	delete time;
	vulkan->StartCleanup();
	delete pool;
	delete game;
	vulkan->EndCleanup();
	delete vulkan;
	delete window;

}

void Framework::Loop()
{
	while (!window->GetClosing()) {
		gui->StartFrame(time->GetDeltaTime());
		vulkan->WaitForFences(window);
		time->Update();
		window->Update();
		game->Update(time->GetDeltaTime(), vulkan->GetCurrentFrame());
		vulkan->ResetFences(window);

		game->Render(vulkan);
		bool open = true;
		ImGui::ShowDemoWindow(&open);

		gui->EndFrame();
		ImDrawData* draw_data = ImGui::GetDrawData();
		vulkan->EndDrawFrame(window, draw_data);
		//game->Update(time->GetDeltaTime());
		//game->Render();
	}
}
