#include "Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Headers/VBO.h"
#include "../Headers/Image.h"
#include "../Headers/Texture.h"
#include "../Headers/UniformBuffer.h"
#include "../Headers/DescriptorSet.h"
#include "../Headers/RenderPipeline.h"
#include "../Headers/GUI.h"
#include "../Headers/RenderPass.h"

Renderer::Renderer(const char* app_name, shared_ptr<GLFW_Window> window)
{
    //Save current window
    target_window = window;

    // Create a new Vulkan instance, which is the entry point for all Vulkan applications
    // The first parameter indicates whether to initialize debug messages and validation
    // (change when implementing debug build and release)

    instance = make_shared<VulkanInstance>(true, app_name);

    // Create a surface object that represents the window surface, which is the window or monitor on which the images will be displayed
    surface = make_shared<Surface>(instance, window);

    // Creates and selects the Device to use
    device = make_shared<VulkanDevice>(instance, surface);

    // Create a swap chain, which is a collection of images that can be presented to the surface
    // Also creates image views, which describe how to access the images in the swap chain
    swapchain = make_shared<Swapchain>(device, surface, window);


    // Create a render pass, which describes the attachments and subpasses used in the rendering process
    // The Default and Viewport Renderpasses have identical fields, but were kept as separate for configuration
    // Default render pass config

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain->get_format().format;

    colorAttachment.samples = device->get_multisampling_flags();

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = device->get_depth_format();
    depthAttachment.samples = device->get_multisampling_flags();

    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapchain->get_format().format;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //Viewport render pass config

    VkAttachmentDescription view_colorAttachment{};
    view_colorAttachment.format = swapchain->get_format().format;
    view_colorAttachment.samples = device->get_multisampling_flags();
    view_colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    view_colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    view_colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    view_colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    view_colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    view_colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription view_depthAttachment{};
    view_depthAttachment.format = device->get_depth_format();
    view_depthAttachment.samples = device->get_multisampling_flags();
    view_depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    view_depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    view_depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    view_depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    view_depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    view_depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription view_colorAttachmentResolve{};
    view_colorAttachmentResolve.format = swapchain->get_format().format;
    view_colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    view_colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    view_colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    view_colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    view_colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    view_colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    view_colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Create a render pass, which describes the attachments and subpasses used in the rendering process

    RenderPassBuilder pass_builder;

    default_renderpass = pass_builder
        .DefineColorAttachment(colorAttachment, 0)
        .DefineDepthAttachment(depthAttachment, 1)
        .DefineColorResolveAttachment(colorAttachmentResolve, 2)
        .DefineSubpass(0, 1, 2)
        .Build(device);

    RenderPassBuilder view_builder;

    viewport_renderpass = view_builder
        .DefineColorAttachment(view_colorAttachment, 0)
        .DefineDepthAttachment(view_depthAttachment, 1)
        .DefineColorResolveAttachment(view_colorAttachmentResolve, 2)
        .DefineSubpass(0, 1, 2)
        .Build(device);
    
    // Descriptor Set Layouts

    VkDescriptorSetLayoutBinding cameraBufferBinding;
    cameraBufferBinding.binding = 0;
    cameraBufferBinding.descriptorCount = 1;
    cameraBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    cameraBufferBinding.pImmutableSamplers = nullptr;
    cameraBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding modelBufferBinding;
    modelBufferBinding.binding = 0;
    modelBufferBinding.descriptorCount = 1;
    modelBufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    modelBufferBinding.pImmutableSamplers = nullptr;
    modelBufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding textureLayoutBinding;
    textureLayoutBinding.binding = 1;
    textureLayoutBinding.descriptorCount = 1;
    textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    textureLayoutBinding.pImmutableSamplers = nullptr;
    textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding skyboxTexLayoutBinding;
    skyboxTexLayoutBinding.binding = 2;
    skyboxTexLayoutBinding.descriptorCount = 1;
    skyboxTexLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    skyboxTexLayoutBinding.pImmutableSamplers = nullptr;
    skyboxTexLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    cameraDescriptorSetLayout = make_shared<DescriptorSetLayout>(device, 0);
    cameraDescriptorSetLayout->AddBindings(cameraBufferBinding);
    cameraDescriptorSetLayout->CreateDescriptorSetLayout();

    skyboxDescriptorSetLayout = make_shared<DescriptorSetLayout>(device, 0);
    skyboxDescriptorSetLayout->AddBindings(cameraBufferBinding);
    modelBufferBinding.binding = 1;
    skyboxDescriptorSetLayout->AddBindings(modelBufferBinding);
    skyboxDescriptorSetLayout->AddBindings(skyboxTexLayoutBinding);
    skyboxDescriptorSetLayout->CreateDescriptorSetLayout();

    lightDescriptorSetLayout = make_shared<DescriptorSetLayout>(device, 1);
    cameraBufferBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    lightDescriptorSetLayout->AddBindings(cameraBufferBinding);
    lightDescriptorSetLayout->CreateDescriptorSetLayout();

    modelDescriptorSetLayout = make_shared<DescriptorSetLayout>(device, 2);
    modelBufferBinding.binding = 0;
    modelDescriptorSetLayout->AddBindings(modelBufferBinding);
    modelDescriptorSetLayout->AddBindings(textureLayoutBinding);
    textureLayoutBinding.binding = 2;
    modelDescriptorSetLayout->AddBindings(textureLayoutBinding);
    modelDescriptorSetLayout->CreateDescriptorSetLayout();

    // Create Pipeline Layouts

    PipelineLayoutBuilder layout_builder;

    graphics_layout = layout_builder
        .AddDescriptorSet(cameraDescriptorSetLayout)
        .AddDescriptorSet(lightDescriptorSetLayout)
        .AddDescriptorSet(modelDescriptorSetLayout)
        .Build(device);

    skybox_layout = layout_builder.ClearDescriptorSets()
        .AddDescriptorSet(skyboxDescriptorSetLayout)
        .Build(device);

    // Compile all Shader Modules

    ShaderModule default_vertex("shaders/target/defaultVert.spv", device);
    ShaderModule default_fragment("shaders/target/defaultFrag.spv", device);

    ShaderModule skybox_vertex("shaders/target/boxVert.spv", device);
    ShaderModule skybox_fragment("shaders/target/boxFrag.spv", device);

    // Create a graphics pipeline, which describes the stages of the rendering pipeline and how data is processed at each stage

    PipelineBuilder graphics_pipeline_builder;

    graphics_pipeline_builder
        .AddVertexFormat(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions())
        .SetMultisamplingFlags(device->get_multisampling_flags())
        .AddVertexShader(default_vertex.handle())
        .AddFragmentShader(default_fragment.handle());

    graphics_pipeline = graphics_pipeline_builder.Build(device, default_renderpass, graphics_layout);
    viewport_pipeline = graphics_pipeline_builder.Build(device, viewport_renderpass, graphics_layout);

    PipelineBuilder skybox_pipeline_builder;

    skybox_pipeline_builder
        .AddVertexFormat(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions())
        .AddVertexShader(skybox_vertex.handle())
        .SetMultisamplingFlags(device->get_multisampling_flags())
        .AddFragmentShader(skybox_fragment.handle())
        .SetCullMode(VK_CULL_MODE_FRONT_BIT)
        .SetDepthTesting(false);

    skybox_pipeline = skybox_pipeline_builder.Build(device, default_renderpass, skybox_layout);

    // Create Sampler
    texture_sampler = make_shared<TextureSampler>(device);
    
    // Create a command pool, which is used to allocate command buffers for rendering commands

    default_command_pool = make_shared<CommandPool>(device, MAX_FRAMES_IN_FLIGHT);
    viewport_command_pool = make_shared<CommandPool>(device, MAX_FRAMES_IN_FLIGHT);

    //Create ImageView Resources for rendering

    CreateColorResources();
    CreateDepthResources();

    // Create framebuffers, which are collections of attachments that represent the render targets for each subpass in the render pass
    CreateFramebuffers();
  

    // Create synchronization objects, which are used to coordinate the execution of commands between the CPU and GPU
    CreateSyncObjects();

    //
    CreateViewport();

}

