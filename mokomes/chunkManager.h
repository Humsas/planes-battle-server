#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <sstream>
#include "chunk.h"
#include "mymath.h"
#include "linkedList.h"
#include "textureManager.h"
#include "declarations.h"
#include "InvisibleObjectsChecker.h"


class ChunkManager
{
private:
	LPDIRECT3DDEVICE9 d3;
	TextureManager* textureManager;
	Kamera* kamera;
	InvisibleObjectsChecker* visibilityChecker;
	
	MyLinkedList<Chunk> *chunksList;
	MyLinkedList<AbstractEntity> *stacicEntityLeftovers;

	MyLinkedList<AbstractEntity> *dynamicEntities;
	
	
	//QuadTree *QTLeftovers;

	void init();

	Chunk *getChunkByPoint(Vector *v);


	//////////////////////////
	// SHIT FOR THE WIN :D
	//MyLinkedList<AbstractEntity> *entityList;
	//////////////////////////



public:
	ChunkManager(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera);
	~ChunkManager();


	double getMapHeightAtPoint(D3DXVECTOR3 pos);
	double getMapHeightAtPoint(Vector &pos);

	void addEntity(AbstractEntity *e);
	

	void Update(float dt);
	void Render();

};


#endif