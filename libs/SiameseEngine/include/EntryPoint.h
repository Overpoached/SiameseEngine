#pragma once
#include "Application.h"

int main()
{
#ifdef SE_DEBUG
	// Flag _CrtDumpMemoryLeak to be called AFTER program ends.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	sengine::Application* app = sengine::CreateApplication();
	app->Init();
	app->Run();
	app->Release();
	delete app;
	
	return 0;
}