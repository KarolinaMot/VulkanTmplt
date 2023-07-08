#include "../Headers/GUI.h"

void GUI::Init(Vulkan* vulkan, DescriptorPool* pool, GLFWindow* window,  Inputs* _inputs, uint scrW, uint scrH)
{
    IMGUI_CHECKVERSION();
    vulkanInstance = vulkan;
    ImGui::CreateContext();
    inputs = _inputs;
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)scrW, (float)scrH);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window->GetWindow(), true);
    vulkan->InitVulkanImGUI(pool);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.GrabRounding = style.FrameRounding = 2.3f;

    std::vector<VkImageView> imageViews = vulkanInstance->GetViewportImageViews();
    VkSampler textureSampler = vulkanInstance->GetTextureSampler();

    m_Dset.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); i++)
        m_Dset[i] = ImGui_ImplVulkan_AddTexture(textureSampler, imageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    std::cout << "" << std::endl;

    viewportW = 1920;
    viewportH = 1080;
}

void GUI::StartFrame(float deltaTime)
{
    ImGuiIO& io = ImGui::GetIO();
    bool oldRpi = false;
    io.DeltaTime = deltaTime;
    io.MousePos = ImVec2(inputs->mousePos.x, inputs->mousePos.y);
    io.MouseDown[0] = inputs->leftMousePress;
    io.MouseDown[1] = inputs->rightMousePress;
    io.MouseDrawCursor = oldRpi;


    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplVulkan_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    //ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id2);
    // ImGui::DockBuilderDockWindow("MyDockedWindow", dockspace_id);
    //ImGui::DockBuilderFinish(ImGui::GetID("MyDockSpace"));
}

void GUI::EndFrame()
{
    // Rendering
    ImGui::Render();
}

void GUI::ViewportWindow()
{
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImVec2 viewportWindowSize = ImGui::GetWindowSize();
    viewportW = viewportWindowSize.x;
    viewportH = viewportWindowSize.y;
    ImGui::Image(m_Dset[vulkanInstance->GetCurrentFrame()], ImVec2{viewportPanelSize.x, viewportPanelSize.y});
    ImGui::End();
}

void GUI::SceneWindow(std::vector<GameObject*>& objects)
{
    ImGui::Begin("Scene hierarchy");
    for (int i = 0; i < objects.size(); i++)
    {
        // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
        char label[128];
        sprintf_s(label, objects[i]->GetName().c_str());
        if (ImGui::Selectable(label, selectedObject == i))
            selectedObject = i;
    }
    ImGui::End();

}

void GUI::DetailsWindow(std::vector<GameObject*>& objects)
{
    std::string name = objects[selectedObject]->GetName();
    vec3 position = objects[selectedObject]->GetTransform()->GetPosition();
    quat rotation = objects[selectedObject]->GetTransform()->GetRotation();
    vec3 rotationVec = glm::degrees(glm::eulerAngles(rotation));
    vec3 scale = objects[selectedObject]->GetTransform()->GetScale();

    if (rotationVec.x == -0.f) rotationVec.x *=-1;
    if (rotationVec.y == -0.f) rotationVec.y *= -1;
    if (rotationVec.z == -0.f) rotationVec.z *= -1;
    if (rotationVec.x == -180.f) rotationVec.x = 180.f;
    if (rotationVec.y == -180.f) rotationVec.y = 180.f;
    if (rotationVec.z == -180.f) rotationVec.z = 180.f;

    ImGui::Begin("Details");

    ImGui::SameLine();
    ImGui::InputText("Name##1", &name);
    ImGui::Separator();
    ImGui::Text("Transform");
    ImGui::DragFloat("Position x", &position.x, 0.5f, -FLT_MAX, +FLT_MAX);
    ImGui::DragFloat("Position Y", &position.y, 0.5f, -FLT_MAX, +FLT_MAX);
    ImGui::DragFloat("Position Z", &position.z, 0.5f, -FLT_MAX, +FLT_MAX);

    ImGui::DragFloat("Rotation x", &rotationVec.x, 0.5f, -360, 360);
    ImGui::DragFloat("Rotation Y", &rotationVec.y, 0.5f, -360, 360);
    ImGui::DragFloat("Rotation Z", &rotationVec.z, 0.5f, -360, 360);

    ImGui::DragFloat("Scale x", &scale.x, 0.01f, -FLT_MAX, +FLT_MAX);
    ImGui::DragFloat("Scale Y", &scale.y, 0.01f, -FLT_MAX, +FLT_MAX);
    ImGui::DragFloat("Scale Z", &scale.z, 0.01f, -FLT_MAX, +FLT_MAX);


    ImGui::End();

    if (rotationVec.x == -0.f) rotationVec.x *= -1;
    if (rotationVec.y == -0.f) rotationVec.y *= -1;
    if (rotationVec.z == -0.f) rotationVec.z *= -1;
    if (rotationVec.x == -180.f) rotationVec.x = 180.f;
    if (rotationVec.y == -180.f) rotationVec.y = 180.f;
    if (rotationVec.z == -180.f) rotationVec.z = 180.f;


    objects[selectedObject]->SetName(name);
    objects[selectedObject]->GetTransform()->Move(position);
    objects[selectedObject]->GetTransform()->Rotate(quat(glm::radians(rotationVec)));
    objects[selectedObject]->GetTransform()->Scale(scale);

}

