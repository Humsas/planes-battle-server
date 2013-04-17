
//#define _FS 1  // uzkomentavus - windowed mode; atkomentavus full screen mode
#ifndef GLOBAL_PLANE_GAME_SETTINGS
#define GLOBAL_PLANE_GAME_SETTINGS

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


#endif