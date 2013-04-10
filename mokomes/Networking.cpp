#include "Networking.h"

Networking::Networking(Console* console, Game* game)
{
	mConsole = console;
	mServer = RakPeerInterface::GetInstance();

	ConnectionData tmp;
	ConnectionData tmp1;
	mConnectionData.push_back(tmp);
	mConnectionData.push_back(tmp1);

	SocketDescriptor sd(SERVER_PORT, 0);
	mServer->Startup(MAX_CLIENTS, &sd, 1);

	mServer->SetMaximumIncomingConnections(MAX_CLIENTS);

	OpenUPNP();
}


void Networking::Update()
{
	for (mPacketForMessages = mServer->Receive(); mPacketForMessages; mServer->DeallocatePacket(mPacketForMessages), mPacketForMessages = mServer->Receive())
	{
		switch (mPacketForMessages->data[0])
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
				mGame->getPlayersId()->push_back(mPacketForMessages->guid);
				for(int i = 0; i < (*mGame->getPlayersId()).size(); i++)
					if(mPacketForMessages->guid == (*mGame->getPlayersId())[i])
					{
						mConnectionData[i].ready = false;
						mConnectionData[i].playerId = mPacketForMessages->guid;
						if(i == 0)
							mShit1.Write((RakNet::MessageID)ID_GAME_MESSAGE_CONNECTION_DATA);
						else if(i == 1)
							mShit2.Write((RakNet::MessageID)ID_GAME_MESSAGE_CONNECTION_DATA);
						//mConnectionData[i].data.Write((RakNet::MessageID)ID_GAME_MESSAGE_CONNECTION_DATA);
						mGame->playerConnected(mConnectionData[i].playerId);
						break;
						//Send message, to begin data sending, Nusiusti, kitam klientui apie kita zaideja
					}
			}
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			{
				mConsole->addLine("A client has disconnected.");
			} 
			break;
		case ID_CONNECTION_LOST:
			{
				mConsole->addLine("A client lost the connection.");
			} 
			break;
		case ID_GAME_MESSAGE_PLAYER_DISCONNECTED:
			{
			}
			break;

		case ID_GAME_MESSAGE_LOADING_COMPLETED:
			{
				for(int i = 0; i < mConnectionData.size(); i++)
					if(mConnectionData[i].playerId == mPacketForMessages->guid)
					{
						mConnectionData[i].ready = true;
					}
			}
			break;
		default:
			{
				CHAR temp[MAX_PATH];
				sprintf_s(temp, "Message with identifier %i has arrived.", mPacketForMessages->data[0]);
				mConsole->addLine(temp);
				break;
			}
		}
	}
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
			RakString str;
			str = "desc: "+*(device->descURL);
			str = str + "\n st: " + (device->st) + "/n";
			mConsole->addLine(str.C_String());
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
				//stringstream ss;
				//ss << "AddPortMapping(" << eport << ", " << iport << ", " << lanaddr << ") failed with code " << r << " (" << strupnperror(r) << ")";
				sprintf_s(temp, "AddPortMapping(%s, %s, %s) failed with code %d (%s)", eport, iport, lanaddr, r, strupnperror(r));
				//StringCbPrintfA(temp, MAX_PATH * sizeof(CHAR), "AddPortMapping(%s, %s, %s) failed with code %d (%s)", eport, iport, lanaddr, r, strupnperror(r));
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
	for (int i = 0; i < mConnectionData.size(); i++)
		if(mConnectionData[i].playerId == id)
		{
			if(i == 0)
			{
				mServer->Send(&mShit1, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_UPDATE, mServer->GetSystemAddressFromGuid(id), false);
				mShit1.Reset();
			}
			else if(i == 1)
			{
				mServer->Send(&mShit2, HIGH_PRIORITY, RELIABLE_ORDERED, GAME_CHANNEL_UPDATE, mServer->GetSystemAddressFromGuid(id), false);
				mShit2.Reset();
			}
			break;
		}
}


BitStream* Networking::GetPlayerConnectionPacket(RakNetGUID id)
{
	for (int i = 0; i < mConnectionData.size(); i++)
	{
		if(mConnectionData[i].playerId == id)
		{
			if(i == 0)
				return &mShit1;
			else if(i == 1)
				return &mShit2;
			break;
		}
	}
	return NULL;
}


bool Networking::ArePlayersReady()
{
	for (int i = 0; i < mConnectionData.size(); i++)
		if(mConnectionData[i].ready == false)
			return false;

	return true;
}