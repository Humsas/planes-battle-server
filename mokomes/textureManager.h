#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "linkedList.h"



class TextureManager
{
private:
	struct listItem
	{
		
		std::wstring		textureName;
		IDirect3DTexture9*	texture;
		
		listItem() 
		{ 
			texture = NULL; 
			textureName = L""; 
		}
		
		~listItem() 
		{
			if(texture != NULL) delete texture;
		}
	};

	LPDIRECT3DDEVICE9 d3;
	MyLinkedList<listItem> *textureList;
	

	bool isTextureInList(std::wstring textureName);

public:
	TextureManager(LPDIRECT3DDEVICE9 &d3);
	~TextureManager();

	IDirect3DTexture9* loadTexture(std::wstring textureName);
	IDirect3DTexture9* getTextureByName(std::wstring textureName);

};

#endif