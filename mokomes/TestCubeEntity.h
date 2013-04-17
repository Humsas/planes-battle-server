#ifndef TESTCUBEENTITY_H_
#define TESTCUBEENTITY_H_

//#include "WinsockWrapper.h"
#include "AbstractEntity.h"
#include "NetworkObject.h"

using namespace RakNet;

class TestCubeEntity : public AbstractEntity, public NetworkObject
{

public:
	TestCubeEntity()
	{
		mType = GAME_ENTITY_CUBE;
	}
	TestCubeEntity(Mesh *m, Vector &position, Vector &rotation, bool canUpdate) : AbstractEntity(m, "kubas", position, rotation, rand() % 100 + 1, ENTITY_STATIC), NetworkObject(canUpdate)
	{
		mType = GAME_ENTITY_CUBE;   
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


	//TOFACKINGDO cia reikia paduoti serverio ar klientu adresus ir nurodyti kam siusti, serveriui ar klientam
	void NetworkUpdate(RakPeerInterface* server)
	{
		// Do not update static entity
		if(mCanUpdate && entityType != ENTITY_STATIC)
		{
			BitStream* stream = Serialize(0);

			// Jei skiriasi streamai siunciam updeita
			if(stream != NULL)
				server->Send(stream, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_UPDATE, UNASSIGNED_SYSTEM_ADDRESS, true);
		}
	}

	BitStream* Serialize(int actionId)
	{
		BitStream stream;
		stream.Write(ID_GAME_MESSAGE_GAME_UPDATE);
		stream.Write(mType);
		stream.Write(mNetworkID);

		stream.Write(position);
		stream.Write(rotarionYawPitchRoll);
		stream.Write(scale);
		stream.Write(entityType);

		if(!IsBitStreamDifferent(&stream))
			return NULL;

		mLastSerializationRezult = &stream;
		return &stream;
	}

	void Deserialize(BitStream* stream)
	{
		int actionId;
		stream->Read(actionId);
		stream->Read(mType);
		stream->Read(mNetworkID);

		stream->Read(position);
		stream->Read(rotarionYawPitchRoll);
		stream->Read(scale);
		stream->Read(entityType);
	}

	BitStream* CreateSerialize()
	{
		BitStream stream;
		stream.Write(ID_GAME_MESSAGE_NEW_OBJECT_CREATED);
		stream.Write(mType);
		stream.Write(mNetworkID);

		stream.Write(position);
		stream.Write(rotarionYawPitchRoll);
		stream.Write(scale);
		stream.Write(entityType);

		mLastSerializationRezult = &stream;
		return &stream;
	}

	void CreateDeserialize(BitStream* stream)
	{
		int actionId;
		stream->Read(actionId);
		stream->Read(mType);
		stream->Read(mNetworkID);

		stream->Read(position);
		stream->Read(rotarionYawPitchRoll);
		stream->Read(scale);
		stream->Read(entityType);
	}

};

#endif