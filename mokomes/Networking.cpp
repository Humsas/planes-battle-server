#include "Networking.h"

Networking::Networking(Console* console, Game* game)
{
	mConsole = console;
	mGame = game;
	mServer = RakPeerInterface::GetInstance();

	//ConnectionData tmp;
	//ConnectionData tmp1;
	//mConnectionData.push_back(tmp);
	//mConnectionData.push_back(tmp1);

	SocketDescriptor sd(SERVER_PORT, 0);
	mServer->Startup(MAX_CLIENTS, &sd, 1);

	mServer->SetMaximumIncomingConnections(MAX_CLIENTS);
	mServer->SetTimeoutTime(300000, UNASSIGNED_SYSTEM_ADDRESS);

	OpenUPNP();
}


void Networking::Update()
{
	Packet*	packet;
	for (packet = mServer->Receive(); packet; mServer->DeallocatePacket(packet), packet = mServer->Receive())
	{
		switch (packet->data[0])
		{
			//ID_CONNECTION_ATTEMPT_FAILED - kai klientui nepavyksta prisijungt
			//Remote siun2ia kitiem klientam =inute kad prisijunge kitas clientas(Man neveikë)
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			mConsole->addLine("Another client has disconnected.");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			mConsole->addLine("Another client has lost the connection.");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			mConsole->addLine("Another client has connected.");
			break;
		case ID_NEW_INCOMING_CONNECTION:
			{
				mConsole->addLine("A connection is incoming.");

				mGame->playerConnected(packet->guid);
				break;
			} 
		case ID_DISCONNECTION_NOTIFICATION:
			{
				mConsole->addLine("A client has disconnected.");
				break;
			} 
		case ID_GAME_MESSAGE_KEYS_UPDATE:
			{
				RakNet::BitStream* bsIn = new RakNet::BitStream(packet->data, packet->length, false);
				bsIn->IgnoreBytes(sizeof(RakNet::MessageID));

				mGame->ProcessKeyMessages(bsIn, packet->guid);
				break;
			} 
		case ID_CONNECTION_LOST:
			{
				mConsole->addLine("A client lost the connection.");
				//Nutraukiam zaidima
				PostQuitMessage(1);

				break;
			} 
		case ID_GAME_MESSAGE_PLAYER_DISCONNECTED:
			{
				break;
			} 
		case ID_GAME_MESSAGE_OBJECT_CREATION_CONFIRMED:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				NetworkID id;
				int type;
				bsIn.Read(id);
				bsIn.Read(type);

				switch (type)
				{
				case GAME_ENTITY_CUBE:
					{
						TestCubeEntity* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<TestCubeEntity*>(id);

						if(te != NULL)
							te->SetCreated(true);
						break;
					}
				case GAME_ENTITY_AIRCRAFT_B17:
					{
						AircraftB17* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<AircraftB17*>(id);

						if(te != NULL)
						{
							te->SetCreated(true);
							te->setReadyToPlay(true);
						}
						break;
					}
				case GAME_ENTITY_PROJECTILE_BOMB:
					{
						ProjectileBomb* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<ProjectileBomb*>(id);

						if(te != NULL)
							te->SetCreated(true);
						break;
					}
				case GAME_ENTITY_BUILDING:
					{
						Building* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<Building*>(id);

						if(te != NULL)
							te->SetCreated(true);
						break;
					}
				default:
					break;
				}
			}
			break;
		case ID_GAME_MESSAGE_DELETE_OBJECT:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				NetworkID id;
				int type;
				bsIn.Read(id);
				bsIn.Read(type);

				DeleteObjectReceived(id, type);
				break;
			} 
		case ID_GAME_MESSAGE_LOADING_COMPLETED:
			{
				//for(int i = 0; i < mConnectionData.size(); i++)
				//	if(mConnectionData[i].playerId == packet->guid)
				//	{
				//		mConnectionData[i].ready = true;
				//	}

				break;
			} 
		case ID_GAME_MESSAGE_CONNECTION_DATA: //Data when player first connects to server
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				NetworkID id;
				int size = 0;

				bsIn.Read(size);
				for(int i = 0; i < size; i++)
				{
					id = -1;
					int type = -1;
					bsIn.Read(id);
					bsIn.Read(type);

					switch (type)
					{
						case GAME_ENTITY_CUBE:
							{
								TestCubeEntity* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<TestCubeEntity*>(id);
								te->CreateSerialize(mServer, packet->guid);
								break;
							}
						case GAME_ENTITY_AIRCRAFT_B17:
							{
								AircraftB17* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<AircraftB17*>(id);
								te->CreateSerialize(mServer, packet->guid);
								break;
							}
						case GAME_ENTITY_PROJECTILE_BOMB:
							{
								ProjectileBomb* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<ProjectileBomb*>(id);
								te->CreateSerialize(mServer, packet->guid);
								break;
							}
						case GAME_ENTITY_BUILDING:
							{
								Building* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<Building*>(id);
								te->CreateSerialize(mServer, packet->guid);
								break;
							}
						default:
							break;
					}
				}

				break;
			} 
		default:
			{
				CHAR temp[MAX_PATH];
				sprintf_s(temp, "Message with identifier %i has arrived.", packet->data[0]);
				mConsole->addLine(temp);
				break;
			}
		}
	}


	MyLinkedList<AbstractEntity> *pEnt = mGame->getScene()->getChunkManager()->getDynamicEntityList();
	pEnt->networkReadIteratorReset();
	AbstractEntity *es = NULL;
	while((es = pEnt->getNextNetwork()) != NULL)
	{
		switch (es->getType())
		{
		case GAME_ENTITY_CUBE:
			((TestCubeEntity*)es)->NetworkUpdate(mServer);
			break;
		case GAME_ENTITY_AIRCRAFT_B17:
			((AircraftB17*)es)->NetworkUpdate(mServer);
			break;
		case GAME_ENTITY_PROJECTILE_BOMB:
			((ProjectileBomb*)es)->NetworkUpdate(mServer);
			break;
		case GAME_ENTITY_BUILDING:
			((Building*)es)->NetworkUpdate(mServer);
			break;
		default:
			break;
		}
	}

	RakSleep(0);
	Sleep(30);
}



