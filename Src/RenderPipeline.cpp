#include "../Headers/RenderPipeline.h"

PipelineBuilder::PipelineBuilder()
{
    //Default configuration

    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
}


PipelineBuilder& PipelineBuilder::AddVertexFormat(VkVertexInputBindingDescription _bindingDescription, vector<VkVertexInputAttributeDescription> _attributeDescriptions)
{
    bindingDescription = _bindingDescription;
    attributeDescriptions = _attributeDescriptions;

    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &(this->bindingDescription);
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(this->attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = this->attributeDescriptions.data();
    return *this;
}

PipelineBuilder& PipelineBuilder::AddVertexShader(VkShaderModule vert_shader)
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vert_shader;
    vertShaderStageInfo.pName = "main";
    shaderStages.push_back(vertShaderStageInfo);
    return *this;
}

PipelineBuilder& PipelineBuilder::AddFragmentShader(VkShaderModule frag_shader)
{

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = frag_shader;
    fragShaderStageInfo.pName = "main";
    
    shaderStages.push_back(fragShaderStageInfo);
    return *this;
}

PipelineBuilder& PipelineBuilder::SetCullMode(VkCullModeFlags mode)
{
    rasterizer.cullMode = mode;
    return *this;
}

PipelineBuilder& PipelineBuilder::SetMultisamplingFlags(VkSampleCountFlagBits sampling_flags)
{
    multisampling.rasterizationSamples = sampling_flags;
    return *this;
}


PipelineBuilder& PipelineBuilder::SetDepthTesting(bool depthTestEnable)
{
    if (depthTestEnable) {
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
    }
    else {
        depthStencil.depthTestEnable = VK_FALSE;
        depthStencil.depthWriteEnable = VK_FALSE;

    }
    return *this;
}

shared_ptr<RenderPipeline> PipelineBuilder::Build(
    shared_ptr<VulkanDevice> device,
    shared_ptr<RenderPass> render_pass,
    shared_ptr<PipelineLayout> layout)

{

    VkGraphicsPipelineCreateInfo pipelineInfo{};

    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;

    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;

    pipelineInfo.pViewportState = &viewportState;

    pipelineInfo.pRasterizationState = &rasterizer;

    pipelineInfo.pMultisampleState = &multisampling;

    pipelineInfo.pDepthStencilState = &depthStencil;

    pipelineInfo.pColorBlendState = &colorBlending;

    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = layout->handle();

    pipelineInfo.renderPass = render_pass->handle();
    pipelineInfo.subpass = 0;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline new_pipeline;
    CHECK_VK(vkCreateGraphicsPipelines(device->handle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &new_pipeline));

    //Return
    return make_shared<RenderPipeline>(new_pipeline, device);
}


RenderPipeline::RenderPipeline(VkPipeline pipeline_object, shared_ptr<VulkanDevice> device)
{
    pipeline = pipeline_object;
    associated_device = device;
}

RenderPipeline::~RenderPipeline()
{
    vkDestroyPipeline(associated_device->handle(), pipeline, nullptr);
}

void RenderPipeline::Bind(VkCommandBuffer command_buffer)
{
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}
