#include "quadTree.h"
#include "console.h"
extern Console gServerConsole;

QuadTree::QuadTree(int level, int cellID, float startX, float startY, float endX, float endY, QuadTree *motherBranch)
{
	this->level			= level;
	this->startX		= startX;
	this->startY		= startY;
	this->endX			= endX;
	this->endY			= endY;

	this->width			= abs(endX - startX);
	this->height		= abs(endY - startY);

	this->motherBranch	= motherBranch;

	list				= new MyLinkedList<AbstractEntity>();

	QT[0] = QT[1] = QT[2] = QT[3] = NULL;

	centerPos = new Vector(startX + width/2, startY + height/2, 0);

	radius = Vector(startX - endX,
		startY - endY,
		0).Magnitude()/2;

	/*
	if(level > 0)
	{
	QT[0]			= new QuadTree(level-1, 0, startX,				startY + height/2,	startX + width/2,	endY,				this);
	QT[1]			= new QuadTree(level-1, 1, startX + width/2,	startY + height/2,	endX,				endY,				this);
	QT[2]			= new QuadTree(level-1, 2, startX,				startY,				startX + width/2,	startY + height/2,	this);
	QT[3]			= new QuadTree(level-1, 3, startX + width/2,	startY,				endX,				startY + height/2,	this);
	}*/
}

QuadTree::~QuadTree()
{
	delete list;
	delete centerPos;
	if(QT[0] != NULL) delete QT[0];
	if(QT[1] != NULL) delete QT[1];
	if(QT[2] != NULL) delete QT[2];
	if(QT[3] != NULL) delete QT[3];
}

bool QuadTree::wouldItFitInQuad(AbstractEntity *e, float startX, float startY, float endX, float endY)
{
	return (e->getPosition()->x - e->getRadius() > startX &&
		e->getPosition()->x + e->getRadius() < endX &&
		e->getPosition()->y - e->getRadius() > startY &&
		e->getPosition()->y + e->getRadius() < endY);
}

bool QuadTree::canItFit(AbstractEntity *op)
{
	return(	op->getPosition()->x - op->getRadius() > startX &&
		op->getPosition()->x + op->getRadius() < endX &&
		op->getPosition()->y - op->getRadius() > startY &&
		op->getPosition()->y + op->getRadius() < endY);
}

bool QuadTree::isColidingWithQuadBoundaries(AbstractEntity *op)
{
	return(	canItFit(op) ||
		op->getPosition()->x-op->getRadius() <= startX && startX	<= op->getPosition()->x+op->getRadius() ||
		op->getPosition()->x-op->getRadius() <= endX	&& endX		<= op->getPosition()->x+op->getRadius() ||
		op->getPosition()->y-op->getRadius() <= startY && startY	<= op->getPosition()->y+op->getRadius() ||
		op->getPosition()->y-op->getRadius() <= endY	&& endY		<= op->getPosition()->y+op->getRadius());
}


//void QuadTree::findColisions(AbstractEntity *op)
//{
//	//TODO: tikrinti kolizijas su savo grandinele
//	list->iteratorReset();
//
//	AbstractEntity *ts = NULL;
//
//	//stringstream ss2;//create a stringstream
//	//ss2 << "Tikrinama su: " << list->count();
//			
//
//			//gServerConsole.addLine(ss2.str());
//
//	while((ts = list->getNext()) != NULL)
//	{
//		//gServerConsole.addLine("ttt");
//
//		if(ts != op && (op->getRadius() > 0 && ts->getRadius() > 0) &&
//			(((ts->getPosition()->x - op->getPosition()->x) * (ts->getPosition()->x - op->getPosition()->x) +
//			(ts->getPosition()->y - op->getPosition()->y) * (ts->getPosition()->y - op->getPosition()->y) +
//			(ts->getPosition()->z - op->getPosition()->z) * (ts->getPosition()->z - op->getPosition()->z)) 
//			<= 	
//			((ts->getRadius()*ts->getRadius()) + (op->getRadius()*op->getRadius())))
//		)
//		{
//			// DO SOMETHING
//			/*/////////////////////////////
//			ts->colisionWith = op;
//			ts->colision = true;
//
//			op->colision = true;
//			*////////////////////////////////
//
//			//stringstream ss1;//create a stringstream
//			//ss1 << "Colision sphere: " << op << " with " << ts;
//			//
//
//			//gServerConsole.addLine(ss1.str(),GAME_CONSOLE_WARNING);
//		}
//	}
//	
//
//
//
//	// tolimesnis persiuntimas gilyn
//	if(level > 0)
//	{
//		for(int i = 0; i < 4; i++)
//		{
//			if(QT[i]->isColidingWithQuadBoundaries(op))
//			{
//				QT[i]->findColisions(op);
//			}
//		}
//	}
//
//}


/*
void QuadTree::addElement(AbstractEntity *op, bool deleteOnDestruction = false)
{	
bool added = false;
if(level > 0)
{
for(int i = 0; i < 4; i++)
{
if(QT[i]->canItFit(op))
{
QT[i]->addElement(op);
added = true;
break;
}
}
}
if(!added)
{
/////////////////////////////////////////////////////
op->_motherBranch = this;
list->add(op, deleteOnDestruction); 	
}
}*/

/*
bool QuadTree::removeElement(AbstractEntity *op)
{
return ((QuadTree*)op->_motherBranch)->getList()->remove(op);
}
*/
/*
void QuadTree::paste(AbstractEntity *op)
{
list->paste(((QuadTree*)op->_motherBranch)->getList()->cut(op));
op->_motherBranch = this;
}
*/
/*
void QuadTree::cutAndPaste(objectPosition *op)
{

}*/

