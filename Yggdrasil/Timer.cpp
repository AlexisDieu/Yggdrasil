#include "Timer.h"
#include "SDL/include/SDL_timer.h"
#include "Globals.h"

Timer::Timer()
{
	Start();
}

Timer::~Timer()
{
}

void Timer::Start()
{
	running = true;
	start_time = SDL_GetTicks();
}

float Timer::Read()
{
	if (running) {
		return SDL_GetTicks() - start_time;
	}
	else {
		return stop_time - start_time;
	}

}

void Timer::Stop()
{
	if (running)
	{
		running = false;
		stop_time = SDL_GetTicks();
	}
}

PerformanceTimer::PerformanceTimer()
{
	Start();
}

PerformanceTimer::~PerformanceTimer()
{
}

void PerformanceTimer::Start()
{
	running = true;
	start_time = SDL_GetPerformanceFrequency();
}

float PerformanceTimer::Read()
{
	static const float frequency = static_cast<float>(SDL_GetPerformanceFrequency());

	unsigned long now_time;
	if (running) {
		now_time = SDL_GetPerformanceCounter() - start_time;
	}
	else {
		now_time = stop_time - start_time;
	}

	current_time = (float)(now_time) * 1000.0 / frequency;
	return current_time;
}

void PerformanceTimer::Stop()
{
	if (running)
	{
		running = false;
		stop_time = SDL_GetPerformanceCounter();
	}
}
