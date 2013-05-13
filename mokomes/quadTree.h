#ifndef _QUADTREE_H
#define _QUADTREE_H

#include "linkedList.h"
#include "mymath.h"
#include "AbstractEntity.h"
#include "InvisibleObjectsChecker.h"

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

	MyLinkedList<AbstractEntity> *list;
	QuadTree *QT[4];
	QuadTree *motherBranch;
	Vector *centerPos;
	float radius;

	//funkcijos	
	bool isColidingWithQuadBoundaries(AbstractEntity *op);

	//void cutAndPaste(objectPosition *op);


public:
	QuadTree(int level, int cellID, float startX, float startY, float endX, float endY, QuadTree *motherBranch = NULL);
	~QuadTree();


	bool wouldItFitInQuad(AbstractEntity *e, float startX, float startY, float endX, float endY);
	bool canItFit(AbstractEntity *op);
	//void findColisions(AbstractEntity *op);
	//void addElement(AbstractEntity *op, bool deleteOnDestruction);

	//bool removeElement(AbstractEntity *op);


	//void paste(AbstractEntity *op);

	MyLinkedList<AbstractEntity> *getList();
	void getAllEntitiesList(MyLinkedList<AbstractEntity> *l);


	//void addElement(objectPosition *op);

	//Vector *getCenterPosition();

	bool add(AbstractEntity *e, bool deleteOnDestruction = false);

	AbstractEntity *searchForColision(AbstractEntity *e);

	void Update(float dt);
	void Render(InvisibleObjectsChecker *visibilityChecker); 
};

#endif