#include "textureManager.h"

#include "console.h"
extern Console cons;

TextureManager::TextureManager(LPDIRECT3DDEVICE9 &d3)
{
	this->d3 = d3;
	textureList = new MyLinkedList<listItem>();
	
	
	// load default "no texture" texture as first item
	listItem* li = new listItem();
	li->textureName = L"NULL";
	D3DXCreateTextureFromFile(d3, L"Textures/missing_texture.jpg", &li->texture);
	textureList->add(li);
}

TextureManager::~TextureManager()
{
	delete textureList;
}


bool TextureManager::isTextureInList(std::wstring textureName)
{
	for(int i = 0; i < textureList->count(); i++)
	{
		if(textureList->get(i)->textureName == textureName) return true;
	}
	return false;
}

IDirect3DTexture9* TextureManager::loadTexture(std::wstring textureName)
{
	listItem* li = new listItem();
	li->textureName = textureName;

	IDirect3DTexture9* texture;

	if((texture = getTextureByName(textureName)) != textureList->get(0)->texture)
	{
		delete li;
		return texture;
	}
	else if(D3DXCreateTextureFromFile(d3, textureName.c_str(), &li->texture) != D3D_OK)
	{
		// i konsole imest klaida
		// ir returnint "no texture" tekstura

		
		string str(textureName.begin(), textureName.end());
		stringstream ss;
		ss << "Tekstura nerasta: " << str;
		cons.add(ss.str(), GAME_CONSOLE_WARNING);

		delete li;
		return textureList->get(0)->texture;
	}
	else
	{
		//OK
		string str(textureName.begin(), textureName.end());
		stringstream ss;
		ss << "Tekstura uzkrauta: " << str;
		cons.add(ss.str(), GAME_CONSOLE_OK);

		textureList->add(li);
		return li->texture;
	}
}


IDirect3DTexture9* TextureManager::getTextureByName(std::wstring textureName)
{
	// neradus grazint nuoroda i "no texture" tekstura
	// pranesti apie tai konsoleje
	for(int i = 0; i < textureList->count(); i++)
	{
		if(textureList->get(i)->textureName == textureName) return textureList->get(i)->texture;
	}
	return textureList->get(0)->texture;
}