#include "../Headers/Game.h"

Game::Game(Inputs* inputs, Vulkan* vulkan)
{
	this->inputs = inputs;
	model = new Model("Assets/Models/Gato.obj", vulkan);
	
}

void Game::Update(float)
{

}

void Game::Render(Vulkan* vulkan)
{
	model->Draw(vulkan);
}
