#pragma once
#include "enet/include/enet.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "proton/variant.hpp"
#include "packet.h"
#include "proton/vector.hpp"
#include "xorstr.hpp"

using namespace std;

string SERVER_HOST = "213.179.209.168";
int SERVER_PORT = 17201;
string gameVersion = "3.76";

/*********  hidden vars  ********/
struct vec2 {
	float x;
	float y;
};
struct vec2i {
	int x;
	int y;
};
class GrowtopiaBot
{
public:

	/*******    For CPPBot.cpp    **********/
	//spammer
	char spamt[120];
	char frienduid[89] = "40090532";
	bool spammer;
	float interval;
	//spammer


	//follow
	bool dofollow;
	int followplayer;
	//follow

	bool dontdo;

	bool litlfix;

	//automsg
	char message[96];
	bool automsg;
	bool insurance;


	bool automsg2;
	int automsg2count;
	//automsg

	//msgallppl
	char msgtext[89];
	bool allppl;
	char selectworld[120];
	int selectmax;
	//msgallppl


	bool gettingppl;
	vector<int> ppluids;
	vector<string> pplnames;

	ENetPeer *peer;
	ENetHost *client;

	int login_user = 0;
	int login_token = 0;

	string currentWorld;
	int timeFromWorldEnter = 0; // in 10mss...

	
	/*********  hidden vars  ********/

	/********** user sutff ***************/
	int owner = -1;
	string ownerUsername;
	string chatoutput;
	
	int localx=-1;
	int localy=-1;
	int localnetid=-1;

	struct ObjectData
	{
		int netId = -1;	 // used to interact with stuff in world
		int userId = -1; // growtopia id
		string name = "";
		string country = "";
		string objectType = ""; // "avatar" is player creature
		float x = -1;
		float y = -1;
		bool isGone = false; // set true if character left
		int rectX;			 // collision stuff
		int rectY;			 // collision stuff
		int rectWidth;		 // collision stuff
		int rectHeight;		 // collision stuff
		bool isMod = false;
		bool isLocal = false;
	};

	vector<ObjectData> objects;

	string uname;
	string upass;
	string worldName; // excepted world name

	bool isFollowing = false;
	bool backwardWalk = false;

	bool connected;

	int respawnX;
	int respawnY;
	/********** user sutff ***************/

	/*********** structs declaration *********/
	struct PlayerMoving
	{
		int netID;
		float x;
		float y;
		int characterState;
		int plantingTree;
		float XSpeed;
		float YSpeed;
		int punchX;
		int punchY;
	};
	struct GamePacket {
		char type;
		char objtype;
		char count1;
		char count2;
		int netid;
		int item;
		int flags;
		float float1;
		int int_data;
		vec2 pos;
		vec2 pos2;
		float float2;
		vec2i tile;
		unsigned int data_size;
		unsigned int data;
	};
	/*********** structs declaration *********/

	/********* user funcs  *********/
	void userRender();
	void userRender2();
	void EnterWorld(std::string name);
	void userLoop();
	void userInit();
	void onLoginRequested();
	bool rep(std::string& str, const std::string& from, const std::string& to);
	void packet_type3(string text);
	void packet_type6(string text);
	void packet_unknown(ENetPacket *packet);
	void OnSendToServer(string address, int port, int userId, int token);
	void move(std::string to, int blocks);
	void sleepforgo();
	void OnConsoleMessage(string message);
	void onShowCaptcha(string text);
	void OnDialogRequest(string text);
	void use();
	void solve_captcha(std::string text);
	void OnPlayPositioned(string sound);
	void OnSetFreezeState(int state);
	void OnRemove(string data);
	void doautomsg2();
	void dartsucks();
	void lol();
	void OnSpawn(string data);
	void OnAction(string command);
	void SetHasGrowID(int state, string name, string password);
	void SetHasAccountSecured(int state);
	void OnTalkBubble(int netID, string bubbleText, int type);
	void SetRespawnPos(int respawnPos);
	void OnEmoticonDataChanged(int val1, string emoticons);
	void OnSetPos(float x, float y);
	void OnAddNotification(string image, string message, string audio, int val1);
	void AtApplyTileDamage(int x, int y);
	void AtApplyLock(int x, int y, int itemId);
	void AtPlayerMoving(PlayerMoving *data);
	void AtAvatarSetIconState(int netID, int state);
	void WhenConnected();
	void WhenDisconnected();

