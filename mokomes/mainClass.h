#include "kamera.h"
#include "valdymas.h"

#include <windows.h>
#include <mmsystem.h>
#pragma comment( lib, "winmm" )


class MainClass : 	public Kamera,
					public Valdymas
{

private:

	//static float moveMultiplier = 0.2;

	void perduotiValdyma();
	double dwLastFrameTime;
    double praejoTime;



public:
	MainClass() { dwLastFrameTime = timeGetTime(); praejoTime = 0;}
	void elapseTime();
	void loop();



};