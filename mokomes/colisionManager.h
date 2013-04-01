#ifndef _COLISIONMANAGER_H
#define _COLISIONMANAGER_H

#include "quadTree.h"
#include "terrain.h"



class ColisionManager
{
private:
	QuadTree	*QT;
	terrain		*ter;

public:
	ColisionManager(terrain *ter);
	~ColisionManager();

	QuadTree *getQuadTree();

	void findColisions(Entity *op);

	float groundLevel(Entity *op);

	/*void addElement()
	{

	}*/

	void update(Entity *op);

};


#endif