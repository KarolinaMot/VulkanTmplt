#include "Surface.h"

Surface::Surface(shared_ptr<VulkanInstance> instance, shared_ptr<GLFW_Window> window)
{
	associated_instance = instance;

	CHECK_VK(glfwCreateWindowSurface(instance->handle(), window->handle(), nullptr, &surface));
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(associated_instance->handle(), surface, nullptr);
}