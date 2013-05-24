// include the basic windows header files and the Direct3D header file
#include "WinsockWrapper.h"
#include <d3d9.h>
#include <d3dx9.h>

#include <omp.h>

#include"mainClass.h"
#include"console.h"
#include"loading.h"
#include "game.h"
#include"matematika.h"
#include "globalSetings.h"
#include "ServerConsole.h"
#include <windows.h>
#include <windowsx.h>
#include "FPSLimiter.h"


//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX0 | D3DFVF_TEX1 );

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
//#pragma comment (lib, "d3dx9d.lib")  // debug mode

#pragma comment (lib, "d3dx9.lib")


// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

FPSLimiter FPSLimitForRender(30);
FPSLimiter FPSLimitForUpdate(60);

// function prototypes
bool initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame();    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics();    // 3D declarations

void SetupPixelFog(DWORD Color, DWORD Mode);
void CreatePointLight(const D3DVECTOR &pos);
void CreateSunLight();

void CheckForCommand(string c);

struct CUSTOMVERTEX {FLOAT X, Y, Z; FLOAT fU, fV; FLOAT fU2, fV2;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_TEX0 | D3DFVF_TEX1)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Console gServerConsole;

//ServerConsole gServerConsole;

MainClass Obj;
Game *game;
Loading* loadingScreen;
LPDIRECT3DDEVICE9* dd = &d3ddev;

//If this window is active - true, if not - false(does not work on console window)
bool gIsActive;

bool loading = false;


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"GameClass";




	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"GameClass", L"Plains Battle",
		WS_MINIMIZEBOX, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX); 

	///////////////
	RAWINPUTDEVICE Rid[3];

	// Keyboard
	Rid[0].usUsagePage = 1;
	Rid[0].usUsage = 6;
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget=NULL;

	// Mouse
	Rid[1].usUsagePage = 1;
	Rid[1].usUsage = 2;
	Rid[1].dwFlags = 0;
	Rid[1].hwndTarget=NULL;

	// Joystick
	Rid[2].usUsagePage = 1;
	Rid[2].usUsage = 4;
	Rid[2].dwFlags = 0;
	Rid[2].hwndTarget=NULL;

	///////////////////

	//gServerConsole = ServerConsole();
	//wcout << "Initializing...\n";

	ShowWindow(hWnd, nCmdShow);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(hWnd,
		(cx - SCREEN_WIDTH) / 2,
		(cy - SCREEN_HEIGHT) / 2,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		TRUE);


	if (RegisterRawInputDevices(Rid,3,sizeof(RAWINPUTDEVICE))==FALSE)
	{
		MessageBox(NULL, L"Input device error", L"Input error", MB_ICONERROR);
	}



	Obj.setPos(500,500,500);
	Obj.refresh(10);



	MSG msg;

	// set up and initialize Direct3D
	if(initD3D(hWnd))
	{

		bool kraunasi = true;
		loading = true;

		//omp_set_dynamic(0);
		omp_set_nested(TRUE);	
#pragma omp parallel sections num_threads(2) shared(kraunasi)
		{

#pragma omp section
			{
				gServerConsole.enableConsole(hWnd);
				d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
				while(kraunasi)
				{

					while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}



					d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250, 210, 180), 1.0f, 0);
					d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
					d3ddev->BeginScene();
					loadingScreen->Render();
					gServerConsole.render(*dd);
					d3ddev->EndScene();
					d3ddev->Present(NULL, NULL, NULL, NULL);
					//Sleep(10);
				}
				d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			}

#pragma omp section
			{
				gServerConsole.addLine("Loading...");
				game = new Game(*dd);
				init_graphics();
				gServerConsole.addLine("Loaded");
				kraunasi = false;
			}
		}

		//MessageBox(NULL, L"Krovimo gijos baige darba", L"OK", MB_ICONERROR);
		kraunasi = false;
		loading = false;
#pragma omp barrier


		//ShowCursor(false);


		// enter the main loop:
		bool suktis = true;

