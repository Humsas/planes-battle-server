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
			addLine(mInputMessage, GAME_CONSOLE_USER_INPUT);

			// If item is selected from old commands, delete old and insert it at the beginning
			if(mInputMessagesListCursorIndex > -1)
				for (int i = 1; i < mInputMessagesList.size(); i++)
				{
					if(mInputMessagesList[i] == mInputMessage)
					{
						mInputMessagesList.erase(mInputMessagesList.begin()+i);
						break;
					}
				}

			mInputMessagesList.push_front(mInputMessage);
			mInputMessagesListCursorIndex = -1;

			// Destroying last list messages
			if(mInputMessagesList.size() > INPUT_MESSAGE_LIST_SIZE)
				mInputMessagesList.pop_back();

			checkForCommand();

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
		// Message can't be too long
		if(mInputMessage.size() >= INPUT_MESSAGE_SIZE)
			return;

		if(!isalnum(c) && !(c==' '))
			return;

		mInputMessage += c;
		break;
	}
}

void Console::addLine(string cont)
{
#pragma omp critical(konsole)
	{
		//mMessagesList->add(cont);
		stringstream ss(cont);
		string line;
		while( getline(ss, line) )
		{
			mMessagesList->add(line);
		}
	}
}

void Console::addLine(string cont, GAME_CONSOLE_ERROR_NUM num)
{
#pragma omp critical(konsole)
	{
		//mMessagesList->add(cont, num);

		stringstream ss(cont);
		string line;
		while( getline(ss, line) )
		{
			mMessagesList->add(line, num);
		}
	}
}

void Console::render(LPDIRECT3DDEVICE9 &d3)
{
	#pragma omp critical(konsole)
	{
		d3->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		d3->SetRenderState(D3DRS_SRCBLENDALPHA, D3DRS_DESTBLENDALPHA);

		IDirect3DVertexBuffer9 *vertexBuffer;
		d3->CreateVertexBuffer(4*sizeof(TLVERTEX), NULL, D3DFVF_TLVERTEX, D3DPOOL_DEFAULT, &vertexBuffer, NULL);

		VOID* pVoid;
		vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, mBorderColor, sizeof(mBorderColor));
		vertexBuffer->Unlock();
		d3->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));
		d3->SetFVF( D3DFVF_TLVERTEX );
		d3->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, mBodyColor, sizeof(mBodyColor));
		vertexBuffer->Unlock();
		d3->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));
		d3->SetFVF( D3DFVF_TLVERTEX );
		d3->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, mInputColor, sizeof(mInputColor));
		vertexBuffer->Unlock();
		d3->SetStreamSource(0, vertexBuffer, 0, sizeof(TLVERTEX));
		d3->SetFVF( D3DFVF_TLVERTEX );
		d3->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		vertexBuffer->Release();

		LPD3DXFONT g_Font = NULL; //font object
		D3DXFONT_DESC FontDesc = {
			20,
			0,
			500,
			0,
			false,
			DEFAULT_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_PITCH,
			L"Calibri"};

		RECT FontPosition;
		FontPosition.left = 10;
		FontPosition.top = 10;
		FontPosition.right = SCREEN_WIDTH;
		FontPosition.bottom = SCREEN_HEIGHT;

		//create font
		D3DXCreateFontIndirect(d3, &FontDesc, &g_Font);

		mMessagesList->setPointer(-(int)((SCREEN_HEIGHT-55)/15));
		string tmpStr;
		console_content tmpCont;
		wstringstream ss;

		while(mMessagesList->get(tmpCont))
		{
			ss.str(L"");
			ss.clear();
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
			case GAME_CONSOLE_USER_INPUT:
				g_Font->DrawText(NULL,
					ss.str().c_str(),
					-1,
					&FontPosition,
					DT_LEFT,
					D3DCOLOR_XRGB(84, 232, 207)); //draw text

				break;
			default:
				g_Font->DrawText(NULL,
					ss.str().c_str(),
					-1,
					&FontPosition,
					DT_LEFT,
					D3DCOLOR_XRGB(255, 255, 255));

				break;
			}

			FontPosition.top += 15;
		}

		FontPosition.top = SCREEN_HEIGHT - 35;
		ss.str(L"");
		ss.clear();
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

void Console::checkForCommand()
{
	int spacePos = mInputMessage.find_first_of(' ');
	string command = "";
	
	if(spacePos != -1)
		command = mInputMessage.substr(0, spacePos);
	else
		command = mInputMessage;
	

	if(command == "trol")
	{
		addLine("Yap troliu geimas!");
		return;
	}
	else if(command == "help")
	{
		addLine("No help for you \\O/");
		return;
	}
	else if(command == "quit")
	{
		addLine("Quiting...");
		PostQuitMessage(1);
		return;
	}
	

	// Command not recognized
	addLine("There is no such command. Type 'Help' for command list.");
}