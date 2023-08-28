#include "GLFW_Window.h"


GLFW_Window::GLFW_Window(int w, int h, const char* name)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    win = glfwCreateWindow(w, h, name, nullptr, nullptr);
    glfwSetFramebufferSizeCallback(win, framebufferResizeCallback);

}

GLFW_Window::~GLFW_Window()
{
    glfwDestroyWindow(win);
    glfwTerminate();
}

void GLFW_Window::Update()
{
    glfwPollEvents();

    //Update Inputs
    double mousePosX, mousePosY;
    glfwGetCursorPos(win, &mousePosX, &mousePosY);
    inputs.mousePos = vec2(mousePosX, mousePosY);

    inputs.w = (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS);
    inputs.a = (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS);
    inputs.s = (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS);
    inputs.d = (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS);
    inputs.q = (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS);
    inputs.e = (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);

    int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
    inputs.leftMousePress = state == GLFW_PRESS;
    state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT);
    inputs.rightMousePress = state == GLFW_PRESS;
}

void GLFW_Window::WindowMinimization()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(win, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(win, &width, &height);
        glfwWaitEvents();
    }
};