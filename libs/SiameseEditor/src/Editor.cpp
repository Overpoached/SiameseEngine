#include <EntryPoint.h>

#include <Application.h>

namespace seditor
{
	class Editor : public sengine::Application
	{
	public:

	private:

	};
}

seditor::Editor::Application* sengine::CreateApplication()
{
	return new seditor::Editor();
}