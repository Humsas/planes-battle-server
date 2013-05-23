#ifndef _KAMERA
#define _KAMERA

#include <cmath>
#include "mymath.h"



enum GAME_CAMERA_MODE{
						GAME_CAMERA_FIRST_PERSON = 1,
						GAME_CAMERA_THIRD_PERSON = 2,
						GAME_CAMERA_CHASE = 3
					} ;

class Kamera
{

private:
	GAME_CAMERA_MODE viewMode;

	float zoom;

	double camMultiplier;

	// judancio tasko pozicija
	//double posX, posY, posZ;
	Vector point;

	Vector prevPoint;

	// ziurejimo taskas
	//double wiewX, wiewY, wiewZ;
	Vector vPoint;

	// kameros pozicija
	//double camPosX, camPosY, camPosZ;
	Vector cPoint;

	// kameros pasukimas
	//double spinX, spinY, spinZ;
	Vector spin;

	// ziurejimo kampas
	double wiewAngleH;		// horizontalus pasukimas
	double wiewAngleV;		// vertikalus pasukimas





	void normalizeAngle();		// jei kampas virsija 360 sumazina jo reiksme iki ekvivalentaus mazesnio kampo
	void spinCam();				// pasuka kamera nurodytu kampu 
	
	void camPos();				// nustato kameros koordinates



	bool chaseUpdate;


public:
	Kamera()
	{ 
		viewMode = GAME_CAMERA_CHASE;

		zoom = 10;

		point.x = 0;
		point.y = 0;
		point.z = 0;

		prevPoint.x = 0;
		prevPoint.y = 0;
		prevPoint.z = 0;

		vPoint.x = 0;
		vPoint.y = 0;
		vPoint.z = 0;

		cPoint.x = 0;
		cPoint.y = 0;
		cPoint.z = 0;

		spin.x = 0;
		spin.y = 0;
		spin.z = 0;

		//camPosX = posX = 0; // deprecated
		//camPosY = posY = 0; // deprecated
		//camPosZ = posZ = 0; // deprecated
		camMultiplier = 3; 
		wiewAngleH = 0; 
		wiewAngleV = 0;  
		chaseUpdate = false;
	}

	void setPos(double X, double Y, double Z);

	//double getCamP(char X); // deprecated
	//double getCamW(char X); // deprecated

	Vector &getCamP();
	Vector &getCamW();
	Vector &getCamSpin();

	void updateAngle(long x, long Y);

	void refresh(int r);
	void Zoom(float z);
	void switchCameraMode();

	void vaziuoti(float multiplier, double time);
	

	void strafe(float multiplier, double time);

	void Update();


	
};

#endif