#ifndef TURRET_H_
#define TURRET_H_

//#include "WinsockWrapper.h"
#include "AbstractEntity.h"
#include "NetworkObject.h"
//#include "chunkManager.h"


using namespace RakNet;

class Turret : public AbstractEntity, public NetworkIDObject,  public NetworkObject
{
private:
	AbstractEntity *target;
	meshInfo *	pMeshBarrel;
	Vector		rotarionYawPitchRollBarrel;
	float		scaleBarrel;
	float		timeFromLastShot;

public:
	Turret()
	{
		mType = GAME_ENTITY_TURRET;
		mCreated = false;
		mMeshID = "cannon";
		target = NULL;
		scaleBarrel = 70;
		rotarionYawPitchRollBarrel = Vector(0, 0, 0);
		timeFromLastShot = 0;
	}
	Turret(Mesh *m, Vector &position, Vector &rotation, bool canUpdate) : AbstractEntity(m, "cannon", position, rotation, 100, ENTITY_DYNAMIC), NetworkObject(canUpdate)
	{
		mType = GAME_ENTITY_TURRET; 
		mCreated = false;
		mMeshID = "cannon";
		target = NULL;
		pMeshBarrel = m->getPointer("cannonBarrel");
		rotarionYawPitchRollBarrel = Vector(rotation);
		scaleBarrel = 70;
		timeFromLastShot = 0;
	}

	~Turret()
	{
	}


	virtual void setMeshManager(Mesh *m)
	{
		pMeshManager = m;
		pMeshBarrel = m->getPointer("cannonBarrel");
	}


	void setTarget(AbstractEntity *t)
	{
		target = t;
	}

	void resetShotTimer()
	{
		timeFromLastShot = 0;
	}

	float getShotTimer()
	{
		return timeFromLastShot;
	}

