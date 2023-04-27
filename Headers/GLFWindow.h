#pragma once
#include "Common.h"
#include <iostream>

struct Inputs {
    double mousePosX = 0;
    double mousePosY = 0;
};

class GLFWindow
{
public:
	GLFWindow(int w, int h, const char* name) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        win = glfwCreateWindow(w, h, name, nullptr, nullptr);
        glfwSetFramebufferSizeCallback(win, framebufferResizeCallback);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::cout << extensionCount << " extensions supported\n"; 
        inputs = new Inputs();
	}

    ~GLFWindow() {
        glfwDestroyWindow(win);
        glfwTerminate();
    }

    inline int GetWidth() { return w; }
    inline int GetHeight() { return h; }

    void Update() { glfwPollEvents();  glfwGetCursorPos(win, &inputs->mousePosX, &inputs->mousePosY); }
    bool GetClosing() { return glfwWindowShouldClose(win); }
    GLFWwindow* GetWindow() { return win; }
    Inputs* GetInputs() { return inputs; }
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        framebufferResized = true;
    }
    void WindowMinimization() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(win, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(win, &width, &height);
            glfwWaitEvents();
        }
    };
    static bool framebufferResized;
    


private:
    GLFWwindow* win = nullptr;
    Inputs* inputs = nullptr;
    uint32_t extensionCount = 0;

    int w = 0;
    int h = 0;
};