	void respawn();
	/********* user funcs  *********/

	/***************    debug stuff   ***************/
	void dbgPrint(string text)
	{
		
	}
	/***************    debug stuff   ***************/

	GrowtopiaBot(string username, string password)
	{
		this->uname = username;
		this->upass = password;
	}

	/******************* enet core *********************/
	void SendPacket(int a1, string a2, ENetPeer* enetPeer)
	{
		if (enetPeer)
		{
			ENetPacket* v3 = enet_packet_create(0, a2.length() + 5, 1);
			memcpy(v3->data, &a1, 4);
			//*(v3->data) = (DWORD)a1;
			memcpy((v3->data) + 4, a2.c_str(), a2.length());

			//cout << std::hex << (int)(char)v3->data[3] << endl;
			enet_peer_send(enetPeer, 0, v3);
		}
	}

	void SendPacketRaw(int a1, void* packetData, size_t packetDataSize, void* a4, ENetPeer* peer, int packetFlag)
	{
		ENetPacket* p;

		if (peer) // check if we have it setup
		{
			if (a1 == 4 && *((BYTE*)packetData + 12) & 8)
			{
				p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
				int four = 4;
				memcpy(p->data, &four, 4);
				memcpy((char*)p->data + 4, packetData, packetDataSize);
				memcpy((char*)p->data + packetDataSize + 4, a4, *((DWORD*)packetData + 13));
				enet_peer_send(peer, 0, p);
			}
			else
			{
				p = enet_packet_create(0, packetDataSize + 5, packetFlag);
				memcpy(p->data, &a1, 4);
				memcpy((char*)p->data + 4, packetData, packetDataSize);
				enet_peer_send(peer, 0, p);
			}
		}
	}

	// Connect with default value
	void connectClient() {
		connectClient(SERVER_HOST, SERVER_PORT);
	}

	void connectClient(string hostName, int port)
	{
		
		client = enet_host_create(NULL,
			1,
			2,
			0,
			0);
		client->usingNewPacket = true;
		if (client == NULL)
		{
			

			exit(EXIT_FAILURE);
		}
		ENetAddress address;

		client->checksum = enet_crc32;
		enet_host_compress_with_range_coder(client);
		enet_address_set_host(&address, hostName.c_str());
		address.port = port;

		peer = enet_host_connect(client, &address, 2, 0);
		if (peer == NULL)
		{
			

			exit(EXIT_FAILURE);
		}
		enet_host_flush(client);
	}
	/******************* enet core *********************/



	/*************** sender sutff **************/

	void RequestItemActivate(unsigned int item)
	{
		BYTE* data = new BYTE[56];
		for (int i = 0; i < 56; i++)
		{
			data[i] = 0;
		}
		BYTE ten = 10;
		memcpy(data + 0, &ten, 1);
		memcpy(data + 20, &item, 1);
		SendPacketRaw(4, data, 0x38u, 0, peer, 1);
		free(data);
	}

	void SetAndBroadcastIconState(int netID, int state)
	{
		BYTE* data = new BYTE[56];
		for (int i = 0; i < 56; i++)
		{
			data[i] = 0;
		}
		BYTE eighteen = 18;
		memcpy(data + 0, &eighteen, 1);
		memcpy(data + 4, &netID, 4); // (a1+40)
		memcpy(data + 44, &state, 4);
		SendPacketRaw(4, data, 0x38u, 0, peer, 1);
		free(data);
	}

	void SendPing()
	{
		BYTE* data = new BYTE[56];
		for (int i = 0; i < 56; i++)
		{
			data[i] = 0;
		}
		BYTE twentytwo = 22;
		memcpy(data + 0, &twentytwo, 1);
		SendPacketRaw(4, data, 56, 0, peer, 1);
		free(data);
	}

	/*************** sender sutff **************/


