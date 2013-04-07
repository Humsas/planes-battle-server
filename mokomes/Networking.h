#ifndef PLANES_BATTLE_SERVER_NETWORKING
#define PLANES_BATTLE_SERVER_NETWORKING

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
#include "Itoa.h"
//#include "Strsafe.h"		// StringCbPrintf funkcijai

#include "console.h"

using namespace std;
using namespace RakNet;

#define MAX_CLIENTS 2
#define SERVER_PORT 53648

enum GameMessages
{
	ID_GAME_MESSAGE_1=ID_USER_PACKET_ENUM+1,
	ID_GAME_MESSAGE_PLAYER_ACTION,
	ID_GAME_MESSAGE_PLAYER_CREATED_FOR_CLIENT,
	ID_GAME_MESSAGE_PLAYER_CREATED,
	ID_GAME_MESSAGE_PUT_BOMB,
	ID_GAME_MESSAGE_BOMB_EXPLOSION,
	ID_GAME_MESSAGE_GAME_UPDATE,
	ID_GAME_MESSAGE_PLAYER_DISCONNECTED
};


class Networking
{
private:

	RakPeerInterface*	mServer;
	Console*			mConsole;
	Packet*				mPacket;



	bool OpenUPNP();

public:
	Networking(Console* console);

};




#endif