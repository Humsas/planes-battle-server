#include "console.h"

void Console::add(string cont)
{
#pragma omp critical(konsole)
	{
	contents->add(cont);
	}
}

void Console::add(string cont, GAME_CONSOLE_ERROR_NUM num)
{
#pragma omp critical(konsole)
	{
	contents->add(cont, num);
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

	FontPosition.top = 10;
	FontPosition.left = 10;
	FontPosition.right = 500;
	FontPosition.bottom = 500;

	//create font
	D3DXCreateFontIndirect(d3,&FontDesc,&g_Font);

	

	//ss << "A" << "\n" << "B" << "\n" << "C";

	contents->setPointer(-15);
	string tmpStr;
	console_content tmpCont;

	while(contents->get(tmpCont))
	{
		wstringstream ss;
		ss << tmpCont.value.c_str();// << tmpCont.num;// << endl;

		
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

	//ss << "textasdadasdadadasd" << endl << "sd";
	//ss << "oooo";

	//str  = ss.str().c_str();
 
	
	
	//g_Font->DrawText(NULL,
	//ss.str().c_str(),
 //                  -1,
 //                  &FontPosition,
 //                  DT_LEFT,
 //                  D3DCOLOR_XRGB(19, 36, 67)); //draw text

	

	
	g_Font->Release();
	}

}