	void Update(float dt)
	{
		// trackinti targeta ir sukiotis
		if(target != NULL)
		{

			if(target->getPosition()->z > position.z)
			{
				timeFromLastShot += dt;


				//bombStruct *bomba = new bombStruct;
				//	bomba->killRadius = 50;
				//
				//
				//	//oo->pozicija.x = lektuvas->Airplane.vPosition.y*0.3048;
				//	//oo->pozicija.y = lektuvas->Airplane.vPosition.z*0.3048;
				//	//oo->pozicija.z = -lektuvas->Airplane.vPosition.x*0.3048;
				//
				float projectileSpeed = 500; // m/s
				//
				//	// paziurim atstuma kokiu lektuvas yra nuo patrankos
				//	// tada paskaiciuojam kiek laiko skris sovinys toki atstuma
				//	// paziurim kur bus lektuvas tuo metu
				//	// saunam i paskaiciuota taska 
				//
				Vector targetVector = (*target->getPosition() - position);
				//float distance = targetVector.Magnitude(); // atstumas iki taikinio
				//float travelTime = distance / projectileSpeed;	// laikas, kuri keliaus sovinys
				//
				//
				//
				/*Vector velocity = Vector(lektuvas->Airplane.vVelocity.y,
				-lektuvas->Airplane.vVelocity.x,
				lektuvas->Airplane.vVelocity.z);*/
				//
				//	Vector imaginaryTarget = targetVector + (velocity*0.3048)*travelTime;
				//
				//	// prakritimas begant laikui
				//	Vector tmpV = imaginaryTarget;
				//
				//	tmpV.Normalize();
				//	tmpV *= projectileSpeed;
				//
				//	float zPos = tmpV.z * travelTime - 0.5 * gravity * travelTime * travelTime;
				//
				//	imaginaryTarget.z = imaginaryTarget.z + (imaginaryTarget.z - zPos)/2;
				//
				//	travelTime = imaginaryTarget.Magnitude() / projectileSpeed;
				//	
				//	imaginaryTarget.Normalize();
				//
				//	Vector cannonBarrel = ec->op->pozicija + (imaginaryTarget * (ec->op->radius+5));
				//
				//	imaginaryTarget *= projectileSpeed;
				//
				//
				//
				//
				float x = targetVector.x;
				float y = targetVector.y;
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
				//
				//
				//				//Vector positionVec = op->pozicija - lektuvas->op->pozicija;
				float pitch = asin(targetVector.z/targetVector.Magnitude())* (180/3.141592654);
				//
				rotarionYawPitchRoll.x = -angle+90;
				rotarionYawPitchRoll.y = 0;
				rotarionYawPitchRoll.z = 0;

				rotarionYawPitchRollBarrel.x = -angle+90;
				rotarionYawPitchRollBarrel.y = -pitch;
				rotarionYawPitchRollBarrel.z = 0;

				/*ec->oo->rotateYPR(-angle+90,0,0);
				ec->ooBarrel->rotateYPR(-angle+90,-pitch,0);*/
				//
				//
				//
				//
				//	bomba->bomba = new ProjectileBomb(cannonBarrel.x,
				//		cannonBarrel.y,
				//		cannonBarrel.z,
				//		imaginaryTarget.x,
				//		imaginaryTarget.y,
				//		imaginaryTarget.z);
				//
				//
				//	/*bomba->bomba = new ProjectileBomb(lektuvas->Airplane.vPosition.y*0.3048,
				//		-lektuvas->Airplane.vPosition.x*0.3048,
				//		lektuvas->Airplane.vPosition.z*0.3048,
				//		lektuvas->Airplane.vVelocity.y*0.3048 + projectileSpeed * (lektuvas->Airplane.vVelocity.y / lektuvas->Airplane.vVelocity.Magnitude()),
				//		-lektuvas->Airplane.vVelocity.x*0.3048 - projectileSpeed * (lektuvas->Airplane.vVelocity.x / lektuvas->Airplane.vVelocity.Magnitude()),
				//		lektuvas->Airplane.vVelocity.z*0.3048 + projectileSpeed * (lektuvas->Airplane.vVelocity.z / lektuvas->Airplane.vVelocity.Magnitude()));
				//*/
				//	//lektuvas->Airplane.vVelocity.x;
				//
				//	/*bomba->bomba->bomba.position.x = -lektuvas->Airplane.vPosition.y*0.3048;
				//	bomba->bomba->bomba.position.y = lektuvas->Airplane.vPosition.z*0.3048;
				//	bomba->bomba->bomba.position.z = lektuvas->Airplane.vPosition.x*0.3048;*/
				//	////lektuvas->Airplane.vPosition.



			}
		}
	}

	void Render()
	{
		rotateYPR(&rotarionYawPitchRoll);
		combine();

		AbstractEntity::Render();

		D3DXMATRIX      rotationMatrixB;
		D3DXMATRIX      scaleMatrixB;
		D3DXMATRIX      transformMatrixB;

		D3DXMatrixScaling(&scaleMatrixB, scaleBarrel/pMeshBarrel->maxBound, scaleBarrel/pMeshBarrel->maxBound, scaleBarrel/pMeshBarrel->maxBound);

		D3DXMatrixRotationYawPitchRoll(&rotationMatrixB, DEGTORAD(rotarionYawPitchRollBarrel.x), DEGTORAD(rotarionYawPitchRollBarrel.y), DEGTORAD(rotarionYawPitchRollBarrel.z));
		transformMatrixB = scaleMatrixB * rotationMatrixB;


		pMeshManager->drawMesh(pMeshBarrel, &positionForRender, transformMatrixB);

	}

	void SetPosition(Vector pos)		{position = pos;}

