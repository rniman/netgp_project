#pragma once
#include <chrono>
#include <array>
#include <Windows.h>

class CGameTimer
{
public:
	CGameTimer();
	~CGameTimer();

	void Tick(const float LockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	double GetDeltaTime() const;
	unsigned long GetFrameRate(LPTSTR lpszString = nullptr, int nCharacters = 0) const;

private:
	std::chrono::time_point<std::chrono::system_clock> m_base;
	std::chrono::time_point<std::chrono::system_clock> m_last;
	std::chrono::time_point<std::chrono::system_clock> m_now;

	std::array<double, 50> m_frameTimes{};
	size_t m_frameTimeCount;

	double m_avgDeltaTime;
	double m_elapsedTime;

	unsigned long m_FPS;
	unsigned long m_frameRate;

	bool m_bStop;
};

