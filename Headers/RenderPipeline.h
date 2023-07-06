#pragma once
#include "Vulkan.h"
#include "DescriptorSet.h"

class PipelineLayout {
public:
	PipelineLayout(Vulkan* vulkan, std::string vert, std::string frag, std::vector<DescriptorSetLayout*> layouts, VkCullModeFlags cullMode, bool onlyVertex);
	~PipelineLayout();

	VkPipelineLayout GetHandle() { return pipelineLayout; }
	VkShaderModule GetVertShaderModule() { return vertShaderModule; }
	VkShaderModule GetFragShaderModule() { return fragShaderModule; }
	std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() { return shaderStages; }
	VkPipelineVertexInputStateCreateInfo& GetVertexInputInfo() { return vertexInputInfo; };
	VkPipelineInputAssemblyStateCreateInfo& GetInputAssemblyInfo() { return inputAssembly; };
	VkPipelineViewportStateCreateInfo& GetViewportStateInfo() { return viewportState; }
	VkPipelineRasterizationStateCreateInfo& GetRasterizerInfo() { return rasterizer; }
	VkPipelineMultisampleStateCreateInfo& GetMultisamplingInfo() { return multisampling; }
	VkPipelineDepthStencilStateCreateInfo& GetDepthInfo() { return depthStencil; }
	VkPipelineColorBlendStateCreateInfo& GetColorBlendingInfo() { return colorBlending; }
	VkPipelineDynamicStateCreateInfo& GetDynamicStateInfo() { return dynamicState; }
private:
	static VkShaderModule CreateShaderModule(Vulkan* vulkan, const std::vector<char>& code);
	VkPipelineLayout pipelineLayout;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	Vulkan* vulkanInstance;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	VkPipelineViewportStateCreateInfo viewportState;    
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineDepthStencilStateCreateInfo depthStencil;
	VkPipelineColorBlendStateCreateInfo colorBlending;
	VkPipelineDynamicStateCreateInfo dynamicState;

	std::vector<VkDynamicState> dynamicStates;
	std::vector<VkDescriptorSetLayout> layout;
	VkVertexInputBindingDescription bindingDescription;
	VkVertexInputAttributeDescription* attributeDescriptions;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;

};

class RenderPipeline
{
public:
	RenderPipeline(Vulkan* vulkan, PipelineLayout* pipelineLayout, VkRenderPass& renderPass);
	~RenderPipeline(){vkDestroyPipeline(vulkanInstance->GetDevice(), pipeline, nullptr); }
	VkPipeline& GetPipeline() { return pipeline; }
	void Bind(Vulkan* vulkan);
private:
	VkPipeline pipeline;
	Vulkan* vulkanInstance;
};

