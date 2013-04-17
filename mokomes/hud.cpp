#include "hud.h"

Hud::Hud(LPDIRECT3DDEVICE9 &d3, TextureManager *textureManager)
{
	this->d3 = d3;
	this->textureManager = textureManager;
	int spacing = 130;
	altimeter	= new Altimeter(d3, textureManager, SCREEN_WIDTH-spacing, SCREEN_HEIGHT-spacing, 1.0);
	speedometer = new Speedometer(d3, textureManager, spacing, SCREEN_HEIGHT-spacing, 1.0);
	anglemetter	= new Anglemeter(d3, textureManager, SCREEN_WIDTH-spacing-250, SCREEN_HEIGHT-spacing, 1.0);
}

Hud::~Hud()
{
	if(altimeter != NULL)	delete altimeter;
	if(speedometer != NULL) delete speedometer;
}

void Hud::Render()
{
	altimeter->Render();
	speedometer->Render();
	anglemetter->Render();
}

Altimeter *Hud::getAltimeter()
{
	return altimeter;
}

Speedometer *Hud::getSpeedometer()
{
	return speedometer;
}

Anglemeter *Hud::getAnglemeter()
{
	return anglemetter;
}