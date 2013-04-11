#ifndef PLANES_BATTLE_SERVER_CONSOLE
#define PLANES_BATTLE_SERVER_CONSOLE

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include <tchar.h>
#include <deque>
#include <omp.h>

#include "masyvas.h"
#include "globalSetings.h"

using namespace std;

#define INPUT_MESSAGE_LIST_SIZE 100
#define INPUT_MESSAGE_SIZE 110

//Custom vertex format
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW| D3DFVF_DIFFUSE | D3DFVF_TEX1;

//Custom vertex
struct TLVERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	D3DCOLOR colour;
	float u;
	float v;
};

class Console
{
private:
	HWND			mMainWindow;
	Masyvas*		mMessagesList;
	string			mInputMessage;
	deque<string>	mInputMessagesList;
	int				mInputMessagesListCursorIndex;

	void sendCopyDataMessage();

	TLVERTEX		mBodyColor[4];
	TLVERTEX		mBorderColor[4];
	TLVERTEX		mInputColor[4];

public:
	Console() 
	{ 
		mInputMessagesList.resize(INPUT_MESSAGE_LIST_SIZE);
		mInputMessagesListCursorIndex = -1;
		mMessagesList = new Masyvas(); 
		mInputMessage = ""; 

		mBodyColor[0].x = 5;
		mBodyColor[0].y = 5;
		mBodyColor[0].z = 0;
		mBodyColor[0].rhw = 1.0f;
		mBodyColor[0].colour = D3DCOLOR_XRGB(127, 0, 0);

		mBodyColor[1].x = SCREEN_WIDTH-5;
		mBodyColor[1].y = 5;
		mBodyColor[1].z = 0;
		mBodyColor[1].rhw = 1.0f;
		mBodyColor[1].colour = D3DCOLOR_XRGB(127, 0, 0);

		mBodyColor[2].x = 5;
		mBodyColor[2].y = SCREEN_HEIGHT-5;
		mBodyColor[2].z = 0;
		mBodyColor[2].rhw = 1.0f;
		mBodyColor[2].colour = D3DCOLOR_XRGB(127, 0, 0);

		mBodyColor[3].x = SCREEN_WIDTH-5;
		mBodyColor[3].y = SCREEN_HEIGHT-5;
		mBodyColor[3].z = 0;
		mBodyColor[3].rhw = 1.0f;
		mBodyColor[3].colour = D3DCOLOR_XRGB(127, 0, 0);

		mBorderColor[0].x = 0;
		mBorderColor[0].y = 0;
		mBorderColor[0].z = 0;
		mBorderColor[0].rhw = 1.0f;
		mBorderColor[0].colour = D3DCOLOR_XRGB(0, 0, 0);

		mBorderColor[1].x = SCREEN_WIDTH;
		mBorderColor[1].y = 0;
		mBorderColor[1].z = 0;
		mBorderColor[1].rhw = 1.0f;
		mBorderColor[1].colour = D3DCOLOR_XRGB(0, 0, 0);

		mBorderColor[2].x = 0;
		mBorderColor[2].y = SCREEN_HEIGHT;
		mBorderColor[2].z = 0;
		mBorderColor[2].rhw = 1.0f;
		mBorderColor[2].colour = D3DCOLOR_XRGB(0, 0, 0);

		mBorderColor[3].x = SCREEN_WIDTH;
		mBorderColor[3].y = SCREEN_HEIGHT;
		mBorderColor[3].z = 0;
		mBorderColor[3].rhw = 1.0f;
		mBorderColor[3].colour = D3DCOLOR_XRGB(0, 0, 0);

		mInputColor[0].x = 7;
		mInputColor[0].y = SCREEN_HEIGHT-35;
		mInputColor[0].z = 0;
		mInputColor[0].rhw = 1.0f;
		mInputColor[0].colour = D3DCOLOR_XRGB(0, 150, 0);

		mInputColor[1].x = SCREEN_WIDTH-10;
		mInputColor[1].y = SCREEN_HEIGHT-35;
		mInputColor[1].z = 0;
		mInputColor[1].rhw = 1.0f;
		mInputColor[1].colour = D3DCOLOR_XRGB(0, 150, 0);

		mInputColor[2].x = 7;
		mInputColor[2].y = SCREEN_HEIGHT-10;
		mInputColor[2].z = 0;
		mInputColor[2].rhw = 1.0f;
		mInputColor[2].colour = D3DCOLOR_XRGB(0, 150, 0);

		mInputColor[3].x = SCREEN_WIDTH-10;
		mInputColor[3].y = SCREEN_HEIGHT-10;
		mInputColor[3].z = 0;
		mInputColor[3].rhw = 1.0f;
		mInputColor[3].colour = D3DCOLOR_XRGB(0, 150, 0);
	}

	void inputMessageAppend(char c);
	void addLine(string cont);
	void addLine(string cont, GAME_CONSOLE_ERROR_NUM num);
	void render(LPDIRECT3DDEVICE9 &d3);

	void enableConsole(HWND mainWindow = NULL);
	void clearInput();
	void increaseInputCursorPos();
	void decreaseInputCursorPos();
	void changeInputCursorPos(int newPos);
};


#endif