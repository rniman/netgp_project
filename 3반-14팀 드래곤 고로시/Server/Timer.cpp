#include "Timer.h"


CGameTimer::CGameTimer()
    : m_base(std::chrono::system_clock::now())
    , m_elapsedTime{ 0 }
    , m_FPS{ 0 }
    , m_frameRate{ 0 }
{
    m_last = m_base;
}

CGameTimer::~CGameTimer()
{
}

void CGameTimer::Tick(const float lockFPS)
{
	if (m_bStop)
	{
		return;
	}

    m_now = std::chrono::system_clock::now();

    std::chrono::duration<double> deltaTime = m_now - m_last;

    if (lockFPS > 0.0f)
    {
        while (deltaTime.count() < 1 / lockFPS)
        {
            m_now = std::chrono::system_clock::now();
            deltaTime = m_now - m_last;
        }
    }

    m_last = m_now;

    if (fabs(deltaTime.count() - m_avgDeltaTime) < 1.0f)
    {
        memmove(&m_frameTimes[1], m_frameTimes.data(), (m_frameTimes.size() - 1) * sizeof(double));
        m_frameTimes[0] = deltaTime.count();
        if (m_frameTimeCount < 50)
        {
            m_frameTimeCount++;
        }
    }

    m_FPS++;
    m_elapsedTime += deltaTime.count();
    if (m_elapsedTime > 1.0f)
    {
        m_frameRate = m_FPS;
        m_FPS = 0;
        m_elapsedTime = 0.0f;
    }

    
    m_avgDeltaTime = 0.0f;
    for (auto& frameTime : m_frameTimes)
    {
        m_avgDeltaTime += frameTime;
    }
    
    if (m_frameTimeCount != 0)
    {
        m_avgDeltaTime /= m_frameTimeCount;
    }
}

void CGameTimer::Start()
{
}

double CGameTimer::GetDeltaTime() const
{
    return m_avgDeltaTime;
}

unsigned long CGameTimer::GetFrameRate(LPTSTR lpszString, int nCharacters) const
{
    if (lpszString)
    {
        _itow_s(m_frameRate, lpszString, nCharacters, 10);
        wcscat_s(lpszString, nCharacters, L" FPS)");
    }

    return(m_frameRate);
}
