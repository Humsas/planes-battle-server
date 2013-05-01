#ifndef PBS_BUILDING_H_
#define PBS_BUILDING_H_

#include "WinsockWrapper.h"
#include "AbstractEntity.h"
#include "NetworkObject.h"
#include "NetworkIDObject.h"


using namespace RakNet;

class Building : public AbstractEntity, public NetworkIDObject,  public NetworkObject
{
private:
	int mMaxHealth;
	int mHealth;

public:
	Building()
	{
		mType = GAME_ENTITY_BUILDING;
		mCreated = false;
		mMaxHealth = -1;
		mHealth = -1;
	}
	Building(Mesh *m, std::string meshId, Vector &position, Vector &rotation, int maxhealth, bool canUpdate) : AbstractEntity(m, meshId, position, rotation, rand() % 100 + 1, ENTITY_DYNAMIC), NetworkObject(canUpdate)
	{
		mType = GAME_ENTITY_BUILDING; 
		mCreated = false;
		mMaxHealth = maxhealth;
		mHealth = maxhealth;
	}

	~Building()
	{
	}

	//Returns true if building is destroyed
	bool TakeDamage(int dmg)
	{
		mHealth -= dmg;
		if(mHealth <= 0)
			return true;

		return false;
	}

	void Update(float dt)
	{
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

		stream->Write(mHealth);

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

		stream->Read(mHealth);
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
		RakString tmp = mMeshID.c_str();
		stream.Write((RakString)tmp);
		stream.Write(mHealth);
		stream.Write(mMaxHealth);

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
		RakString tmp = mMeshID.c_str();
		stream.Write((RakString)tmp);
		stream.Write(mHealth);
		stream.Write(mMaxHealth);

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
		RakString tmpStr;
		stream->Read(tmpStr);
		mMeshID = tmpStr.C_StringUnsafe();
		stream->Read(mHealth);
		stream->Read(mMaxHealth);

		pMesh = pMeshManager->getPointer(mMeshID);

		setScale(scale);
		rotateYPR(&rotarionYawPitchRoll);
		combine();
	}

};

#endif