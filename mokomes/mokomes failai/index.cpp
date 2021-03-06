// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>






#include"mainClass.h"
#include"mesh.h"
#include"terrain.h"



// define the screen resolution
//#define SCREEN_WIDTH (1280)
//#define SCREEN_HEIGHT (1024)
#define SCREEN_WIDTH (GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT (GetSystemMetrics(SM_CYSCREEN))

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1);

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame();    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics();    // 3D declarations

void generate_terain();

void CreatePointLight(const D3DVECTOR &pos);
void CreateSunLight();

struct CUSTOMVERTEX {FLOAT X, Y, Z; DWORD COLOR;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


LPD3DXBUFFER materialBuffer;
DWORD numMaterials;            // Note: DWORD is a typedef for unsigned long
LPD3DXMESH mesh;




D3DXMATERIAL* d3dxMaterials;

D3DMATERIAL9 *meshMaterials;
LPDIRECT3DTEXTURE9 *meshTextures;


//////////////////



//int *drawMap;
LPDIRECT3DINDEXBUFFER9 m_ib;

WORD *drawMap=NULL;
CUSTOMVERTEX vert[10201];



//struct meshInfo
//{
//LPD3DXBUFFER materialBuffer;
//DWORD numMaterials;            
//LPD3DXMESH mesh;
//
//D3DXMATERIAL* d3dxMaterials;
//D3DMATERIAL9 *meshMaterials;
//LPDIRECT3DTEXTURE9 *meshTextures;
//
//};
//
//struct meshList
//{
//meshInfo mesh;
//meshList *next;
//};


//meshList *MESH = NULL;
//
//
//void loadMesh(string path, string filename, float x, float y, float z, LPDIRECT3DDEVICE9 &d3 );
//void drawMesh(LPDIRECT3DDEVICE9 &d3);







//////////////////////


D3DXVECTOR3 kubelis = D3DXVECTOR3(270,220,50);










MainClass Obj;
Mesh Meshai;
terrain Ter;

LPDIRECT3DDEVICE9* dd = &d3ddev;


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
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";


	

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
                          WS_POPUP | WS_VISIBLE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);




	///////////////
	RAWINPUTDEVICE Rid[2];

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

    ///////////////////



    ShowWindow(hWnd, nCmdShow);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
    MoveWindow(hWnd,
				(cx - SCREEN_WIDTH) / 2,
				(cy - SCREEN_HEIGHT) / 2,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				TRUE);


	if (RegisterRawInputDevices(Rid,2,sizeof(RAWINPUTDEVICE))==FALSE)
	{
	 MessageBox(NULL, L"Input device error", L"Klaida", MB_ICONERROR);
	}


	Ter.setVal();
	//Ter.generateTerain(*dd);

	

	Obj.setPos(250,250,10);
	Obj.refresh();

	ShowCursor(false);


	


	Meshai.setDev(d3ddev);



	//Meshai.d3d = dd;
	

    // set up and initialize Direct3D
    initD3D(hWnd);






    // enter the main loop:

    MSG msg;

    while(TRUE)
    {
		SetCursorPos(cx / 2,cy / 2);

		Obj.loop();

        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		if(msg.message == WM_QUIT)
		break; 










        render_frame();
    }

    // clean up DirectX and COM
    cleanD3D();

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	
	
    switch(message)
    { 
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;



	case WM_INPUT:
	{
		
	// Determine how big the buffer should be
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof (RAWINPUTHEADER));

	// Create a buffer of the correct size - but see note below
	BYTE *buffer=new BYTE[bufferSize];

	// Call the function again, this time with the buffer to get the data
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &bufferSize, sizeof (RAWINPUTHEADER));


	RAWINPUT *raw = (RAWINPUT*) buffer;
	if (raw->header.dwType== RIM_TYPEMOUSE)
	// read the mouse data
	{
	Obj.mouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
	
	}
   

   
    if (raw->header.dwType== RIM_TYPEKEYBOARD)
	// read the keyboard data
    {
	USHORT keyCode=raw->data.keyboard.VKey;
	bool keyUp=raw->data.keyboard.Flags & RI_KEY_BREAK;
	
	switch (keyCode)
	{

	case VK_SHIFT:
		(keyUp) ? Obj.keymap.shift = false : Obj.keymap.shift = true;
		break;

	case 'W':
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
		break;

	}
		


	}
   


	} break; 



    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);


/////






//pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
					//D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					//&d3dpp, &d3dDevice)

