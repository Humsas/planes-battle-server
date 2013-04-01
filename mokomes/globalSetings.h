
//#define _FS 1  // uzkomentavus - windowed mode; atkomentavus full screen mode

#ifndef _FS
	#define FULL_SCREEN (0)
	#define SCREEN_WIDTH (1536)
	#define SCREEN_HEIGHT (864)
#else
	#define FULL_SCREEN (1)
	#define SCREEN_WIDTH (GetSystemMetrics(SM_CXSCREEN))
	#define SCREEN_HEIGHT (GetSystemMetrics(SM_CYSCREEN))
#endif