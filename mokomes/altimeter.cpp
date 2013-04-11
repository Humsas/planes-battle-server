#include "altimeter.h"
void Altimeter::SetAltitude(float alt)
{
	altitude = alt;
}

void Altimeter::Render()
{

	altSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	D3DXMATRIX mat;

	float scale = 0.5 * masterScale;
	int textureCenter = 256;
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(posX-textureCenter*scale, posY-textureCenter*scale);

	// Texture being used is 512 by 512:
	
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(textureCenter*scale, textureCenter*scale);


	D3DXVECTOR2 scaling(scale, scale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,0,&trans);

	// Tell the sprite about the matrix
	altSprite->SetTransform(&mat);

	altSprite->Draw(backgroundTexture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));


	scale = 1.0 * masterScale;
	textureCenter = 128; // 256*256
	trans = D3DXVECTOR2(posX-textureCenter*scale, posY-textureCenter*scale);
	spriteCentre = D3DXVECTOR2(textureCenter*scale, textureCenter*scale);
	scaling.x = scale;
	scaling.y = scale;
	float rotation = altitude * 0.00628f;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
	altSprite->SetTransform(&mat);

	altSprite->Draw(arrowRedLong, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));



	scale = 0.8 * masterScale;
	textureCenter = 128; // 256*256
	trans = D3DXVECTOR2(posX-textureCenter*scale, posY-textureCenter*scale);
	spriteCentre = D3DXVECTOR2(textureCenter*scale, textureCenter*scale);
	scaling.x = scale;
	scaling.y = scale;
	rotation = altitude * 0.000628f;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
	altSprite->SetTransform(&mat);

	altSprite->Draw(arrowBlackLong, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));


	scale = 0.2 * masterScale;
	textureCenter = 128; // 256*256
	trans = D3DXVECTOR2(posX-textureCenter*scale, posY+  87*0.5*masterScale -textureCenter*scale);
	spriteCentre = D3DXVECTOR2(textureCenter*scale, textureCenter*scale);
	scaling.x = scale;
	scaling.y = scale;
	rotation = altitude * 0.0628f;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
	altSprite->SetTransform(&mat);

	altSprite->Draw(arrowBlackLong, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));

	altSprite->End();
}