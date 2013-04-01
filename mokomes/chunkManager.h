#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <sstream>
#include "chunk.h"
#include "mymath.h"
#include "linkedList.h"
#include "textureManager.h"


class ChunkManager
{
private:
	LPDIRECT3DDEVICE9 d3;
	TextureManager* textureManager;
	Kamera* kamera;
	MyLinkedList<Chunk> *chunksList;

	void init();

	Chunk *getChunkByPoint(Vector *v);

public:
	ChunkManager(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera);
	~ChunkManager();


	double getMapHeightAtPoint(D3DXVECTOR3 pos)
	{
		Vector *v = new Vector(pos.x, pos.z, pos.y);
		Chunk *c = getChunkByPoint(v);

		double result = 0;

		if(c != NULL)
		{
			result = c->getTerrain()->getH(pos);
		}

		return result;


	}
	

	void Update();
	void Render();

};


#endif