Renderer::~Renderer()
{
    CleanupSwapchain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device->handle(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device->handle(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device->handle(), inFlightFences[i], nullptr);
    }


    for (auto framebuffer : viewportFramebuffers) {
        vkDestroyFramebuffer(device->handle(), framebuffer, nullptr);
    }

    for (auto imageView : viewportImageViews) {
        vkDestroyImageView(device->handle(), imageView, nullptr);
    }

    for (auto images : viewportImages) {
        delete images;
    }
}

void Renderer::UIRenderPass(ImDrawData* draw_data)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(default_command_pool->get_buffer(currentFrame), &beginInfo) != VK_SUCCESS) {
        throw runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = default_renderpass->handle();
    renderPassInfo.framebuffer = swapChainFramebuffers[currentImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapchain->get_extent();

    array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(default_command_pool->get_buffer(currentFrame), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(default_command_pool->get_buffer(currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline->handle());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchain->get_extent().width;
    viewport.height = (float)swapchain->get_extent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(default_command_pool->get_buffer(currentFrame), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchain->get_extent();
    vkCmdSetScissor(default_command_pool->get_buffer(currentFrame), 0, 1, &scissor);

    ImGui_ImplVulkan_RenderDrawData(draw_data, default_command_pool->get_buffer(currentFrame));

    vkCmdEndRenderPass(default_command_pool->get_buffer(currentFrame));

    if (vkEndCommandBuffer(default_command_pool->get_buffer(currentFrame)) != VK_SUCCESS) {
        throw runtime_error("failed to record command buffer!");
    }
}

void* Renderer::MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size)
{
    void* mappedData = nullptr;
    VkResult result = vkMapMemory(device->handle(), memory, offset, size, 0, &mappedData);

    if (result != VK_SUCCESS) {
        throw runtime_error("Failed to map memory!");
        return nullptr;
    }

    return mappedData;
}

void Renderer::UnmapMemory(VkDeviceMemory memory)
{
    vkUnmapMemory(device->handle(), memory);
}

void Renderer::CreateViewport()
{
    //Create Images
    viewportImages.resize(swapchain->get_image_count());
    viewportImageViews.resize(viewportImages.size());

    for (int i = 0; i < viewportImages.size(); i++) {
        viewportImages[i] = new Image(this, swapchain->get_extent().width, swapchain->get_extent().height, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1, VK_SAMPLE_COUNT_1_BIT, 1);
        viewportImages[i]->TransitionImageLayout(this, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,1, 0);
        viewportImageViews[i] = Image::CreateImageView(this, viewportImages[i]->GetImage(), VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, 1, 1, 0);
    }

    //Create frame buffers
    viewportFramebuffers.resize(viewportImageViews.size());

    for (size_t i = 0; i < viewportImageViews.size(); i++)
    {
        array<VkImageView, 3> attachments = {
                colorImageView,
                depthImageView,
                viewportImageViews[i],
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = viewport_renderpass->handle();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchain->get_extent().width;
        framebufferInfo.height = swapchain->get_extent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->handle(), &framebufferInfo, nullptr, &viewportFramebuffers[i]) != VK_SUCCESS)
        {
            throw runtime_error("failed to create framebuffer!");
        }
    }
    

}


void Renderer::WaitForFences(shared_ptr<GLFW_Window> window)
{
    // Wait for the previous frame to finish before starting a new one
    // This is done to avoid overwriting or accessing resources that are still in use
    vkWaitForFences(device->handle(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // Get the index of the next available image in the swap chain
    currentResult = vkAcquireNextImageKHR(device->handle(), swapchain->handle(),
        UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &currentImageIndex);

    // If the swap chain needs to be recreated, do so and return without drawing the current frame
    if (currentResult == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapchain();
        return;
    }
    // If acquiring the image failed for any other reason, throw a runtime error
    else if (currentResult != VK_SUCCESS && currentResult != VK_SUBOPTIMAL_KHR) {
        throw runtime_error("failed to acquire swap chain image!");
    }
}

void Renderer::ResetFences(shared_ptr<GLFW_Window> window)
{
    // Reset the fence to the unsignaled state before submitting the command buffer
    // This is necessary because vkQueueSubmit waits on the fence to know when the command buffer has finished executing
    vkResetFences(device->handle(), 1, &inFlightFences[currentFrame]);

    // Record the command buffer that will draw the scene onto the acquired image
    vkResetCommandBuffer(default_command_pool->get_buffer(currentFrame), 0);
}

void Renderer::EndDrawFrame(shared_ptr<GLFW_Window> window)
{
    // Submit the command buffer to the graphics queue for execution
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Specify which semaphores to wait on and in which pipeline stage(s) to wait
    // We want to wait for the acquired image to be available before writing colors to it
    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    // Specify which command buffers to submit for execution
    // We only have one command buffer, so we submit it
    vector<VkCommandBuffer> buffers;
    buffers.push_back(default_command_pool->get_buffer(currentFrame));
    buffers.push_back(viewport_command_pool->get_buffer(currentFrame));

    submitInfo.commandBufferCount = 2;
    submitInfo.pCommandBuffers = buffers.data();

    // Specify which semaphores to signal once the command buffer(s) have finished execution
    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit the command buffer to the graphics queue
    // The fence is signaled when the command buffer has finished executing
    if (vkQueueSubmit(device->get_graphics_queue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw runtime_error("failed to submit draw command buffer!");
    }

    // Present the rendered image to the screen by submitting it back to the swap chain
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    // Specify which semaphore to wait on before presenting the image
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // Specify the swap chain and image to present
    VkSwapchainKHR swapChains[] = { swapchain->handle() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &currentImageIndex;
    presentInfo.pResults = nullptr; // Optional

    // Submit the present request to the presentation queue
    // The function returns a result code indicating whether the presentation was successful or not
    currentResult = vkQueuePresentKHR(device->get_present_queue(), &presentInfo);

    // If the swap chain needs to be recreated, do so and return without drawing the current frame
    if (currentResult == VK_ERROR_OUT_OF_DATE_KHR || currentResult == VK_SUBOPTIMAL_KHR || window->framebufferResized) {
        window->framebufferResized = false;
        RecreateSwapchain();
    }
    // If presenting the image failed for any other reason, throw a runtime error
    else if (currentResult != VK_SUCCESS) {
        throw runtime_error("failed to present swap chain image!");
    }

    // Move to the next frame by incrementing the current frame index
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


void Renderer::RecreateSwapchain()
{
    target_window->WindowMinimization();
    device->Flush();

    CleanupSwapchain();

    swapchain = make_shared<Swapchain>(device, surface, target_window);
    CreateColorResources();
    CreateDepthResources();
    CreateFramebuffers();
}

void Renderer::CleanupSwapchain()
{
    vkDestroyImageView(device->handle(), depthImageView, nullptr);
    delete depthImage;

    vkDestroyImageView(device->handle(), colorImageView, nullptr);
    delete colorImage;

    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device->handle(), framebuffer, nullptr);
    }

}

void Renderer::CreateFramebuffers()
{
    swapChainFramebuffers.resize(swapchain->get_image_count());

    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {

        array<VkImageView, 3> attachments = {
                    colorImageView,
                    depthImageView,
                    swapchain->get_view(i)
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = default_renderpass->handle();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchain->get_extent().width;
        framebufferInfo.height = swapchain->get_extent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->handle(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw runtime_error("failed to create framebuffer!");
        }
    }
}

void Renderer::StartRenderPass()
{
    for (int i = 0; i < viewportImages.size(); i++)
    {
        viewportImages[i]->TransitionImageLayout(this, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,1, 0);
    }

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(viewport_command_pool->get_buffer(currentFrame), &beginInfo) != VK_SUCCESS) {
        throw runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = viewport_renderpass->handle();
    renderPassInfo.framebuffer = viewportFramebuffers[currentImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapchain->get_extent();

    array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(viewport_command_pool->get_buffer(currentFrame), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    //vkCmdBindPipeline(viewportCommandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, viewportPipeline->GetPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchain->get_extent().width;
    viewport.height = (float)swapchain->get_extent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(viewport_command_pool->get_buffer(currentFrame), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchain->get_extent();
    vkCmdSetScissor(viewport_command_pool->get_buffer(currentFrame), 0, 1, &scissor);
}

void Renderer::EndRenderPass()
{
    vkCmdEndRenderPass(viewport_command_pool->get_buffer(currentFrame));

    if (vkEndCommandBuffer(viewport_command_pool->get_buffer(currentFrame)) != VK_SUCCESS) {
        throw runtime_error("failed to record command buffer!");
    }

}

void Renderer::CreateSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    //Create the fence in the signaled state, so that the first call to vkWaitForFences()
    //returns immediately since the fence is already signaled.
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device->handle(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device->handle(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device->handle(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Renderer::CreateColorResources()
{
    VkFormat colorFormat = swapchain->get_format().format;

    colorImage = new Image(this, swapchain->get_extent().width, swapchain->get_extent().height, colorFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        1, device->get_multisampling_flags(), 1
    );

    colorImageView = Image::CreateImageView(this, colorImage->GetImage(), VK_IMAGE_VIEW_TYPE_2D, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1,1, 0);
}


void Renderer::CreateDepthResources()
{
    VkFormat depthFormat = device->get_depth_format();

    depthImage = new Image(this, swapchain->get_extent().width, swapchain->get_extent().height, depthFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        1, device->get_multisampling_flags(), 1
    );

    depthImageView = Image::CreateImageView(this, depthImage->GetImage(), VK_IMAGE_VIEW_TYPE_2D, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, 1, 0);
    //depthImage->TransitionImageLayout(this, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1, 0);
}


VkCommandBuffer Renderer::BeginSingleTimeCommands()
{
    VkCommandBuffer commandBuffer = default_command_pool->allocate_one_time_buffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Renderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->get_graphics_queue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->get_graphics_queue());

    default_command_pool->free_one_time_buffer(commandBuffer);
}

void Renderer::InitVulkanImGUI(DescriptorPool* pool)
{
    ImGui_ImplVulkan_InitInfo init_info = GetImGUIInitInfo(pool);
    ImGui_ImplVulkan_Init(&init_info, default_renderpass->handle());
    
    // Use any command queue
    VkCommandPool command_pool = default_command_pool->handle();
    VkCommandBuffer command_buffer = default_command_pool->get_buffer(currentFrame);

    VkResult err = vkResetCommandPool(device->handle(), command_pool, 0);
    CHECK_VK(err);
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(command_buffer, &begin_info);
    CHECK_VK(err);

    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &command_buffer;
    err = vkEndCommandBuffer(command_buffer);
    CHECK_VK(err);
    err = vkQueueSubmit(device->get_graphics_queue(), 1, &end_info, VK_NULL_HANDLE);
    CHECK_VK(err);

    err = vkDeviceWaitIdle(device->handle());
    CHECK_VK(err);
    ImGui_ImplVulkan_DestroyFontUploadObjects();

}

VkPipelineLayout Renderer::GetViewportPipelineLayout()
{
    return graphics_layout->handle();
}

VkPipelineLayout Renderer::GetBoxPipelineLayout()
{
    return skybox_layout->handle();
}

ImGui_ImplVulkan_InitInfo Renderer::GetImGUIInitInfo(DescriptorPool* pool)
{
    if (instance->handle() == VK_NULL_HANDLE)
        cout << "Instance not initialized - Null Handle" << endl;

    ImGui_ImplVulkan_InitInfo init_info;
    init_info.Instance = instance->handle();
    init_info.PhysicalDevice = device->physical();
    init_info.Device = device->handle();
    init_info.QueueFamily = device->get_queue_family_indices().graphicsFamily.value();
    init_info.Queue = device->get_graphics_queue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = pool->handle();
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = device->get_multisampling_flags();
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = CHECK_VK;
    return init_info;
}



