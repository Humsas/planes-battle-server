#include "game.h"

#include "console.h"
extern Console gServerConsole;

Game::Game(LPDIRECT3DDEVICE9 &d3)
{
	this->d3 = d3;

	sound		=		new Sound();
	mNetworkIdManager = NetworkIDManager::GetInstance();
	mNetwork    =		new Networking(&gServerConsole, this);

	loadSounds();

	scena		= new SceneManager(d3);
	kolizijos	= new ColisionManager(scena->getTerrain());

	timer		= new Timer();
	
	//lektuvas = NULL;
	//lektuvas	= new AircraftB17(scena->getMeshManager(), Vector(20000, 7500, scena->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(20000, 4000, 7500))), Vector(0, 0, 0), true, scena->getChunkManager());


	//enemyBaseList	= new MyLinkedList<enemyBaseStruct>();
	//bombList		= new MyLinkedList<bombStruct>();
	//cannonsList	= new MyLinkedList<enemyCannonStruct>();

	fireOn = false;
	lastShotTime = timeGetTime();

	dropOn = false;
	lastDropTime = timeGetTime();

	//scena->getChunkManager()->addEntity(lektuvas);

	TestGameInit();
	sound->playSound("engine_loop", true);

	lastWarningTime = 0.0;
}

Game::~Game()
{
	if(sound != NULL)		delete sound;
	if(mNetwork != NULL)	delete mNetwork;
	if(scena != NULL)		delete scena;
	if(kolizijos != NULL)	delete kolizijos;

	for(int i = mLektuvai.size(); i > 0; i--)
		if(mLektuvai[i] != NULL)	
			delete mLektuvai[i];
							//delete enemyBaseList;
							//delete bombList;
							//delete cannonsList;
}

void Game::Render()
{
	scena->Render();
}

void Game::Update()
{
	// update all the game physics
	timer->Update();
	

	// testcraft
	//scena->getHud()->getSpeedometer()->SetSpeed(lektuvas->getSpeed());
	//scena->getHud()->getAltimeter()->SetAltitude(lektuvas->getPosition()->z);
	//scena->getHud()->getAnglemeter()->SetAngle(-lektuvas->getRotation()->z);

	scena->Update(timer->getDeltaT());

	sound->update();
}