/////

    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = true;
	
	
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;  // antialiasing
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	//
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;  // fps limitai
	//d3dpp.FullScreen_RefreshRateInHz = 50;

	//CreatePointLight(D3DXVECTOR3(-40,20,10));



    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);



/////////////////////////////////////////////////
	




// Load the mesh from the specified file
//HRESULT hr=D3DXLoadMeshFromX(L"skybox/skybox.x", D3DXMESH_SYSTEMMEM, 
//                             d3ddev, NULL, 
//                             &materialBuffer,NULL, &numMaterials, 
//                             &mesh );




//d3dxMaterials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
//
//meshMaterials = new D3DMATERIAL9[numMaterials];
//meshTextures  = new LPDIRECT3DTEXTURE9[numMaterials];
//
//LPSTR path = "skybox\\";
//int p_l = strlen(path);
//
//for (DWORD i=0; i<numMaterials; i++)
//{
//
// // Copy the material
// meshMaterials[i] = d3dxMaterials[i].MatD3D;
//
// // Set the ambient color for the material (D3DX does not do this)
// meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
//     
// // Create the texture if it exists - it may not
// meshTextures[i] = NULL;
// if (d3dxMaterials[i].pTextureFilename)
//
// {
//   int ln = strlen(d3dxMaterials[i].pTextureFilename);
//	 char *b = new char[4+ln];
//
//	 for(int o = 0; o < p_l; o++)
//	 {
//	 b[o] = path[o];
//	 }
//	 
//	 for(int o = 0; o < ln; o++)
//	 {
//
//		 b[p_l+o] = d3dxMaterials[i].pTextureFilename[o];
//	 }
//
//
//     D3DXCreateTextureFromFileA(d3ddev,  b,     &meshTextures[i]); 
//
//
// }
////;
//}
// 
//materialBuffer->Release();

Meshai.loadMesh("skybox/","skybox.x","skybox");
Meshai.loadMesh("kol/","kolonke.x","kolonke");

Meshai.loadMesh("skybox/","grid.x","grid");



Meshai.loadMesh("kol/","kol3.x","k3");
Meshai.loadMesh("media/","kubas.x","kubas");
Meshai.loadMesh("kol/","kol su box.x","bass");






//Meshai.loadMesh("kol/","kol3.x",0,0,0);

//Meshai.loadMesh("media/","temple.x",0,0,0);




	
//////////////////////////////////////////////////////
    //init_graphics();    // call the function to initialize the cube
	generate_terain();

	Ter.generateTerain(*dd);

	

	d3ddev->SetRenderState(D3DRS_AMBIENT,RGB(150,150,150));

    d3ddev->SetRenderState(D3DRS_LIGHTING, true);    // turn off the 3D lighting

	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);  // ijungia z buferi

	d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//d3ddev->SetRenderState(D3DRS_DEPTHBIAS , 2 ); 
	
	//d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE); // antialiasing

	CreateSunLight();


	//CreatePointLight(D3DXVECTOR3(0,0,-20));

	//d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	
	
	
}


// this is the function used to render a single frame
void render_frame()
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250, 210, 180), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();

    // select which vertex format we are using
    d3ddev->SetFVF(CUSTOMFVF);

   

    D3DXMATRIX matView;    // the view transform matrix

////////////////////////////////
	////////////////////////////

// teraino fjos

//	float  cZ;
//
//if((Obj.getCamP('x') > -50 && Obj.getCamP('y') > -50) &&
//	((Obj.getCamP('x') < (-50 + (100 * 15)))  && (Obj.getCamP('y') < (-50 + (100 * 15)))))
//
//{
//
//int cellX = (Obj.getCamP('x') - -50) / 15;
//int cellY = (Obj.getCamP('y') - -50) / 15;
//
//
//int dx= (-50 + (cellX * 15)) - Obj.getCamP('x');
//int dy= (-50 + (cellY * 15)) - Obj.getCamP('y');
//
////int dx=Obj.getCamP('x')- -50;
////int dy=Obj.getCamP('y')- -50;
//
//
//int cNr = cellY * 100 + cellX;
//int start = 0;
//
//
//
//
//
//D3DXVECTOR3 A, B, N;
//
//if(dx < dy) 
//{
//// pirmas trikampis
//start = cNr * 6;
//
//}
//else
//{
//// antras trikampis
//start = cNr * 6 + 3;
//
//}
//
//A.x = (vert[drawMap[start+1]].X - vert[drawMap[start+2]].X);
//A.y = (vert[drawMap[start+1]].Y - vert[drawMap[start+2]].Y);
//A.z = (vert[drawMap[start+1]].Z - vert[drawMap[start+2]].Z);
//
//B.x = (vert[drawMap[start]].X - vert[drawMap[start+2]].X);
//B.y = (vert[drawMap[start]].Y - vert[drawMap[start+2]].Y);
//B.z = (vert[drawMap[start]].Z - vert[drawMap[start+2]].Z);
//
//
//N.x = (A.y * B.z - B.y * A.z);
//N.y = -(A.x * B.z - B.x * A.z);
//N.z = (A.x * B.y - B.x * A.y);
//
////cZ = ((Nx * (dx - vert[drawMap[start+2]].X) + Ny * (dy -vert[drawMap[start+2]].Y)) / -Nz) + vert[drawMap[start+2]].Z;
//
////P.y = V0.y+ (N.x * dx + N.z * dz ) / -N.y
//
//cZ = vert[drawMap[start+2]].Z +
//	((N.x * dx + N.y * dy) / N.z) + 10;
//
//
////cZ = vert[drawMap[start]].Z;
//
//
//
//
//
//
//
//}
//else 
//{
//cZ = Obj.getCamP('z');
//
//}



	///////////////////////////
