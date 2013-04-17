#include <d3d9.h>
#include <d3dx9.h>
#include <omp.h>

#include "globalSetings.h"

class Loading
{
private:
	LPD3DXSPRITE splashSprite;
	LPDIRECT3DDEVICE9 d3;
	LPDIRECT3DTEXTURE9 backgroundTexture;
	LPDIRECT3DTEXTURE9 loadingTexture;

public:
	Loading(LPDIRECT3DDEVICE9 &d3)
	{
		splashSprite = NULL;
		this->d3 = d3;
		D3DXCreateSprite(d3, &splashSprite);
		D3DXCreateTextureFromFile(d3, L"Textures/splash.jpg", &backgroundTexture);
		D3DXCreateTextureFromFile(d3, L"Textures/loading.png", &loadingTexture);

		//if( FAILED(hr) ) MessageBox(NULL,L"Device error.",L"Load Error",MB_OK);
	}

	~Loading()
	{
		loadingTexture->Release();
		backgroundTexture->Release();
		splashSprite->Release();		
	}


	void Render();


};