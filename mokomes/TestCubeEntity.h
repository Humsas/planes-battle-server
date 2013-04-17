#ifndef TESTCUBEENTITY_H_
#define TESTCUBEENTITY_H_

#include "WinsockWrapper.h"
#include "AbstractEntity.h"

#include "ReplicaManager3.h"
#include "NetworkIDManager.h"

using namespace RakNet;

class TestCubeEntity : public AbstractEntity, public Replica3
{


public:
	TestCubeEntity(){}
	TestCubeEntity(Mesh *m, Vector &position, Vector &rotation) : AbstractEntity(m, "kubas", position, rotation, rand() % 100 + 1, ENTITY_STATIC)
	{
		
	}
	void Create(Mesh *m, Vector &position, Vector &rotation)
	{
		this->position = Vector(position);
		this->rotarionYawPitchRoll = Vector(rotation);
		this->scale = rand() % 100 + 1;
		this->entityType = ENTITY_STATIC;

		this->pMeshManager = m;
		this->pMesh = m->getPointer("kubas");

		//gCons.add("ASDF", GAME_CONSOLE_OK);

		setScale(rand() % 100 + 1);
		rotateYPR(&rotarionYawPitchRoll);
		combine();
	}

	void addD3andSHIT(LPDIRECT3DDEVICE9 *d3i, Mesh *m)
	{
		this->d3 = d3i;
		this->pMeshManager = m;

		this->pMesh = m->getPointer(mType);
	}

	~TestCubeEntity()
	{
	}


	void Update(float dt)
	{
		// do nothing :D
		// nes nejuda sitas daiktas

	}

	/*void Render()
	{


	}*/



	virtual RakNet::RakString GetName(void) const 
	{
		return RakNet::RakString("Cube");
	}
	virtual void WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const {}
	virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) 
	{
		return QueryConstruction_ServerConstruction(destinationConnection, true);
	}
	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection) 
	{
		return QueryRemoteConstruction_ServerConstruction(sourceConnection, true);
	}

	virtual RakNet::RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection) 
	{
		return QuerySerialization_ServerSerializable(destinationConnection, true);
	}

	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const 
	{
		return QueryActionOnPopConnection_Server(droppedConnection);
	}

	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) 
	{
		gServerConsole.addLine("sukurem");
		constructionBitstream->Write(position);
		constructionBitstream->Write(rotarionYawPitchRoll);
		constructionBitstream->Write(scale);
		constructionBitstream->Write(entityType);
		constructionBitstream->Write("kubas");
	}
	virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) 
	{
		constructionBitstream->Read(position);
		constructionBitstream->Read(rotarionYawPitchRoll);
		constructionBitstream->Read(scale);
		constructionBitstream->Read(entityType);
		constructionBitstream->Read(mType);

		setScale(rand() % 100 + 1);
		rotateYPR(&rotarionYawPitchRoll);
		combine();

		return true;
	}

	//Iraðo duomenis ant sunaikinimo
	virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection) {}
	// Nuskaito duomenis ant sunaikinimo
	virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) {return true;}

	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection) {delete this;}


	// Suraðom duomenis apie objektà
	//http://www.jenkinssoftware.com/raknet/manual/replicamanager3.html
	virtual RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters)
	{
		serializeParameters->outputBitstream[0].Write(position);
		serializeParameters->outputBitstream[0].Write(rotarionYawPitchRoll);
		serializeParameters->outputBitstream[0].Write(this->scale);
		serializeParameters->outputBitstream[0].Write(this->entityType);
		serializeParameters->outputBitstream[0].Write("kubas");

		// Memory compares against last outputBitstream write. If changed, writes everything on the changed channel(s), which can be wasteful in this case if only health or score changed, and not both
		return RM3SR_BROADCAST_IDENTICALLY;
	}

	//Nuskaitom duomenis
	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters)
	{
		if (deserializeParameters->bitstreamWrittenTo[0])
		{
			deserializeParameters->serializationBitstream[0].Read(position);
			deserializeParameters->serializationBitstream[0].Read(rotarionYawPitchRoll);
			deserializeParameters->serializationBitstream[0].Read(scale);
			deserializeParameters->serializationBitstream[0].Read(entityType);
			deserializeParameters->serializationBitstream[0].Read(mType);

			setScale(rand() % 100 + 1);
			rotateYPR(&rotarionYawPitchRoll);
			combine();
		}
	}

};

#endif