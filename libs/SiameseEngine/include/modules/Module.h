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