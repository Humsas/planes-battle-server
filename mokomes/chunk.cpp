#include "chunk.h"

Chunk::Chunk(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera, string mapname, int x, int y)
{
	// TODO
	this->d3 = d3;
	this->textureManager = textureManager;
	this->kamera = kamera;
	this->x = x;
	this->y = y;
	ter = new Terrain(d3, textureManager, kamera, mapname, x, y);
	ter->generateTerain();

	this->w = ter->getWidth();
	this->h = ter->getHeight();

	this->QT = new QuadTree(QUADTREE_DEPTH, 0, x, y, x+ter->getWidth(), y+ter->getHeight());

	ter->Update();
}

Chunk::~Chunk()
{
	// TODO
	if(ter != NULL) delete ter;
	delete QT;
}

Terrain *Chunk::getTerrain()
{
	return ter;
}

QuadTree *Chunk::getQuadTree()
{
	return QT;
}

bool Chunk::isPointInChunk(Vector *v)
{
	return (v->x >= x && v->x <= x+w && v->y >= y && v->y <= y+h);
}

double Chunk::getTerrainHeight()
{
	return 0;//ter->getH();
}

void Chunk::getAllEntitiesList(MyLinkedList<AbstractEntity> *list)
{
	QT->getAllEntitiesList(list);
}


AbstractEntity *Chunk::searchForColision(AbstractEntity *e)
{
	if(e != NULL)
	{
		return QT->searchForColision(e);
	}
	return NULL;
}

void Chunk::Update(float dt)
{
	// TODO
	ter->Update();
	QT->Update(dt);
}

void Chunk::Render(InvisibleObjectsChecker *visibilityChecker)
{
	// TODO
	ter->Render();
	QT->Render(visibilityChecker);

}