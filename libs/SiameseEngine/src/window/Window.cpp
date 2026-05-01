#include "sepch.h"

#include "window/Window.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "core/Log.h"
#include "core/Clock.h"
#include "input/InputManager.h"
#include "graphics/DeviceManager.h"

using namespace sengine;

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

bool sengine::Window::CreateSwapChain(SSharedPtr<DeviceManager> deviceManager)
{
	UINT windowStyle = m_config.startFullscreen
		? (WS_POPUP | WS_SYSMENU | WS_VISIBLE)
		: m_config.startMaximized
		? (WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE)
		: (WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	RECT rect = { 0, 0, LONG(m_config.backBufferWidth), LONG(m_config.backBufferHeight) };
	AdjustWindowRect(&rect, windowStyle, FALSE);

	if (deviceManager->MoveWindowOntoAdapter(rect))
	{
		glfwSetWindowPos(m_window, rect.left, rect.top);
	}
	m_hWnd = glfwGetWin32Window(m_window);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);
	UINT width = clientRect.right - clientRect.left;
	UINT height = clientRect.bottom - clientRect.top;

	ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
	m_swapChainDesc.Width = width;
	m_swapChainDesc.Height = height;
	m_swapChainDesc.SampleDesc.Count = m_config.swapChainSampleCount;
	m_swapChainDesc.SampleDesc.Quality = 0;
	m_swapChainDesc.BufferUsage = m_config.swapChainUsage;
	m_swapChainDesc.BufferCount = m_config.swapChainBufferCount;
	m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	m_swapChainDesc.Flags = m_config.allowModeSwitch ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;

	// Special processing for sRGB swap chain formats.
	// DXGI will not create a swap chain with an sRGB format, but its contents will be interpreted as sRGB.
	// So we need to use a non-sRGB format here, but store the true sRGB format for later framebuffer creation.
	switch (m_config.swapChainFormat)  // NOLINT(clang-diagnostic-switch-enum)
	{
	case nvrhi::Format::SRGBA8_UNORM:
		m_swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case nvrhi::Format::SBGRA8_UNORM:
		m_swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;
	default:
		m_swapChainDesc.Format = nvrhi::d3d12::convertFormat(m_config.swapChainFormat);
		break;
	}

	if (deviceManager->GetTearingSupported())
	{
		m_swapChainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}

	m_fullscreenDesc = {};
	m_fullscreenDesc.RefreshRate.Numerator = m_config.refreshRate;
	m_fullscreenDesc.RefreshRate.Denominator = 1;
	m_fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	m_fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	m_fullscreenDesc.Windowed = !m_config.startFullscreen;
	
	//TODO: abort and clean up if window cannot create swap chain properly
	if(!deviceManager->CreateSwapChain(m_hWnd, m_swapChainDesc, m_fullscreenDesc, m_swapChain, m_swapChainBuffers, m_rhiSwapChainBuffers))
		return false;

	//create the fence and events for triple buffering
	deviceManager->CreateFrameFench(m_frameFence);
	for (UINT bufferIndex = 0; bufferIndex < m_swapChainDesc.BufferCount; bufferIndex++)
	{
		m_frameFenceEvents.push_back(CreateEvent(nullptr, false, true, nullptr));
	}

	return true;
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

void sengine::Window::GlfwErrorCallback(int _error, const char* description)
{
    SENGINE_ERROR("GLFW Error ({0}): {1}", _error, description);
}
