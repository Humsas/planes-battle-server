#ifndef PLANES_BATTLE_SERVER_NETWORKING
#define PLANES_BATTLE_SERVER_NETWORKING

#include <vector>

#include "WinsockWrapper.h"
#include "RakPeerInterface.h"
#include "RakString.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
//#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "ReplicaManager3.h"


#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"
#include "RakNetSmartPtr.h"
#include "RakNetSocket2.h"
#include "RakSleep.h"
#include "Itoa.h"
//#include "Strsafe.h"		// StringCbPrintf funkcijai

#include "console.h"
#include "game.h"
#include "TestCubeEntity.h"

using namespace std;
using namespace RakNet;

#define MAX_CLIENTS 2
#define SERVER_PORT 53648

class Game;

enum GameMessages
{
	ID_GAME_MESSAGE_CONNECTION_DATA=ID_USER_PACKET_ENUM+1,
	ID_GAME_MESSAGE_LOADING_COMPLETED,
	ID_GAME_MESSAGE_PLAYER_CREATED_FOR_CLIENT,
	ID_GAME_MESSAGE_PLAYER_CREATED,
	ID_GAME_MESSAGE_PLAYERS_READY,
	ID_GAME_MESSAGE_NEW_OBJECTS_CREATED,
	ID_GAME_MESSAGE_GAME_UPDATE,
	ID_GAME_MESSAGE_PLAYER_DISCONNECTED
};

enum GameChannels
{
	GAME_CHANNEL_UPDATE,
	GAME_CHANNEL_NEW_DATA,
	GAME_CHANNEL_CHAT_MESSAGES
};


// Required by ReplicaManager3. Acts as a class factory for Replica3 derived instances
class SampleConnectionRM3 : public Connection_RM3
{
public:
	SampleConnectionRM3(const SystemAddress &_systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid) {}
	virtual ~SampleConnectionRM3() {}

	// See documentation - Makes all messages between ID_REPLICA_MANAGER_DOWNLOAD_STARTED and ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE arrive in one tick
	bool QueryGroupDownloadMessages(void) const {return true;}

	virtual Replica3 *AllocReplica(RakNet::BitStream *allocationIdBitstream, ReplicaManager3 *replicaManager3)
	{
		RakString objectType;
		// Types are written by WriteAllocationID()
		allocationIdBitstream->Read(objectType);
		if (objectType =="Cube") return new TestCubeEntity;
		RakAssert("Unknown type in AllocReplica" && 0);
		return 0;
	}
};


// Required by ReplicaManager3. Acts as a class factory for Connection_RM3 derived instances
class SampleRM3 : public ReplicaManager3
{
public:
	SampleRM3() {}
	virtual ~SampleRM3() {}
	virtual Connection_RM3* AllocConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID) const {return new SampleConnectionRM3(systemAddress,rakNetGUID);}
	virtual void DeallocConnection(Connection_RM3 *connection) const {delete connection;}
};




struct ConnectionData 
{
	RakNetGUID	playerId;
	bool		ready;
};


class Networking
{
private:

	RakPeerInterface*		mServer;
	Console*				mConsole;
	vector<ConnectionData>	mConnectionData;
	BitStream				mShit1;
	BitStream				mShit2;
	Game*					mGame;

	bool OpenUPNP();

public:
	Networking(Console* console, Game* game, NetworkIDManager* idManager, ReplicaManager3* netManager);

	void Update();
	void SendConnectionDataToPlayer(RakNetGUID id);
	BitStream* GetPlayerConnectionPacket(RakNetGUID id);
	bool ArePlayersReady();
	void SendCreatedObjectsIDs(vector<NetworkID> ids);
};




#endif