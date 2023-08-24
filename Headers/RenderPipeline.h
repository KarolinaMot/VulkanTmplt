#pragma once
#include "RenderPass.h"
#include "PipelineLayout.h"

class RenderPipeline
{
public:

	RenderPipeline(VkPipeline pipeline_object, shared_ptr<VulkanDevice> device);
	~RenderPipeline();

	VkPipeline handle() { return pipeline; }
	void Bind(VkCommandBuffer command_buffer);

private:

	VkPipeline pipeline;
	shared_ptr<VulkanDevice> associated_device;
};

class PipelineBuilder {
public:

	PipelineBuilder();

	PipelineBuilder& AddVertexFormat(VkVertexInputBindingDescription bindingDescription, vector<VkVertexInputAttributeDescription> attributeDescriptions);

	PipelineBuilder& AddVertexShader(VkShaderModule frag_shader);
	PipelineBuilder& AddFragmentShader(VkShaderModule frag_shader);

	PipelineBuilder& SetCullMode(VkCullModeFlags mode);
	PipelineBuilder& SetMultisamplingFlags(VkSampleCountFlagBits sampling_flags);

	PipelineBuilder& SetDepthTesting(bool depthTestEnable);

	shared_ptr<RenderPipeline> Build(
		shared_ptr<VulkanDevice> device,
		shared_ptr<RenderPass> render_pass,
		shared_ptr<PipelineLayout> layout
	);


	//VkPipelineLayout GetHandle() { return pipelineLayout; }
	//VkShaderModule GetVertShaderModule() { return vertShaderModule; }
	//VkShaderModule GetFragShaderModule() { return fragShaderModule; }
	
	/*vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() { return shaderStages; }
	VkPipelineVertexInputStateCreateInfo& GetVertexInputInfo() { return vertexInputInfo; };
	VkPipelineInputAssemblyStateCreateInfo& GetInputAssemblyInfo() { return inputAssembly; };
	VkPipelineViewportStateCreateInfo& GetViewportStateInfo() { return viewportState; }
	VkPipelineRasterizationStateCreateInfo& GetRasterizerInfo() { return rasterizer; }
	VkPipelineMultisampleStateCreateInfo& GetMultisamplingInfo() { return multisampling; }
	VkPipelineDepthStencilStateCreateInfo& GetDepthInfo() { return depthStencil; }
	VkPipelineColorBlendStateCreateInfo& GetColorBlendingInfo() { return colorBlending; }
	VkPipelineDynamicStateCreateInfo& GetDynamicStateInfo() { return dynamicState; }*/

private:

	
	//static VkShaderModule CreateShaderModule(Vulkan* vulkan, const vector<char>& code);

	//VkPipelineLayout pipelineLayout;

	//VkShaderModule vertShaderModule;
	//VkShaderModule fragShaderModule;

	vector<VkPipelineShaderStageCreateInfo> shaderStages;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	VkPipelineViewportStateCreateInfo viewportState;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineDepthStencilStateCreateInfo depthStencil;
	VkPipelineColorBlendStateCreateInfo colorBlending;

	VkPipelineDynamicStateCreateInfo dynamicState;
	vector<VkDynamicState> dynamicStates;

	vector<VkVertexInputAttributeDescription> attributeDescriptions;

	VkVertexInputBindingDescription bindingDescription;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;

};


