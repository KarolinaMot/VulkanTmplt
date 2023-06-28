#include "../Headers/GUI.h"

void GUI::Init(Vulkan* vulkan, DescriptorPool* pool, GLFWindow* window,  Inputs* _inputs, uint scrW, uint scrH)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    inputs = _inputs;
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)scrW, (float)scrH);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window->GetWindow(), true);
    vulkan->InitVulkanImGUI(pool);


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
}

void GUI::EndFrame()
{
    // Rendering
    ImGui::Render();
}

void GUI::SimpleWindow()
{
    ImGuiIO& io = ImGui::GetIO();
    float f = 0.f;
    int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
