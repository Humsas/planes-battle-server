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

#define INPUT_MESSAGE_LIST_SIZE 5

class Console
{
private:
	HWND			mMainWindow;
	Masyvas*		mMessagesList;
	string			mInputMessage;
	deque<string>	mInputMessagesList;
	int				mInputMessagesListCursorIndex;

	void sendCopyDataMessage();

public:
	Console() 
	{ 
		mInputMessagesList.resize(INPUT_MESSAGE_LIST_SIZE);
		mInputMessagesListCursorIndex = -1;
		mMessagesList = new Masyvas(); 
		mInputMessage = ""; 
	}
	//~Console() { if(contents != NULL) { contents->~Masyvas(); } }

	void inputMessageAppend(char c);
	void add(string cont);
	void add(string cont, GAME_CONSOLE_ERROR_NUM num);
	void render(LPDIRECT3DDEVICE9 &d3);

	void enableConsole(HWND mainWindow = NULL);
	void clearInput();
	void increaseInputCursorPos();
	void decreaseInputCursorPos();
	void changeInputCursorPos(int newPos);
};