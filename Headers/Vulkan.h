#pragma once
#include "Common.h"
#include "GLFWindow.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <optional>
#include <set>
#include <algorithm>
#include <array>
#include <chrono>


using namespace glm;
class VBO;
class Image;
class Texture;
class UniformBuffer;
class DescriptorSetLayout;
class DescriptorSet;
class DescriptorPool;
class GUI;
class RenderPipeline;
class PipelineLayout;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 norm;
	static VkVertexInputBindingDescription GetBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	static VkVertexInputAttributeDescription GetVertexAttributeDescriptions();
};

class Vulkan
{
public:
	Vulkan(std::string _appName, GLFWwindow* win) { appName = _appName;  InitVulkan(win); }

	void WaitForFences(GLFWindow* win);
	void ResetFences(GLFWindow* win);
	void StartRenderPass();
	void EndRenderPass();
	void EndDrawFrame(GLFWindow* win);
	void StartCleanup();
	void EndCleanup();
	void UIRenderPass(ImDrawData* draw_data);

	void* MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size);
	void  UnmapMemory(VkDeviceMemory memory);


	VkDevice& GetDevice() { return device; };
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkCommandBuffer BeginSingleTimeCommands();
	VkSampler GetTextureSampler() { return textureSampler; }
	//VkPipelineLayout GetPipelineLayout() { return pipelineLayout; }
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	VkCommandBuffer GetCommandBuffer() { return viewportCommandBuffers[currentFrame]; }
	const int GetMaxFramesInFlight() { return MAX_FRAMES_IN_FLIGHT; }
	const int GetCurrentFrame() { return currentFrame; }
	DescriptorSetLayout* GetCameraSetLayout() { return cameraDescriptorSetLayout; }
	DescriptorSetLayout* GetModelSetLayout() { return modelDesctiptorSetLayout; }
	DescriptorSetLayout* GetSkyboxSetLayout() { return skyboxDesctiptorSetLayout; }
	VkExtent2D GetSwapchainExtent() {return swapChainExtent;}
	VkFormatProperties GetFormatProperties(VkFormat imageFormat){
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);
		return formatProperties;
	}
	void InitVulkanImGUI(DescriptorPool* pool);
	std::vector<VkImageView> &GetViewportImageViews() { return viewportImageViews; }
	VkRenderPass GetViewportRenderPass() { return viewportRenderPass; }
	VkRenderPass GetUIRenderPass() { return renderPass; }
	VkPipelineLayout GetViewportPipelineLayout();
	VkPipelineLayout GetSkyboxPipelineLayout();
	VkSampleCountFlagBits GetMsaaSamples() { return msaaSamples; }
	RenderPipeline* GetSkyboxPipeline() { return skyboxPipeline; }
	RenderPipeline* GetViewportPipeline() { return viewportPipeline; }

private:
	struct QueueFamilyIndices {
		//Data structure to distinguish between the case of a value existing or not:
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	void InitVulkan(GLFWwindow* win);
	void SetupDebugMessenger();
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void CreateImageViews();
	void CreateInstance();
	void CreateSurface(GLFWwindow* win);
	bool CheckValidationLayerSupport();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateViewport();

	void CreateSwapchain(GLFWwindow* win);
	void RecreateSwapchain(GLFWindow* win);
	void CleanupSwapchain();

	//DescriptorSetLayout* globalDescriptorSetLayout;
	//DescriptorPool* descriptorPool;
	//std::vector<DescriptorSet*> globalDescriptorSet;
	

	//void CreateGraphicsPipeline();
	void CreateRenderPass(VkRenderPass* pass);
	void CreateFramebuffers();
	void CreateCommandPool(VkCommandPool* pool);
	void CreateCommandBuffers();

	void CreateSyncObjects();

	void CreateTextureSampler();
	void CreateColorResources();
	VkSampleCountFlagBits CalculateMaxUsableSampleCount();
	ImGui_ImplVulkan_InitInfo GetImGUIInitInfo(DescriptorPool* pool);


	void CreateDepthResources();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	std::vector<const char*> GetRequiredExtensions();

	VkResult currentResult;
	uint32_t currentImageIndex;
	DescriptorSetLayout* cameraDescriptorSetLayout;
	DescriptorSetLayout* modelDesctiptorSetLayout;
	DescriptorSetLayout* skyboxDesctiptorSetLayout;

	VkSampler textureSampler;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	Image* colorImage;
	VkImageView colorImageView;

	std::vector<Image*> viewportImages;
	std::vector<VkImageView> viewportImageViews;
	VkRenderPass viewportRenderPass;
	RenderPipeline* viewportPipeline;
	VkCommandPool viewportCommandPool;
	std::vector<VkFramebuffer> viewportFramebuffers;
	std::vector<VkCommandBuffer> viewportCommandBuffers;


	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	//Almost every operation in Vulkan, anything from drawing to uploading textures, requires commands to be submitted to a queue.
	//There are different types of queues that originate from different queue families and each family of queues allows only a
	//subset of commands.
	//This checks which queue families are supported by the device and which one of these supports the commands that we want to use.
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	VkInstance inst;
	VkDevice device;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	uint32_t graphicsQueueFamily;
	uint32_t presentQueueFamily;
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkExtent2D swapChainExtent;
	PipelineLayout* pipelineLayout;
	PipelineLayout* skyboxPipelineLayout;
	VkRenderPass renderPass;
	RenderPipeline* graphicsPipeline;
	RenderPipeline* skyboxPipeline;
	VkPipelineCache pipelineCache;
	//Command pools manage the memory that is used to store the buffers and command buffers are allocated from them. 
	VkCommandPool commandPool;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t currentFrame = 0;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImage> swapChainImages;


	std::string appName;
	//std::vector<VkDescriptorSet> descriptorSets;

	Image* depthImage;
	VkImageView depthImageView;

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	static void check_vk_result(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}



	//Just like extensions, validation layers need to be enabled by specifying their name.
	//All of the useful standard validation is bundled into a layer included in the SDK
	//that is known as VK_LAYER_KHRONOS_validation
	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	std::vector<VkDynamicState> dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

};