//void Game::dropBomb()
//{
//	stringstream ss1;//create a stringstream
//	ss1 << "Nuke droped";
//	cons.add(ss1.str(),GAME_CONSOLE_WARNING);
//
//	bombStruct *bomba = new bombStruct;
//	bomba->killRadius = 250;
//
//
//	//oo->pozicija.x = lektuvas->Airplane.vPosition.y*0.3048;
//	//oo->pozicija.y = lektuvas->Airplane.vPosition.z*0.3048;
//	//oo->pozicija.z = -lektuvas->Airplane.vPosition.x*0.3048;
//
//	bomba->bomba = new ProjectileBomb(lektuvas->Airplane.vPosition.y*0.3048,
//		-lektuvas->Airplane.vPosition.x*0.3048,
//		lektuvas->Airplane.vPosition.z*0.3048,
//		lektuvas->Airplane.vVelocity.y*0.3048,
//		-lektuvas->Airplane.vVelocity.x*0.3048,
//		lektuvas->Airplane.vVelocity.z*0.3048);
//
//	lektuvas->Airplane.vVelocity.x;
//
//	/*bomba->bomba->bomba.position.x = -lektuvas->Airplane.vPosition.y*0.3048;
//	bomba->bomba->bomba.position.y = lektuvas->Airplane.vPosition.z*0.3048;
//	bomba->bomba->bomba.position.z = lektuvas->Airplane.vPosition.x*0.3048;*/
//	////lektuvas->Airplane.vPosition.
//
//			bomba->TTL = timer->getTime() + 120 * 1000;
//	
//			bomba->oo = scena->addObject("bomba"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);
//
//			bomba->oo->pozicija.x = bomba->bomba->bomba.position.y;
//			bomba->oo->pozicija.y = bomba->bomba->bomba.position.z;
//			bomba->oo->pozicija.z = bomba->bomba->bomba.position.x;
//	
//			bomba->oo->scale(5, 5, 5);
//			bomba->oo->rotateYPR(0, 0, 0);
//
//			
//			bomba->op = new objectPosition(Vector(bomba->oo->pozicija.x, bomba->oo->pozicija.z, bomba->oo->pozicija.y), -1000/*bomba->oo->mesh->radius*(0)*/);
//			kolizijos->getQuadTree()->addElement(bomba->op, true);
//
//
//			bombList->add(bomba, true);
//	
//}
//
//
//void Game::shotBomb()
//{
//	stringstream ss1;//create a stringstream
//	ss1 << "Nuke shoted";
//	cons.add(ss1.str(),GAME_CONSOLE_WARNING);
//
//	bombStruct *bomba = new bombStruct;
//	bomba->killRadius = 50;
//
//
//	//oo->pozicija.x = lektuvas->Airplane.vPosition.y*0.3048;
//	//oo->pozicija.y = lektuvas->Airplane.vPosition.z*0.3048;
//	//oo->pozicija.z = -lektuvas->Airplane.vPosition.x*0.3048;
//
//	float projectileSpeed = 500; // m/s
//
//	bomba->bomba = new ProjectileBomb(lektuvas->Airplane.vPosition.y*0.3048,
//		-lektuvas->Airplane.vPosition.x*0.3048,
//		lektuvas->Airplane.vPosition.z*0.3048,
//		lektuvas->Airplane.vVelocity.y*0.3048 + projectileSpeed * (lektuvas->Airplane.vVelocity.y / lektuvas->Airplane.vVelocity.Magnitude()),
//		-lektuvas->Airplane.vVelocity.x*0.3048 - projectileSpeed * (lektuvas->Airplane.vVelocity.x / lektuvas->Airplane.vVelocity.Magnitude()),
//		lektuvas->Airplane.vVelocity.z*0.3048 + projectileSpeed * (lektuvas->Airplane.vVelocity.z / lektuvas->Airplane.vVelocity.Magnitude()));
//
//	lektuvas->Airplane.vVelocity.x;
//
//	/*bomba->bomba->bomba.position.x = -lektuvas->Airplane.vPosition.y*0.3048;
//	bomba->bomba->bomba.position.y = lektuvas->Airplane.vPosition.z*0.3048;
//	bomba->bomba->bomba.position.z = lektuvas->Airplane.vPosition.x*0.3048;*/
//	////lektuvas->Airplane.vPosition.
//
//			bomba->TTL = timer->getTime() + 10 * 1000;
//	
//			bomba->oo = scena->addObject("bomba"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);
//
//			bomba->oo->pozicija.x = bomba->bomba->bomba.position.y;
//			bomba->oo->pozicija.y = bomba->bomba->bomba.position.z;
//			bomba->oo->pozicija.z = bomba->bomba->bomba.position.x;
//	
//			bomba->oo->scale(5, 5, 5);
//			bomba->oo->rotateYPR(0, 0, 0);
//
//			
//			bomba->op = new objectPosition(Vector(bomba->oo->pozicija.x, bomba->oo->pozicija.z, bomba->oo->pozicija.y), -1000/*bomba->oo->mesh->radius*(0)*/);
//			kolizijos->getQuadTree()->addElement(bomba->op, true);
//
//
//			bombList->add(bomba, true);
//	
//}
//
//void Game::cannonShot(enemyCannonStruct *ec)
//{
//	stringstream ss1;//create a stringstream
//	ss1 << "Cannon SHOT";
//	cons.add(ss1.str(),GAME_CONSOLE_WARNING);
//
//	bombStruct *bomba = new bombStruct;
//	bomba->killRadius = 50;
//
//
//	//oo->pozicija.x = lektuvas->Airplane.vPosition.y*0.3048;
//	//oo->pozicija.y = lektuvas->Airplane.vPosition.z*0.3048;
//	//oo->pozicija.z = -lektuvas->Airplane.vPosition.x*0.3048;
//
//	float projectileSpeed = 800; // m/s
//
//	// paziurim atstuma kokiu lektuvas yra nuo patrankos
//	// tada paskaiciuojam kiek laiko skris sovinys toki atstuma
//	// paziurim kur bus lektuvas tuo metu
//	// saunam i paskaiciuota taska 
//
//	Vector targetVector = (lektuvas->op->pozicija - ec->op->pozicija);
//	float distance = targetVector.Magnitude(); // atstumas iki taikinio
//	float travelTime = distance / projectileSpeed;	// laikas, kuri keliaus sovinys
//
//
//
//	Vector velocity = Vector(lektuvas->Airplane.vVelocity.y,
//		-lektuvas->Airplane.vVelocity.x,
//		lektuvas->Airplane.vVelocity.z);
//
//	Vector imaginaryTarget = targetVector + (velocity*0.3048)*travelTime;
//
//	// prakritimas begant laikui
//	Vector tmpV = imaginaryTarget;
//
//	tmpV.Normalize();
//	tmpV *= projectileSpeed;
//
//	float zPos = tmpV.z * travelTime - 0.5 * gravity * travelTime * travelTime;
//
//	imaginaryTarget.z = imaginaryTarget.z + (imaginaryTarget.z - zPos)/2;
//
//	travelTime = imaginaryTarget.Magnitude() / projectileSpeed;
//	
//	imaginaryTarget.Normalize();
//
//	Vector cannonBarrel = ec->op->pozicija + (imaginaryTarget * (ec->op->radius+5));
//
//	imaginaryTarget *= projectileSpeed;
//
//
//
//
//				float x = imaginaryTarget.x;
//				float y = imaginaryTarget.y;
//				// ilgis vienetinis
//				float len = sqrt(x*x + y*y);
//
//				float angle = acos(abs(x) / len) * (180/3.141592654);
//
//				// kampo taisymas
//				if(x >= 0 && y >= 0)
//				{ // I
//				}
//				else if(x < 0 && y >= 0)
//				{ // II
//					angle = 180 - angle;
//				}
//				else if(x < 0 && y < 0)
//				{ // III
//					angle = 180 + angle;
//				}
//				else if(x >= 0 && y < 0)
//				{ // IV
//					angle = 360 - angle;
//				}
//
//
//				//Vector positionVec = op->pozicija - lektuvas->op->pozicija;
//				float pitch = asin(imaginaryTarget.z/imaginaryTarget.Magnitude())* (180/3.141592654);
//
//				ec->oo->rotateYPR(-angle+90,0,0);
//				ec->ooBarrel->rotateYPR(-angle+90,-pitch,0);
//
//
//
//
//	bomba->bomba = new ProjectileBomb(cannonBarrel.x,
//		cannonBarrel.y,
//		cannonBarrel.z,
//		imaginaryTarget.x,
//		imaginaryTarget.y,
//		imaginaryTarget.z);
//
//
//	/*bomba->bomba = new ProjectileBomb(lektuvas->Airplane.vPosition.y*0.3048,
//		-lektuvas->Airplane.vPosition.x*0.3048,
//		lektuvas->Airplane.vPosition.z*0.3048,
//		lektuvas->Airplane.vVelocity.y*0.3048 + projectileSpeed * (lektuvas->Airplane.vVelocity.y / lektuvas->Airplane.vVelocity.Magnitude()),
//		-lektuvas->Airplane.vVelocity.x*0.3048 - projectileSpeed * (lektuvas->Airplane.vVelocity.x / lektuvas->Airplane.vVelocity.Magnitude()),
//		lektuvas->Airplane.vVelocity.z*0.3048 + projectileSpeed * (lektuvas->Airplane.vVelocity.z / lektuvas->Airplane.vVelocity.Magnitude()));
//*/
//	//lektuvas->Airplane.vVelocity.x;
//
//	/*bomba->bomba->bomba.position.x = -lektuvas->Airplane.vPosition.y*0.3048;
//	bomba->bomba->bomba.position.y = lektuvas->Airplane.vPosition.z*0.3048;
//	bomba->bomba->bomba.position.z = lektuvas->Airplane.vPosition.x*0.3048;*/
//	////lektuvas->Airplane.vPosition.
//
//
//			bomba->TTL = timer->getTime() + travelTime*1000;
//	
//			bomba->oo = scena->addObject("bomba"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);
//
//			bomba->oo->pozicija.x = bomba->bomba->bomba.position.y;
//			bomba->oo->pozicija.y = bomba->bomba->bomba.position.z;
//			bomba->oo->pozicija.z = bomba->bomba->bomba.position.x;
//	
//			bomba->oo->scale(5, 5, 5);
//			bomba->oo->rotateYPR(0, 0, 0);
//
//			
//			bomba->op = new objectPosition(Vector(bomba->oo->pozicija.x, bomba->oo->pozicija.z, bomba->oo->pozicija.y), -1000/*bomba->oo->mesh->radius*(0)*/);
//			kolizijos->getQuadTree()->addElement(bomba->op, true);
//
//
//			bombList->add(bomba, true);
//			sound->playSound("turret_shot");
//}


