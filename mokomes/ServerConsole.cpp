#include "ServerConsole.h"

ServerConsole::ServerConsole()
{
	mConsoleCommand = L"";
	mConsoleCommandSpacesCount = 0;

	//Console window creation
	AllocConsole();

	mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (mConsoleHandle == INVALID_HANDLE_VALUE) 
		MessageBox(NULL, L"Error creating console", L"Console error", MB_ICONERROR);

	SetConsoleMode(mConsoleHandle, ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_PROCESSED_INPUT);

	// Linkinam į konsolę cout, cin, wcout, wcin
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	mConsoleHandle2 = GetConsoleWindow();
}


VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
	printf("Key event: ");

	if(ker.bKeyDown)
		printf("key pressed\n");
	else printf("key released\n");
}

void ServerConsole::Update()
{
	DWORD cNumRead, fdwMode, i; 
	INPUT_RECORD irInBuf[128]; 
	PeekConsoleInput(mConsoleHandle, irInBuf, 128, &cNumRead);
	for (i = 0; i < cNumRead; i++) 
	{
		switch(irInBuf[i].EventType) 
		{ 
		case KEY_EVENT: // keyboard input 
			KeyEventProc(irInBuf[i].Event.KeyEvent); 
			break; 

		case MOUSE_EVENT: // mouse input 
			//MouseEventProc(irInBuf[i].Event.MouseEvent); 
			break; 

		case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
			//ResizeEventProc( irInBuf[i].Event.WindowBufferSizeEvent ); 
			break; 

		case FOCUS_EVENT:  // disregard focus events 

		case MENU_EVENT:   // disregard menu events 
			break; 

		default: 
			//ErrorExit("Unknown event type"); 
			break; 
		} 
	}
	wstring tmp;
	getline(wcin, tmp);
	ProcessCommand(tmp);
}

void ServerConsole::ProcessCommand(wstring commandText)
{
	if(commandText.size() < 1)
		return;

	int spacePos = commandText.find_first_of(L' ');
	wstring command = L"";

	if(spacePos != -1)
		command = commandText.substr(0, spacePos);
	else
		command = commandText;

	if(command == L"trol")
	{
		wcout << "Yap troliu geimas!" << endl;
		return;
	}
	else if(command == L"help")
	{
		wcout << L"No help for you \\O/" << endl;  //\(￣▽￣)/
	}

	// Command not recognised
	wcout << "There is no such command. Type 'Help' for command list." << endl;
}

void ServerConsole::Write(wstring text)
{
	COORD oldCoords;
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	//PCONSOLE_CURSOR_INFO info;
	//GetConsoleCursorInfo(mConsoleHandle, info);
	GetConsoleScreenBufferInfo(mConsoleHandle, &SBInfo);
	oldCoords = SBInfo.dwCursorPosition;
	//GetConsoleOutputCP()
	COORD newPos = {0, oldCoords.Y};
	SetConsoleCursorPosition(mConsoleHandle, newPos);

	wcout << text << endl;

	//Restore cursor pos
	oldCoords.Y = oldCoords.Y+1;
	SetConsoleCursorPosition(mConsoleHandle, oldCoords);
}

bool ServerConsole::IsActiveWindow()
{
	return (GetForegroundWindow() == mConsoleHandle2);
}