#pragma omp parallel sections num_threads(3) shared(suktis)
		{

			while(suktis)
			{
				if(hWnd == GetForegroundWindow())
				{ 
					//ShowCursor(false);
					//SetCursorPos(cx / 2,cy / 2); 
					Obj.loop();
					render_frame();
				}
				else
				{
					//ShowCursor(true);
					//Sleep(500);
				}

				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					if(msg.message == WM_QUIT) 
						suktis = false;
				}

				if(msg.message == WM_QUIT)	
				{
					//MessageBox(NULL, L"Uzdarymas", L"Klaida", MB_ICONERROR);
					break; 
				}
				FPSLimitForRender.doSleep();
			}

#pragma omp section
			while(suktis)
			{
				//if(game->getNetwork()->ArePlayersReady())
				game->Update();
				FPSLimitForUpdate.doSleep();
			}

#pragma omp section
			while(suktis)
			{
				if(!kraunasi)
					game->getNetwork()->Update();
			}


		}

		// clean up DirectX and COM
		delete game;
		cleanD3D();
		return msg.wParam;
	}

	return -1;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*ZeroRudder();
	ZeroAilerons();
	ZeroElevators();*/

	switch(message)
	{ 
	case WM_CHAR:
		{
			gServerConsole.inputMessageAppend((char)wParam);
			break;
		}

		// Message from console class passing its message
	case WM_USER:
		{
			COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
			if (pcds->dwData == 1)
			{
				LPCSTR lpszString = (LPCSTR)(pcds->lpData);
				CheckForCommand(lpszString);

				//MessageBoxA(NULL, lpszString, "Klaida", MB_ICONERROR);
			}


			//MessageBox(NULL, L"Atvyko copy data", L"Klaida", MB_ICONERROR);
			break;
		}

	case WM_CREATE:
		{
			/*
			if(joySetCapture(hWnd, JOYSTICKID1, NULL, FALSE)) 
			{ 
			MessageBeep(MB_ICONEXCLAMATION); 
			MessageBox(hWnd, L"Couldn't capture the joystick.", NULL, 
			MB_OK | MB_ICONEXCLAMATION); 
			//PostMessage(hWnd,WM_CLOSE,0,0L); 
			} */
			break;
		}
	case WM_DESTROY:
		{
			//MessageBox(NULL, L"WM_DESTROY", L"Klaida", MB_ICONERROR);
			PostQuitMessage(1);

			return 0;
			break;
		} 
	case WM_CLOSE:
		{
			//MessageBox(NULL, L"WM_CLOSE", L"Klaida", MB_ICONERROR);

			if(MessageBox(hWnd, L"Ar tikrai?", L"Close", MB_OKCANCEL | MB_TOPMOST ) == 1)
				DestroyWindow(hWnd);
			return 0;
			break;

		}
		//Gets message on app activation, deactivation
	case WM_ACTIVATEAPP:
		gIsActive = (bool)wParam;
		break;

	case WM_QUIT:
		{
			//MessageBox(NULL, L"WM_QUIT", L"Klaida", MB_ICONERROR);
			return 0;
			break;
		}

	case WM_INPUT:
		{
			// Determine how big the buffer should be
			UINT bufferSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof (RAWINPUTHEADER));

			// Create a buffer of the correct size - but see note below
			BYTE *buffer = new BYTE[bufferSize];

			// Call the function again, this time with the buffer to get the data
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &bufferSize, sizeof (RAWINPUTHEADER));

			// read the mouse data
			RAWINPUT *raw = (RAWINPUT*) buffer;
			if (raw->header.dwType == RIM_TYPEMOUSE/* && !loading*/)
			{ 
				//game->getScene()->getCamera()->updateAngle(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

				if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
				{ 

					// read the mouse data
					if((SHORT)raw->data.mouse.usButtonData > 0) 
					{
						//Obj.mouseRatas(30);
						//game->getScene()->getCamera()->Zoom(10);
						gServerConsole.increaseDrawingPointer();
					}
					else
					{
						gServerConsole.decreaseDrawingPointer();
						//Obj.mouseRatas(-30);
						//game->getScene()->getCamera()->Zoom(-10);
					}

				}

				/*
				if(raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
				{
				game->fire(true);
				}

				if(raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
				{
				game->fire(false);
				}

				if(raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
				{
				game->drop(true);
				}

				if(raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
				{
				game->drop(false);
				}
				*/
			}

			if (raw->header.dwType== RIM_TYPEKEYBOARD/* && !loading*/)
			{ 
				// read the keyboard data
				USHORT keyCode=raw->data.keyboard.VKey;
				bool keyUp=raw->data.keyboard.Flags & RI_KEY_BREAK;

				switch (keyCode)
				{
				case VK_HOME:
					gServerConsole.drawingHome();
					break;
				case VK_END:
					gServerConsole.drawingEnd();
					break;
					//case VK_OEM_3:
					//	(keyUp)? "" : cons.enableDisableConsole();
					//break;
					/*
					case VK_ESCAPE:
					//PostMessage(hWnd, WM_CLOSE, 0, 0);
					//PostQuitMessage(WM_QUIT);
					//DestroyWindow(hWnd);
					break;

					case VK_SHIFT:
					(keyUp) ? Obj.keymap.shift = false : Obj.keymap.shift = true;
					break;
					case VK_SPACE:
					{
					//wcout << L" ";
					}
					break;
					case VK_RETURN:
					{
					//TODO: Padaryti komandu handlinima
					//wcout << endl << L"kiaule" << endl << gConsoleCommand.c_str() << endl;
					//gConsoleCommand = L"";
					}
					break;

					*/

					/*case 'W':
					(keyUp) ? Obj.keymap.w = false : Obj.keymap.w = true;
					break;

					case 'S':
					(keyUp) ? Obj.keymap.s = false : Obj.keymap.s = true;
					break;

					case 'A':
					(keyUp) ? Obj.keymap.a = false : Obj.keymap.a = true;
					break;

					case 'D':
					(keyUp) ? Obj.keymap.d = false : Obj.keymap.d = true;
					break;*/

					/*						ZeroRudder();
					ZeroAilerons();
					ZeroElevators();*/

					/*case 'I':
					(keyUp)? game->lektuvas->ZeroElevators() : game->lektuvas->PitchDown();
					break;

					case 'K':
					(keyUp)? game->lektuvas->ZeroElevators() : game->lektuvas->PitchUp();
					break;

					case 'J':
					(keyUp)? game->lektuvas->ZeroAilerons() : game->lektuvas->RollLeft();
					break;

					case 'L':
					(keyUp)? game->lektuvas->ZeroAilerons() : game->lektuvas->RollRight();
					break;

					case 'U':
					(keyUp)? game->lektuvas->ZeroRudder() : game->lektuvas->LeftRudder();
					break;

					case 'O':
					(keyUp)? game->lektuvas->ZeroRudder() : game->lektuvas->RightRudder();
					break;

					case 'Y':
					(keyUp)? "" : game->lektuvas->IncThrust();
					break;

					case 'H':
					(keyUp)? "" : game->lektuvas->DecThrust();
					break;

					case 'R':
					(keyUp)? "" : game->getScene()->changeRenderMode();
					break;

					case 'Z':
					(keyUp)? game->testCraft->startEngine(false) : game->testCraft->startEngine(true);
					break;

					case 'X':
					(keyUp)? "" : game->testCraft->stopEngine();
					break;


					case 'W':
					(keyUp)? game->testCraft->setElevator(0) : game->testCraft->setElevator(1.0);
					break;

					case 'S':
					(keyUp)? game->testCraft->setElevator(0) : game->testCraft->setElevator(-1.0);
					break;


					case 'A':
					(keyUp)? game->testCraft->setElerons(0) : game->testCraft->setElerons(1.0);
					break;

					case 'D':
					(keyUp)? game->testCraft->setElerons(0) : game->testCraft->setElerons(-1.0);
					break;


					case 'Q':
					(keyUp)? game->testCraft->setRuder(0) : game->testCraft->setRuder(-1.0);
					break;

					case 'E':
					(keyUp)? game->testCraft->setRuder(0) : game->testCraft->setRuder(1.0);
					break;


					case 'C':
					(keyUp)? "" : game->getScene()->getCamera()->switchCameraMode();
					break;


					case 'B':
					(keyUp)? game->testCraft->setBrakes(false) : game->testCraft->setBrakes(true);
					break;

					case 'F':
					(keyUp)? game->testCraft->setFlaps(0.0) : game->testCraft->setFlaps(1.0);
					break;

					case 'G':
					(keyUp)? "" : game->testCraft->gearUpDown();
					break;
					*/
				case VK_UP:
					(keyUp)? "" : gServerConsole.increaseInputCursorPos();
					//(keyUp)? "" : game->testCraft->increaseThrottle();
					break;

				case VK_DOWN:
					(keyUp)? "" : gServerConsole.decreaseInputCursorPos();
					//(keyUp)? "" : game->testCraft->decreaseThrottle();
					break;
					/*
					case VK_RIGHT:
					(keyUp)? "" : game->testCraft->increasePropAdvance();
					break;

					case VK_LEFT:
					(keyUp)? "" : game->testCraft->decreasePropAdvance();
					break;

					//game->testCraft->increaseThrottle();


					case 'P':
					(keyUp)? (AP = false) : (AP = true);
					break;

					case 'T':
					(keyUp)? (false) : (drop = true);
					break;*/



					/*case 'Z':
					(keyUp)? "" : game->getScene()->slope(-0.05);
					break;

					case 'X':
					(keyUp)? "" : game->getScene()->slope(0.05);
					break;

					case 'C':
					(keyUp)? "" : game->getScene()->bias(-0.05);
					break;

					case 'V':
					(keyUp)? "" : game->getScene()->bias(0.05);
					break;*/
				}
			}
			break; 
		} 
	}

	return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
