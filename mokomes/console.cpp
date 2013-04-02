#include "console.h"

void Console::inputMessageAppend(char c)
{
	switch(c)
	{
		// ignore
	case '\t':
		break;

	case '\r\n':
	case '\r':
		if(mInputMessage.size() > 0)
		{
			// Jei indexas nelygus 0, t1 =inut3 perkelti 5 prieki
			add(mInputMessage);
			mInputMessagesList.push_front(mInputMessage);
			mInputMessagesListCursorIndex = -1;

			// Destroying last list messages
			if(mInputMessagesList.size() > INPUT_MESSAGE_LIST_SIZE-1)
				mInputMessagesList.pop_back();
			sendCopyDataMessage();
			mInputMessage = "";
		}
		break;
	case '\x1b': //Escape
		mInputMessage = "";
		mInputMessagesListCursorIndex = -1;
		break;
	//Backspace - deleting one symbol
	case '\b':
		{
			// Deleting character resets index position
			mInputMessagesListCursorIndex = -1;
			int strSize = mInputMessage.size();

			if(strSize == 0)
				return;
			if(strSize == 1)
			{
				mInputMessage = "";
				return;
			}

			mInputMessage.resize(strSize-1);

			break;
		}

	default:
		mInputMessage += c;
		break;
	}
}

void Console::add(string cont)
{
#pragma omp critical(konsole)
	{
		mMessagesList->add(cont);
	}
}

void Console::add(string cont, GAME_CONSOLE_ERROR_NUM num)
{
#pragma omp critical(konsole)
	{
		mMessagesList->add(cont, num);
	}
}

void Console::render(LPDIRECT3DDEVICE9 &d3)
{
	#pragma omp critical(konsole)
	{
		LPD3DXFONT g_Font = NULL; //font object
		D3DXFONT_DESC FontDesc = {20,
			0,
			400,
			0,
			false,
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_PITCH,
			L"Calibri"};
		RECT FontPosition;

		FontPosition.left = 0;
		FontPosition.top = 0;
		FontPosition.right = SCREEN_WIDTH;
		FontPosition.bottom = SCREEN_HEIGHT;

		//create font
		D3DXCreateFontIndirect(d3,&FontDesc,&g_Font);

		mMessagesList->setPointer(-15);
		string tmpStr;
		console_content tmpCont;

		while(mMessagesList->get(tmpCont))
		{
			wstringstream ss;
			ss << tmpCont.value.c_str();


			switch(tmpCont.num)
			{
			case GAME_CONSOLE_OK:
				g_Font->DrawText(NULL,
					ss.str().c_str(),
					-1,
					&FontPosition,
					DT_LEFT,
					D3DCOLOR_XRGB(19, 36, 67)); //draw text

				break;


			case GAME_CONSOLE_WARNING:
				g_Font->DrawText(NULL,
					ss.str().c_str(),
					-1,
					&FontPosition,
					DT_LEFT,
					D3DCOLOR_XRGB(255, 201, 14)); //draw text

				break;

			case GAME_CONSOLE_ERROR:
				g_Font->DrawText(NULL,
					ss.str().c_str(),
					-1,
					&FontPosition,
					DT_LEFT,
					D3DCOLOR_XRGB(242, 85, 91)); //draw text

				break;


			default:
				g_Font->DrawText(NULL,
					ss.str().c_str(),
					-1,
					&FontPosition,
					DT_LEFT,
					D3DCOLOR_XRGB(255, 255, 255)); //draw text

				break;
			}

			FontPosition.top += 15;
		}

		wstringstream ss;
		ss << mInputMessage.c_str();

		// rasomas tekstas i konsole
		g_Font->DrawText(NULL,
			ss.str().c_str(),
			-1,
			&FontPosition,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255)); //draw text

		g_Font->Release();
	}
}


void Console::enableConsole(HWND mainWindow)
{
	mMainWindow = mainWindow;
}

void Console::sendCopyDataMessage()
{
	LPCSTR lpszString = (LPCSTR)mInputMessage.c_str();
	COPYDATASTRUCT cds;
	cds.dwData = 1; // can be anything
	cds.cbData = sizeof(CHAR) * (strlen(lpszString) + 1);
	cds.lpData = (void*)lpszString;
	SendMessage(mMainWindow, WM_USER, (WPARAM)mMainWindow, (LPARAM)(LPVOID)&cds);
}


void Console::clearInput()
{
	mInputMessage = "";
}


void Console::increaseInputCursorPos()
{
	if((mInputMessagesListCursorIndex < mInputMessagesList.size()-1) || (mInputMessagesListCursorIndex == -1))
	{
		mInputMessagesListCursorIndex++;
		mInputMessage = mInputMessagesList[mInputMessagesListCursorIndex];
	}
}


void Console::decreaseInputCursorPos()
{
	if(mInputMessagesListCursorIndex > -1)
	{
		mInputMessagesListCursorIndex--;
		if(mInputMessagesListCursorIndex > -1)
			mInputMessage = mInputMessagesList[mInputMessagesListCursorIndex];
		else
			mInputMessage = "";
	}
}

void Console::changeInputCursorPos(int newPos)
{
	if(newPos < mInputMessagesList.size() && newPos >= -1)
	{
		mInputMessagesListCursorIndex = newPos;
		if(newPos != -1)
			mInputMessage = mInputMessagesList[mInputMessagesListCursorIndex];
		else
			mInputMessage = "";
	}
}