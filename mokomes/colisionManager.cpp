#include "colisionManager.h"
#include "console.h"
extern Console gServerConsole;

ColisionManager::ColisionManager(Terrain *ter)
{
	QT = new QuadTree(8, 0, 0, 0, ter->getWidth(), ter->getHeight());
	this->ter = ter;
}

ColisionManager::~ColisionManager()
{
	if(QT != NULL) delete QT;
}

QuadTree *ColisionManager::getQuadTree()
{
	return QT;
}

void ColisionManager::findColisions(AbstractEntity *op)
{
	//QT->findColisions(op);

	stringstream sso1,sso2,sso3;//create a stringstream
		sso1 << "X: " << op->getPosition()->x;
		sso2 << "Y: " << op->getPosition()->y;
		sso3 << "Z: " << op->getPosition()->z;
			

		/*gServerConsole.addLine(sso1.str(), GAME_CONSOLE_ERROR);
		gServerConsole.addLine(sso2.str(), GAME_CONSOLE_ERROR);
		gServerConsole.addLine(sso3.str(), GAME_CONSOLE_ERROR);*/

	if(op->getPosition()->z - op->getRadius() <= ter->getH(D3DXVECTOR3(op->getPosition()->x,op->getPosition()->z,op->getPosition()->y)))
	{

		// DO SOMETHING
		stringstream ss1;//create a stringstream
		ss1 << "Colision sphere: Ground";
			

		gServerConsole.addLine(ss1.str(), GAME_CONSOLE_ERROR);

		///////////////////////
		//op->colision = true;
	}

}

float ColisionManager::groundLevel(AbstractEntity *op)
{
	return op->getPosition()->z - ter->getH(D3DXVECTOR3(op->getPosition()->x,op->getPosition()->z,op->getPosition()->y));
}

void ColisionManager::update(AbstractEntity *op)
{

}