///////////////////////////////


	
	
float cZ = Ter.getH(D3DXVECTOR3(Obj.getCamP('x'),Obj.getCamP('y'),Obj.getCamP('z')));




    D3DXMatrixLookAtLH(&matView,

//&D3DXVECTOR3 (Obj.getCamP('x'), Obj.getCamP('y'), Obj.getCamP('z') ),    // the camera position
//&D3DXVECTOR3 (Obj.getCamW('x'), Obj.getCamW('y'), Obj.getCamW('z') ),    // the l


		&D3DXVECTOR3 (Obj.getCamP('x'), Obj.getCamP('y'), cZ ),    // the camera position
        &D3DXVECTOR3 (Obj.getCamW('x'), Obj.getCamW('y'), cZ +(Obj.getCamW('z') - Obj.getCamP('z')) ),    // the look-at position
        &D3DXVECTOR3 (0.0f, 0.0f, 1.0f));    // the up direction

	

	

    d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

    D3DXMATRIX matProjection;     // the projection transform matrix

    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               1000.0f);    // the far view-plane

    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

    // select the vertex buffer to display
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));



d3ddev->SetRenderState(D3DRS_FILLMODE, 2);



   //d3ddev->SetMaterial( &m_material );
   d3ddev->SetTexture(0,NULL);

   
   //d3ddev->SetStreamSource( 0, m_vb,0, sizeof(CUBEVERTEX) );
   //d3ddev->SetFVF( D3DFVF_CUBEVERTEX );
   /*d3ddev->SetIndices(m_ib);*/
   

   Ter.drawTerain(*dd);

   //d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   //// draw a triangle list using 24 vertices and 12 triangles
   //d3ddev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,10201,0,20000); 
   //d3ddev->SetRenderState(D3DRS_CULLMODE, 0);

   d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

//Meshai.drawMesh(*dd,"kubas",Obj.getCamP('x') , Obj.getCamP('y') ,cZ, 1);


 //   // copy the vertex buffer to the back buffer
 /*   d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);*/

	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 1, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 26, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 28, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 30, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 32, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 34, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 36, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 38, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 40, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 42, 1);

	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 44, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 46, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 48, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 50, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 52, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 54, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 56, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 58, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 60, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 62, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 64, 1);
	//d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 66, 1);

	
   D3DXVECTOR3 N = Ter.getNormal(kubelis);

   static double spdX = 0;
   static double spdY = 0;

   spdX += N.x * 0.0000033;
   spdY += N.y * 0.0000033;

   if(spdX > 0) { spdX -= 0.00001; }
   if(spdY > 0) { spdY -= 0.00001; }

   if(spdX < 0) { spdX += 0.00001; }
   if(spdY < 0) { spdY += 0.00001; }

   kubelis.x -= spdX;
   kubelis.y -= spdY;

   

   /*kubelis.x -= N.x * 0.0013;
   kubelis.y -= N.y * 0.0013;*/

   kubelis.z = Ter.getH(kubelis);
   
   
   Meshai.drawMesh(*dd,"kubas",kubelis.x , kubelis.y ,kubelis.z, 5);



static double po = 0;
//po += 0.001;

  //Meshai.drawMesh(*dd,"kubas",30,30,30);

 // for(int i = 0; i < 1; i++)
 // {
	//  //Meshai.drawMesh(*dd,"kubas",i *10 * po, 10 * po,10*po, 1);
 //for(int j = 0; j < 1; j++)
 // {
 //for(int k = 0; k < 1; k++)
 // {
  Meshai.drawMesh(*dd,"bass",10 , 10 ,10, 1);

  /*}

  }

  }*/

