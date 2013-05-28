#include "mainClass.h"




void MainClass::loop()
{
	elapseTime();
	if(m_update == true) { updateAngle(mouseX, mouseY);
	m_update = false;
	}
	perduotiValdyma();
	refresh();
}

void MainClass::perduotiValdyma()
{
	float multiplier = (keymap.shift) ? 1.0 : 0.5;

	if(keymap.w == true) { vaziuoti(multiplier, praejoTime); } 
	if(keymap.s == true) { vaziuoti(-multiplier, praejoTime); } 

	if(keymap.a == true) { strafe(multiplier, praejoTime); } 
	if(keymap.d == true) { strafe(-multiplier, praejoTime); } 

}

void MainClass::elapseTime()
{
praejoTime = timeGetTime() - dwLastFrameTime;
dwLastFrameTime = timeGetTime();
}