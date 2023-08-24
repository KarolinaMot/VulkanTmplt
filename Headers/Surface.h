#pragma once
#include "VulkanInstance.h"
#include "GLFW_Window.h"

class Surface
{
private:

	shared_ptr<VulkanInstance> associated_instance;

	VkSurfaceKHR surface = nullptr;

public:

	Surface(shared_ptr<VulkanInstance> instance, shared_ptr<GLFW_Window> window);
	~Surface();

	VkSurfaceKHR handle() { return surface; }
};

