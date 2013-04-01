#ifndef _SPEEDOMETER
#define _SPEEDOMETER

#include <d3d9.h>
#include <d3dx9.h>
#include <omp.h>

#include "globalSetings.h"
#include "textureManager.h"

class Speedometer
{
private:
	LPD3DXSPRITE speedSprite;
	LPDIRECT3DDEVICE9 d3;
	TextureManager *textureManager;

	LPDIRECT3DTEXTURE9 backgroundTexture;
	LPDIRECT3DTEXTURE9 arrowBlackLong;
	LPDIRECT3DTEXTURE9 arrowRedLong;


	float speed;
	float masterScale;
	int posX, posY;

public:
	Speedometer(LPDIRECT3DDEVICE9 &d3, TextureManager *textureManager, int x, int y, float scale)
	{
		posX = x;
		posY = y;
		masterScale = scale;
		speed = 0;
		speedSprite = NULL;
		this->d3 = d3;
		this->textureManager = textureManager;
		D3DXCreateSprite(d3, &speedSprite);

		backgroundTexture = textureManager->loadTexture(L"Textures/SpeedometerBackground.png");
		arrowBlackLong = textureManager->loadTexture(L"Textures/arrow_black.png");
		arrowRedLong = textureManager->loadTexture(L"Textures/arrow_red.png");

		/*
		D3DXCreateTextureFromFile(d3, L"Textures/SpeedometerBackground.png", &backgroundTexture);
		D3DXCreateTextureFromFile(d3, L"Textures/arrow_black.png", &arrowBlackLong);
		//D3DXCreateTextureFromFile(d3, L"Textures/arrow_black_short.png", &arrowBlackShort);
		D3DXCreateTextureFromFile(d3, L"Textures/arrow_red.png", &arrowRedLong);
		*/
		//if( FAILED(hr) ) MessageBox(NULL,L"Device error.",L"Load Error",MB_OK);
	}

	~Speedometer()
	{
		/*
		backgroundTexture->Release();
		arrowBlackLong->Release();
		//arrowBlackShort->Release();
		arrowRedLong->Release();

		speedSprite->Release();	
		*/
	}


	void Render();
	void SetSpeed(float speed);


};

#endif