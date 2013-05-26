#include "kamera.h"

#include "console.h"
extern Console gServerConsole;

void Kamera::normalizeAngle()
{
	if(this->wiewAngleH > 360 || this->wiewAngleH < -360) { this->wiewAngleH-= 360 * ((int)this->wiewAngleH/360); }
	//if(this->wiewAngleV > 360 || this->wiewAngleV < -360) { this->wiewAngleV-= 360 * ((int)this->wiewAngleV/360); }

	if(this->wiewAngleV > 89) { this->wiewAngleV = 89; }
	if(this->wiewAngleV < -89) { this->wiewAngleV = -89; }
}

void Kamera::spinCam()
{
	//this->spinY = sin(this->wiewAngleV * 3.14159265/180); // aukstis nuo horizontalios plokstumos
	//this->spinX = cos(this->wiewAngleH * 3.14159265/180) * cos(this->wiewAngleV * 3.14159265/180);
	//this->spinZ = sin(this->wiewAngleH * 3.14159265/180) * cos(this->wiewAngleV * 3.14159265/180);

	spin.y = sin(this->wiewAngleV * 3.14159265/180); // aukstis nuo horizontalios plokstumos
	spin.x = cos(this->wiewAngleH * 3.14159265/180) * cos(this->wiewAngleV * 3.14159265/180);
	spin.z = sin(this->wiewAngleH * 3.14159265/180) * cos(this->wiewAngleV * 3.14159265/180);
}



void Kamera::camPos()
{
	switch(viewMode)
	{
		// pirmo asmens kamera
		case GAME_CAMERA_FIRST_PERSON:
			// pozicija
			//this->camPosX = this->posX;
			//this->camPosY = this->posY;
			//this->camPosZ = this->posZ;
			//// look at
			//this->wiewX = this->camPosX + this->spinX;
			//this->wiewY = this->camPosY + this->spinY;
			//this->wiewZ = this->camPosZ + this->spinZ;


			cPoint = point;
			vPoint = cPoint + spin;

		break;

		// trecio asmens kamera
		case GAME_CAMERA_THIRD_PERSON:
			// pozicija
			//this->camPosX = this->posX - this->spinX*zoom;
			//this->camPosY = this->posY - this->spinY*zoom;
			//this->camPosZ = this->posZ - this->spinZ*zoom;
			//// look at
			//this->wiewX = this->posX;
			//this->wiewY = this->posY;
			//this->wiewZ = this->posZ;

			

			cPoint = point - spin*zoom;
			vPoint = point;

		break;

		// besivaikanti kamera
		case GAME_CAMERA_CHASE:

			// skaiciuojam kampa tarp vektoriu

			if(point.x != prevPoint.x &&
				point.y != prevPoint.y &&
				point.z != prevPoint.z && chaseUpdate)
			{

				float horizontalAngle = RadiansToDegrees(atan2(prevPoint.x-cPoint.x, prevPoint.z-cPoint.z) - atan2(point.x-cPoint.x, point.z-cPoint.z));
				if(horizontalAngle < 360 && horizontalAngle > -360)
				{
					wiewAngleH += horizontalAngle ;
					this->spinCam();
				}

				
				
				float verticalAngle = RadiansToDegrees(atan2(((prevPoint.z-cPoint.z)+(prevPoint.x-cPoint.x))/2, prevPoint.y-cPoint.y) - atan2(((point.z-cPoint.z)+(point.x-cPoint.x))/2, point.y-cPoint.y));

				// tan(P) = sqrt(x^2 + y^2)/z

				verticalAngle = RadiansToDegrees(	atan(sqrt((point.z-cPoint.z)*(point.z-cPoint.z)+(point.x-cPoint.x)*(point.x-cPoint.x)) / (point.y-cPoint.y))-
													atan(sqrt((prevPoint.z-cPoint.z)*(prevPoint.z-cPoint.z)+(prevPoint.x-cPoint.x)*(prevPoint.x-cPoint.x)) / (prevPoint.y-cPoint.y))
													);

				

				

				if(verticalAngle < -5)	verticalAngle += 180;
				if(verticalAngle > 5)	verticalAngle -= 180;
					//float verticalAngle = (verticalAngle1+verticalAngle2)/2;
				if(verticalAngle < 5 && verticalAngle > -5)
				{
					wiewAngleV -= verticalAngle;
					this->spinCam();
				}


			
				
				chaseUpdate = false;
			}

			cPoint = point - spin*zoom;
			vPoint = point;


		break;

	}

}

void Kamera::setPos(double X, double Y, double Z)
{
	/*this->posX = X;
	this->posY = Y;
	this->posZ = Z;*/

	prevPoint.x = point.x;
	prevPoint.y = point.y;
	prevPoint.z = point.z;

	point.x = X;
	point.y = Y;
	point.z = Z;

	chaseUpdate = true;
}


void Kamera::refresh(int r)
{
	this->spinCam();	
	this->camPos();
	//this->lookAt(r);
}

void Kamera::Zoom(float z)
{
	zoom += z;
	if(zoom < 10) zoom = 10;
	if(zoom >1000) zoom = 1000;
}

void Kamera::switchCameraMode()
{
	switch(viewMode)
	{
	case GAME_CAMERA_FIRST_PERSON:
		{
			viewMode = GAME_CAMERA_THIRD_PERSON;
			break;
		}

	case GAME_CAMERA_THIRD_PERSON:
		{
			viewMode = GAME_CAMERA_CHASE;
			break;
		}

	case GAME_CAMERA_CHASE:
		{
			viewMode = GAME_CAMERA_FIRST_PERSON;
			break;
		}

	}
}

void Kamera::updateAngle(long x, long Y)
{
	this->wiewAngleH -= x * 0.35;
	this->wiewAngleV -= Y * 0.35;
	this->normalizeAngle();
}


//double Kamera::getCamP(char x)
//{
//	switch(x)
//	{
//	case 'x':
//		{
//		return this->camPosX;
//		break;
//		}
//
//	case 'y':
//		{
//		return this->camPosY;
//		break;
//		}
//
//	case 'z':
//		{
//		return this->camPosZ;
//		break;
//		}
//	}
//	return 0;
//}



//double Kamera::getCamW(char x)
//{
//	switch(x)
//	{
//	case 'x':
//		{
//		return this->wiewX;
//		break;
//		}
//
//	case 'y':
//		{
//		return this->wiewY;
//		break;
//		}
//
//	case 'z':
//		{
//		return this->wiewZ;
//		break;
//		}
//	}
//	return 0;
//
//}




void Kamera::vaziuoti(float multiplier, double time)
{
	/*posX += spinX * multiplier * (time / 1000);
	posY += spinY * multiplier * (time / 1000);
	posZ += spinZ * multiplier * (time / 1000);*/
}



void Kamera::strafe(float multiplier, double time)
{
	/*this->posX -= cos((this->wiewAngleH - 90) * 3.14159265/180) * multiplier * (time / 1000);
	this->posZ -= sin((this->wiewAngleH - 90) * 3.14159265/180) * multiplier * (time / 1000);*/
}


Vector &Kamera::getCamP()
{
	return cPoint;
}

Vector &Kamera::getCamW()
{
	return vPoint;
}

Vector &Kamera::getCamSpin()
{
	return spin;
}

void Kamera::Update()
{
	this->spinCam();	
	this->camPos();
}