SceneManager *Game::getScene()
{
	return scena;
}

ColisionManager *Game::getColisionManager()
{
	return kolizijos;
}

Networking* Game::getNetwork()
{
	return mNetwork;
}

NetworkIDManager* Game::getNetworkIDManager()
{
	return mNetworkIdManager;
}

//vector<RakNet::RakNetGUID> Game::getPlayersId()
//{
//	return mPlayersList;
//}


void Game::playerConnected(RakNet::RakNetGUID playerID)
{
	//Start data sending here
	AircraftB17* lektuvas = new AircraftB17(scena->getMeshManager(), Vector(20000, 7500, scena->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(20000, 4000, 7500))), Vector(0, 0, 0), true, scena->getChunkManager());
	lektuvas->SetNetworkIDManager(mNetworkIdManager);
	lektuvas->setOwnerId(playerID);
	lektuvas->CreateSerialize(mNetwork->GetServer());
	lektuvas->setReadyToPlay(false);

	scena->getChunkManager()->addEntity(lektuvas);
	mLektuvai.push_back(lektuvas);
}


void Game::CreateCubes()
{
	vector<NetworkID> ids;

	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 100; j++)
		{
			// Gal galima i6 karto sukurti su paturbintu konstruktorium
			TestCubeEntity *testEntity = new TestCubeEntity(scena->getMeshManager(), Vector(20000+i*150, 7500+j*150, scena->getChunkManager()->getMapHeightAtPoint(Vector(20000+i*150, 7500+j*150, 0))), Vector(0, 0, 0), true);
			testEntity->SetNetworkIDManager(mNetworkIdManager);
			testEntity->CreateSerialize(mNetwork->GetServer());

			scena->getChunkManager()->addEntity(testEntity);
		}
	}
}

