#include "sepch.h"
#include "Application.h"

//inifile-cpp
#include "inicpp.h"

#include "core/Log.h"
#include "core/CrashHandler.h"

void sengine::Application::Init()
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
}

void sengine::Application::Run()
{
}

void sengine::Application::Release()
{
}

void sengine::Application::LoadConfig(const std::string& filename)
{
	ini::IniFile configFile;
	configFile.load("data/configs/" + filename);

	m_config.logFileName = configFile["sengine"]["logFileName"].as<std::string>();
	m_config.targetFrameRate = configFile["sengine"]["targetFrameRate"].as<double>();
	m_config.targetFrameTime = 1.0 / m_config.targetFrameRate;
}
