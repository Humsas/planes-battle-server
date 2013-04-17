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
	Networking(Console* console, Game* game);

	void Update();
	void SendConnectionDataToPlayer(RakNetGUID id);
	BitStream* GetPlayerConnectionPacket(RakNetGUID id);
	bool ArePlayersReady();
	void SendCreatedObjectsIDs(vector<NetworkID> ids);
};




#endif