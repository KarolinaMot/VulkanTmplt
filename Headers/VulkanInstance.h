#pragma once
#include "Common.h"

class VulkanInstance
{
private:

	VkInstance instance = nullptr;
	VkDebugUtilsMessengerEXT debug_messenger = nullptr;

public:

	VulkanInstance(bool debug, const char* app_name);
	~VulkanInstance();

	VkInstance handle() { return instance; }

	//Debug logging
	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		cerr << pCallbackData->pMessage << endl << endl;

		return VK_FALSE;
	}

private:

	//Helper functions
	bool check_validation_support(const vector<const char*>& requested_validation);

	//Lookup
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger
	);

	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator
	);

private:

	//Configuration

	inline static vector<const char*> instance_extensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		"VK_KHR_win32_surface"
	};

	inline static vector<const char*> debug_instance_extensions = {
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	inline static vector<const char*> validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};

};
