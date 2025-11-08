#include "sepch.h"
#include "Application.h"

//inifile-cpp
#include "inicpp.h"

#include "core/Log.h"
#include "core/CrashHandler.h"
#include "core/Clock.h"

#include "input/InputManager.h"

using namespace sengine;

void Application::Init()
{
	//load the config file
	LoadConfig(CONFIG_PATH);
	//initialize the logger
	m_engineLogger = std::make_shared<Logger>("SENGINE", m_config.logFileName);
	//set the logger service to use the engine logger as well for areas where access from global scope is needed
	LoggerService::Register(m_engineLogger);
    m_engineLogger->Info("Engine logger initialized");
	//setup crash handler
	CrashHandler::Install();
	//create the clock
	m_clock = std::make_shared<Clock>(this);
	//create the input manager
	m_inputManager = std::make_shared<InputManager>();
	m_inputManager->SetRepeatDelay(m_config.inputConfig.repeatDelay);
	m_inputManager->SetRepeatRate(m_config.inputConfig.repeatRate);

	//create modules
}

void Application::Run()
{
	while (m_running)
	{
		Tick();
	}
}

void Application::Release()
{
}

void sengine::Application::Tick()
{
	BeginFrame();

	m_updateAccumulator += m_clock->DeltaTime();
	m_fixedUpdateAccumulator += m_clock->DeltaTime();
	while (m_updateAccumulator >= m_config.targetFrameTime)
	{
		double dt = m_config.targetFrameTime;
		PreUpdate(dt);
		Update(dt);
		PostUpdate(dt);
		m_updateAccumulator -= m_config.targetFrameTime;
	}
	while (m_fixedUpdateAccumulator >= m_config.targetFixedFrameTime)
	{
		double dt = m_config.targetFixedFrameTime;
		PreFixedUpdate(dt);
		FixedUpdate(dt);
		PostFixedUpdate(dt);
		m_fixedUpdateAccumulator -= m_config.targetFixedFrameTime;
	}

	EndFrame();
}

void sengine::Application::BeginFrame()
{
	//ticks the clock first
	m_clock->Update();
}

void sengine::Application::PreUpdate(double dt)
{
}

void sengine::Application::Update(double dt)
{
	//updates the input system first with the current time
	m_inputManager->Update(m_clock->CurrentTime());
}

void sengine::Application::PostUpdate(double dt)
{
}

void sengine::Application::PreFixedUpdate(double dt)
{
}

void sengine::Application::FixedUpdate(double dt)
{
}

void sengine::Application::PostFixedUpdate(double dt)
{
}

void sengine::Application::EndFrame()
{
}

void Application::LoadConfig(const std::string& filename)
{
	ini::IniFile configFile;
	//TODO: check if file exist, print a warning and let the config default init
	configFile.load("data/configs/" + filename);

	m_config.logFileName = configFile["sengine"]["logFileName"].as<std::string>();
	m_config.targetFrameRate = configFile["sengine"]["targetFrameRate"].as<double>();
	m_config.targetFrameTime = 1.0 / m_config.targetFrameRate;
	m_config.targetFixedFrameRate = configFile["sengine"]["targetFixedFrameRate"].as<double>();
	m_config.targetFixedFrameTime = 1.0 / m_config.targetFixedFrameRate;
	m_config.maxDeltaTime = configFile["sengine"]["maxDeltaTime"].as<double>();

	m_config.inputConfig.repeatDelay = configFile["sengine.input"]["repeatDelay"].as<double>();
	m_config.inputConfig.repeatRate = configFile["sengine.input"]["repeatRate"].as<double>();
}
