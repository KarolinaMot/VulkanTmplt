#pragma once
#include "Common.h"


//New headers
#include "GLFW_Window.h"
#include "VulkanInstance.h"
#include "Surface.h"
#include "VulkanDevice.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "DescriptorSetLayout.h"
#include "DescriptorPool.h"
#include "RenderPipeline.h"
#include "ShaderModule.h"
#include "CommandPool.h"
#include "TextureSampler.h"
#include "Framebuffer.h"

#include "Vertex.h"

// These forward declares should be removed once
// these classes dont include the renderer class (and include their more specific components)

class VBO;
class Image;
class Texture;
class UniformBuffer;

class DescriptorSet;
class GUI;

class Renderer
{
private:

	//New Refactor by Santiago
	shared_ptr<GLFW_Window> target_window;

	shared_ptr<VulkanInstance> instance;
	shared_ptr<Surface> surface;
	shared_ptr<VulkanDevice> device;
	shared_ptr<Swapchain> swapchain;

	shared_ptr<RenderPass> default_renderpass;
	shared_ptr<RenderPass> viewport_renderpass;

	shared_ptr<DescriptorSetLayout> cameraDescriptorSetLayout;
	shared_ptr<DescriptorSetLayout> modelDescriptorSetLayout;
	shared_ptr<DescriptorSetLayout> skyboxDescriptorSetLayout;
	shared_ptr<DescriptorSetLayout> lightDescriptorSetLayout;

	shared_ptr<PipelineLayout> graphics_layout;
	shared_ptr<PipelineLayout> skybox_layout;

	shared_ptr<RenderPipeline> graphics_pipeline;
	shared_ptr<RenderPipeline> skybox_pipeline;
	shared_ptr<RenderPipeline> viewport_pipeline;

	shared_ptr<CommandPool> default_command_pool;
	shared_ptr<CommandPool> viewport_command_pool;

	shared_ptr<TextureSampler> texture_sampler;

	// Unique since these occupy memory and its not desirable 
	// to handle multiple references (we need control when they are deallocated)

	vector<unique_ptr<Image>> viewport_images;

	vector<unique_ptr<Framebuffer>> swapchain_framebuffers;
	vector<unique_ptr<Framebuffer>> viewport_framebuffers;

	//For swapchain (replace with smart ptr later)
	Image* colorImage;
	VkImageView colorImageView;

	Image* depthImage;
	VkImageView depthImageView;

	//For viewport (replace with smart ptr later)
	Image* view_colorImage;
	VkImageView view_colorImageView;

	Image* view_depthImage;
	VkImageView view_depthImageView;

public:

	Renderer(const char* appName, shared_ptr<GLFW_Window> window);
	~Renderer();

	void WaitForFences(shared_ptr<GLFW_Window> window);
	void ResetFences(shared_ptr<GLFW_Window> window);

	void StartRenderPass();
	void EndRenderPass();

	void UIRenderPass(ImDrawData* draw_data);
	void EndDrawFrame(shared_ptr<GLFW_Window> window);

	void* MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size);
	void  UnmapMemory(VkDeviceMemory memory);

	shared_ptr<VulkanDevice> GetDevice() { return device; }
	shared_ptr<Swapchain> GetSwapchain() { return swapchain; }

	shared_ptr<DescriptorSetLayout> GetCameraSetLayout() { return cameraDescriptorSetLayout; }
	shared_ptr<DescriptorSetLayout> GetModelSetLayout() { return modelDescriptorSetLayout; }
	shared_ptr<DescriptorSetLayout> GetSkyboxSetLayout() { return skyboxDescriptorSetLayout; }
	shared_ptr<DescriptorSetLayout> GetLightSetLayout() { return lightDescriptorSetLayout; }

	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	VkSampler GetTextureSampler() { return texture_sampler->handle(); }

	VkCommandBuffer GetCommandBuffer() { return viewport_command_pool->get_buffer(currentFrame); }

	const int GetMaxFramesInFlight() { return MAX_FRAMES_IN_FLIGHT; }
	const int GetCurrentFrame() { return currentFrame; }

	VkPipelineLayout GetViewportPipelineLayout();
	VkPipelineLayout GetBoxPipelineLayout();

	void InitVulkanImGUI(DescriptorPool* pool);
	vector<VkImageView> &GetViewportImageViews() { return viewportImageViews; }

	RenderPipeline* GetViewportPipeline() { return viewport_pipeline.get(); }
	RenderPipeline* GetBoxPipeline() { return skybox_pipeline.get(); }

	VkFormatProperties GetFormatProperties(VkFormat imageFormat) {
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(device->physical(), imageFormat, &formatProperties);
		return formatProperties;
	}

private:

	void RecreateSwapchain();

	void CreateViewportResources();
	void CreateSwapchainResources();

	void CleanupSwapchain();


	void CreateSyncObjects();

	ImGui_ImplVulkan_InitInfo GetImGUIInitInfo(DescriptorPool* pool);


	VkResult currentResult;
	uint32_t currentImageIndex;

	DescriptorSet* skyboxDescriptorSet;

	vector<VkImageView> viewportImageViews;
	
	const uint MAX_FRAMES_IN_FLIGHT = 2;
	uint currentFrame = 0;

	vector<VkSemaphore> imageAvailableSemaphores;
	vector<VkSemaphore> renderFinishedSemaphores;
	vector<VkFence> inFlightFences;

};

