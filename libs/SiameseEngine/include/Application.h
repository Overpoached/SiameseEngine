#pragma once

namespace sengine
{
    class Application
    {
    public:
        virtual void Init();
        virtual void Run();
        virtual void Release();

    private:
    };

    Application* CreateApplication();
}