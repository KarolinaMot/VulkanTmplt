#include "../Headers/Framework.h"
#include <fstream>

bool GLFW_Window::framebufferResized = false;

Framework::Framework()
{
	cout << "Initializing GLFW window" << endl;
	window = make_shared<GLFW_Window>(width, height, "Vulkan template");

	render_engine = make_shared<Renderer>("Karolina's Amazing 3D Engine", window);


	time = new TimeManager();
	game = new Game(window->GetInputs(), render_engine.get(), render_engine->GetDescriptorPool().get());
}

Framework::~Framework()
{
	//Flush awaits all exectution of commands to end
	render_engine->GetDevice()->Flush();

	//Some smart pointers need to be explicitly freed for proper ordering (since not every pointer is smart)

	delete time;
	delete game;

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
	game->Update(time->GetDeltaTime(), render_engine->GetCurrentFrame(), render_engine->GetGUI().get());
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
	auto gui = render_engine->GetGUI();

	gui->StartFrame(time->GetDeltaTime());
	bool open = true;
	gui->FPSWindow(time->GetFPS());
	gui->SceneWindow(game->GetSceneObjects());
	gui->DetailsWindow(game->GetSceneObjects());
	gui->ViewportWindow(render_engine->GetCurrentFrame());
	gui->EndFrame();

	ImDrawData* draw_data = ImGui::GetDrawData();
	render_engine->UIRenderPass(draw_data);

	render_engine->EndDrawFrame(window);

}