//d3ddev->SetRenderState(D3DRS_FILLMODE, 2);
//Meshai.drawMesh(*dd,"grid", 0, 0, 0, 100);
//d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


  
 /* Meshai.drawMesh(*dd,"kubas",60,60,60);
  
  Meshai.drawMesh(*dd,"kubas",po,-po,-po);
  Meshai.drawMesh(*dd,"kubas",-po,-po,po);
  Meshai.drawMesh(*dd,"kubas",-po,po,-po);
  Meshai.drawMesh(*dd,"kubas",-po,-po,-po);
  Meshai.drawMesh(*dd,"kubas",-po,po,po);

  Meshai.drawMesh(*dd,"kubas",po*2,-po*2,-po);
  Meshai.drawMesh(*dd,"kubas",-po*2,-po,po*2);
  Meshai.drawMesh(*dd,"kubas",-po*2,po*2,-po);
  Meshai.drawMesh(*dd,"kubas",-po*2,-po*2,-po*2);
  Meshai.drawMesh(*dd,"kubas",-po*2,po*2,po*2);*/

 


	d3ddev->SetRenderState( D3DRS_LIGHTING, FALSE );
	Meshai.drawMesh(*dd,"skybox",Obj.getCamP('x'), Obj.getCamP('y'), cZ /*Obj.getCamP('z')*/, 20);
	d3ddev->SetRenderState( D3DRS_LIGHTING, TRUE );
    d3ddev->EndScene();

    d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
   // v_buffer->Release();    // close and release the vertex buffer
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}


// this is the function that puts the 3D models into video RAM
void init_graphics()
{
    // create the vertices using the CUSTOMVERTEX struct
//    CUSTOMVERTEX vertices[] = 
//    {
//        
//        
//        { -10.0f, 10.0f, -10.0f, D3DCOLOR_XRGB(30, 60, 215), },
//		{ 10.0f, 10.0f, -10.0f, D3DCOLOR_XRGB(40, 80, 195), },
//		{ -10.0f, -10.0f,-10.0f, D3DCOLOR_XRGB(10, 20, 255), },
//		{ 10.0f, -10.0f, -10.0f, D3DCOLOR_XRGB(20, 40, 235), },
//
//				
//		{ 10.0f, -10.0f, -10.0f, D3DCOLOR_XRGB(215, 92, 0), },
//		{ 10.0f, 10.0f, -10.0f, D3DCOLOR_XRGB(195, 82, 0), },
//		{ 10.0f, -10.0f, 10.0f, D3DCOLOR_XRGB(255, 122, 0), },
//		{ 10.0f, 10.0f, 10.0f, D3DCOLOR_XRGB(235, 112, 0), },
//
//		{ -10.0f, -10.0f, 10.0f, D3DCOLOR_XRGB(255, 122, 122), },
//		{ -10.0f, 10.0f, 10.0f, D3DCOLOR_XRGB(235, 112, 112), },
//		{ -10.0f, -10.0f, -10.0f, D3DCOLOR_XRGB(215, 92, 92), },
//		{ -10.0f, 10.0f, -10.0f, D3DCOLOR_XRGB(195, 82, 82), },
//
//		{ -10.0f, 10.0f, -10.0f, D3DCOLOR_XRGB(0, 122, 122), },
//		{ -10.0f, 10.0f, 10.0f, D3DCOLOR_XRGB(0, 112, 132), },
//		{ 10.0f, 10.0f, -10.0f, D3DCOLOR_XRGB(0, 92, 142), },
//		{ 10.0f, 10.0f, 10.0f, D3DCOLOR_XRGB(0, 82, 152), },
//		
//		{ 10.0f, -10.0f, 10.0f, D3DCOLOR_XRGB(10, 122, 40), },
//		{ 10.0f, 10.0f, 10.0f, D3DCOLOR_XRGB(20, 142, 30), },
//		{ -10.0f, -10.0f, 10.0f, D3DCOLOR_XRGB(30, 162, 20), },
//		{ -10.0f, 10.0f, 10.0f, D3DCOLOR_XRGB(40, 182, 10), },
//		
//		{ 10.0f, -10.0f, -10.0f, D3DCOLOR_XRGB(200, 200, 200), },
//		{ 10.0f, -10.0f, 10.0f, D3DCOLOR_XRGB(210, 210, 210), },
//		{ -10.0f, -10.0f, -10.0f, D3DCOLOR_XRGB(220, 220, 220), },
//		{ -10.0f, -10.0f, 10.0f, D3DCOLOR_XRGB(230, 230, 230), },
//// gridas
//
//		{ -50.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, 40.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, 40.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, 30.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, 30.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, 20.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, 20.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, 10.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, 10.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, -10.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, -10.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, -20.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, -20.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, -30.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, -30.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, -40.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, -40.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -50.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//
//
//		{ -50.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ -50.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -40.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ -40.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -30.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ -30.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -20.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ -20.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ -10.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ -10.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ 00.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 00.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ 10.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 10.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ 20.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 20.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ 30.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 30.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ 40.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 40.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//		{ 50.0f, 50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//		{ 50.0f, -50.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 0), },
//
//
//
//    };
//
//    // create a vertex buffer interface called v_buffer
//    d3ddev->CreateVertexBuffer(68*sizeof(CUSTOMVERTEX),
//                               0,
//                               CUSTOMFVF,
//                               D3DPOOL_MANAGED,
//                               &v_buffer,
//                               NULL);
//
//    VOID* pVoid;    // a void pointer
//
//    // lock v_buffer and load the vertices into it
//    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
//    memcpy(pVoid, vertices, sizeof(vertices));
//    v_buffer->Unlock();
}






