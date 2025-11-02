#include "sepch.h"
#include "Application.h"

//inifile-cpp
#include "inicpp.h"

#include "core/Log.h"

void sengine::Application::Init()
{
	//load the config file
	LoadConfig(CONFIG_PATH);
	//initialize the logger
	m_engineLogger = std::make_shared<Logger>("SENGINE", m_config.logFileName);
    m_engineLogger->Info("Engine logger initialized");
}

void sengine::Application::Run()
{
	while (1);
}

void sengine::Application::Release()
{
}

void sengine::Application::LoadConfig(const std::string& filename)
{
	ini::IniFile configFile;
	configFile.load("data/configs/" + filename);

	m_config.logFileName = configFile["sengine"]["logFileName"].as<std::string>();
}
