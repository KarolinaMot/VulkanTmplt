#include "../Headers/Framework.h"
#include <fstream>

bool GLFW_Window::framebufferResized = false;

Framework::Framework()
{
	cout << "Initializing GLFW window" << endl;
	window = make_shared<GLFW_Window>(width, height, "Vulkan template");

	vulkan = new Renderer("Karolina's Amazing 3D Engine", window);

	DescriptorPoolBuilder pool_builder;

	descriptor_pool = pool_builder
		.WithMaxSets(90)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 20 * vulkan->GetMaxFramesInFlight())
		.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20 * vulkan->GetMaxFramesInFlight())
		.Build(vulkan->GetDevice());

	gui = make_shared<GUI>(vulkan, descriptor_pool.get(), window.get(), window->GetInputs(), window->GetWidth(), window->GetHeight());

	time = new TimeManager();
	game = new Game(window->GetInputs(), vulkan, descriptor_pool.get());
}

Framework::~Framework()
{
	//Flush awaits all exectution of commands to end
	vulkan->GetDevice()->Flush();

	//Some smart pointers need to be explicitly freed for proper ordering (since not every pointer is smart)

	delete time;
	delete game;

	gui.reset();

	// Smart pointer must explicitely be freed here to not leak after vulkan renderer is destroyed
	// Fix: Make descriptor pool also apart of renderer

	descriptor_pool.reset();
	delete vulkan;
}

void Framework::Run()
{
	while (!window->GetClosing()) {
		//window->Update();
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
	game->Update(time->GetDeltaTime(), vulkan->GetCurrentFrame(), gui.get());
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
	gui->FPSWindow(time->GetFPS());
	gui->SceneWindow(game->GetSceneObjects());
	gui->DetailsWindow(game->GetSceneObjects());
	gui->ViewportWindow();
	gui->EndFrame();

	ImDrawData* draw_data = ImGui::GetDrawData();
	vulkan->UIRenderPass(draw_data);

	vulkan->EndDrawFrame(window);

}