bool Networking::OpenUPNP()
{	
	struct UPNPDev * devlist = 0;
	devlist = upnpDiscover(2000, 0, 0, 0, 0, 0);
	if (devlist)
	{
		mConsole->addLine("List of UPNP devices found on the network :");
		struct UPNPDev * device;
		for(device = devlist; device; device = device->pNext)
		{
			CHAR temp[MAX_PATH];
			sprintf_s(temp, "      desc: %s \n      st: %s", (device->descURL), (device->st));
			mConsole->addLine(temp);
		}

		char lanaddr[64];	// my ip address on the LAN
		struct UPNPUrls urls;
		struct IGDdatas data;
		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==1)
		{
			// Use same external and internal ports
			DataStructures::List<RakNetSocket2* > sockets;
			mServer->GetSockets(sockets);
			char iport[32];
			Itoa(sockets[0]->GetBoundAddress().GetPort(),iport,10);
			char eport[32];
			strcpy(eport, iport);

			// Version miniupnpc-1.6.20120410
			int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
				eport, iport, lanaddr, 0, "UDP", 0, "0");

			if(r!=UPNPCOMMAND_SUCCESS)
			{
				CHAR temp[MAX_PATH];
				sprintf_s(temp, "AddPortMapping(%s, %s, %s) failed with code %d (%s)", eport, iport, lanaddr, r, strupnperror(r));
				mConsole->addLine(temp);
			}

			char intPort[6];
			char intClient[16];

			// Version miniupnpc-1.6.20120410
			char desc[128];
			char enabled[128];
			char leaseDuration[128];
			r = UPNP_GetSpecificPortMappingEntry(urls.controlURL,
				data.first.servicetype,
				eport, "UDP",
				intClient, intPort,
				desc, enabled, leaseDuration);

			if(r!=UPNPCOMMAND_SUCCESS)
			{
				CHAR temp[MAX_PATH];
				sprintf_s(temp, "GetSpecificPortMappingEntry() failed with code %d (%s)", r, strupnperror(r));
				//StringCbPrintfA(temp, MAX_PATH * sizeof(CHAR), "GetSpecificPortMappingEntry() failed with code %d (%s)", r, strupnperror(r));
				mConsole->addLine(temp);

				return false;
			}
			else
				return true;
		}
		else
			return false;
	}
	else
		return false;

	return true;
}



void Networking::SendConnectionDataToPlayer(RakNetGUID id)
{
	//for (int i = 0; i < mConnectionData.size(); i++)
	//	if(mConnectionData[i].playerId == id)
	//	{
	//		if(i == 0)
	//		{
	//			mServer->Send(&mShit1, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_UPDATE, mServer->GetSystemAddressFromGuid(id), false);
	//			mShit1.Reset();
	//		}
	//		else if(i == 1)
	//		{
	//			mServer->Send(&mShit2, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_UPDATE, mServer->GetSystemAddressFromGuid(id), false);
	//			mShit2.Reset();
	//		}
	//		break;
	//	}
}

bool Networking::ArePlayersReady()
{
	if(mGame->mPlayers.size() < 2)
		return false;

	for (int i = 0; i < mGame->mPlayers.size(); i++)
		if(mGame->mPlayers[i]->isReadyToPlay() == false)
			return false;

	return true;
}

//Send message to all clients about object deletion
void Networking::DeleteObjectSend(NetworkID id, int type)
{
	BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_DELETE_OBJECT);
	bsOut.Write((NetworkID)id);
	bsOut.Write(type);
	mServer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_NEW_DATA, UNASSIGNED_SYSTEM_ADDRESS, true);
}

//Received message to delete specific object
void Networking::DeleteObjectReceived(NetworkID id, int type)
{
	switch (type)
	{
	case GAME_ENTITY_CUBE:
		{
			TestCubeEntity* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<TestCubeEntity*>(id);

			if(te != NULL)
			{

			}
			break;
		}
	case GAME_ENTITY_AIRCRAFT_B17:
		{
			AircraftB17* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<AircraftB17*>(id);

			if(te != NULL)
			{

			}
			break;
		}
	case GAME_ENTITY_PROJECTILE_BOMB:
		{
			ProjectileBomb* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<ProjectileBomb*>(id);

			if(te != NULL)
			{

			}
			break;
		}
	case GAME_ENTITY_BUILDING:
		{
			Building* te = mGame->getNetworkIDManager()->GET_OBJECT_FROM_ID<Building*>(id);

			if(te != NULL)
			{

			}
			break;
		}
	default:
		break;
	}

	// Resending message to clients about deletion
	DeleteObjectSend(id, type);
}

/*
void Networking::SendCreatedObjectsIDs(vector<NetworkID> ids)
{
	BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_NEW_OBJECTS_CREATED);
	bsOut.Write(ids.size());
	RakString type = "Cube";
	bsOut.Write(type);
	for(int i = 0; i < ids.size(); i++)
		bsOut.Write(ids[i]);
		
	mServer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_NEW_DATA, UNASSIGNED_SYSTEM_ADDRESS, true); // Siunciam visiem prisijungusiam objektam
}

*/