#pragma once
#include "Vulkan.h"
#include "DescriptorSet.h"

class PipelineLayout {
public:
	PipelineLayout(Vulkan* vulkan);
	void AddVertexFormat(VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions);
	void AddVertexShader(std::string vert);
	void AddFragmentShader(std::string frag);
	void SetCullMode(VkCullModeFlags mode);
	void AddDescriptorSet(DescriptorSetLayout* set);
	void Build();
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
	Vulkan* vulkanInstance;
	static VkShaderModule CreateShaderModule(Vulkan* vulkan, const std::vector<char>& code);
	VkPipelineLayout pipelineLayout;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
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
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

	VkVertexInputBindingDescription bindingDescription;
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

