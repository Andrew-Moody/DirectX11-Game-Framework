#pragma once

#include <chrono>

class FrameTime
{
public:

	FrameTime() noexcept;

	void tick() noexcept;

	float deltaTime() const noexcept;

private:

	std::chrono::steady_clock m_clock{};
	std::chrono::steady_clock::time_point m_timePoint{};
	std::chrono::duration<float, std::milli> m_deltaTime{};
};