	int GetMessageTypeFromPacket(ENetPacket* packet)
	{
		int result;

		if (packet->dataLength > 3u)
		{
			result = *(packet->data);
		}
		else
		{
			
			result = 0;
		}
		return result;
	}

	char* GetTextPointerFromPacket(ENetPacket* packet)
	{
		char zero = 0;
		memcpy(packet->data + packet->dataLength - 1, &zero, 1);
		return (char*)(packet->data + 4);
	}

	struct OnSendToServerStruct
	{
		string address;
		int port;
		int userId;
		int token;
	};

	struct OnConsoleMessageStruct
	{
		string message;
	};

	struct onShowCaptchaStruct
	{
		string text;
	};

	struct OnDialogRequestStruct
	{
		string text;
	};

	struct OnPlayPositionedStruct
	{
		string sound;
	};

	struct OnSetFreezeStateStruct
	{
		int state;
	};

	struct OnRemoveStruct
	{
		string data;
	};

	struct OnSpawnStruct
	{
		string data;
	};

	struct OnActionStruct
	{
		string data;
	};

	struct SetHasGrowIDStruct
	{
		int state;
		string name;
		string password;
	};

	struct SetHasAccountSecuredStruct
	{
		int state;
	};

	struct OnTalkBubbleStruct
	{
		int netID;
		string bubbleText;
		int type;
	};

	struct SetRespawnPosStruct
	{
		int respawnPos;
	};

	struct OnEmoticonDataChangedStruct
	{
		int val1;
		string emoticons;
	};

	struct OnSetPosStruct
	{
		float x;
		float y;
	};

	struct OnAddNotificationStruct
	{
		string image;
		string message;
		string audio;
		int val1;
	};

