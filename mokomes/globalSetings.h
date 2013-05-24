//#define _FS 1  // uzkomentavus - windowed mode; atkomentavus full screen mode
#ifndef GLOBAL_PLANE_GAME_SETTINGS
#define GLOBAL_PLANE_GAME_SETTINGS

#include "MessageIdentifiers.h"

#ifndef _FS
#define FULL_SCREEN (0)
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)
#else
#define FULL_SCREEN (1)
#define SCREEN_WIDTH (GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT (GetSystemMetrics(SM_CYSCREEN))
#endif


enum GameEntitiesList
{
	GAME_ENTITY_CUBE,
	GAME_ENTITY_AIRCRAFT_B17,
	GAME_ENTITY_PROJECTILE_BOMB,
	GAME_ENTITY_BUILDING
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
	ID_GAME_MESSAGE_DELETE_OBJECT,
	ID_GAME_MESSAGE_OBJECT_CREATION_CONFIRMED,
	ID_GAME_MESSAGE_OBJECT_CREATION_CANCELED,
	ID_GAME_MESSAGE_GAME_UPDATE,
	ID_GAME_MESSAGE_PLAYER_DISCONNECTED,
	ID_GAME_MESSAGE_KEYS_UPDATE,
	ID_GAME_MESSAGE_PLAYER_WON
};

#define MAX_HEALTH_PLANE						30
#define MAX_HEALTH_BUILDING						2

#define DAMAGE_PROJECTILE_BOMB					10

//Additional Game keys
#define GAME_KEY_QUIT_GAME						0x1A0
#define GAME_KEY_JOY_BUTTON1					0x1A1
#define GAME_KEY_JOY_BUTTON2					0x1A2
#define GAME_KEY_JOY_BUTTON3					0x1A3
#define GAME_KEY_JOY_BUTTON4					0x1A4
#define GAME_KEY_JOY_BUTTON5					0x1A5
#define GAME_KEY_JOY_BUTTON6					0x1A6
#define GAME_KEY_JOY_BUTTON7					0x1A7
#define GAME_KEY_JOY_BUTTON8					0x1A8
#define GAME_KEY_JOY_BUTTON9					0x1A9
#define GAME_KEY_JOY_BUTTON10					0x1AA
#define GAME_KEY_MOUSE_MOVE						0x1B0
#define GAME_KEY_MOUSE_WHEEL					0x1B1
#define GAME_KEY_MOUSE_LEFT_BUTTON_DOWN			0x1B2
#define GAME_KEY_MOUSE_LEFT_BUTTON_UP			0x1B3
#define GAME_KEY_MOUSE_RIGHT_BUTTON_DOWN		0x1B4
#define GAME_KEY_MOUSE_RIGHT_BUTTON_UP			0x1B5

#define GAME_CONFIG_PROJECTILE_WEAPON_COOLDOWN			250

#endif