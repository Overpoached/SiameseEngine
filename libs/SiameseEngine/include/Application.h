#pragma once

namespace sengine
{
    class Logger;
    struct ApplicationConfig
    {
        std::string logFileName;
        double targetFrameRate;
        double targetFrameTime;
    };
    class Application
    {
    public:
        static inline const char* CONFIG_PATH = "sengine.ini";

        virtual void Init();
        virtual void Run();
        virtual void Release();

        void LoadConfig(const std::string& filename);

        ApplicationConfig m_config;
        std::shared_ptr<Logger> m_engineLogger;

    private:
    };

    Application* CreateApplication();
}