	//SerializeFromMem(GetExtendedDataPointerFromTankPacket(structPointer), *(int*)(structPointer + 52), 0, *(int*)(structPointer + 4));
	void SerializeFromMem(byte* pSrc, int bufferSize, int* pBytesReadOut, int netId)
	{
		string action = "";
		BYTE* dataStruct = NULL;
		byte* startPtr = pSrc;
		byte* pStartPos = pSrc;
		byte count = pSrc[0]; pSrc++;
		bool isNetIdHandled = false;

		for (int i = 0; i < 10; i++) cout << (int)*(pSrc + i) << endl;
		for (int i = 0; i < count; i++)
		{
			byte index = pSrc[0]; pSrc++;
			byte type = pSrc[0]; pSrc++;
			
			switch (type)
			{
			case 2:
			{
				int strlen;
				memcpy(&strlen, pSrc, 4); pSrc += 4;

				string v;
				v.resize(strlen);
				memcpy(&v[0], pSrc, strlen); pSrc += strlen;
				if (index == 0)
				{
					action = v;
					if (action == "OnSendToServer")
					{
						dataStruct = (BYTE*)new OnSendToServerStruct;
					}
					else if (action == "OnConsoleMessage")
					{
						dataStruct = (BYTE*)new OnConsoleMessageStruct;
					}
					else if (action == "onShowCaptcha")
					{
						dataStruct = (BYTE*)new onShowCaptchaStruct;
					}
					else if (action == "OnDialogRequest")
					{
						dataStruct = (BYTE*)new OnDialogRequestStruct;
					}
					else if (action == "OnPlayPositioned")
					{
						dataStruct = (BYTE*)new OnPlayPositionedStruct;
					}
					else if (action == "OnSetFreezeState")
					{
						dataStruct = (BYTE*)new OnSetFreezeStateStruct;
					}
					else if (action == "OnRemove")
					{
						dataStruct = (BYTE*)new OnRemoveStruct;
					}
					else if (action == "OnSpawn")
					{
						dataStruct = (BYTE*)new OnSpawnStruct;
					}
					else if (action == "OnAction")
					{
						dataStruct = (BYTE*)new OnActionStruct;
					}
					else if (action == "SetHasGrowID")
					{
						dataStruct = (BYTE*)new SetHasGrowIDStruct;
					}
					else if (action == "SetHasAccountSecured")
					{
						dataStruct = (BYTE*)new SetHasAccountSecuredStruct;
					}
					else if (action == "OnTalkBubble")
					{
						dataStruct = (BYTE*)new OnTalkBubbleStruct;
					}
					else if (action == "SetRespawnPos")
					{
						dataStruct = (BYTE*)new SetRespawnPosStruct;
					}
					else if (action == "OnEmoticonDataChanged")
					{
						dataStruct = (BYTE*)new OnEmoticonDataChangedStruct;
					}
					else if (action == "OnSetPos")
					{
						dataStruct = (BYTE*)new OnSetPosStruct;
					}
					else if (action == "OnAddNotification")
					{
						dataStruct = (BYTE*)new OnAddNotificationStruct;
					} {
						
					}
				}
				if (action == "OnSendToServer" && index == 4)
				{
					v.resize(v.length() - 1);
					((OnSendToServerStruct*)dataStruct)->address = v;
				}
				else if (action == "OnConsoleMessage" && index == 1)
				{
					((OnConsoleMessageStruct*)dataStruct)->message = v;
				}
				else if (action == "onShowCaptcha" && index == 1)
				{
					((onShowCaptchaStruct*)dataStruct)->text = v;
				}
				else if (action == "OnDialogRequest" && index == 1)
				{
					((OnDialogRequestStruct*)dataStruct)->text = v;
				}
				else if (action == "OnPlayPositioned" && index == 1)
				{
					((OnPlayPositionedStruct*)dataStruct)->sound = v;
				}
				else if (action == "OnRemove" && index == 1)
				{
					((OnRemoveStruct*)dataStruct)->data = v;
				}
				else if (action == "OnSpawn" && index == 1)
				{
					((OnRemoveStruct*)dataStruct)->data = v;
				}
				else if (action == "OnAction" && index == 1)
				{
					((OnRemoveStruct*)dataStruct)->data = v;
				}
				else if (action == "SetHasGrowID" && index == 2)
				{
					((SetHasGrowIDStruct*)dataStruct)->name = v;
				}
				else if (action == "SetHasAccountSecured" && index == 3)
				{
					((SetHasGrowIDStruct*)dataStruct)->password = v;
				}
				else if (action == "OnTalkBubble" && index == 2)
				{
					((OnTalkBubbleStruct*)dataStruct)->bubbleText = v;
				}
				else if (action == "OnEmoticonDataChanged" && index == 2)
				{
					((OnEmoticonDataChangedStruct*)dataStruct)->emoticons = v;
				}
				else if (action == "OnAddNotification" && index == 1)
				{
					((OnAddNotificationStruct*)dataStruct)->image = v;
				}
				else if (action == "OnAddNotification" && index == 2)
				{
					((OnAddNotificationStruct*)dataStruct)->message = v;
				}
				else if (action == "OnAddNotification" && index == 3)
				{
					((OnAddNotificationStruct*)dataStruct)->audio = v;
				}
				else if (index != 0)
				{
					
				}
				break;
			}

			case 5:
			{
				int v;
				memcpy(&v, pSrc, sizeof(int));
				pSrc += sizeof(int);
				if (action == "OnSetFreezeState" && index == 1)
				{
					((OnSetFreezeStateStruct*)dataStruct)->state = v;
				}
				else if (action == "SetHasGrowID" && index == 1)
				{
					((SetHasGrowIDStruct*)dataStruct)->state = v;
				}
				else if (action == "SetHasAccountSecured" && index == 1)
				{
					((SetHasAccountSecuredStruct*)dataStruct)->state = v;
				}
				else if (action == "OnTalkBubble" && index == 1)
				{
					((OnTalkBubbleStruct*)dataStruct)->netID = v;
				}
				else if (action == "OnTalkBubble" && index == 3)
				{
					((OnTalkBubbleStruct*)dataStruct)->type = v;
				}
				else if (action == "SetRespawnPos" && index == 1)
				{
					((SetRespawnPosStruct*)dataStruct)->respawnPos = v;
				}
				else if (action == "OnAddNotification" && index == 4)
				{
					((OnAddNotificationStruct*)dataStruct)->val1 = v;
				}
				else {
					
				}
				break;
			}
			case 9:
			{
				int v;
				memcpy(&v, pSrc, sizeof(int));
				pSrc += sizeof(int);
				if (action == "OnSendToServer" && index == 1)
				{
					((OnSendToServerStruct*)dataStruct)->port = v;
				}
				else if (action == "OnSendToServer" && index == 2)
				{
					((OnSendToServerStruct*)dataStruct)->token = v;
				}
				else if (action == "OnSendToServer" && index == 3)
				{
					((OnSendToServerStruct*)dataStruct)->userId = v;
				}
				else if (action == "OnEmoticonDataChanged" && index == 1)
				{
					((OnEmoticonDataChangedStruct*)dataStruct)->val1 = v;
				}
				else {
					
				}
				break;
			}

			case 1:
			{
				float v;
				memcpy(&v, pSrc, sizeof(float));
				pSrc += sizeof(float);
				

				break;
			}

			case 3:
			{

				float v1;
				memcpy(&v1, pSrc, sizeof(float));
				pSrc += sizeof(float);
				float v2;
				memcpy(&v2, pSrc, sizeof(float));
				pSrc += sizeof(float);

				if (action == "OnSetPos" && index == 1)
				{
					((OnSetPosStruct*)dataStruct)->x = v1;
					((OnSetPosStruct*)dataStruct)->y = v2;
				}
				else {
					
				}
				break;

			}

			case 4:
			{
				float v1;
				memcpy(&v1, pSrc, sizeof(float));
				pSrc += sizeof(float);
				float v2;
				memcpy(&v2, pSrc, sizeof(float));
				pSrc += sizeof(float);
				float v3;
				memcpy(&v3, pSrc, sizeof(float));
				pSrc += sizeof(float);
				
				break;
			}

			case 8:
			{
			
				return;
				break;
			}

			default:
				
				return;
				//pSrc += 4;
			}
		}
		if (action == "OnSendToServer")
		{
			OnSendToServer(((OnSendToServerStruct*)dataStruct)->address, ((OnSendToServerStruct*)dataStruct)->port, ((OnSendToServerStruct*)dataStruct)->userId, ((OnSendToServerStruct*)dataStruct)->token);
		}
		else if (action == "OnConsoleMessage")
		{
			OnConsoleMessage(((OnConsoleMessageStruct*)dataStruct)->message);
		}
		else if (action == "onShowCaptcha")
		{
			onShowCaptcha(((onShowCaptchaStruct*)dataStruct)->text);
		}
		else if (action == "OnDialogRequest")
		{
			OnDialogRequest(((OnDialogRequestStruct*)dataStruct)->text);
		}
		else if (action == "OnPlayPositioned")
		{
			OnPlayPositioned(((OnPlayPositionedStruct*)dataStruct)->sound);
		}
		else if (action == "OnSetFreezeState")
		{
			OnSetFreezeState(((OnSetFreezeStateStruct*)dataStruct)->state);
		}
		else if (action == "OnRemove")
		{
			OnRemove(((OnRemoveStruct*)dataStruct)->data);
		}
		else if (action == "OnSpawn")
		{
			OnSpawn(((OnSpawnStruct*)dataStruct)->data);
		}
		else if (action == "OnAction")
		{
			OnAction(((OnActionStruct*)dataStruct)->data);
		}
		else if (action == "SetHasGrowID")
		{
			SetHasGrowID(((SetHasGrowIDStruct*)dataStruct)->state, ((SetHasGrowIDStruct*)dataStruct)->name, ((SetHasGrowIDStruct*)dataStruct)->password);
		}
		else if (action == "SetHasAccountSecured")
		{
			SetHasAccountSecured(((SetHasAccountSecuredStruct*)dataStruct)->state);
		}
		else if (action == "OnTalkBubble")
		{
			OnTalkBubble(((OnTalkBubbleStruct*)dataStruct)->netID, ((OnTalkBubbleStruct*)dataStruct)->bubbleText, ((OnTalkBubbleStruct*)dataStruct)->type);
		}
		else if (action == "SetRespawnPos")
		{
			SetRespawnPos(((SetRespawnPosStruct*)dataStruct)->respawnPos);
		}
		else if (action == "OnEmoticonDataChanged")
		{
			OnEmoticonDataChanged(((OnEmoticonDataChangedStruct*)dataStruct)->val1, ((OnEmoticonDataChangedStruct*)dataStruct)->emoticons);
		}
		else if (action == "OnSetPos")
		{
			OnSetPos(((OnSetPosStruct*)dataStruct)->x, ((OnSetPosStruct*)dataStruct)->y);
		}
		else if (action == "OnAddNotification")
		{
			OnAddNotification(((OnAddNotificationStruct*)dataStruct)->image, ((OnAddNotificationStruct*)dataStruct)->message, ((OnAddNotificationStruct*)dataStruct)->audio, ((OnAddNotificationStruct*)dataStruct)->val1);
		}
		if (dataStruct != NULL)
		{
			free(dataStruct);
		}
		if (!isNetIdHandled && netId != -1)
		{
			
		}
	}