bool initD3D(HWND hWnd)
{
	bool ret = true;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = (FULL_SCREEN)?false:true;

	D3DDISPLAYMODE d3ddm;
	d3d->GetAdapterDisplayMode(0, &d3ddm);



	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;  // antialiasing
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferCount = 3;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	//
	d3dpp.EnableAutoDepthStencil = TRUE; // gylio testas
	//d3dpp.AutoDepthStencilFormat = D3DFMT_X8R8G8B8; // /*D3DFMT_D24S8*/;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // gylio buferio formatas



	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;  // fps limitai (FPS limitas isjungtas)



	// create a device class using this information and the info from the d3dpp stuct

	HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp,
		&d3ddev);

	if( FAILED(hr) ) 
	{
		ret = false;
		MessageBox(NULL,L"Hardware/software not supported DirectX parameters",L"DirectX error",MB_OK);
	}


	if(ret) 
	{

		d3ddev->SetRenderState(D3DRS_AMBIENT,RGB(200,200,200));
		d3ddev->SetRenderState(D3DRS_LIGHTING, true);    // turn off the 3D lighting
		d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);  // ijungia z buferi


		// Allow multiple passes to blend together correctly
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		/*
		// Allow multiple passes to blend together correctly
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		//d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		*/

		/*
		for(int i = 0; i < 11; i++)
		{ // teksturu filtravimai

		d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		d3ddev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		d3ddev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		//d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		d3ddev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		d3ddev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
		}
		*/


		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


		// alpha testo nustatymai
		// cia ijungtas visisko permatomumo forcinimas kai alfa >= 128 (medziu lapam ir panasiai)
		d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		d3ddev->SetRenderState(D3DRS_ALPHAREF, 128);


		// rukas
		SetupPixelFog(D3DCOLOR_XRGB(176, 196, 222), D3DFOG_LINEAR);


		//d3ddev->SetRenderState(D3DRS_DEPTHBIAS , 2 ); 
		//d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE); // antialiasing

		CreateSunLight();


		//CreatePointLight(D3DXVECTOR3(0,0,-20));
		//d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


		loadingScreen = new Loading(*dd);

	}

	return ret;
}