/*
void QuadTree::updateElement(AbstractEntity *op)
{
bool added = false;
if(level > 0)
{
for(int i = 0; i < 4; i++)
{
if(QT[i]->canItFit(op))
{
QT[i]->updateElement(op);
added = true;
break;
}
}
}
if(!added)
{
//op->_motherBranch = this;
paste(op);
op->_motherBranch = this;
//list->add(op); 	
}



}
*/


MyLinkedList<AbstractEntity> *QuadTree::getList()
{
	return list;
}

void QuadTree::getAllEntitiesList(MyLinkedList<AbstractEntity> *l)
{
	for(int i = 0; i < 4; i++)
	{
		if(QT[i] != NULL) QT[i]->getAllEntitiesList(l);
	}

	list->readAllListsIteratorReset();
	AbstractEntity *e = NULL;
	while((e = list->getNextReadAllLists()) != NULL)
	{
		l->add(e, false);
	}
}


/*
Vector *QuadTree::getCenterPosition()
{
return centerPos;
}*/

bool QuadTree::add(AbstractEntity *e, bool deleteOnDestruction)
{
	if(canItFit(e))
	{
		bool isInserted = false;

		// QUAD 1
		if(QT[0] != NULL)
		{
			if(QT[0]->add(e, deleteOnDestruction)) isInserted = true;
		}
		else
		{
			if(level > 0 && !isInserted && wouldItFitInQuad(e, startX, startY + height/2, startX + width/2, endY))
			{
				QT[0] = new QuadTree(level-1, 0, startX, startY + height/2, startX + width/2, endY, this);
				if(QT[0]->add(e, deleteOnDestruction)) isInserted = true;
			}
		}

		// QUAD 2
		if(QT[1] != NULL)
		{
			if(QT[1]->add(e, deleteOnDestruction)) isInserted = true;
		}
		else
		{
			if(level > 0 && !isInserted && wouldItFitInQuad(e, startX + width/2, startY + height/2, endX, endY))
			{
				QT[1] = new QuadTree(level-1, 1, startX + width/2, startY + height/2, endX, endY, this);
				if(QT[1]->add(e, deleteOnDestruction)) isInserted = true;
			}
		}

		// QUAD 3
		if(QT[2] != NULL)
		{
			if(QT[2]->add(e, deleteOnDestruction)) isInserted = true;
		}
		else
		{
			if(level > 0 && !isInserted && wouldItFitInQuad(e, startX, startY, startX + width/2, startY + height/2))
			{
				QT[2] = new QuadTree(level-1, 2, startX, startY, startX + width/2, startY + height/2, this);
				if(QT[2]->add(e, deleteOnDestruction)) isInserted = true;
			}
		}

		// QUAD 4
		if(QT[3] != NULL)
		{
			if(QT[3]->add(e, deleteOnDestruction)) isInserted = true;
		}
		else
		{
			if(level > 0 && !isInserted && wouldItFitInQuad(e, startX + width/2, startY, endX, startY + height/2))
			{
				QT[3] = new QuadTree(level-1, 3, startX + width/2, startY, endX, startY + height/2, this);
				if(QT[3]->add(e, deleteOnDestruction)) isInserted = true;
			}
		}

		if(!isInserted)
		{
			e->_motherBranch = this;
			list->add(e, deleteOnDestruction); 
			//gServerConsole.addLine("OOO");
		}

		// perskaiciuojam spinduli ir atnaujinam jei reikia
		// centro taskas ir spindulys naudojamas patikrinimui ar quadas patenka i matymo zona
		float r = Vector(centerPos->x - e->getPosition()->x,
			centerPos->y - e->getPosition()->y,
			centerPos->z - e->getPosition()->z).Magnitude()+ e->getRadius();

		if(r > radius) radius = r;

		return true;
	}
	return false;
}



AbstractEntity *QuadTree::searchForColision(AbstractEntity *e)
{
	if(e != NULL && isColidingWithQuadBoundaries(e))
	{
		list->updateIteratorReset();
		AbstractEntity *ee = NULL;
		while((ee = list->getNextUpdate()) != NULL)
		{
			if(ee->checkForBaseColision(e))
			{
				return ee;
			}
		}


		AbstractEntity *tmpE = NULL;
		for(int i = 0; i < 4; i++)
		{
			if(QT[i] != NULL) 
			{
				tmpE = QT[i]->searchForColision(e);
				if(tmpE != NULL)
				{
					return tmpE;
				}
			}
		}
		return NULL;
	}
	return NULL;
}


void QuadTree::Update(float dt)
{
	for(int i = 0; i < 4; i++)
	{
		if(QT[i] != NULL) QT[i]->Update(dt);
	}

	list->updateIteratorReset();
	AbstractEntity *e = NULL;
	while((e = list->getNextUpdate()) != NULL)
	{
		e->Update(dt);
	}
}

void QuadTree::Render(InvisibleObjectsChecker *visibilityChecker)
{
	if(visibilityChecker->isVisible(centerPos, radius))
	{
		for(int i = 0; i < 4; i++)
		{
			if(QT[i] != NULL) QT[i]->Render(visibilityChecker);
		}

		list->renderIteratorReset();
		AbstractEntity *e = NULL;
		while((e = list->getNextRender()) != NULL)
		{
			if(visibilityChecker->isVisible(e->getPosition(), e->getRadius()))
				e->Render();
		}
	}
}