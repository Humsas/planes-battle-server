#include "loading.h"
void Loading::Render()
{
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;
	


	splashSprite->Begin(D3DXSPRITE_ALPHABLEND);
	



	// Texture being used is 1920 by 1080:
	//D3DXVECTOR2 spriteCentre=D3DXVECTOR2(1.0f,1.0f);

	// Screen position of the sprite
	D3DXVECTOR2 trans=D3DXVECTOR2(0.0f,0.0f);

	// Rotate based on the time passed
	//float rotation=timeGetTime()/500.0f;

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	//D3DXVECTOR2 scaling(1080.0f * (w/1920.0f) /1000/2 ,1080.0f * (h/1080.0f) / 1000/2 );

	D3DXVECTOR2 scaling((1.0f/2048.0f)*w , (1.0f/2048.0f)*h );

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);
	
	
	// Tell the sprite about the matrix
	splashSprite->SetTransform(&mat);

	splashSprite->Draw(backgroundTexture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	

	float scale = 0.8f;

	// Texture being used is 128 by 128:
	D3DXVECTOR2 spriteCentre=D3DXVECTOR2(64.0f*scale,64.0f*scale);

	// Screen position of the sprite
	trans=D3DXVECTOR2(w*0.76354-64*scale,h*0.58148-64*scale);

	// Rotate based on the time passed
	float rotation=timeGetTime()/1000.0f;

	// Build our matrix to rotate, scale and position our sprite


	D3DXVECTOR2 scaling2(scale, scale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling2,&spriteCentre,rotation,&trans);

	// Tell the sprite about the matrix
	splashSprite->SetTransform(&mat);

	splashSprite->Draw(loadingTexture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));










	splashSprite->End();

	

}