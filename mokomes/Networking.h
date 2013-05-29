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
#include "Tree1Entity.h"
#include "Turret.h"



using namespace std;
using namespace RakNet;

#define MAX_CLIENTS 2
#define SERVER_PORT 53648

class Game;

class Networking
{
private:

	RakPeerInterface*		mServer;
	Console*				mConsole;
	Game*					mGame;

	bool OpenUPNP();
	void DeleteObjectReceived(NetworkID id, int type);

public:
	Networking(Console* console, Game* game);

	void Update();
	RakPeerInterface*		GetServer(){return mServer;}


	void SendConnectionDataToPlayer(RakNetGUID id);
	bool ArePlayersReady();
	void DeleteObjectSend(NetworkID id, int type);
	void SendWinMessage(RakNetGUID winner, RakNetGUID loser);

	//void SendCreatedObjectsIDs(vector<NetworkID> ids);
};




#endif