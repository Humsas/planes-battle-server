#include "quadTree.h"
#include "console.h"
extern Console cons;

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

	list				= new MyLinkedList<Entity>();

	QT[0] = QT[1] = QT[2] = QT[3] = NULL;
	if(level > 0)
	{
		QT[0]			= new QuadTree(level-1, 0, startX,				startY + height/2,	startX + width/2,	endY,				this);
		QT[1]			= new QuadTree(level-1, 1, startX + width/2,	startY + height/2,	endX,				endY,				this);
		QT[2]			= new QuadTree(level-1, 2, startX,				startY,				startX + width/2,	startY + height/2,	this);
		QT[3]			= new QuadTree(level-1, 3, startX + width/2,	startY,				endX,				startY + height/2,	this);
	}
}


QuadTree::~QuadTree()
{
	delete list;
	if(QT[0] != NULL) delete QT[0];
	if(QT[1] != NULL) delete QT[1];
	if(QT[2] != NULL) delete QT[2];
	if(QT[3] != NULL) delete QT[3];
}


bool QuadTree::canItFit(Entity *op)
	{
		bool r = false;

		if(	op->getPosition()->x - op->getRadius() > startX &&
			op->getPosition()->x + op->getRadius() < endX &&
			op->getPosition()->y - op->getRadius() > startY &&
			op->getPosition()->y + op->getRadius() < endY)
		{
			r = true;
		}

		return r;
	}


bool QuadTree::isColidingWithQuadBoundaries(Entity *op)
{
	bool r = false;

	if(	canItFit(op) ||
		op->getPosition()->x-op->getRadius() <= startX && startX	<= op->getPosition()->x+op->getRadius() ||
		op->getPosition()->x-op->getRadius() <= endX	&& endX		<= op->getPosition()->x+op->getRadius() ||
		op->getPosition()->y-op->getRadius() <= startY && startY	<= op->getPosition()->y+op->getRadius() ||
		op->getPosition()->y-op->getRadius() <= endY	&& endY		<= op->getPosition()->y+op->getRadius())
	{
		r = true;
	}

	return r;
}


void QuadTree::findColisions(Entity *op)
{
	//TODO: tikrinti kolizijas su savo grandinele
	list->iteratorReset();

	Entity *ts = NULL;

	//stringstream ss2;//create a stringstream
	//ss2 << "Tikrinama su: " << list->count();
			

			//cons.add(ss2.str());

	while((ts = list->getNext()) != NULL)
	{
		//cons.add("ttt");

		if(ts != op && (op->getRadius() > 0 && ts->getRadius() > 0) &&
			(((ts->getPosition()->x - op->getPosition()->x) * (ts->getPosition()->x - op->getPosition()->x) +
			(ts->getPosition()->y - op->getPosition()->y) * (ts->getPosition()->y - op->getPosition()->y) +
			(ts->getPosition()->z - op->getPosition()->z) * (ts->getPosition()->z - op->getPosition()->z)) 
			<= 	
			((ts->getRadius()*ts->getRadius()) + (op->getRadius()*op->getRadius())))
		)
		{
			// DO SOMETHING
			/*/////////////////////////////
			ts->colisionWith = op;
			ts->colision = true;

			op->colision = true;
			*////////////////////////////////

			//stringstream ss1;//create a stringstream
			//ss1 << "Colision sphere: " << op << " with " << ts;
			//

			//cons.add(ss1.str(),GAME_CONSOLE_WARNING);
		}
	}
	



	// tolimesnis persiuntimas gilyn
	if(level > 0)
	{
		for(int i = 0; i < 4; i++)
		{
			if(QT[i]->isColidingWithQuadBoundaries(op))
			{
				QT[i]->findColisions(op);
			}
		}
	}

}



void QuadTree::addElement(Entity *op, bool deleteOnDestruction = false)
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
}


bool QuadTree::removeElement(Entity *op)
{
	return ((QuadTree*)op->_motherBranch)->getList()->remove(op);
}


void QuadTree::paste(Entity *op)
{
	list->paste(((QuadTree*)op->_motherBranch)->getList()->cut(op));
	op->_motherBranch = this;
}

/*
void QuadTree::cutAndPaste(objectPosition *op)
{

}*/

void QuadTree::updateElement(Entity *op)
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



MyLinkedList<Entity> *QuadTree::getList()
{
	return list;
}