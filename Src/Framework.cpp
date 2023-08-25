#include "../Headers/Framework.h"
#include <fstream>

unsigned int Random::seed = 0;


bool GLFW_Window::framebufferResized = false;

Framework::Framework()
{
	cout << "Initializing GLFW window" << endl;
	window = make_shared<GLFW_Window>(width, height, "Vulkan template");

	render_engine = make_shared<Renderer>("Karolina's Amazing 3D Engine", window);

	//Descriptor Pool creation (maybe move to inside the renderer class)
	DescriptorPoolBuilder pool_builder;

	descriptor_pool = pool_builder
		.WithMaxSets(90)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 20 * render_engine->GetMaxFramesInFlight())
		.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20 * render_engine->GetMaxFramesInFlight())
		.Build(render_engine->GetDevice());

	gui = make_shared<GUI>(render_engine, descriptor_pool, window);
	Random::seed = 10;
	time = new TimeManager();
	game = new Game(window->GetInputs(), render_engine.get(), descriptor_pool.get());
}

Framework::~Framework()
{
	//Flush awaits all exectution of commands to end
	render_engine->GetDevice()->Flush();

	//Some smart pointers need to be explicitly freed for proper ordering (since not every pointer is smart)

	delete time;
	delete game;

	// Smart pointer must explicitely be freed here to not leak after vulkan renderer is destroyed
	// Fix: Make descriptor pool also apart of renderer

	descriptor_pool.reset();
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
	render_engine->WaitForFences(window);
	time->Update();
	window->Update();
	game->Update(time->GetDeltaTime(), render_engine->GetCurrentFrame(), gui.get());
	render_engine->ResetFences(window);
}

void Framework::RenderGame()
{

	render_engine->StartRenderPass();
	game->Render(render_engine.get());
	render_engine->EndRenderPass();

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
	render_engine->UIRenderPass(draw_data);

	render_engine->EndDrawFrame(window);

}
