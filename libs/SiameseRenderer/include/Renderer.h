#pragma once

#include <Application.h>

namespace sshared
{
	class Window;
}
namespace srenderer
{
	class Renderer : public sengine::Application
	{
	public:
		virtual void Init() override;
		virtual void Release() override;
		
		virtual void Tick() override;

	private:
		sshared::Window* m_mainWindow;
	};
}