// this is the function used to render a single frame
void render_frame()
{
	game->Render();
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	// v_buffer->Release();    // close and release the vertex buffer
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
	//cloud->~Clouds();
	loadingScreen->~Loading();
}


// this is the function that puts the 3D models into video RAM
void init_graphics()
{

}






void CreatePointLight(const D3DVECTOR &pos)
{

	//// Fill in a light structure defining our light
	//D3DLIGHT9 light;
	//ZeroMemory( &light, sizeof(D3DLIGHT9) );
	//light.Type       = D3DLIGHT_POINT;
	//light.Diffuse.r  = 1.0f;
	//light.Diffuse.g  = 1.0f;
	//light.Diffuse.b  = 1.0f;
	//  
	//// Point lights have no direction but do have a position
	//light.Position = pos;
	//  
	//// Tell the device about the light and turn it on
	//light.Attenuation0 = 0.5f;
	//light.Range=200.0f;
	//d3ddev->SetLight( 1, &light );
	//d3ddev->LightEnable( 1, TRUE ); 

}

void CreateSunLight()
{


	// Fill in a light structure defining our light
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Diffuse.a  = 1.0f;
	light.Range      = 1000.0f;

	// Create a direction for our light - it must be normalized  
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(1.0f,-1.1f,1.0f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

	// Tell the device about the light and turn it on
	d3ddev->SetLight( 0, &light );
	d3ddev->LightEnable( 0, TRUE );



}

void SetupPixelFog(DWORD Color, DWORD Mode)
{
	float Start   = 5000.0f;    // For linear mode
	float End     = 25000.8f;
	float Density = 0.66f;   // For exponential modes

	// Enable fog blending.
	d3ddev->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// Set the fog color.
	d3ddev->SetRenderState(D3DRS_FOGCOLOR, Color);

	// Set fog parameters.
	if( Mode == D3DFOG_LINEAR )
	{
		d3ddev->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		d3ddev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		d3ddev->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
	}
	else
	{
		d3ddev->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		d3ddev->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
	}
}

/*
// Instaliuojam global・keyhook・
void installhook()          // su WH_JOURNALPLAYBACK galima insertinti savo signalus
{
gHookHandle = NULL;
gHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyEvent, GetModuleHandle(NULL), 0);
if(gHookHandle == NULL)
MessageBox(NULL, L"Unable to install hook", L"Error!", MB_OK);
}

// Cia pareina keyhooko signalai, tinkant・signal・ jau nepersiun鑛u kitoms programoms
LRESULT WINAPI KeyEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
bool keyDown = (wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN);
if( (nCode == HC_ACTION) && keyDown )
{
KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

switch (kbdStruct.vkCode)
{
case VK_RETURN:
if(gServerConsole.IsActiveWindow())
wcout << L"Labas" << endl;
//return 1;
break;
case VK_SPACE:
gServerConsole.mConsoleCommandSpacesCount++;
break;
case VK_ESCAPE:
case VK_LEFT:
case VK_RIGHT:
case VK_UP:
case VK_DOWN:
if(gServerConsole.IsActiveWindow())
return 1;
break;
default:
break;
}
}

return CallNextHookEx(gHookHandle, nCode, wParam, lParam);      //pass control to next hook in the hook chain.
}
*/




void CheckForCommand(string c)
{
	while(c[0] == ' ')
		c = c.substr(1, c.size()-1);

	while(c[c.size()-1] == ' ')
		c.resize(c.size()-1);

	int spacePos = c.find_first_of(' ');
	string command = "";
	vector<RakString> parametersList;

	if(spacePos != -1)
	{
		command = c.substr(0, spacePos);
		int j = -1;
		int cycles = 0;
		while((j = c.find_first_of(' ', spacePos+1)) != -1)
		{
			string tmp = c.substr(spacePos+1, j-(spacePos+1));
			if(tmp.size() == 0)
			{
				spacePos = j;
				continue;
			}

			while(tmp[0] == ' ')
				tmp = tmp.substr(1, tmp.size()-1);

			while(tmp[tmp.size()-1] == ' ')
				tmp.resize(tmp.size()-1);

			RakString la = tmp.c_str();

			parametersList.push_back(la);
			spacePos = j;
			cycles++;
			if(cycles >= 10)
				break;
		}
		if(c.size() > spacePos+1)
		{
			string tmp = c.substr(spacePos+1, c.size()-(spacePos+1));
			if(tmp.size() != 0)
			{
				while(tmp[0] == ' ')
					tmp = tmp.substr(1, tmp.size()-1);

				while(tmp[tmp.size()-1] == ' ')
					tmp.resize(tmp.size()-1);

				RakString la = tmp.c_str();
				parametersList.push_back(la);
			}
		}
	}
	else
		command = c;


	if(command == "trol")
	{
		gServerConsole.addLine("Yap troliu geimas!");
		return;
	}
	else if(command == "help")
	{
		gServerConsole.addLine("No help for you \\O/");
		return;
	}
	else if(command == "spawn")
	{
		game->CreateCubes();
		return;
	}
	else if(command == "move")
	{
		game->MoveCubes();
		return;
	}
	else if(command == "quit")
	{
		gServerConsole.addLine("Quiting...");
		PostQuitMessage(1);
		return;
	}


	// Command not recognized
	gServerConsole.addLine("There is no such command. Type 'Help' for command list.");
}