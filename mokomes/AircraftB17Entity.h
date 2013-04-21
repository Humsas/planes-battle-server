#ifndef AIRCRAFTB17ENTITY_H_
#define AIRCRAFTB17ENTITY_H_

#include "WinsockWrapper.h"
#include "AbstractEntity.h"
#include "NetworkObject.h"
#include "NetworkIDObject.h"

#include "chunkManager.h"

#include "FGFDMExec.h"
#include "FGState.h"
#include "models\propulsion\FGPiston.h"

using namespace RakNet;
using namespace JSBSim;

class AircraftB17 : public AbstractEntity, public NetworkIDObject,  public NetworkObject
{
private:
	FGFDMExec *FDMExec;	

	Vector initPos;
	//ColisionManager *colisionManager;

	// controls
	float throttle;
	float ailerons;
	float elevator;
	float ruder;
	float flaps;
	float propAdvance;
	int starter;
	int magnetos;
	float mixture;
	float speed;
	float brakes;
	int gearDown;

	bool isClientStub;

	// returns
	double stallWarn;

	void copyToJSBSim();
	void copyFromJSBSim();

public:
	AircraftB17()
	{
		mType = GAME_ENTITY_AIRCRAFT_B17;
		mCreated = false;
		isClientStub = true;


		FDMExec = new JSBSim::FGFDMExec();
		FDMExec->SetDebugLevel(0);
		FDMExec->SetOutputFileName("");
		FDMExec->SetAircraftPath("aircraft");
		FDMExec->SetEnginePath("engine");
		FDMExec->SetSystemsPath("systems");

		FDMExec->LoadScript("scripts\\testScript");
		FDMExec->GetPropulsion()->InitRunning(-1);
		

		FDMExec->DoTrim(JSBSim::tGround);

		// initial settings
		throttle	= 0.0;
		ailerons	= 0.0;
		elevator	= 0.0;
		ruder		= 0.0;
		flaps		= 0.0;
		propAdvance = 0.0;
		starter		= 0;
		magnetos	= 0;
		mixture		= 1.0;
		speed		= 0.0;
		brakes		= 0.0;
		gearDown	= 1;
		stallWarn	= 0.0;

		/*
		this->position.x = initPos.x;
		this->position.y = initPos.y;
		this->position.z = initPos.z;
		*/

		copyToJSBSim();
		FDMExec->RunIC();
		copyFromJSBSim();
	}
	AircraftB17(Mesh *m, Vector &position, Vector &rotation, bool canUpdate, ChunkManager *cm) : AbstractEntity(m, "B17", position, rotation, 31, ENTITY_DYNAMIC), NetworkObject(canUpdate)
	{
		mType = GAME_ENTITY_AIRCRAFT_B17; 
		mCreated = false;
		isClientStub = false;
		CM = cm;

		this->initPos = position;


		FDMExec = new JSBSim::FGFDMExec();
		FDMExec->SetDebugLevel(0);
		FDMExec->SetOutputFileName("");
		FDMExec->SetAircraftPath("aircraft");
		FDMExec->SetEnginePath("engine");
		FDMExec->SetSystemsPath("systems");

		//FDMExec->LoadModel("B17");

		//FDMExec->GetPropulsion()->InitRunning(-1);

		FDMExec->LoadScript("scripts\\testScript");
		FDMExec->GetPropulsion()->InitRunning(-1);
		FDMExec->GetIC()->SetTerrainElevationFtIC(initPos.z/0.3048);
		/*FDMExec->GetIC()->SetUBodyFpsIC(0.0);
		FDMExec->GetIC()->SetVBodyFpsIC(0.0);
		FDMExec->GetIC()->SetWBodyFpsIC(0.0);

		FDMExec->GetIC()->SetLatitudeDegIC(0);
		FDMExec->GetIC()->SetLongitudeDegIC(0);

		FDMExec->GetIC()->SetPhiDegIC(0.0);
		FDMExec->GetIC()->SetThetaDegIC(0.0);
		FDMExec->GetIC()->SetPsiDegIC(12.0);

		FDMExec->GetIC()->SetAltitudeAGLFtIC(18000);
		*/

		/*FDMExec->GetAtmosphere()->SetExTemperature(9.0/5.0*(1+273.15) );
		FDMExec->GetAtmosphere()->SetExDensity(1);
		FDMExec->GetAtmosphere()->SetExPressure(200);*/
		/*FDMExec->GetPropagate()->SetSeaLevelRadius(0);
		FDMExec->GetAtmosphere()->SetWindNED(1,0,0);
		FDMExec->GetAtmosphere()->SetWindspeed(10);*/

		//FDMExec->GetIC()->Load("reset00");
		//FDMExec->RunIC();

		FDMExec->DoTrim(JSBSim::tGround);

		//startEngine();

		//FDMExec->GetPropagate()->GetLocation().

		// initial settings
		throttle	= 0.0;
		ailerons	= 0.0;
		elevator	= 0.0;
		ruder		= 0.0;
		flaps		= 0.0;
		propAdvance = 0.0;
		starter		= 0;
		magnetos	= 0;
		mixture		= 1.0;
		speed		= 0.0;
		brakes		= 0.0;
		gearDown	= 1;
		stallWarn	= 0.0;

		this->position.x = initPos.x;
		this->position.y = initPos.y;
		this->position.z = initPos.z;

		copyToJSBSim();
		FDMExec->RunIC();
		copyFromJSBSim();
	}