	class EntityComponent {

	};

	BYTE* GetExtendedDataPointerFromTankPacket(BYTE* a1)
	{
		return (BYTE*)((intptr_t)(a1 + 56));
	}


	void HandlePacketTileChangeRequest(BYTE* data)
	{
		switch ((*(int*)data) - 8)
		{
		case 0:
			AtApplyTileDamage(*(int*)(data + 44), *(int*)(data + 48));

			break;
		case 4:
			break;
		case 7:
			AtApplyLock(*(int*)(data + 44), *(int*)(data + 48), *(int*)(data + 20));
			break;
		default:
			break;
		}
	}

	struct WorldThingStruct
	{

	};

	struct WorldStruct
	{
		int XSize;
		int YSize;
		int tileCount;
		string name;
		__int16* foreground;
		__int16* background;
		WorldThingStruct* specials;
	};

	BYTE* packPlayerMoving(PlayerMoving* dataStruct)
	{
		BYTE* data = new BYTE[56];
		for (int i = 0; i < 56; i++)
		{
			data[i] = 0;
		}
		memcpy(data + 4, &dataStruct->netID, 4);
		memcpy(data + 12, &dataStruct->characterState, 4);
		memcpy(data + 20, &dataStruct->plantingTree, 4);
		memcpy(data + 24, &dataStruct->x, 4);
		memcpy(data + 28, &dataStruct->y, 4);
		memcpy(data + 32, &dataStruct->XSpeed, 4);
		memcpy(data + 36, &dataStruct->YSpeed, 4);
		memcpy(data + 44, &dataStruct->punchX, 4);
		memcpy(data + 48, &dataStruct->punchY, 4);
		return data;
	}

