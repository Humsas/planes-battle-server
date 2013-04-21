#ifndef NETWORK_OBJECT_PLANE_BATTLE
#define NETWORK_OBJECT_PLANE_BATTLE

#include "RakPeerInterface.h"

using namespace RakNet;

class NetworkObject
{
protected:
	BitStream*				mLastSerializationRezult;
	//int						mNetworkID;
	bool					mCanUpdate;
	// If object is created on the other client or server then true
	bool					mCreated;

public:


	NetworkObject()
	{
		mCanUpdate = false;
		mLastSerializationRezult = NULL;
	}

	NetworkObject(bool canUpdate)
	{
		mCanUpdate = canUpdate;
		mLastSerializationRezult = NULL;
	}

	~NetworkObject()
	{
	}


	virtual void			NetworkUpdate(RakPeerInterface* peer) = 0;
	virtual BitStream*		Serialize(int actionId) = 0;
	virtual void			Deserialize(BitStream* stream) = 0;

	// Sukurimo updeitai
	virtual void			CreateSerialize(RakPeerInterface* peer) = 0;
	virtual void			CreateDeserialize(BitStream* stream, RakPeerInterface* peer) = 0;

	//void					SetNetworkID(int id){mNetworkID = id;}
	//int						GetNetworkId(){return mNetworkID;}
	void					SetCreated(bool state){mCreated = state;}

	bool					IsBitStreamDifferent(BitStream* stream)
	{
		if(stream->GetNumberOfBitsUsed() > 0 && mLastSerializationRezult == NULL)
			return true;

		if (stream->GetNumberOfBitsUsed() > 0 &&
			((stream->GetNumberOfBitsUsed() != mLastSerializationRezult->GetNumberOfBitsUsed() || 
			memcmp(stream->GetData(), mLastSerializationRezult->GetData(), stream->GetNumberOfBytesUsed()) != 0)))
		{
			return true;
		}
		return false;
	}
};

#endif