	~AircraftB17();



	void startEngine(bool starter);
	void stopEngine();

	void setElevation(double elev);




	// controls
	void setElerons(float pos);
	void setElevator(float pos);
	void setRuder(float pos);
	void setFlaps(float pos);

	void setThrottle(float pos);
	void increaseThrottle();
	void decreaseThrottle();

	void increaseMixture();
	void decreaseMixture();

	void increasePropAdvance();
	void decreasePropAdvance();

	void setBrakes(bool b);
	void gearUpDown();

	float getSpeed();
	bool getStall();

	void Update(float dt);







	void Move(int amount)
	{
		position.z = position.z + amount;
	}




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
		//stream.Write(scale);
		//stream.Write(entityType);

		if(!IsBitStreamDifferent(stream))
			return NULL;

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
		//stream->Read(scale);
		//stream->Read(entityType);
	}

	void CreateSerialize(RakPeerInterface* peer)
	{
		BitStream stream;
		stream.Write((RakNet::MessageID)ID_GAME_MESSAGE_NEW_OBJECT_CREATED);
		stream.Write(mType);
		stream.Write((NetworkID)this->GetNetworkID());

		stream.Write((Vector)initPos);
		stream.Write((FGColumnVector3)FDMExec->GetPropagate()->GetEuler());
		stream.Write((double)FDMExec->GetFCS()->GetGearPos());
		stream.Write((double)FDMExec->GetFCS()->GetDfPos());
		stream.Write((double)FDMExec->GetFCS()->GetDaLPos());
		stream.Write((double)FDMExec->GetFCS()->GetDaRPos());
		stream.Write((double)FDMExec->GetFCS()->GetDePos());
		stream.Write((double)FDMExec->GetFCS()->GetDrPos());
		
		for(int i = 0; i < FDMExec->GetPropulsion()->GetNumEngines(); i++)
		{
			stream.Write((double)FDMExec->GetFCS()->GetThrottlePos(i));
			stream.Write((double)FDMExec->GetFCS()->GetMixturePos(i));
			stream.Write((double)FDMExec->GetFCS()->GetPropAdvance(i));
		}

		stream.Write((Vector)position);
		stream.Write((Vector)rotarionYawPitchRoll);
		stream.Write((float)scale);
		stream.Write(entityType);
		stream.Write(mMeshID);

		peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_NEW_DATA, UNASSIGNED_SYSTEM_ADDRESS, true);
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



		double tmpData;


		stream->Read(initPos);
		
		FGColumnVector3 fgv3;
		stream->Read(fgv3);
		//FDMExec->GetPropagate()->
		//FDMExec->GetPropagate()->SetEuler(&fgv3);


		stream->Read(tmpData);
		FDMExec->GetFCS()->SetGearPos(tmpData);

		stream->Read(tmpData);
		FDMExec->GetFCS()->SetDfPos(ofRad, tmpData);

		stream->Read(tmpData);
		FDMExec->GetFCS()->SetDaLPos(ofRad, tmpData);

		stream->Read(tmpData);
		FDMExec->GetFCS()->SetDaRPos(ofRad, tmpData);

		stream->Read(tmpData);
		FDMExec->GetFCS()->SetDePos(ofRad, tmpData);

		stream->Read(tmpData);
		FDMExec->GetFCS()->SetDrPos(ofRad, tmpData);

		for(int i = 0; i < FDMExec->GetPropulsion()->GetNumEngines(); i++)
		{
			stream->Read(tmpData);
			FDMExec->GetFCS()->SetThrottlePos(i, tmpData);

			stream->Read(tmpData);
			FDMExec->GetFCS()->SetMixturePos(i,tmpData);

			stream->Read(tmpData);
			FDMExec->GetFCS()->SetPropAdvance(i, tmpData);
		}


		stream->Read(position);
		stream->Read(rotarionYawPitchRoll);
		stream->Read(scale);
		stream->Read(entityType);
		stream->Read(mMeshID);

		pMesh = pMeshManager->getPointer(mMeshID);

		setScale(scale);
		rotateYPR(&rotarionYawPitchRoll);
		combine();
	}

};

#endif