#include "frametime.h"


FrameTime::FrameTime() noexcept
	: m_clock{}, m_timePoint{}, m_deltaTime{}
{
	tick();
}

void FrameTime::tick() noexcept
{
	const std::chrono::steady_clock::time_point prevTimePoint{ m_timePoint };

	m_timePoint = m_clock.now();

	m_deltaTime = m_timePoint - prevTimePoint;
}


float FrameTime::deltaTime() const noexcept
{
	return m_deltaTime.count();
}
