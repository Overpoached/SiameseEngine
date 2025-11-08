#pragma once

#include "core/Core.h"

namespace sengine
{
	class Application;
	class Clock
	{
	public:
#if defined(CLOCK_MILLISECOND)
		using Duration = std::chrono::milliseconds;
		static constexpr inline double TIME_UNIT_CONVERSION{ 1000.0 };
#elif defined(CLOCK_MICROSECOND)
		using Duration = std::chrono::microseconds;
		static constexpr inline double TIME_UNIT_CONVERSION{ 1000000.0 };
#elif defined(CLOCK_NANOSECOND)
		using Duration = std::chrono::nanoseconds;
		static constexpr inline double TIME_UNIT_CONVERSION{ 1000000000.0 };
#endif
		Clock(Application* app) : m_applicationRef{ app } {}
		~Clock() = default;

		void Start();
		void Update();

		double DeltaTime() const { return m_deltaTime; }
		double CurrentTime() const { return m_currentTime; }
		int64_t CurrentTick() const { return m_currentTick; }
		
	private:
		Application* m_applicationRef;

		std::chrono::steady_clock::time_point m_currentTimepoint;
		std::chrono::steady_clock::time_point m_previousTimepoint;
		int64_t m_currentTick{};
		int64_t m_previousTick{};
		double m_currentTime;
		double m_deltaTime{};
	};
}