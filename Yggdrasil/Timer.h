#pragma once


class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	float Read();
	void Stop();



private:
	bool running = false;
	unsigned long start_time;
	unsigned long  stop_time;
	float current_time;

};

class PerformanceTimer
{
public:

	PerformanceTimer();
	~PerformanceTimer();

    void Start();
	float Read();
	void Stop();

private:
    bool running = false;
	unsigned long start_time;
	unsigned long  stop_time;
	float current_time;
};