void Game::MoveCubes()
{
	MyLinkedList<AbstractEntity> *pEnt = getScene()->getChunkManager()->getDynamicEntityList();
	pEnt->networkReadIteratorReset();
	AbstractEntity *es = NULL;
	while((es = pEnt->getNextNetwork()) != NULL)
	{
		switch (es->getType())
		{
		case GAME_ENTITY_CUBE:
			((TestCubeEntity*)es)->Move(100);
			break;
		default:
			break;
		}
	}
}

// Processes messages from the clients
//TODO: reikia atskyrimo kam skirtos zinutes(kuris klientas)
void Game::ProcessKeyMessages(BitStream* stream, RakNetGUID playerID)
{
	AircraftB17* plane;
	for (int i = 0; i < mLektuvai.size(); i++)
		if(mLektuvai[i]->getOwnerId() == playerID)
		{
			plane = mLektuvai[i];
			break;
		}

	USHORT keyCode;
	bool isUp;
	stream->Read(keyCode);

	switch (keyCode)
	{
	case VK_SHIFT:
		break;
	case 'Z':
		stream->Read(isUp);
		(isUp)? plane->startEngine(false) : plane->startEngine(true);

		break;
	case 'X':
		stream->Read(isUp);
		(isUp)? "" : plane->stopEngine();

		break;
	case 'W':
		stream->Read(isUp);
		(isUp)? plane->setElevator(0) : plane->setElevator(1.0);

		break;
	case 'S':
		stream->Read(isUp);
		(isUp)? plane->setElevator(0) : plane->setElevator(-1.0);

		break;
	case 'A':
		stream->Read(isUp);
		(isUp)? plane->setElerons(0) : plane->setElerons(1.0);

		break;
	case 'D':
		stream->Read(isUp);
		(isUp)? plane->setElerons(0) : plane->setElerons(-1.0);

		break;
	case 'Q':
		stream->Read(isUp);
		(isUp)? plane->setRuder(0) : plane->setRuder(-1.0);

		break;
	case 'E':
		stream->Read(isUp);
		(isUp)? plane->setRuder(0) : plane->setRuder(1.0);

		break;
	case 'B':
		stream->Read(isUp);
		(isUp)? plane->setBrakes(false) : plane->setBrakes(true);

		break;
	case 'F':
		stream->Read(isUp);
		(isUp)? plane->setFlaps(0.0) : plane->setFlaps(1.0);

		break;
	case 'G':
		stream->Read(isUp);
		(isUp)? "" : plane->gearUpDown();

		break;
	case VK_UP:
		stream->Read(isUp);
		(isUp)? "" : plane->increaseThrottle();

		break;
	case VK_DOWN:
		stream->Read(isUp);
		(isUp)? "" : plane->decreaseThrottle();

		break;
	case GAME_KEY_QUIT_GAME:
		gServerConsole.addLine("Klienas quitina");
		PostQuitMessage(1);
		break;
	case MM_JOY1MOVE:
		{
			float elerons, elevator;
			stream->Read(elerons);
			stream->Read(elevator);

			break;
		}
	case MM_JOY1ZMOVE:
		{
			float throttle;
			stream->Read(throttle);

			break;
		}
	case GAME_KEY_JOY_BUTTON1:
		{
			stream->Read(isUp);
			if(!isUp)
				plane->decreaseThrottle();
			break;
		}
	case GAME_KEY_JOY_BUTTON2:
		{
			stream->Read(isUp);
			if(!isUp)
				plane->increaseMixture();
			break;
		}
	case GAME_KEY_JOY_BUTTON3:
		{
			stream->Read(isUp);
			if(!isUp)
				plane->decreaseMixture();
			break;
		}
	case GAME_KEY_JOY_BUTTON4:
		{
			stream->Read(isUp);
			if(!isUp)
				plane->increaseThrottle();
			break;
		}
	case GAME_KEY_JOY_BUTTON5:
		{
			stream->Read(isUp);
			(isUp)? plane->setRuder(0.0) : plane->setRuder(-1.0);

			break;
		}
	case GAME_KEY_JOY_BUTTON6:
		break;
	case GAME_KEY_JOY_BUTTON7:
		{
			stream->Read(isUp);
			(isUp)? plane->setRuder(0.0) : plane->setRuder(1.0);

			break;
		}
	case GAME_KEY_JOY_BUTTON8:
		break;
	case GAME_KEY_JOY_BUTTON9:
		{
			stream->Read(isUp);
			(isUp)? plane->startEngine(false) : plane->startEngine(true);

			break;
		}
	case GAME_KEY_JOY_BUTTON10:
		{
			stream->Read(isUp);
			if(!isUp)
				plane->stopEngine();
			break;
		}
	case GAME_KEY_MOUSE_MOVE:
		{
			LONG x, y;
			stream->Read(x);
			stream->Read(y);
			//getScene()->getCamera()->updateAngle(x, y);

			break;
		}
	case GAME_KEY_MOUSE_WHEEL:
		break;
	case GAME_KEY_MOUSE_LEFT_BUTTON_DOWN:
		fire(true);
		break;
	case GAME_KEY_MOUSE_LEFT_BUTTON_UP:
		fire(false);
		break;
	case GAME_KEY_MOUSE_RIGHT_BUTTON_DOWN:
		drop(true);
		break;
	case GAME_KEY_MOUSE_RIGHT_BUTTON_UP:
		drop(false);
		break;
	default:
		gServerConsole.addLine("Atkeliavo Neatpazintas mygtuko signalas.");
		break;
	}
}


