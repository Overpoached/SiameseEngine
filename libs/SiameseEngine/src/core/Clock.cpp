#include "sepch.h"

#include "core/Clock.h"

#include "Application.h"

void sengine::Clock::Start()
{
	m_currentTimepoint = m_previousTimepoint = std::chrono::steady_clock::now();
	m_currentTick = m_previousTick = 0;
	m_deltaTime = 0.0;
}

void sengine::Clock::Update()
{
	m_currentTimepoint = std::chrono::steady_clock::now();

	auto currentDuration = std::chrono::duration_cast<Duration>(
		m_currentTimepoint.time_since_epoch());

	m_previousTick = m_currentTick;
	m_currentTick = currentDuration.count();
	m_currentTime = m_currentTick / TIME_UNIT_CONVERSION;
	m_deltaTime = static_cast<double>(m_currentTick - m_previousTick) / TIME_UNIT_CONVERSION;
	m_deltaTime = std::min(m_applicationRef->m_config.maxDeltaTime, m_deltaTime);

	m_previousTimepoint = m_currentTimepoint;
}
