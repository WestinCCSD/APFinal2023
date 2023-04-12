#include "Timer.h"

void Timer::Stop()
{
	m_RemainingTicks = SDL_GetTicks() - m_StartTicks;
	SDL_RemoveTimer(m_ID);
	m_Stopped = true;
}

void Timer::Start(uint32_t p_Time)
{
	m_StartTicks = SDL_GetTicks();
	m_ID = SDL_AddTimer
	(
		(p_Time == -1 ? m_WaitTime : p_Time),
		callback,
		this
	);
	m_Stopped = false;
}

void Timer::Resume()
{
	if (m_RemainingTicks != 0)
	{
		Start(m_RemainingTicks);
		m_RemainingTicks = 0;
	}
	else
	{
		Start(-1);
	}
}