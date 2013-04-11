#ifndef _QUADTREE_H
#define _QUADTREE_H

#include "linkedList.h"
#include "mymath.h"
#include "Entity.h"

struct objectPosition
{
	Vector pozicija;
	float radius;
	bool colision;

	void *_motherBranch;

	objectPosition *colisionWith;

	objectPosition()
	{
		colision = false;
		colisionWith = NULL;
		_motherBranch = NULL;
	}
	objectPosition(Vector pos, float rad)
	{
		objectPosition();
		pozicija	= pos;
		radius		= rad;
	}
};

class QuadTree
{
private:
	// kintamieji
	float startX, startY, endX, endY;
	float width, height;
	int level;

	int cellID; // 0, 1, 2, 3

	MyLinkedList<Entity> *list;

	QuadTree *QT[4];
	QuadTree *motherBranch;

	//funkcijos
	bool canItFit(Entity *op);
	bool isColidingWithQuadBoundaries(Entity *op);

	//void cutAndPaste(objectPosition *op);


public:
	QuadTree(int level, int cellID, float startX, float startY, float endX, float endY, QuadTree *motherBranch = NULL);
	~QuadTree();

	
	void findColisions(Entity *op);
	void addElement(Entity *op, bool deleteOnDestruction);
	bool removeElement(Entity *op);


	void updateElement(Entity *op);

	void paste(Entity *op);

	MyLinkedList<Entity> *getList();
	

	//void addElement(objectPosition *op);

};


#endif