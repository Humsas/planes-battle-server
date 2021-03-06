#include "chunkManager.h"

#include "console.h"
extern Console gServerConsole;

ChunkManager::ChunkManager(LPDIRECT3DDEVICE9 &d3, TextureManager* textureManager, Kamera* kamera)
{
	// TODO
	this->d3 = d3;
	this->textureManager = textureManager;
	this->kamera = kamera;
	
	visibilityChecker = new InvisibleObjectsChecker(d3);
	chunksList = new MyLinkedList<Chunk>();
	
	//this->QTLeftovers = new QuadTree(0, 0, 0, 0, 0, 0);


	stacicEntityLeftovers = new MyLinkedList<AbstractEntity>();
	dynamicEntities = new MyLinkedList<AbstractEntity>();

	deletedEntities = new MyLinkedList<AbstractEntity>();

	//entityList = new MyLinkedList<AbstractEntity>();

	init();
}

ChunkManager::~ChunkManager()
{
	// TODO
	if(chunksList != NULL) delete chunksList;
	if(visibilityChecker != NULL) delete visibilityChecker;
	//delete QTLeftovers;
	delete stacicEntityLeftovers;
	delete dynamicEntities;
	delete deletedEntities;
	//delete entityList;
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

double ChunkManager::getMapHeightAtPoint(D3DXVECTOR3 pos)
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

double ChunkManager::getMapHeightAtPoint(Vector &pos)
{
	Vector *v = new Vector(pos);
	Chunk *c = getChunkByPoint(v);

	double result = 0;

	if(c != NULL)
	{
		result = c->getTerrain()->getH(D3DXVECTOR3(pos.x, 1, pos.y));
	}

	return result;
}

void ChunkManager::addEntity(AbstractEntity *e)
{
	if(e->getEntityType() == ENTITY_STATIC)
	{
		bool arIdeta = false;

		for(int i = 0; i < chunksList->count(); i++)
		{
			if(chunksList->get(i)->getQuadTree()->canItFit(e))
			{
				chunksList->get(i)->getQuadTree()->add(e);
				arIdeta = true;
				break;
			}
		}

	
		if(!arIdeta)
		{
			stacicEntityLeftovers->add(e, true);
		}
	}
	else
	{
		dynamicEntities->add(e);
	}
	
	//entityList->add(e);

}


AbstractEntity *ChunkManager::searchForColision(AbstractEntity *e)
{
	if(e != NULL)
	{
		dynamicEntities->updateIteratorReset();
		AbstractEntity *ed = NULL;
		while((ed = dynamicEntities->getNextUpdate()) != NULL)
		{
			if(ed != e && ed->checkForBaseColision(e))
			{
				return ed;
			}
		}

		stacicEntityLeftovers->updateIteratorReset();
		AbstractEntity *es = NULL;
		while((es = stacicEntityLeftovers->getNextUpdate()) != NULL)
		{
			if(es != e && es->checkForBaseColision(e))
			{
				return es;
			}
		}

		AbstractEntity *tmpE = NULL;
		for(int i = 0; i < chunksList->count(); i++)
		{
			tmpE = chunksList->get(i)->searchForColision(e);
			if (tmpE != NULL)
			{
				return tmpE;
			}
		}

		return NULL;
	}
	return NULL;
}

bool ChunkManager::checkGroundColision(AbstractEntity *e)
{
	return getMapHeightAtPoint(*e->getPosition()) > e->getPosition()->z - e->getScale();
}

void ChunkManager::safeRemove(AbstractEntity *e)
{
	if(e != NULL)
	{
		dynamicEntities->safeRemove(e);
		stacicEntityLeftovers->safeRemove(e);

		for(int i = 0; i < chunksList->count(); i++)
		{
			chunksList->get(i)->safeRemove(e);
		}


		deletedEntities->readAllListsIteratorReset();
		bool found = false;
		AbstractEntity *de = NULL;
		while((de = stacicEntityLeftovers->getNextReadAllLists()) != NULL)
		{
			if(de == e) { found = true; }
		}

		if(!found) { deletedEntities->add(e,true); }

	}
}

MyLinkedList<AbstractEntity> *ChunkManager::getDynamicEntityList()
{
	return dynamicEntities;
}


void ChunkManager::getAllEntitiesList(MyLinkedList<AbstractEntity> *list)
{
	for(int i = 0; i < chunksList->count(); i++)
	{
		chunksList->get(i)->getAllEntitiesList(list);
	}


	stacicEntityLeftovers->readAllListsIteratorReset();
	AbstractEntity *es = NULL;
	while((es = stacicEntityLeftovers->getNextReadAllLists()) != NULL)
	{
		list->add(es, false);
	}


	dynamicEntities->readAllListsIteratorReset();
	AbstractEntity *ed = NULL;
	while((ed = dynamicEntities->getNextReadAllLists()) != NULL)
	{
		list->add(ed, false);
	}
}


void ChunkManager::Update(float dt)
{
	for(int i = 0; i < chunksList->count(); i++)
	{
		chunksList->get(i)->Update(dt);
	}


	stacicEntityLeftovers->updateIteratorReset();
	AbstractEntity *es = NULL;
	while((es = stacicEntityLeftovers->getNextUpdate()) != NULL)
	{
		es->Update(dt);
	}


	dynamicEntities->updateIteratorReset();
	AbstractEntity *ed = NULL;
	while((ed = dynamicEntities->getNextUpdate()) != NULL)
	{
		ed->Update(dt);
	}
}

void ChunkManager::Render()
{
	Vector *cameraPosition = new Vector(kamera->getCamP().x, kamera->getCamP().z, kamera->getCamP().y);

	visibilityChecker->updateCamera(cameraPosition);

	for(int i = 0; i < chunksList->count(); i++)
	{
		chunksList->get(i)->Render(visibilityChecker);
	}


	stacicEntityLeftovers->renderIteratorReset();
	AbstractEntity *es = NULL;
	while((es = stacicEntityLeftovers->getNextRender()) != NULL)
	{
		if(visibilityChecker->isVisible(es->getPosition(), es->getRadius()))
			es->Render();
	}
	

	dynamicEntities->renderIteratorReset();
	AbstractEntity *ed = NULL;
	while((ed = dynamicEntities->getNextRender()) != NULL)
	{
		if(visibilityChecker->isVisible(ed->getPosition(), ed->getRadius()))
			ed->Render();
	}

	/*
	stringstream ss;
	ss << visibilityChecker->getAllCheckedObjectsCount() << ":" << visibilityChecker->getPassedObjectsCount();
	gServerConsole.addLine(ss.str());
	*/

	delete cameraPosition;
}

void ChunkManager::init()
{
	int mapDimensions = 50 * 512;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			//"Maps/Map0/map.BMP"
			stringstream mapname;
			mapname << "Maps/Map0/tiles/X" << i << "_Y" << j << ".BMP";

			chunksList->add(new Chunk(d3, textureManager, kamera, mapname.str(),j * mapDimensions, i * mapDimensions), true);

		}
	}
}