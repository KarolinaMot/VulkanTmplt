#pragma once
#include "Common.h"
#include <iostream>

using namespace glm;
struct Inputs {
    vec2 mousePos;
    bool w = false, a = false, s = false, d = false, q = false, e = false;
    int scroll = 0;
    bool leftMousePress = false;
    bool rightMousePress = false;
};

class GLFW_Window
{
public:
    GLFW_Window(int w, int h, const char* name);
    ~GLFW_Window();

    GLFWwindow* handle() { return win; }

    void Update();
    void WindowMinimization();

    void GetSize(int* width, int* height) { glfwGetFramebufferSize(win, width, height); }

    bool GetClosing() { return glfwWindowShouldClose(win); }
    Inputs* GetInputs() { return &inputs; }

    //Callback for Window resize
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        framebufferResized = true;
    }

    static bool framebufferResized;

private:
    
    GLFWwindow* win;
    Inputs inputs;

    uint32_t extensionCount = 0;

    int w = 0;
    int h = 0;
};

