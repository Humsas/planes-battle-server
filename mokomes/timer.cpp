#include "timer.h"

Timer::Timer()
{
	isPaused		= false;
	speedMultiplier = 1.0;
	deltaTime		= 0.0;
	startTime		= timeGetTime();
	lastTime		= startTime;
}

Timer::~Timer()
{
	//nothing to do here
}


void Timer::pause()
{
	isPaused = true;
}

void Timer::resume()
{
	isPaused = false;
}


void Timer::Update()
{
	DWORD t = timeGetTime();	

	if(isPaused)
	{
		deltaTime = 0.0;
	}
	else
	{
		deltaTime = (float) (t - lastTime)/1000;
	}

	if(deltaTime > 1.0) deltaTime = 0;
	deltaTime *= speedMultiplier;

	lastTime = t;
}


void Timer::setSpeed(float speed)
{
	speedMultiplier = speed;
}

bool Timer::getPause()
{
	return isPaused;
}

float Timer::getSpeed()
{
	return speedMultiplier;
}

float Timer::getDeltaT()
{
	return deltaTime;
}

DWORD Timer::getTime()
{
	return lastTime;
}