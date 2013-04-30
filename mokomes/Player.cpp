#include "Player.h"


Player::Player(Game* game, RakNetGUID playerID) 
{ 
	mGame = game;
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