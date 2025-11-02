#pragma once

namespace sengine
{
	class Application;
	//base class for all engine module to inherit from
	class Module
	{
	public:

	protected:
		std::shared_ptr<Application> m_application;
	};
}

#define SENGINE_TRACE(...) m_application->m_engineLogger->trace(__VA_ARGS__)
#define SENGINE_DEBUG(...) m_application->m_engineLogger->debug(__VA_ARGS__)
#define SENGINE_INFO(...) m_application->m_engineLogger->info(__VA_ARGS__)
#define SENGINE_WARN(...) m_application->m_engineLogger->warn(__VA_ARGS__)
#define SENGINE_ERROR(...) m_application->m_engineLogger->error(__VA_ARGS__)
#define SENGINE_CRITICAL(...) m_application->m_engineLogger->critical(__VA_ARGS__)