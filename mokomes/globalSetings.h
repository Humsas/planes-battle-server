//#define _FS 1  // uzkomentavus - windowed mode; atkomentavus full screen mode
#ifndef GLOBAL_PLANE_GAME_SETTINGS
#define GLOBAL_PLANE_GAME_SETTINGS

#include "MessageIdentifiers.h"

#ifndef _FS
	#define FULL_SCREEN (0)
	#define SCREEN_WIDTH (1536)
	#define SCREEN_HEIGHT (864)
#else
	#define FULL_SCREEN (1)
	#define SCREEN_WIDTH (GetSystemMetrics(SM_CXSCREEN))
	#define SCREEN_HEIGHT (GetSystemMetrics(SM_CYSCREEN))
#endif


enum GameEntitiesList
{
	GAME_ENTITY_CUBE
};

enum GameChannels
{
	GAME_CHANNEL_UPDATE,
	GAME_CHANNEL_NEW_DATA,
	GAME_CHANNEL_CHAT_MESSAGES
};

enum GameMessages
{
	ID_GAME_MESSAGE_CONNECTION_DATA=ID_USER_PACKET_ENUM+1,
	ID_GAME_MESSAGE_LOADING_COMPLETED,
	ID_GAME_MESSAGE_PLAYER_CREATED_FOR_CLIENT,
	ID_GAME_MESSAGE_PLAYER_CREATED,
	ID_GAME_MESSAGE_PLAYERS_READY,
	ID_GAME_MESSAGE_NEW_OBJECT_CREATED,
	ID_GAME_MESSAGE_GAME_UPDATE,
	ID_GAME_MESSAGE_PLAYER_DISCONNECTED
};


#endif