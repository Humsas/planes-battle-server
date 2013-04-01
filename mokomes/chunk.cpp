#include "chunk.h"

Chunk::Chunk(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera, string mapname, int x, int y)
{
	// TODO
	this->d3 = d3;
	this->textureManager = textureManager;
	this->kamera = kamera;
	this->x = x;
	this->y = y;
	ter = new terrain(d3, textureManager, kamera, mapname, x, y);
	ter->generateTerain();

	this->w = ter->getWidth();
	this->h = ter->getHeight();

	this->QT = new QuadTree(6, 0, x, y, x+ter->getWidth(), y+ter->getHeight());

	ter->Update();
}

Chunk::~Chunk()
{
	// TODO
	if(ter != NULL) delete ter;
	delete QT;
}

terrain *Chunk::getTerrain()
{
	return ter;
}

bool Chunk::isPointInChunk(Vector *v)
{
	return (v->x >= x && v->x <= x+w && v->y >= y && v->y <= y+h);
}

double Chunk::getTerrainHeight()
{
	return 0;//ter->getH();
}


void Chunk::Update()
{
	// TODO
	ter->Update();
}

void Chunk::Render()
{
	// TODO
	ter->Render();
}