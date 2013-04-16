#ifndef _GAME
#define _GAME

#include "WinsockWrapper.h"
#include <Mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Sound.h"
#include "sceneManager.h"
#include "colisionManager.h"
#include "aircraft.h"
#include "Networking.h"
#include "RakPeerInterface.h"

#include "JSBSIMAircraft.h"

#include "bomb.h"

#include "timer.h"

using namespace RakNet;

class Networking;


struct enemyBaseStruct
{
	objektas *oo;			// grafinis objektas
	objectPosition *op;		// fizinis objektas

	enemyBaseStruct()
	{
		oo = NULL;
		op = NULL;
	}
};


struct enemyCannonStruct
{
	objektas *oo;
	objektas *ooBarrel;
	objectPosition *op;

	DWORD lastShotTime;
	
	enemyCannonStruct()
	{
		oo = NULL;
		op = NULL;
		ooBarrel = NULL;
		lastShotTime = timeGetTime();
	}
	~enemyCannonStruct()
	{

	}
};


struct bombStruct
{
	objektas *oo;			// grafinis objektas
	objectPosition *op;		// fizinis objektas
	ProjectileBomb *bomba;	// bomba

	DWORD TTL;
	DWORD hitTime;
	float killRadius;

	bombStruct()
	{
		oo = NULL;
		op = NULL;
		bomba = NULL;
		hitTime = 0;
		killRadius = 10;
		TTL = timeGetTime() + 1000 * 5;
	}
	~bombStruct()
	{
		if(bomba != NULL) delete bomba;
	}
};

class Game
{
private:
	LPDIRECT3DDEVICE9 d3;
	Sound			*sound;
	SceneManager	*scena;
	ColisionManager *kolizijos;
	Networking		*mNetwork;

	// ReplicaManager3 requires NetworkIDManager to lookup pointers from numbers.
	RakNet::NetworkIDManager*	mNetworkIdManager;
	// The system that performs most of our functionality for this demo
	RakNet::ReplicaManager3*	mReplicaManager;

	Timer			*timer;


	DWORD		OldTime, NewTime;
	float		dt;

	//objektas *oo;
	objektas *ooo;

	//MyLinkedList<enemyBaseStruct>	*enemyBaseList;
	//MyLinkedList<bombStruct>		*bombList;
	//MyLinkedList<enemyCannonStruct> *cannonsList;

	bool		fireOn;
	DWORD		lastShotTime;

	bool		dropOn;
	DWORD		lastDropTime;

	float lastWarningTime;


	

public:
	//AircraftPhysics* lektuvas;

	TestCraft* testCraft;
	//vector<RakNet::RakNetGUID>		mPlayersList;

	Game(LPDIRECT3DDEVICE9 &d3);
	~Game();

	SceneManager*		getScene();
	ColisionManager*	getColisionManager();
	Networking*			getNetwork();
	ReplicaManager3*	getReplicaManager();
	NetworkIDManager*	getNetworkIDManager();



	void loadSounds();

	// Receives message about connected player
	void playerConnected(RakNet::RakNetGUID playerID);

	//void dropBomb();
	//void shotBomb();
	//void cannonShot(enemyCannonStruct *op);

	void TestGameInit();

	void fire(bool set);
	void drop(bool set);

	void Update();
	void Render();
	void CreateCubes();

};


#endif