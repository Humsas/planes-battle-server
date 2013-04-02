//#pragma once
//
//#include <windows.h>
//#include <string>
//#include <iostream>
//
//using namespace std;
//
//class ServerConsole
//{
//private:
//	HHOOK			mHookHandle;
//	HANDLE			mConsoleHandle;
//	HANDLE			mConsoleHandleInput;
//	HWND			mConsoleHandle2;   //For checking if console window is active
//
//public:
//
//	wstring			mConsoleCommand;
//	int				mConsoleCommandSpacesCount;
//public:
//	ServerConsole();
//	~ServerConsole(){};
//
//	//Checks for new commands
//	void Update();
//
//	//Processes commands
//	void ProcessCommand(wstring commandText);
//
//	//Writes text in new line to the console
//	void Write(wstring text);
//
//	//Checks if console window is active
//	bool IsActiveWindow();
//	HANDLE GetConsoleHandle(){return mConsoleHandle;};
//};