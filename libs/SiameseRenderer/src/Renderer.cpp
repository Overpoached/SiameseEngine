#include <EntryPoint.h>

#include <Renderer.h>

srenderer::Renderer::Application* sengine::CreateApplication()
{
	return new srenderer::Renderer();
}