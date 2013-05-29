#include "Player.h"

#include "console.h"
extern Console gServerConsole;


Player::Player(Game* game, RakNetGUID playerID) 
{ 
	mGame = game;
	mPlayerId = playerID;
}

Player::~Player()
{
}

void Player::Render()
{
}

void Player::Update()
{
	vector<int> bombsToRemove;

	//Shooting
	if(mPlane->isReadyToShootProjectileBomb())
		fire(mPlane, mPlane->GetOwnerId());

	/*for(int i = 0; i < mTurretsList.size(); i++)
	{
		if(mTurretsList[i]->getShotTimer() >= 2)
		{
			ProjectileBomb *bomb = new ProjectileBomb(mGame->getScene()->getMeshManager(), *mTurretsList[i]->getPosition(), mTurretsList[i]->getShotDirrection(), 300, true);
			bomb->SetNetworkIDManager(mGame->getNetworkIDManager());
			bomb->SetOwnerId(mPlayerId);
			bomb->CreateSerialize(mGame->getNetwork()->GetServer());

			mGame->getScene()->getChunkManager()->addEntity(bomb);
			mBombList.push_back(bomb);

			mTurretsList[i]->resetShotTimer();
		}
	}*/

	for (int i = 0; i < mBombList.size(); i++)
	{
		//Kolizija su zeme
		if(mGame->getScene()->getChunkManager()->checkGroundColision(mBombList[i]))
		{
			bombsToRemove.push_back(i);
			mGame->getScene()->getChunkManager()->safeRemove(mBombList[i]);
			mGame->getNetwork()->DeleteObjectSend(mBombList[i]->GetNetworkID(), mBombList[i]->getType());

			continue;
		}

		AbstractEntity * obj = mGame->getScene()->getChunkManager()->searchForColision(mBombList[i]);

		//Collision Detected
		if(obj != NULL)
		{
			switch (obj->getType())
			{
			case GAME_ENTITY_AIRCRAFT_B17:
				{
					//Same owner, do not damage
					if(((AircraftB17*)obj)->GetOwnerId() == mPlayerId)
						continue;
					bombsToRemove.push_back(i);

					gServerConsole.addLine("Kolizija");
					if(((AircraftB17*)obj)->TakeDamage(mBombList[i]->GetDamage()))
					{
						//Sunaikintas lektuvas
						((AircraftB17*)obj)->reset();
					}
					break;
				}
			case GAME_ENTITY_PROJECTILE_BOMB:
				{
					//Same owner, do not damage
					if(((ProjectileBomb*)obj)->GetOwnerId() == mPlayerId)
						continue;
					bombsToRemove.push_back(i);

					//Abi susiprastina
					mGame->getScene()->getChunkManager()->safeRemove(obj);

					// Siunciam istrynimo zinute klientams
					mGame->getNetwork()->DeleteObjectSend(((ProjectileBomb*)obj)->GetNetworkID(), obj->getType());

					break;
				}
			case GAME_ENTITY_BUILDING:
				//Same owner, do not damage
				if(((Building*)obj)->GetOwnerId() == mPlayerId)
					continue;
				bombsToRemove.push_back(i);

				if(((Building*)obj)->TakeDamage(mBombList[i]->GetDamage()))
				{
					for(int i = 0; i < mGame->mPlayers.size(); i++)
					{
						if(mGame->mPlayers[i]->GetOwnerId() != mPlayerId)
						{
							// Jei true, tai sitas zaidejas(viso objekto savininkas) laimejo
							if(mGame->mPlayers[i]->RemoveBuildingFromList((Building*)obj))
							{
								gServerConsole.addLine("Game ended. Please, restart, the game.", GAME_CONSOLE_WARNING);

								mGame->getNetwork()->SendWinMessage(mPlayerId, mGame->mPlayers[i]->GetOwnerId());

								//stop game here
								mGame->getTimer()->pause();
							}
						}
					}

					//Sunaikintas pastatas
					mGame->getScene()->getChunkManager()->safeRemove(obj);
					mGame->getNetwork()->DeleteObjectSend(((Building*)obj)->GetNetworkID(), obj->getType());
				}
				break;
			default:
				break;
			}

			mGame->getScene()->getChunkManager()->safeRemove(mBombList[i]);
			mGame->getNetwork()->DeleteObjectSend(mBombList[i]->GetNetworkID(), mBombList[i]->getType());
		}
	}

	

	if(bombsToRemove.size() > 0)
	{
		for (int i = bombsToRemove.size()-1; i >= 0; i--)
		{
			mBombList.erase(mBombList.begin()+bombsToRemove[i]);
		}
	}
}

