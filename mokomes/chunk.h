#ifndef CHUNK_H
#define CHUNK_H

#include "terrain.h"
#include "mymath.h"
#include "kamera.h"
#include "textureManager.h"
#include "Entity.h"
#include "quadTree.h"

class Chunk
{
private:
	LPDIRECT3DDEVICE9 d3;
	TextureManager* textureManager;
	Kamera* kamera;
	float x, y, w, h;
	terrain *ter;
	QuadTree *QT;


public:
	Chunk(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera, string mapname, int x, int y);
	~Chunk();

	terrain *getTerrain();
	bool isPointInChunk(Vector *v);

	double getTerrainHeight();


	void Update();
	void Render();

};


#endif