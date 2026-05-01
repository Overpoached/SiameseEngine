#include <EntryPoint.h>

#include <Renderer.h>

#include <core/Log.h>

#include <window/Window.h>

using namespace srenderer;

Renderer::Application* sengine::CreateApplication()
{
	return new srenderer::Renderer();
}

void Renderer::Init()
{
	//call the base app init
	sengine::Application::Init();

	//create and setup the glfw window
	sengine::Window::InitGlfw();
	//creating the window creates the swap chain etc
	m_mainWindow = sengine::Window::Create({ "SiameseRenderer", 1280, 720 });
	m_mainWindow->SetUpGlfwInputCallbacks(m_inputManager, m_clock);
	m_mainWindow->CreateSwapChain(m_deviceManager);
	//create and add all the renderpasses
}

void srenderer::Renderer::Tick()
{
	//poll the events first
	m_mainWindow->PollEvents();
	m_running = !m_mainWindow->ShouldClose();
	//do the base app tick
	Application::Tick();
	//grab the final buffer and copies it onto the screen, then get glfw to swap it
	//maybe allow a system that can force the rdg to keep certain debug buffer so we can display them
}

void Renderer::Release()
{
	delete m_mainWindow;
	sengine::Window::TerminateGlfw();
}