void Player::fire(AircraftB17* plane, RakNetGUID id)
{
	//Vector bombPosition(*plane->getPosition());
	ProjectileBomb *bomb = new ProjectileBomb(mGame->getScene()->getMeshManager(), *plane->getPosition(), *plane->getRotation(), 300+plane->getSpeed(), true);
	bomb->SetNetworkIDManager(mGame->getNetworkIDManager());
	bomb->SetOwnerId(id);
	bomb->CreateSerialize(mGame->getNetwork()->GetServer());

	mGame->getScene()->getChunkManager()->addEntity(bomb);
	mBombList.push_back(bomb);
}

bool Player::isReadyToPlay()
{
	return mPlane->isReadyToPlay();
}

//Returns true if no buildings are left in the list
bool Player::RemoveBuildingFromList(Building* b)
{
	for(int i = 0; i < mBuildingsList.size(); i++)
		if(mBuildingsList[i] == b)
		{
			mBuildingsList.erase(mBuildingsList.begin()+i);
			break;
		}

	return mBuildingsList.size() == 0;
}

void Player::BuildBase(int count)
{
	if(count == 1)
	{
		Turret *t1 = new Turret(mGame->getScene()->getMeshManager(), Vector(30227.9, 25874, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30227.9, 1315, 25874))+20), Vector(0,0,0), true);
		Turret *t2 = new Turret(mGame->getScene()->getMeshManager(), Vector(30827.9, 25896, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30827.9, 1315, 25896))+20), Vector(0,0,0), true);
		Turret *t3 = new Turret(mGame->getScene()->getMeshManager(), Vector(29167.6, 26346.1, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(29167.6, 1315, 26346.1))+20), Vector(0,0,0), true);
		Turret *t4 = new Turret(mGame->getScene()->getMeshManager(), Vector(30258.9, 26531.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30258.9, 1315, 26531.4))+20), Vector(0,0,0), true);
		Turret *t5 = new Turret(mGame->getScene()->getMeshManager(), Vector(30790.9, 26535.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30790.9, 1315, 26535.4))+20), Vector(0,0,0), true);

		AddTurret(t1);
		AddTurret(t2);
		AddTurret(t3);
		AddTurret(t4);
		AddTurret(t5);

		Building* building = new Building(mGame->getScene()->getMeshManager(), "pilis", Vector(29883.3, 26343.8, 1175.33), Vector(-86, 0, 0), 358, MAX_HEALTH_BUILDING, true);
		Building* building1 = new Building(mGame->getScene()->getMeshManager(), "fish_house", Vector(30435.3, 26237.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30435.3, 1315, 26237.8))), Vector(-32, 0, 0), 67, MAX_HEALTH_BUILDING, true);
		Building* building2 = new Building(mGame->getScene()->getMeshManager(), "gas_station", Vector(30674.4, 26388.3, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30674.4, 1315, 26388.3))+1), Vector(92, 2, 0), 74, MAX_HEALTH_BUILDING, true);
		Building* building3 = new Building(mGame->getScene()->getMeshManager(), "lusna", Vector(30590.1, 26351.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30590.1, 1315, 26351.4))), Vector(394, 0, 0), 88, MAX_HEALTH_BUILDING, true);
		Building* building4 = new Building(mGame->getScene()->getMeshManager(), "lusna2", Vector(30595.3, 26265.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30595.3, 1315, 26265.4))), Vector(-116, 0, 0), 83, MAX_HEALTH_BUILDING, true);
		Building* building5 = new Building(mGame->getScene()->getMeshManager(), "mansion", Vector(30558.5, 26251.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30558.5, 1315, 26251.8))), Vector(340, 0, 0), 48, MAX_HEALTH_BUILDING, true);
		Building* building6 = new Building(mGame->getScene()->getMeshManager(), "namas01", Vector(30471.6, 26193.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30471.6, 1315, 26193.8))), Vector(240, 0, 0), 63, MAX_HEALTH_BUILDING, true);
		Building* building7 = new Building(mGame->getScene()->getMeshManager(), "namas1", Vector(30664.6, 26214.1, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30664.6, 1315, 26214.1))), Vector(264, 0, 0), 79, MAX_HEALTH_BUILDING, true);
		Building* building8 = new Building(mGame->getScene()->getMeshManager(), "namas2", Vector(30495.8, 26140.2, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30495.8, 1315, 26140.2))), Vector(240, 0, 0), 60, MAX_HEALTH_BUILDING, true);
		Building* building9 = new Building(mGame->getScene()->getMeshManager(), "namas5", Vector(30524.6, 26398, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30524.6, 1315, 26398))), Vector(-56, 0, 0), 73, MAX_HEALTH_BUILDING, true);
		Building* building10 = new Building(mGame->getScene()->getMeshManager(), "namas6", Vector(30331.7, 26153.3, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30331.7, 1315, 26153.3))), Vector(336, 0, 0), 101, MAX_HEALTH_BUILDING, true);
		Building* building11 = new Building(mGame->getScene()->getMeshManager(), "namas7", Vector(30271.7, 26131.6, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30271.7, 1315, 26131.6))), Vector(154, 0, 0), 90, MAX_HEALTH_BUILDING, true);
		Building* building12 = new Building(mGame->getScene()->getMeshManager(), "namas8", Vector(30274.9, 26213.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30274.9, 1315, 26213.8))), Vector(-28, 0, 0), 96, MAX_HEALTH_BUILDING, true);
		Building* building13 = new Building(mGame->getScene()->getMeshManager(), "namas9", Vector(30480, 26056.7, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30480, 1315, 26056.7))), Vector(-50, 0, 0), 86, MAX_HEALTH_BUILDING, true);
		Building* building14 = new Building(mGame->getScene()->getMeshManager(), "store", Vector(30357.7, 26269.3, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30357.7, 1315, 26269.3))), Vector(248, 0, 0), 73, MAX_HEALTH_BUILDING, true);
		Building* building15 = new Building(mGame->getScene()->getMeshManager(), "tavern", Vector(30379, 26047.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(30379, 1315, 26047.8))), Vector(196, 0, 0), 96, MAX_HEALTH_BUILDING, true);
		AddBuilding(building);
		AddBuilding(building1);
		AddBuilding(building2);
		AddBuilding(building3);
		AddBuilding(building4);
		AddBuilding(building5);
		AddBuilding(building6);
		AddBuilding(building7);
		AddBuilding(building8);
		AddBuilding(building9);
		AddBuilding(building10);
		AddBuilding(building11);
		AddBuilding(building12);
		AddBuilding(building13);
		AddBuilding(building14);
		AddBuilding(building15);
	}
	else
	{
		Turret *t1 = new Turret(mGame->getScene()->getMeshManager(), Vector(50974.1, 45817.2, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50974.1, 1315, 45817.2))+20), Vector(0,0,0), true);
		Turret *t2 = new Turret(mGame->getScene()->getMeshManager(), Vector(50374.1, 45795.2, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50374.1, 1315, 45795.2))+20), Vector(0,0,0), true);
		Turret *t3 = new Turret(mGame->getScene()->getMeshManager(), Vector(52034.4, 45345.1, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(52034.4, 1315, 45345.1))+20), Vector(0,0,0), true);
		Turret *t4 = new Turret(mGame->getScene()->getMeshManager(), Vector(50943.1, 45159.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50943.1, 1315, 45159.8))+20), Vector(0,0,0), true);
		Turret *t5 = new Turret(mGame->getScene()->getMeshManager(), Vector(50411.1, 45155.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50411.1, 1315, 45155.8))+20), Vector(0,0,0), true);

		AddTurret(t1);
		AddTurret(t2);
		AddTurret(t3);
		AddTurret(t4);
		AddTurret(t5);	

		Building* building = new Building(mGame->getScene()->getMeshManager(), "pilis", Vector(51318.7, 45347.4, 1175.33-44), Vector(-86-178, 0, 0), 358, MAX_HEALTH_BUILDING, true);
		Building* building1 = new Building(mGame->getScene()->getMeshManager(), "fish_house", Vector(50766.7, 45453.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50766.7, 1315, 45453.4))), Vector(-32-178, 0, 0), 67, MAX_HEALTH_BUILDING, true);
		Building* building2 = new Building(mGame->getScene()->getMeshManager(), "gas_station", Vector(50527.6, 45302.9, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50527.6, 1315, 45302.9))+1), Vector(92-178, 6, 2), 74, MAX_HEALTH_BUILDING, true);
		Building* building3 = new Building(mGame->getScene()->getMeshManager(), "lusna", Vector(50611.9, 45339.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50611.9, 1315, 45339.8))), Vector(394-178, 3, 0), 88, MAX_HEALTH_BUILDING, true);
		Building* building4 = new Building(mGame->getScene()->getMeshManager(), "lusna2", Vector(50606.7, 45425.8, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50606.7, 1315, 45425.8))), Vector(-116-178, -2, -3), 83, MAX_HEALTH_BUILDING, true);
		Building* building5 = new Building(mGame->getScene()->getMeshManager(), "mansion", Vector(50643.5, 45439.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50643.5, 1315, 45439.4))-2), Vector(340-178, 0, -4), 48, MAX_HEALTH_BUILDING, true);
		Building* building6 = new Building(mGame->getScene()->getMeshManager(), "namas01", Vector(50730.4, 45497.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50730.4, 1315, 45497.4))), Vector(240-178, 0, 0), 63, MAX_HEALTH_BUILDING, true);
		Building* building7 = new Building(mGame->getScene()->getMeshManager(), "namas1", Vector(50537.4, 45477.1, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50537.4, 1315, 45477.1))), Vector(264-178, 0, 0), 79, MAX_HEALTH_BUILDING, true);
		Building* building8 = new Building(mGame->getScene()->getMeshManager(), "namas2", Vector(50706.2, 45551, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50706.2, 1315, 45551))), Vector(240-178, 0, 0), 60, MAX_HEALTH_BUILDING, true);
		Building* building9 = new Building(mGame->getScene()->getMeshManager(), "namas5", Vector(50677.4, 45293.2, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50677.4, 1315, 45293.2))), Vector(-56-178, 0, 0), 73, MAX_HEALTH_BUILDING, true);
		Building* building10 = new Building(mGame->getScene()->getMeshManager(), "namas6", Vector(50870.3, 45537.9, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50870.3, 1315, 45537.9))), Vector(336-178, 0, 0), 101, MAX_HEALTH_BUILDING, true);
		Building* building11 = new Building(mGame->getScene()->getMeshManager(), "namas7", Vector(50930.3, 45559.6, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50930.3, 1315, 45559.6))), Vector(154-178, 0, 0), 90, MAX_HEALTH_BUILDING, true);
		Building* building12 = new Building(mGame->getScene()->getMeshManager(), "namas8", Vector(50927.1, 45477.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50927.1, 1315, 45477.4))), Vector(-28-178, 0, 0), 96, MAX_HEALTH_BUILDING, true);
		Building* building13 = new Building(mGame->getScene()->getMeshManager(), "namas9", Vector(50722, 45634.5, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50722, 1315, 45634.5))), Vector(-50-178, 0, 0), 86, MAX_HEALTH_BUILDING, true);
		Building* building14 = new Building(mGame->getScene()->getMeshManager(), "store", Vector(50844.3, 45421.9, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50844.3, 1315, 45421.9))), Vector(248-178, 0, 0), 73, MAX_HEALTH_BUILDING, true);
		Building* building15 = new Building(mGame->getScene()->getMeshManager(), "tavern", Vector(50823, 45643.4, 
			mGame->getScene()->getChunkManager()->getMapHeightAtPoint(D3DXVECTOR3(50823, 1315, 45643.4))), Vector(196-178, 0, 0), 96, MAX_HEALTH_BUILDING, true);
		AddBuilding(building);
		AddBuilding(building1);
		AddBuilding(building2);
		AddBuilding(building3);
		AddBuilding(building4);
		AddBuilding(building5);
		AddBuilding(building6);
		AddBuilding(building7);
		AddBuilding(building8);
		AddBuilding(building9);
		AddBuilding(building10);
		AddBuilding(building11);
		AddBuilding(building12);
		AddBuilding(building13);
		AddBuilding(building14);
		AddBuilding(building15);
	}
}

void Player::AddBuilding(Building* b)
{
	b->SetNetworkIDManager(mGame->getNetworkIDManager());
	b->SetOwnerId(mPlayerId);
	b->CreateSerialize(mGame->getNetwork()->GetServer());

	mBuildingsList.push_back(b);
	mGame->getScene()->getChunkManager()->addEntity(b);
}


void Player::AddTurret(Turret* tt)
{
	tt->SetNetworkIDManager(mGame->getNetworkIDManager());
	tt->SetOwnerId(mPlayerId);
	tt->CreateSerialize(mGame->getNetwork()->GetServer());

	mTurretsList.push_back(tt);
	mGame->getScene()->getChunkManager()->addEntity(tt);
}


void Player::SetTurretsTarget(AircraftB17* p)
{
	for (int i = 0; i < mTurretsList.size(); i++)
		mTurretsList[i]->setTarget(p);
}