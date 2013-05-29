#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>

#include "AircraftB17Entity.h"
#include "Building.h"
#include "Game.h"

using namespace RakNet;

class Game;

class Player
{
private:
	AircraftB17*			mPlane;
	Game*					mGame;
	RakNetGUID				mPlayerId;
	vector<ProjectileBomb*> mBombList;
	vector<Building*>		mBuildingsList;
	vector<Turret*>			mTurretsList;


	void AddBuilding(Building* b);
	void AddTurret(Turret* b);


public:
	Player(Game* game, RakNetGUID playerID);
	~Player();

	void Update();
	void Render();

	void fire(AircraftB17* plane, RakNetGUID id);
	bool isReadyToPlay();

	AircraftB17*			GetPlane()						{return mPlane;}
	void					SetPlane(AircraftB17* plane)	{mPlane = plane;}
	RakNetGUID				GetOwnerId()					{return mPlayerId;}
	void					BuildBase(int count);
	bool					RemoveBuildingFromList(Building* b);
	void					SetTurretsTarget(AircraftB17* p);
};


#endif