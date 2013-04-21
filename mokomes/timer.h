#ifndef _TIMER
#define _TIMER

#include <Windows.h>


class Timer
{
private:
	bool isPaused;
	float speedMultiplier;
	float deltaTime;
	DWORD startTime;
	DWORD lastTime;



public:
	Timer();
	~Timer();

	void pause();
	void resume();

	void Update();

	void setSpeed(float multiplier);


	bool getPause();
	float getSpeed();
	float getDeltaT();
	DWORD getTime();

};
//timeGetTime();

#endif