#include "../Headers/Framework.h"
#include <fstream>

bool GLFWindow::framebufferResized = false;

Framework::Framework()
{
	std::cout << "Initializing GLFW window" << std::endl;
	window = new GLFWindow(width, height, "Vulkan template");
	vulkan = new Vulkan("App", window->GetWindow());

	pool = new DescriptorPool(vulkan, 90);
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 20 * vulkan->GetMaxFramesInFlight());
	pool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20* vulkan->GetMaxFramesInFlight());
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
		Update();
		RenderGame();
		RenderUI();
	}
}

void Framework::Update()
{
	vulkan->WaitForFences(window);
	time->Update();
	window->Update();
	game->Update(time->GetDeltaTime(), vulkan->GetCurrentFrame(), gui);
	vulkan->ResetFences(window);

}

void Framework::RenderGame()
{
	vulkan->StartRenderPass();
	game->Render(vulkan);
	vulkan->EndRenderPass();

}

void Framework::RenderUI()
{
	gui->StartFrame(time->GetDeltaTime());
	bool open = true;
	gui->SceneWindow(game->GetSceneObjects());
	gui->DetailsWindow(game->GetSceneObjects());
	gui->ViewportWindow();
	gui->EndFrame();

	ImDrawData* draw_data = ImGui::GetDrawData();
	vulkan->UIRenderPass(draw_data);

	vulkan->EndDrawFrame(window);

}
