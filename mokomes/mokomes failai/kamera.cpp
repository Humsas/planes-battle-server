#include "kamera.h"

void Kamera::normalizeAngle()
{
	if(this->wiewAngleH > 360 || this->wiewAngleH < -360) { this->wiewAngleH-= 360 * ((int)this->wiewAngleH/360); }
	//if(this->wiewAngleV > 360 || this->wiewAngleV < -360) { this->wiewAngleV-= 360 * ((int)this->wiewAngleV/360); }

	if(this->wiewAngleV > 89) { this->wiewAngleV = 89; }
	if(this->wiewAngleV < -89) { this->wiewAngleV = -89; }
}

void Kamera::spinCam()
{
	this->spinZ = sin(this->wiewAngleV * 3.14159265/180); // aukstis nuo horizontalios plokstumos
	this->spinX = cos(this->wiewAngleH * 3.14159265/180) * cos(this->wiewAngleV * 3.14159265/180);
	this->spinY = sin(this->wiewAngleH * 3.14159265/180) * cos(this->wiewAngleV * 3.14159265/180);
}


void Kamera::lookAt()
{
	this->wiewX = this->camPosX + this->spinX*10;
	this->wiewY = this->camPosY + this->spinY*10;
	this->wiewZ = this->camPosZ + this->spinZ*10;
}


void Kamera::camPos()
{
	this->camPosX = this->posX + this->spinX * camMultiplier;
	this->camPosY = this->posY + this->spinY * camMultiplier;
	this->camPosZ = this->posZ + this->spinZ * camMultiplier;
}

void Kamera::setPos(double X, double Y, double Z)
{
	this->posX = X;
	this->posY = Y;
	this->posZ = Z;
}


void Kamera::refresh()
{
	this->spinCam();	
	this->camPos();
	this->lookAt();
}

void Kamera::updateAngle(long x, long Y)
{
this->wiewAngleH += x * 0.35;
this->wiewAngleV -= Y * 0.35;
this->normalizeAngle();
}


double Kamera::getCamP(char x)
{
switch(x)
{
case 'x':
	{
	return this->camPosX;
	break;
	}

case 'y':
	{
	return this->camPosY;
	break;
	}

case 'z':
	{
	return this->camPosZ;
	break;
	}
}
return 0;
}



double Kamera::getCamW(char x)
{
switch(x)
{
case 'x':
	{
	return this->wiewX;
	break;
	}

case 'y':
	{
	return this->wiewY;
	break;
	}

case 'z':
	{
	return this->wiewZ;
	break;
	}
}
return 0;

}




void Kamera::vaziuoti(float multiplier, double time)
{
posX += spinX * multiplier * (time / 10);
posY += spinY * multiplier * (time / 10);
posZ += spinZ * multiplier * (time / 10);
}



void Kamera::strafe(float multiplier, double time)
{
this->posX += cos((this->wiewAngleH - 90) * 3.14159265/180) * multiplier * (time / 10);
this->posY += sin((this->wiewAngleH - 90) * 3.14159265/180) * multiplier * (time / 10);
}