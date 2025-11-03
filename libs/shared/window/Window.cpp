#include "sepch.h"

#include "Window.h"

#include <GLFW/glfw3.h>

sshared::Window::~Window()
{
    if (m_window)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}

void sshared::Window::PollEvents() const
{
    glfwPollEvents();
}

bool sshared::Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

sshared::Window* sshared::Window::Create(const WindowConfig& config)
{

    // You can choose the API here if needed, e.g., DX12 or OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* windowImpl = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (!windowImpl)
        return nullptr;

    Window* ret = new Window();
    ret->m_config = config;
    glfwSetWindowUserPointer(windowImpl, ret);
    ret->m_window = windowImpl;

    return ret;
}

void sshared::Window::InitGlfw()
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");
}

void sshared::Window::TerminateGlfw()
{
    glfwTerminate();
}
