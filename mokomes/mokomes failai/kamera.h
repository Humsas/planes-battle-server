#include <cmath>

class Kamera
{

private:

	double camMultiplier;

	// judancio tasko pozicija
	double posX, posY, posZ;

	// ziurejimo taskas
	double wiewX, wiewY, wiewZ;

	// kameros pozicija
	double camPosX, camPosY, camPosZ;

	// kameros pasukimas
	double spinX, spinY, spinZ;

	// ziurejimo kampas
	double wiewAngleH;		// horizontalus pasukimas
	double wiewAngleV;		// vertikalus pasukimas





	void normalizeAngle();		// jei kampas virsija 360 sumazina jo reiksme iki ekvivalentaus mazesnio kampo
	void spinCam();				// pasuka kamera nurodytu kampu 
	void lookAt();				// pritaiko pasukima pagal esama vieta
	void camPos();				// nustato kameros koordinates






public:
	Kamera() { posX = -40; posY = 20; posZ = 10; camMultiplier = 3; wiewAngleH = 0; wiewAngleV = 0;  }

	void setPos(double X, double Y, double Z);

	double getCamP(char X);
	double getCamW(char X);

	void updateAngle(long x, long Y);

	void refresh();


	void vaziuoti(float multiplier, double time);
	//void atgal(float multiplier);

	void strafe(float multiplier, double time);


	
};