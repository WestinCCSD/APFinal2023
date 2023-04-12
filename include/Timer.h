#include <SDL_timer.h>
#pragma once

// simple timer class
// meant to be inherited from
class Timer 
{
public:
	Timer() {}

	virtual uint32_t onTimeout() { return 0; }

	void Start(uint32_t p_Time);
	void Stop();
	void Resume();

	void setWaitTime(uint32_t p_WaitTime = -1)
	{
		m_WaitTime = p_WaitTime;
	}
	
	uint32_t getWaitTime() { return m_WaitTime; }

protected:
	SDL_TimerID m_ID = 0;
	uint32_t m_WaitTime = 1000;
	uint32_t m_StartTicks = 0;
	uint32_t m_RemainingTicks = 0;

};

// exists to easily integrate several timers into one class
// the parent/owning class must pass itself as a parameter into the constructor
// the parent/owning class must have a function called "onTimeout" that returns a uint32_t
template <class T>
class TimerComponent : public Timer
{
public:
	TimerComponent(T& p_Parent)
		: m_Parent(p_Parent)
	{}
	


	inline uint32_t onTimeout() override
	{
		return m_Parent.onTimeout(this);
	}

private:
	T& m_Parent;

};

inline uint32_t callback(uint32_t p_WaitTime, void* p_Timer)
{
	Timer* timer = (Timer*)(p_Timer); // cast void* into timer*
	uint32_t time = timer->onTimeout();
	if (time == -1)
	{
		return timer->getWaitTime();
	}
	else
	{
		return time;
	}
}
