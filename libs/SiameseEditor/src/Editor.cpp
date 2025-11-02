#include <EntryPoint.h>

#include <Editor.h>

seditor::Editor::Application* sengine::CreateApplication()
{
	return new seditor::Editor();
}