void CreatePointLight(const D3DVECTOR &pos)
{

  // Fill in a light structure defining our light
  D3DLIGHT9 light;
  ZeroMemory( &light, sizeof(D3DLIGHT9) );
  light.Type       = D3DLIGHT_POINT;
  light.Diffuse.r  = 1.0f;
  light.Diffuse.g  = 1.0f;
  light.Diffuse.b  = 1.0f;
    
  // Point lights have no direction but do have a position
  light.Position = pos;
    
  // Tell the device about the light and turn it on
  light.Attenuation0 = 0.5f;
  light.Range=200.0f;
  d3ddev->SetLight( 1, &light );
  d3ddev->LightEnable( 1, TRUE ); 

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
  vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
  D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );

  // Tell the device about the light and turn it on
  d3ddev->SetLight( 0, &light );
  d3ddev->LightEnable( 0, TRUE ); 

}




void generate_terain()
{



int startX = -50;
int startY = -50;

int cellsX = 100;
int cellsY = 100;
int vertX = cellsX +1;
int vertY = cellsY +1;

double stepX = 15;
double stepY = 15;
//const int size =vertX;

//CUSTOMVERTEX *vert = new CUSTOMVERTEX[vertX * vertY];




// cellsX * cellsY * 6;
//int *drawMap;
//drawMap = new int[cellsX * cellsY * 6];






int count = 0;


for(int y = 0; y < vertY; y++)
{
	for(int x = 0; x < vertX; x++)
	{
    
	vert[count].X = startX + (stepX * x);
	vert[count].Y = startY + (stepY * y);
	vert[count].Z = 50* sin((double) x/10)+ 50* sin((double) y/10);
		//(5* sin((double) x/10) * cos((double) y)) + sin((double) x);
	vert[count].COLOR = D3DCOLOR_XRGB(255, 155, 0);

	count++;
	}
}




// create a vertex buffer interface called v_buffer
    d3ddev->CreateVertexBuffer(vertX * vertY *sizeof(CUSTOMVERTEX) ,
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vert, sizeof(vert));
    v_buffer->Unlock();





d3ddev->CreateIndexBuffer((cellsX * cellsY * 2)*3*2,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_ib, NULL);


m_ib->Lock( 0, 0, (void**)&drawMap, 0 );


//int drawMap[96];
count = 0;
int vIndex=0;
for (int y = 0; y < cellsY; y++)
{
    for (int x = 0; x<cellsX; x++)
    {
        // first triangle

		drawMap[count++]=vIndex+vertX+1;
        drawMap[count++]=vIndex+vertX;
        drawMap[count++]=vIndex;



        /*drawMap[count++]=vIndex+vertX+1;
        drawMap[count++]=vIndex+vertX;
        drawMap[count++]=vIndex;*/

                // second triangle
		drawMap[count++]=vIndex + 1;
        drawMap[count++]=vIndex+vertX+1;
        drawMap[count++]=vIndex;
        

		/*drawMap[count++]=vIndex+1;
        drawMap[count++]=vIndex+vertX+1;
        drawMap[count++]=vIndex;*/

        vIndex++;
     }
     vIndex++;
}



m_ib->Unlock();






}