	//TOFACKINGDO cia reikia paduoti serverio ar klientu adresus ir nurodyti kam siusti, serveriui ar klientam
	void NetworkUpdate(RakPeerInterface* peer)
	{
		// Do not update static entity
		if(mCanUpdate && (entityType != ENTITY_STATIC) && mCreated)
		{
			BitStream* stream = Serialize(0);

			// Jei skiriasi streamai siunciam updeita
			if(stream != NULL)
			{
				peer->Send(stream, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_UPDATE, UNASSIGNED_SYSTEM_ADDRESS, true);
			}
		}
	}

	BitStream* Serialize(int actionId)
	{
		BitStream *stream = new BitStream();
		stream->Write((RakNet::MessageID)ID_GAME_MESSAGE_GAME_UPDATE);
		stream->Write(mType);
		stream->Write((NetworkID)this->GetNetworkID());

		stream->Write(position);
		stream->Write(rotarionYawPitchRoll);
		stream->Write(rotarionYawPitchRollBarrel);

		//stream.Write(scale);
		//stream.Write(entityType);

		if(!IsBitStreamDifferent(stream))
		{
			delete stream;
			return NULL;
		}

		delete mLastSerializationRezult;
		mLastSerializationRezult = stream;

		return stream;
	}

	void Deserialize(BitStream* stream)
	{
		stream->ResetReadPointer();
		stream->IgnoreBytes(sizeof(RakNet::MessageID));
		stream->Read(mType);
		stream->IgnoreBytes(sizeof(NetworkID));

		stream->Read(position);
		stream->Read(rotarionYawPitchRoll);
		stream->Read(rotarionYawPitchRollBarrel);

		//stream->Read(scale);
		//stream->Read(entityType);
	}

	void CreateSerialize(RakPeerInterface* peer)
	{
		BitStream stream;
		stream.Write((RakNet::MessageID)ID_GAME_MESSAGE_NEW_OBJECT_CREATED);
		stream.Write(mType);
		stream.Write((NetworkID)this->GetNetworkID());

		stream.Write((Vector)position);
		stream.Write((Vector)rotarionYawPitchRoll);
		stream.Write((float)scale);

		stream.Write((Vector)rotarionYawPitchRollBarrel);
		stream.Write((float)scaleBarrel);

		stream.Write(entityType);
		//stream.Write(mMeshID);

		peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_NEW_DATA, UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	void CreateSerialize(RakPeerInterface* peer, RakNetGUID idToSendTo)
	{
		BitStream stream;
		stream.Write((RakNet::MessageID)ID_GAME_MESSAGE_NEW_OBJECT_CREATED);
		stream.Write(mType);
		stream.Write((NetworkID)this->GetNetworkID());

		stream.Write((Vector)position);
		stream.Write((Vector)rotarionYawPitchRoll);
		stream.Write((float)scale);

		stream.Write((Vector)rotarionYawPitchRollBarrel);
		stream.Write((float)scaleBarrel);

		stream.Write(entityType);
		//stream.Write(mMeshID);

		peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_NEW_DATA, idToSendTo, false);
	}

	void CreateDeserialize(BitStream* stream, RakPeerInterface* peer)
	{
		stream->ResetReadPointer();
		stream->IgnoreBytes(sizeof(RakNet::MessageID));
		stream->Read(mType);
		NetworkID tmp;
		stream->Read((NetworkID)tmp);
		this->SetNetworkID(tmp);
		mCreated = true;
		BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_OBJECT_CREATION_CONFIRMED);
		bsOut.Write(tmp);
		bsOut.Write(mType);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_NEW_DATA, UNASSIGNED_SYSTEM_ADDRESS, true);

		stream->Read(position);
		stream->Read(rotarionYawPitchRoll);
		stream->Read(scale);
		stream->Read(rotarionYawPitchRollBarrel);
		stream->Read(scaleBarrel);
		stream->Read(entityType);
		//stream->Read(mMeshID);

		pMesh = pMeshManager->getPointer(mMeshID);

		setScale(scale);
		rotateYPR(&rotarionYawPitchRoll);
		combine();
	}

};


#endif