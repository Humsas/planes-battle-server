#ifndef TREE1_H_
#define TREE1_H_


//#include "WinsockWrapper.h"
#include "AbstractEntity.h"
#include "NetworkObject.h"


using namespace RakNet;

class Tree1 : public AbstractEntity, public NetworkIDObject,  public NetworkObject
{

public:
	Tree1()
	{
		mType = GAME_ENTITY_TREE1;
		mCreated = false;
		mMeshID = "t1";
	}
	Tree1(Mesh *m, Vector &position, Vector &rotation, bool canUpdate) : AbstractEntity(m, "t1", position, rotation, rand() % 100 + 1, ENTITY_STATIC), NetworkObject(canUpdate)
	{
		mType = GAME_ENTITY_TREE1; 
		mCreated = false;
	}

	~Tree1()
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
		//stream.Write(rotarionYawPitchRoll);
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
		//stream->Read(rotarionYawPitchRoll);
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
		stream->Read(entityType);
		//stream->Read(mMeshID);

		pMesh = pMeshManager->getPointer(mMeshID);

		setScale(scale);
		rotateYPR(&rotarionYawPitchRoll);
		combine();
	}
};

#endif