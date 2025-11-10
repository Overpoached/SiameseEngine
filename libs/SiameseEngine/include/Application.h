#pragma once

namespace sengine
{
    class InputManager;
    class Logger;
    class Clock;
    class DeviceManager;
    struct ApplicationConfig
    {
        std::string logFileName{ "siamese_engine" };
        double targetFrameRate{ 60 };
        double targetFrameTime{ 1.0 / 60.0 };
        double targetFixedFrameRate{ 60.0 };
        double targetFixedFrameTime{ 1.0 / 60.0 };
        double maxDeltaTime{ 0.016666 };

        struct InputConfig
        {
            double repeatDelay{ 0.5 };
            double repeatRate{ 0.05 };
        } inputConfig;
    };
    class Application
    {
    public:
        static inline const char* CONFIG_PATH = "sengine.ini";

        virtual void Init();
        virtual void Run();
        virtual void Release();

        virtual void Tick();
        void BeginFrame();
        void PreUpdate(double dt);
        void Update(double dt);
        void PostUpdate(double dt);
        void PreFixedUpdate(double dt);
        void FixedUpdate(double dt);
        void PostFixedUpdate(double dt);
        void EndFrame();

        void LoadConfig(const std::string& filename);

        ApplicationConfig m_config;
        bool m_running{ true };
        std::shared_ptr<Logger> m_engineLogger;
        std::shared_ptr<Clock> m_clock;
        std::shared_ptr<InputManager> m_inputManager;
        std::shared_ptr<DeviceManager> m_deviceManager;

        //modules that needs to be called at the start of the frame

    private:
        double m_updateAccumulator{};
        double m_fixedUpdateAccumulator{};
    };

    Application* CreateApplication();
}