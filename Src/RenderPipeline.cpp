#include "../Headers/RenderPipeline.h"

PipelineLayout::PipelineLayout(Vulkan* vulkan, std::string vert, std::string frag, std::vector<DescriptorSetLayout*> layouts, VkCullModeFlags cullMode, bool onlyVertex)
{
    vulkanInstance = vulkan;
    auto vertShaderCode = Common::ReadShaderFile(vert);
    auto fragShaderCode = Common::ReadShaderFile(frag);

    vertShaderModule = CreateShaderModule(vulkan, vertShaderCode);
    fragShaderModule = CreateShaderModule(vulkan, fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    bindingDescription = Vertex::GetBindingDescription();
    uint attributeCount = onlyVertex ? 1 : 4;
    attributeDescriptions = new VkVertexInputAttributeDescription[attributeCount];
    if (onlyVertex) {
        attributeDescriptions[0] = Vertex::GetVertexAttributeDescriptions();
    }
    else {
        attributeDescriptions[0] = Vertex::GetAttributeDescriptions()[0];
        attributeDescriptions[1] = Vertex::GetAttributeDescriptions()[1];
        attributeDescriptions[2] = Vertex::GetAttributeDescriptions()[2];
        attributeDescriptions[3] = Vertex::GetAttributeDescriptions()[3];
    }

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = attributeCount;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

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
    rasterizer.cullMode = cullMode; //VK_CULL_MODE_BACK_BIT
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vulkan->GetMsaaSamples();

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


    for (int i = 0; i < layouts.size(); i++) {
        layout.push_back(layouts[i]->GetHandle());
    }
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layout.size());
    pipelineLayoutInfo.pSetLayouts = layout.data();

    if (vkCreatePipelineLayout(vulkan->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyShaderModule(vulkanInstance->GetDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(vulkanInstance->GetDevice(), vertShaderModule, nullptr);
    vkDestroyPipelineLayout(vulkanInstance->GetDevice(), pipelineLayout, nullptr);
    delete[] attributeDescriptions;
}

VkShaderModule PipelineLayout::CreateShaderModule(Vulkan* vulkan, const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(vulkan->GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

RenderPipeline::RenderPipeline(Vulkan* vulkan, PipelineLayout* pipelineLayout, VkRenderPass& renderPass)
{

    vulkanInstance = vulkan;
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = pipelineLayout->GetShaderStages().size();
    pipelineInfo.pStages = pipelineLayout->GetShaderStages().data();
    pipelineInfo.pVertexInputState = &pipelineLayout->GetVertexInputInfo();
    pipelineInfo.pInputAssemblyState = &pipelineLayout->GetInputAssemblyInfo();
    pipelineInfo.pViewportState = &pipelineLayout->GetViewportStateInfo();
    pipelineInfo.pRasterizationState = &pipelineLayout->GetRasterizerInfo();
    pipelineInfo.pMultisampleState = &pipelineLayout->GetMultisamplingInfo();
    pipelineInfo.pDepthStencilState = &pipelineLayout->GetDepthInfo();
    pipelineInfo.pColorBlendState = &pipelineLayout->GetColorBlendingInfo();
    pipelineInfo.pDynamicState = &pipelineLayout->GetDynamicStateInfo();
    pipelineInfo.layout = pipelineLayout->GetHandle();
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;




    if (vkCreateGraphicsPipelines(vulkan->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

void RenderPipeline::Bind(Vulkan* vulkan)
{
    vkCmdBindPipeline(vulkan->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}