	PlayerMoving* unpackPlayerMoving(BYTE* data)
	{
		PlayerMoving* dataStruct = new PlayerMoving;
		memcpy(&dataStruct->netID, data + 4, 4);
		memcpy(&dataStruct->characterState, data + 12, 4);
		memcpy(&dataStruct->plantingTree, data + 20, 4);
		memcpy(&dataStruct->x, data + 24, 4);
		memcpy(&dataStruct->y, data + 28, 4);
		memcpy(&dataStruct->XSpeed, data + 32, 4);
		memcpy(&dataStruct->YSpeed, data + 36, 4);
		memcpy(&dataStruct->punchX, data + 44, 4);
		memcpy(&dataStruct->punchY, data + 48, 4);
		return dataStruct;
	}

	WorldStruct* world = NULL;

	void ProcessTankUpdatePacket(float someVal, EntityComponent* entityComponent, BYTE* structPointer)
	{
		switch (*(char*)structPointer)
		{
		case 1:
		{
			try {
				SerializeFromMem(GetExtendedDataPointerFromTankPacket(structPointer), *(int*)(structPointer + 52), 0, *(int*)(structPointer + 4));

			}
			catch (int e)
			{

			}
			break;
		}
		case 0x15:
		{
			
			break;
		}
		case 0x10:
		{
			BYTE* itemsData = GetExtendedDataPointerFromTankPacket(structPointer);
			__int16 val1 = *(__int16*)itemsData;
			int itemsCount = *(int*)(itemsData + 2);
			break;
		}
		case 0xF:
		{
			HandlePacketTileChangeRequest(structPointer);
			break;
		}
		case 8:
		{
			HandlePacketTileChangeRequest(structPointer);
			break;
		}
		case 9:
		{
			BYTE* playerItems = GetExtendedDataPointerFromTankPacket(structPointer);
			
			break;
		}

		case 5:
		{
			BYTE* tileData = GetExtendedDataPointerFromTankPacket(structPointer);
			
		}
		case 0x16:
		{
			
		}
		break;
		case 0x12:
		{
			AtAvatarSetIconState(*(int*)(structPointer + 4), *(int*)(structPointer + 44));
			break;
		}
		case 0x14:
		{
			
			break;
		}
		case 0xC:
		{
			string x = std::to_string(*(int*)(structPointer + 44));
			string y = std::to_string(*(int*)(structPointer + 48));
			
			break;
		}
		case 0xE:
		{
			
		}
		case 0x23u:
			
			break;
		case 3:
		
			break;
		case 4:
		{
			
			BYTE* worldPtr = GetExtendedDataPointerFromTankPacket(structPointer); // World::LoadFromMem
			world = new WorldStruct;
			worldPtr += 6;
			__int16 strLen = *(__int16*)worldPtr;
			worldPtr += 2;
			world->name = "";
			for (int i = 0; i < strLen; i++)
			{
				world->name += worldPtr[0];
				worldPtr++;
			}
			world->XSize = *(int*)worldPtr;
			worldPtr += 4;
			world->YSize = *(int*)worldPtr;
			worldPtr += 4;
			world->tileCount = *(int*)worldPtr;
			worldPtr += 4;
			world->foreground = (__int16*)malloc(world->tileCount * sizeof(__int16));
			world->background = (__int16*)malloc(world->tileCount * sizeof(__int16));
			
			currentWorld = world->name;
			
			
			
			break;
		}
		case 0: // AvatarPacketReceiver::LerpState
		{
			PlayerMoving* structCahce = unpackPlayerMoving(structPointer);
			AtPlayerMoving(structCahce);
			free(structCahce);
			break;
		}
		default:
			
			break;
		}

	}

