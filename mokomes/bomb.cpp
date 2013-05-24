#include "bomb.h"
//#include "console.h"
//extern Console gCons;


void ProjectileBomb::Update(float deltaT)
{
	speed.z = speed.z - gravity * deltaT;

	position.x += speed.x * deltaT;
	position.y += speed.y * deltaT;

	position.z += speed.z * deltaT - 0.5 * gravity * deltaT * deltaT;

	rotarionYawPitchRoll.x = -yaw() + 90;
	rotarionYawPitchRoll.y = -pitch();

	//stringstream ss;
	//ss << "Y: " << rotarionYawPitchRoll.x << " P: " << rotarionYawPitchRoll.y << " R: " << rotarionYawPitchRoll.z;
	//gCons.add(ss.str());
	rotateYPR(&rotarionYawPitchRoll);
	combine();
}


void ProjectileBomb::Reset(float x, float y, float z, Vector speed)
{
	position.x = x;
	position.y = y;
	position.z = z;

	speed.x = speed.x;
	speed.y = speed.y;
	speed.z = speed.z;
}


float ProjectileBomb::yaw()
{
	return angle(speed.x, speed.y);
	//// ilgis vienrtinis
	//float len = sqrt(speed.x*speed.x + speed.y*speed.y);

	//float angle = acos(abs(speed.x) / len) * (180/3.141592654);

	//// kampo taisymas
	//if(speed.x >= 0 && speed.y >= 0)
	//{ // I
	//}
	//else if(speed.x < 0 && speed.y >= 0)
	//{ // II
	//	angle = 180 - angle;
	//}
	//else if(speed.x < 0 && speed.y < 0)
	//{ // III
	//	angle = 180 + angle;
	//}
	//else if(speed.x >= 0 && speed.y < 0)
	//{ // IV
	//	angle = 360 - angle;
	//}
	//return angle;
}


float ProjectileBomb::pitch()
{
	//pitch = acos(w.x / cos(yaw))
	//double pitch = Math.asin(deltaY/distance);
	return asin(speed.z/speed.Magnitude())* (180/3.141592654);
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