#include <EntryPoint.h>

#include <Renderer.h>

#include <window/Window.h>

srenderer::Renderer::Application* sengine::CreateApplication()
{
	return new srenderer::Renderer();
}

void srenderer::Renderer::Init()
{
	sengine::Application::Init();

	sshared::Window::InitGlfw();
	m_mainWindow = sshared::Window::Create({ "SiameseRenderer", 1280, 720 });
}

void srenderer::Renderer::Run()
{
	while (!m_mainWindow->ShouldClose())
	{
		m_mainWindow->PollEvents();
	}
}

void srenderer::Renderer::Release()
{
	delete m_mainWindow;
	sshared::Window::TerminateGlfw();
}
