#ifndef _ALTIMETER
#define _ALTIMETER

#include <d3d9.h>
#include <d3dx9.h>
#include <omp.h>

#include "globalSetings.h"
#include "textureManager.h"

class Altimeter
{
private:
	LPD3DXSPRITE altSprite;
	LPDIRECT3DDEVICE9 d3;
	TextureManager *textureManager;

	LPDIRECT3DTEXTURE9 backgroundTexture;
	LPDIRECT3DTEXTURE9 arrowBlackLong;
	LPDIRECT3DTEXTURE9 arrowRedLong;


	float altitude;
	float masterScale;
	int posX, posY;

public:
	Altimeter(LPDIRECT3DDEVICE9 &d3, TextureManager *textureManager, int x, int y, float scale)
	{
		posX = x;
		posY = y;
		masterScale = scale;
		altitude = 0;
		altSprite = NULL;
		this->d3 = d3;
		this->textureManager = textureManager;
		D3DXCreateSprite(d3, &altSprite);


		backgroundTexture = textureManager->loadTexture(L"Textures/altimeterBackground.png");
		arrowBlackLong = textureManager->loadTexture(L"Textures/arrow_black.png");
		arrowRedLong = textureManager->loadTexture(L"Textures/arrow_red.png");

		/*
		D3DXCreateTextureFromFile(d3, L"Textures/altimeterBackground.png", &backgroundTexture);
		D3DXCreateTextureFromFile(d3, L"Textures/arrow_black.png", &arrowBlackLong);
		//D3DXCreateTextureFromFile(d3, L"Textures/arrow_black_short.png", &arrowBlackShort);
		D3DXCreateTextureFromFile(d3, L"Textures/arrow_red.png", &arrowRedLong);
		*/
		
	}

	~Altimeter()
	{

		//backgroundTexture->Release();
		//arrowBlackLong->Release();
		//arrowBlackShort->Release();
		//arrowRedLong->Release();

		//altSprite->Release();		
	}


	void Render();
	void SetAltitude(float alt);


};

#endif