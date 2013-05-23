#ifndef BOMB_H_
#define BOMB_H_

#include "WinsockWrapper.h"
#include "AbstractEntity.h"
#include "NetworkObject.h"
#include "NetworkIDObject.h"

using namespace RakNet;

#define gravity 9.8


class ProjectileBomb : public AbstractEntity, public NetworkIDObject,  public NetworkObject
{
private:
	Vector speed;
	int		mDamage;

	float angle(float x, float y);


public:
	//ProjectileBomb(float x, float y, float z, float sx, float sy, float sz);


	ProjectileBomb()
	{
		mDamage = DAMAGE_PROJECTILE_BOMB;
		mType = GAME_ENTITY_PROJECTILE_BOMB;
		mCreated = false;
		speed = Vector(0, 0, 0);
	}
	ProjectileBomb(Mesh *m, Vector &position, Vector &rotation, int speed, bool canUpdate) : AbstractEntity(m, "bomba", position, rotation, 3, ENTITY_DYNAMIC), NetworkObject(canUpdate)
	{
		mDamage = DAMAGE_PROJECTILE_BOMB;
		mType = GAME_ENTITY_PROJECTILE_BOMB; 
		mCreated = false;

		Vector r(rotation);
		r.x = -r.x - 90;
		float x = cos(DEGTORAD(r.x))*cos(DEGTORAD(r.y));
		float y = sin(DEGTORAD(r.x))*cos(DEGTORAD(r.y));
		float z = sin(DEGTORAD(r.y));
		this->speed = Vector(x*speed, y*speed, z*speed);
	}

	~ProjectileBomb()
	{
	}

	// test func
	void Reset(float x, float y, float z, Vector speed);


	float yaw();
	float pitch();


	void Update(float deltaT);




	int GetDamage()
	{
		return mDamage;
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
		stream->Write(speed);
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
		stream->Read(speed);
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
		stream.Write((Vector)speed);
		stream.Write((float)scale);
		stream.Write(entityType);
		stream.Write(mMeshID);

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
		stream.Write((Vector)speed);
		stream.Write((float)scale);
		stream.Write(entityType);
		stream.Write(mMeshID);

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
		stream->Read(speed);
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