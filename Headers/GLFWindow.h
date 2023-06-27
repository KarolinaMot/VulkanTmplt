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

    void Update() { glfwPollEvents();  UpdateInputs(); }
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
    void UpdateInputs() {
        double mousePosX, mousePosY;
        glfwGetCursorPos(win, &mousePosX, &mousePosY);
        inputs->mousePos = vec2(mousePosX, mousePosY);
        inputs->w = (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS);
        inputs->a = (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS);
        inputs->s = (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS);
        inputs->d = (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS);
        inputs->q = (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS);
        inputs->e = (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);

        int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
        inputs->leftMousePress = state == GLFW_PRESS;
        state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT);
        inputs->rightMousePress = state == GLFW_PRESS;
    }
    GLFWwindow* win = nullptr;
    Inputs* inputs = nullptr;
    uint32_t extensionCount = 0;

    int w = 0;
    int h = 0;
};

