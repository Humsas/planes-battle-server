#include <d3d9.h>
#include <d3dx9.h>
#include <omp.h>

#include "globalSetings.h"
#include "textureManager.h"

class Anglemeter
{
private:
	LPD3DXSPRITE altSprite;
	LPDIRECT3DDEVICE9 d3;
	TextureManager *textureManager;
	LPDIRECT3DTEXTURE9 backgroundTexture;
	LPDIRECT3DTEXTURE9 ringTexture;



	float angle;
	float masterScale;
	int posX, posY;

public:
	Anglemeter(LPDIRECT3DDEVICE9 &d3, TextureManager *textureManager, int x, int y, int scale)
	{
		posX = x;
		posY = y;
		masterScale = scale;
		angle = 0;
		altSprite = NULL;
		this->d3 = d3;
		this->textureManager = textureManager;
		D3DXCreateSprite(d3, &altSprite);

		backgroundTexture = textureManager->loadTexture(L"Textures/AnglemeterBackgroundHorizont.png");
		ringTexture = textureManager->loadTexture(L"Textures/AnglemeterBackgroundRing.png");

		/*
		D3DXCreateTextureFromFile(d3, L"Textures/AnglemeterBackgroundHorizont.png", &backgroundTexture);
		D3DXCreateTextureFromFile(d3, L"Textures/AnglemeterBackgroundRing.png", &ringTexture);
		*/

		//if( FAILED(hr) ) MessageBox(NULL,L"Device error.",L"Load Error",MB_OK);
	}

	~Anglemeter()
	{
		/*
		backgroundTexture->Release();
		ringTexture->Release();

		altSprite->Release();
		*/
	}


	void Render();
	void SetAngle(float ang);


};