	BYTE* GetStructPointerFromTankPacket(ENetPacket* packet)
	{
		unsigned int packetLenght = packet->dataLength;
		BYTE* result = NULL;
		if (packetLenght >= 0x3C)
		{
			BYTE* packetData = packet->data;
			result = packetData + 4;

			if (*(BYTE*)(packetData + 16) & 8)
			{
				
				if (packetLenght < *(int*)(packetData + 56) + 60)
				{
					
					result = 0;
				}
			}
			else
			{
				int zero = 0;
				memcpy(packetData + 56, &zero, 4);
			}
		}
		return result;
	}

	void ProcessPacket(ENetEvent* event, ENetPeer* peer)
	{
		int messageType = GetMessageTypeFromPacket(event->packet);
		switch (messageType) {
		case 1:
			onLoginRequested();
			break;
		default:
			packet_unknown(event->packet);
			break;
		case 3:
			packet_type3(GetTextPointerFromPacket(event->packet));
			break;
		case 4:
		{
			BYTE* tankUpdatePacket = GetStructPointerFromTankPacket(event->packet);
			if (tankUpdatePacket)
			{
				ProcessTankUpdatePacket(0, NULL, tankUpdatePacket);
			}

			else {
				
			}
		}
		break;
		case 5:

			break;
		case 6:
			packet_type6(GetTextPointerFromPacket(event->packet));
			break;
		}
	}


	void eventLoop()
	{
		ENetEvent event;
		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_NONE:
			
				break;
			case ENET_EVENT_TYPE_CONNECT:
				WhenConnected();
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				WhenDisconnected();
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				ProcessPacket(&event, peer);
				enet_packet_destroy(event.packet);
				break;
			default:
				
				break;
			}
		}
		userLoop();
	}
};


void init() {
	if (enet_initialize() != 0) {
		exit(0);
	}
	atexit(enet_deinitialize);
	srand(time(NULL));
}