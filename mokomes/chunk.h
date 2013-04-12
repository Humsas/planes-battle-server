#ifndef CHUNK_H
#define CHUNK_H

#include "terrain.h"
#include "mymath.h"
#include "kamera.h"
#include "textureManager.h"
#include "AbstractEntity.h"
#include "quadTree.h"
#include "declarations.h"
#include "InvisibleObjectsChecker.h"

class Chunk
{
private:
	LPDIRECT3DDEVICE9 d3;
	TextureManager* textureManager;
	Kamera* kamera;
	float x, y, w, h;
	Terrain *ter;
	QuadTree *QT;


public:
	Chunk(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera, string mapname, int x, int y);
	~Chunk();

	Terrain *getTerrain();
	QuadTree *getQuadTree();
	bool isPointInChunk(Vector *v);

	double getTerrainHeight();


	void Update(float dt);
	void Render(InvisibleObjectsChecker *visibilityChecker);

};


#endif