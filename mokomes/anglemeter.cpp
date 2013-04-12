#include "Anglemeter.h"
void Anglemeter::SetAngle(float ang)
{
	this->angle = ang;
}

void Anglemeter::Render()
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
	float rotation = angle * 3.141592653589f / 180;
	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);

	// Tell the sprite about the matrix
	altSprite->SetTransform(&mat);

	altSprite->Draw(backgroundTexture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	

	scale = 0.5 * masterScale;
	textureCenter = 256; // 512*512
	trans = D3DXVECTOR2(posX-textureCenter*scale, posY-textureCenter*scale);
	spriteCentre = D3DXVECTOR2(textureCenter*scale, textureCenter*scale);
	scaling.x = scale;
	scaling.y = scale;
	rotation = 0.0f;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
	altSprite->SetTransform(&mat);

	altSprite->Draw(ringTexture, NULL, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	
	altSprite->End();
}