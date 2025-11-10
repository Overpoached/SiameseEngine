#include "sepch.h"

#include "Window.h"

#include <GLFW/glfw3.h>

#include "core/Log.h"
#include "input/InputManager.h"
#include "core/Clock.h"

using namespace sshared;

static std::shared_ptr<sengine::InputManager> s_inputManagerPtr = nullptr;
static std::shared_ptr<sengine::Clock> s_clock = nullptr;

Window::~Window()
{
    if (m_window)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::PollEvents() const
{
    glfwPollEvents();
}

void Window::SetUpGlfwInputCallbacks(std::shared_ptr<sengine::InputManager> inputManagerPtr, std::shared_ptr<sengine::Clock> clockPtr)
{
    if (!m_window)
    {
        SENGINE_ERROR("Tried to add glfw callbacks before the glfw window has been created!");
        return;
    }
    if (s_inputManagerPtr)
    {
        SENGINE_ERROR("glfw callbacks have already been added in this runtime!");
        return;
    }
    s_inputManagerPtr = inputManagerPtr;
    s_clock = clockPtr;
    glfwSetKeyCallback(m_window, [](GLFWwindow*, int key, int, int action, int)
        {
            if (s_inputManagerPtr)
            {
                sengine::KeyState actionImpl{ sengine::KeyState::Idle };
                if (action == GLFW_PRESS) actionImpl = sengine::KeyState::Pressed;
                else if (action == GLFW_RELEASE) actionImpl = sengine::KeyState::Released;
                else return;
                s_inputManagerPtr->ProcessKey(static_cast<sengine::KeyCode>(key), actionImpl, s_clock->CurrentTick());
            }
        });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow*, int button, int action, int)
        {
            if (s_inputManagerPtr)
            {
                sengine::KeyState actionImpl{ sengine::KeyState::Idle };
                if (action == GLFW_PRESS) actionImpl = sengine::KeyState::Pressed;
                else if (action == GLFW_RELEASE) actionImpl = sengine::KeyState::Released;
                else return;
                s_inputManagerPtr->ProcessMouseButton(static_cast<sengine::MouseButton>(button), actionImpl, s_clock->CurrentTick());
            }
        });

    glfwSetCharCallback(m_window, [](GLFWwindow*, unsigned int codepoint)
        {
            if (s_inputManagerPtr)
                s_inputManagerPtr->ProcessChar(codepoint);
        });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow*, double x, double y)
        {
            if (s_inputManagerPtr)
                s_inputManagerPtr->ProcessMousePosition(x, y);
        });

    glfwSetScrollCallback(m_window, [](GLFWwindow*, double xoffset, double yoffset)
        {
            if (s_inputManagerPtr)
                s_inputManagerPtr->ProcessScroll(xoffset, yoffset);
        });
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

Window* Window::Create(const WindowConfig& config)
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

void Window::InitGlfw()
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");
    glfwSetErrorCallback(GlfwErrorCallback);
}

void Window::TerminateGlfw()
{
    glfwTerminate();
}

void sshared::Window::GlfwErrorCallback(int _error, const char* description)
{
    SENGINE_ERROR("GLFW Error ({0}): {1}", _error, description);
}
