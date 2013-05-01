#include "Player.h"


Player::Player(Game* game, RakNetGUID playerID) 
{ 
	mGame = game;
	mPlayerId = playerID;
}

Player::~Player()
{
	if(mPlane != NULL)	
		delete mPlane;
}

void Player::Render()
{
}

void Player::Update()
{

	//Shooting
	if(mPlane->isReadyToShootProjectileBomb())
		fire(mPlane, mPlane->GetOwnerId());

}

void Player::fire(AircraftB17* plane, RakNetGUID id)
{
	ProjectileBomb *bomb = new ProjectileBomb(mGame->getScene()->getMeshManager(), *plane->getPosition(), *plane->getRotation(), 300+plane->getSpeed(), true);
	bomb->SetNetworkIDManager(mGame->getNetworkIDManager());
	bomb->SetOwnerId(id);
	bomb->CreateSerialize(mGame->getNetwork()->GetServer());

	mGame->getScene()->getChunkManager()->addEntity(bomb);
}

bool Player::isReadyToPlay()
{
	return mPlane->isReadyToPlay();
}


void Player::BuildBase()
{
	int maxBuildingHealth = 100;

	Building* building = new Building(mGame->getScene()->getMeshManager(), "fish_house", Vector(30000, 30000, 
		mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30000, 4000, 30000))), Vector(0, 0, 0), maxBuildingHealth, true);

	building->SetNetworkIDManager(mGame->getNetworkIDManager());
	building->SetOwnerId(mPlayerId);
	building->CreateSerialize(mGame->getNetwork()->GetServer());

	mGame->getScene()->getChunkManager()->addEntity(building);
}