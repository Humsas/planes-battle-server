#include "chunkManager.h"

#include "console.h"
extern Console cons;

ChunkManager::ChunkManager(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera)
{
	// TODO
	this->d3 = d3;
	this->textureManager = textureManager;
	this->kamera = kamera;
	chunksList = new MyLinkedList<Chunk>();

	init();
}

ChunkManager::~ChunkManager()
{
	// TODO
	if(chunksList != NULL) delete chunksList;
}



Chunk *ChunkManager::getChunkByPoint(Vector *v)
{
	Chunk *C = NULL;
	Chunk *tmp;

	for(int i = 0; i < chunksList->count(); i++)
	{
		tmp = chunksList->get(i);
		if(tmp->isPointInChunk(v))
		{
			C = tmp;
			break;
		}
	}	

	return C;
}


void ChunkManager::Update()
{
	// TODO
	for(int i = 0; i < chunksList->count(); i++)
	{
		chunksList->get(i)->Update();
	}
}

void ChunkManager::Render()
{
	// TODO
	for(int i = 0; i < chunksList->count(); i++)
	{
		chunksList->get(i)->Render();
	}
}


void ChunkManager::init()
{
	int mapDimensions = 50 * 512;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			//"Maps/Map0/map.BMP"
			stringstream mapname;
			mapname << "Maps/Map0/tiles/X" << i << "_Y" << j << ".BMP";

			chunksList->add(new Chunk(d3, textureManager, kamera, mapname.str(),j * mapDimensions, i * mapDimensions), true);

		}
	}
}