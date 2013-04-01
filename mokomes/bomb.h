#ifndef _BOMB
#define _BOMB

#include "mymath.h"

#define gravity 9.8

struct ProjectileStruct
{
	Vector position;			// meters
	Vector speed;				// m/s
	float dragCoeficient;
};

class ProjectileBomb
{
private:
	float angle(float x, float y);


public:
	ProjectileStruct bomba;

public:
	ProjectileBomb(float x, float y, float z, float sx, float sy, float sz);
	
	// test func
	void Reset(float x, float y, float z, Vector speed);


	float yaw();
	float pitch();
	

	void Update(float deltaT);

};




#endif