#ifndef _COLISIONMANAGER_H
#define _COLISIONMANAGER_H

#include "quadTree.h"
#include "terrain.h"



class ColisionManager
{
private:
	QuadTree	*QT;
	Terrain		*ter;

public:
	ColisionManager(Terrain *ter);
	~ColisionManager();

	QuadTree *getQuadTree();

	void findColisions(AbstractEntity *op);

	float groundLevel(AbstractEntity *op);

	/*void addElement()
	{

	}*/

	void update(AbstractEntity *op);

};


#endif