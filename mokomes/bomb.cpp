#include "bomb.h"

void ProjectileBomb::Update(float deltaT)
{
	bomba.speed.z = bomba.speed.z - gravity * deltaT;

	bomba.position.x += bomba.speed.x * deltaT;
	bomba.position.y += bomba.speed.y * deltaT;

	bomba.position.z += bomba.speed.z * deltaT - 0.5 * gravity * deltaT * deltaT;

}

ProjectileBomb::ProjectileBomb(float x, float y, float z, float sx, float sy, float sz)
{
	bomba.position.x = x;
	bomba.position.y = y;
	bomba.position.z = z;

	bomba.speed.x = sx;
	bomba.speed.y = sy;
	bomba.speed.z = sz;
}


void ProjectileBomb::Reset(float x, float y, float z, Vector speed)
{
	bomba.position.x = x;
	bomba.position.y = y;
	bomba.position.z = z;

	bomba.speed.x = speed.x;
	bomba.speed.y = speed.y;
	bomba.speed.z = speed.z;
}


float ProjectileBomb::yaw()
{
	return angle(bomba.speed.x, bomba.speed.y);
	//// ilgis vienrtinis
	//float len = sqrt(bomba.speed.x*bomba.speed.x + bomba.speed.y*bomba.speed.y);

	//float angle = acos(abs(bomba.speed.x) / len) * (180/3.141592654);

	//// kampo taisymas
	//if(bomba.speed.x >= 0 && bomba.speed.y >= 0)
	//{ // I
	//}
	//else if(bomba.speed.x < 0 && bomba.speed.y >= 0)
	//{ // II
	//	angle = 180 - angle;
	//}
	//else if(bomba.speed.x < 0 && bomba.speed.y < 0)
	//{ // III
	//	angle = 180 + angle;
	//}
	//else if(bomba.speed.x >= 0 && bomba.speed.y < 0)
	//{ // IV
	//	angle = 360 - angle;
	//}
	//return angle;
}


float ProjectileBomb::pitch()
{
	//pitch = acos(w.x / cos(yaw))
	//double pitch = Math.asin(deltaY/distance);
	return asin(bomba.speed.z/bomba.speed.Magnitude())* (180/3.141592654);
}

float ProjectileBomb::angle(float x, float y)
{
	// ilgis vienetinis
	float len = sqrt(x*x + y*y);

	float angle = acos(abs(x) / len) * (180/3.141592654);

	// kampo taisymas
	if(x >= 0 && y >= 0)
	{ // I
	}
	else if(x < 0 && y >= 0)
	{ // II
		angle = 180 - angle;
	}
	else if(x < 0 && y < 0)
	{ // III
		angle = 180 + angle;
	}
	else if(x >= 0 && y < 0)
	{ // IV
		angle = 360 - angle;
	}
	return angle;
}