void Game::TestGameInit()
{
	// add some tree

	int iS = 1;
	int iR = 1;
	
	string medziai[7] = {"t1", "t1", "t3", "t4", "t5", "t6", "t7"};

	objektas *oo = NULL;

	objectPosition *op = NULL;

	//for(int i = 0; i < 100; i++)
	//{
	//
	//	for(int j = 0; j < 100; j++)
	//	{
	//		iS = rand() % 10 + 1;
	//		iR = rand() % 360 + 1;

	//		iS = 20;
	//		//iSX = rand() % 10 + 1;
	//
	//		oo = scena->addObject("eglute"/*medziai[rand() % 7]*//*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);
	//		
	//		oo->pozicija.x = i*20 + 20000;
	//		oo->pozicija.y = scena->getTerrain()->getH(D3DXVECTOR3(i*20+20000,1,j*20+7500))+0;
	//		oo->pozicija.z = j*20 + 7500;
	//
	//		oo->scale(1.0+iS/1.0, 1.0+iS/1.0, 1.0+iS/1.0);
	//		oo->rotateYPR(iR, 0, 0);

	//		
	//		op = new objectPosition(Vector(oo->pozicija.x, oo->pozicija.z, oo->pozicija.y), oo->mesh->radius*(1.0+iS/1.0));
	//		kolizijos->getQuadTree()->addElement(op, true);
	//
	//	}
	//
	//}


	//// inicijuojamos priesu bazes
	//enemyBaseStruct *enemyBase = NULL;

	//for(int i = 20; i < 40; i++)
	//{
	//
	//	for(int j = 20; j < 40; j++)
	//	{
	//		enemyBase = new enemyBaseStruct;

	//		iS = rand() % 10 + 1;
	//		iR = rand() % 360 + 1;
	//		//iSX = rand() % 10 + 1;
	//
	//		enemyBase->oo = scena->addObject("kubas"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);

	//		enemyBase->oo->pozicija.x = i*500;
	//		enemyBase->oo->pozicija.y = scena->getTerrain()->getH(D3DXVECTOR3(i*500,1,j*500))+0;
	//		enemyBase->oo->pozicija.z = j*500;
	//
	//		enemyBase->oo->scale(50.0, 50.0, 50.0);
	//		enemyBase->oo->rotateYPR(iR, 0, 0);

	//		
	//		enemyBase->op = new objectPosition(Vector(enemyBase->oo->pozicija.x, enemyBase->oo->pozicija.z, enemyBase->oo->pozicija.y), enemyBase->oo->mesh->radius*(50));
	//		kolizijos->getQuadTree()->addElement(enemyBase->op, true);


	//		enemyBaseList->add(enemyBase, true);
	//
	//	}
	//
	//}


	//for(int i = 0; i < 5; i++)
	//{
	//// inicijuojamos priesu patrankos
	//enemyCannonStruct *enemyCannon = NULL;

	///*for(int i = 20; i < 40; i++)
	//{
	//
	//	for(int j = 20; j < 40; j++)
	//	{*/
	//		enemyCannon = new enemyCannonStruct;

	//		//iS = rand() % 10 + 1;
	//		//iR = rand() % 360 + 1;
	//		//iSX = rand() % 10 + 1;
	//
	//		enemyCannon->oo = scena->addObject("cannon"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);

	//		enemyCannon->oo->pozicija.x = 8940 + i * 250;
	//		enemyCannon->oo->pozicija.z = 7800;
	//		enemyCannon->oo->pozicija.y = scena->getTerrain()->getH(D3DXVECTOR3(enemyCannon->oo->pozicija.x,1,enemyCannon->oo->pozicija.z))+30;
	//		
	//
	//		enemyCannon->oo->scale(50.0, 50.0, 50.0);
	//		enemyCannon->oo->rotateYPR(0, 0, 0);

	//		
	//		enemyCannon->op = new objectPosition(Vector(enemyCannon->oo->pozicija.x, enemyCannon->oo->pozicija.z, enemyCannon->oo->pozicija.y), enemyCannon->oo->mesh->radius*(50));
	//		kolizijos->getQuadTree()->addElement(enemyCannon->op, true);

	//		// vamzdis
	//		enemyCannon->ooBarrel = scena->addObject("cannonBarrel"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);

	//		
	//		enemyCannon->ooBarrel->pozicija = enemyCannon->oo->pozicija;
	//		
	//
	//		enemyCannon->ooBarrel->scale(70.0, 70.0, 70.0);
	//		enemyCannon->ooBarrel->rotateYPR(0, 0, 0);


	//		cannonsList->add(enemyCannon, true);
	//}
	//	}
	//
	//}
	

	//oo = scena->addObject("sfera"/*, i*500, Ter.getH(D3DXVECTOR3(i*500,1,j*500))+0 ,j*500, iSecret*/);

	//oo->pozicija.x = 8850;
	//oo->pozicija.z = 8000;
	//oo->pozicija.y = scena->getTerrain()->getH(D3DXVECTOR3(oo->pozicija.x,1,oo->pozicija.z))+0;
	//
	//
	//oo->scale(50.0, 50.0, 50.0);
	//oo->rotateYPR(1, 0, 0);



}

void Game::fire(bool set)
{
	this->fireOn = set;
}

void Game::drop(bool set)
{
	this->dropOn = set;
}


void Game::loadSounds()
{
	sound->loadSound("audio\\mp3\\BTS.mp3","fonas");
	sound->loadSound("audio\\sounds\\firework_explosion.mp3","explosion");
	sound->loadSound("audio\\sounds\\cannon_shot.mp3","turret_shot");
	sound->loadSound("audio\\sounds\\engine_loop.mp3","engine_loop");
	sound->loadSound("audio\\sounds\\AircraftAlarm.mp3", "stall_warning");
}