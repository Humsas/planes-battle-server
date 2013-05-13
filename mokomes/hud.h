#ifndef _HUD
#define _HUD

#include "textureManager.h"
#include "altimeter.h"
#include "speedometer.h"
#include "anglemeter.h"

#include "globalSetings.h"

class Hud
{
private:
	LPDIRECT3DDEVICE9 d3;
	TextureManager *textureManager;

	Altimeter	*altimeter;
	Speedometer *speedometer;
	Anglemeter	*anglemetter;

public:
	Hud(LPDIRECT3DDEVICE9 &d3, TextureManager *textureManager);
	~Hud();

	void Render();

	Altimeter	*getAltimeter();
	Speedometer *getSpeedometer();
	Anglemeter	*getAnglemeter();

};

#endif