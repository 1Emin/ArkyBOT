#pragma once
#include <thread>
#include <chrono>
#include <sstream>
#include <vector>
#include <limits>
#include <math.h>
#include <string>
#include <iostream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <ctime>
#include "utilsfunc.h"
#include "corefunc.h"
#include "userfunc.h"
#include "enet/include/enet.h"
#include <ctime>
#include "proton/rtparam.hpp"
#include <fstream>
#include <fstream>
#include <filesystem>
#include <future>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include "utils.h"
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <fstream>
#include <chrono>
#include "corefunc.h"
#include <sstream>
#include "userfunc.h"
#include "json.hpp"
#include <string>
#include <windows.h>
#include <direct.h>
#include <d3d9.h>
#include "gui.h"
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include <algorithm>
#include "imgui/TextEditor.h"
#include <fstream>
#include <fstream>
#include <filesystem>
#include <future>
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include "bindings.h"



using namespace std;

char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string hexStr(unsigned char data)
{
	std::string s(2, ' ');
	s[0] = hexmap[(data & 0xF0) >> 4];
	s[1] = hexmap[data & 0x0F];
	return s;
}

string generateMeta()
{
	string x;
	for (int i = 0; i < 9; i++)
	{
		x += hexStr(rand());
	}
	x += ".com";
	return x;
}

string generateMac()
{
	string x;
	for (int i = 0; i < 6; i++)
	{
		x += hexStr(rand());
		if (i != 5)
			x += ":";
	}
	return x;
}

string generateRid()
{
	string x;
	for (int i = 0; i < 16; i++)
	{
		x += hexStr(rand());
	}
	for (auto & c : x) c = toupper(c);
	return x;
}

bool GrowtopiaBot::rep(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void GrowtopiaBot::solve_captcha(std::string text) {
	GrowtopiaBot::rep(text,
		"set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following "
		"numbers|left|\nadd_textbox|",
		"");
	GrowtopiaBot::rep(text, "|left|\nadd_text_input|captcha_answer|Answer:||32|\nend_dialog|captcha_submit||Submit|", "");
	auto number1 = text.substr(0, text.find(" +"));
	auto number2 = text.substr(number1.length() + 3, text.length());
	int result = atoi(number1.c_str()) + atoi(number2.c_str());
	SendPacket(2, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(result), peer);
}

string stripMessage(string msg) {
	regex e("\\x60[a-zA-Z0-9!@#$%^&*()_+\\-=\\[\\]\\{};':\"\\\\|,.<>\\/?]");
	string result = regex_replace(msg, e, "");
	result.erase(std::remove(result.begin(), result.end(), '`'), result.end());
	return result;
}

void GrowtopiaBot::onLoginRequested()
{
	
	string token;
	if (!login_user && !login_token) {
		token = "";
	} else {
		token = "\nuser|" + std::to_string(login_user) + "\ntoken|" + std::to_string(login_token);
	}
	string ver = gameVersion;
	string hash = std::to_string((unsigned int)rand());
	string hash2 = std::to_string((unsigned int)rand());
	string packet = "tankIDName|" + uname + "\ntankIDPass|" + upass + "\nrequestedName|SmileZero\nf|1\nprotocol|84\ngame_version|" + ver + "\nfz|5367464\nlmode|0\ncbits|0\nplayer_age|18\nGDPR|1\nhash2|" + hash2 + "\nmeta|" + generateMeta() + "\nfhash|-716928004\nrid|" + generateRid() + "\nplatformID|\ndeviceVersion|0\ncountry|us\nhash|" + hash + "\nmac|" + generateMac() + "\nwk|" + generateRid() + "\nzf|-496303939" + token;
	cout << packet;
	SendPacket(2, "tankIDName|" + uname + "\ntankIDPass|" + upass + "\nrequestedName|SmileZero\nf|1\nprotocol|84\ngame_version|" + ver + "\nfz|5367464\nlmode|0\ncbits|0\nplayer_age|18\nGDPR|1\nhash2|" + hash2 + "\nmeta|" + generateMeta() + "\nfhash|-716928004\nrid|" + generateRid() + "\nplatformID|0\ndeviceVersion|0\ncountry|us\nhash|" + hash + "\nmac|" + generateMac() + "\nwk|" + generateRid() + "\nzf|-496303939" + token, peer);

	currentWorld = "";
}

void GrowtopiaBot::packet_type3(string text)
{
	dbgPrint("Some text is here: " + text);
	if (text.find("LOGON ATTEMPTS") != string::npos)
	{
		
	}
	if (text.find("password is wrong") != string::npos)
	{
		
	}
	if (text.find("action|logon_fail") != string::npos)
	{
		connectClient();
		objects.clear();
		currentWorld = "";
	}
}

void GrowtopiaBot::packet_type6(string text)
{
	SendPacket(2, "action|enter_game\n", peer);
	enet_host_flush(client);
}

void GrowtopiaBot::packet_unknown(ENetPacket* packet)
{

}

void GrowtopiaBot::OnSendToServer(string address, int port, int userId, int token)
{
	login_user = userId;
	login_token = token;
	connectClient(address, port);
}

void GrowtopiaBot::onShowCaptcha(string text) {
	solve_captcha(text);
	cout << text << endl;
}

void GrowtopiaBot::OnDialogRequest(string text) {
	if (text.find("end_dialog|captcha_submit||Submit|") != -1)
		solve_captcha(text);
	cout << text << endl;
}

void GrowtopiaBot::use() {
	GamePacket pkt{ 0 };
	pkt.type = 7;
	pkt.tile.x = (int)(localx / 32);
	pkt.tile.y = (int)(localy / 32);
	SendPacketRaw(4, &pkt, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
}

void GrowtopiaBot::move(std::string to, int blocks) {
	if (to == "right") {
		GamePacket pkt{ 0 };
		pkt.type = 0;
		pkt.pos.x = localx + 32 * blocks;
		pkt.pos.y = localy;
		SendPacketRaw(4, &pkt, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		localy = localy;
		localx = localx + 32 * blocks;
	}
	else if (to == "left") {
		GamePacket pkt{ 0 };
		pkt.type = 0;
		pkt.pos.x = localx - 32 * blocks;
		pkt.pos.y = localy;
		SendPacketRaw(4, &pkt, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		localy = localy;
		localx = localx - 32 * blocks;
	}
	else if (to == "down") {
		GamePacket pkt{ 0 };
		pkt.type = 0;
		pkt.pos.x = localx;
		pkt.pos.y = localy + 32 * blocks;
		SendPacketRaw(4, &pkt, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		localy = localy + 32 * blocks;
		localx = localx;
	}
	else if (to == "up") {
		GamePacket pkt{ 0 };
		pkt.type = 0;
		pkt.pos.x = localx;
		pkt.pos.y = localy - 32 * blocks;
		SendPacketRaw(4, &pkt, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		localy = localy - 32 * blocks;
		localx = localx;
	}
}
bool niegegr = false;
void GrowtopiaBot::sleepforgo() {
	niegegr = true;
	SendPacket(2, "action|input\ntext|/go", peer);
	Sleep(17000);
	niegegr = false;
}
void GrowtopiaBot::OnConsoleMessage(string message) {
	string strippedMessage = stripMessage((string)message);
	cout << strippedMessage << endl;
	chatoutput = chatoutput + strippedMessage + "\n";
	if (strippedMessage.find("[SB]") != -1 && automsg2) {
			SendPacket(2, "action|input\n|text|/go", peer);
		//if (automsg2count <= 4) {
			
			//automsg2count++;
		//}
		//else {
		//	automsg2 = false;
		///	automsg2count = 0;
		//}	
	}
	if (strippedMessage.find("[SB]") != -1 && !niegegr && gettingppl) {
		if (!dontdo) {
			std::thread x(&GrowtopiaBot::sleepforgo, this);
			x.detach();
		}
	}
}

void GrowtopiaBot::OnPlayPositioned(string sound)
{

}

void GrowtopiaBot::OnSetFreezeState(int state)
{

}

void GrowtopiaBot::OnRemove(string data) // "netID|x\n"
{
	std::stringstream ss(data.c_str());
	std::string to;
	int netID = -1;
	while (std::getline(ss, to, '\n')) {
		string id = to.substr(0, to.find("|"));
		string act = to.substr(to.find("|") + 1, to.length() - to.find("|"));
		if (id == XorStr("netID"))
		{
			netID = atoi(act.c_str());
		}
		else {
			
		}
	}
	for (int i = 0; i < objects.size(); i++) {
		if (objects.at(i).netId == netID) {
			objects.at(i).isGone = true;
		}
	}
}

std::string gen_random(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}

rtvar var2;

bool Wait(const unsigned long& Time)
{
	clock_t Tick = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
	if (Tick < 0) // if clock() fails, it returns -1
		return 0;
	clock_t Now = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
	if (Now < 0)
		return 0;
	while ((Now - Tick) < Time)
	{
		Now = clock_t(float(clock()) / float(CLOCKS_PER_SEC) * 1000.f);
		if (Now < 0)
			return 0;
	}
	return 1;
}
void GrowtopiaBot::doautomsg2() {
	
}
namespace types {
	typedef	std::chrono::duration<double> elapsed;
	typedef std::chrono::system_clock::time_point time;
}



bool run_at_interval(types::time& timer, double interval) {
	types::time now = std::chrono::system_clock::now();
	types::elapsed elapsed_sec = now - timer;


	if (elapsed_sec.count() >= interval) {
		timer = now;
		return true;
	}
	return false;
}

bool eh;


void GrowtopiaBot::dartsucks() {
	static types::time timerlol = std::chrono::system_clock::now();
	EnterWorld(selectworld);
	Sleep(10000);
	for (int i = 0; i <	ppluids.size(); i++) {
		std::string packet1; //first
		std::string packet2;
		std::string packet3;
		std::string packet4;
		std::string packet5; //last

		packet1 = "action|friends\ndelay|0";
		packet2 = "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|showfriend";
		packet3 = "action|dialog_return\ndialog_name|friends\nbuttonClicked|" + (string)frienduid;
		packet4 = "action|dialog_return\ndialog_name|friends_edit\nfriendID|" + (string)frienduid + "|\nbuttonClicked|msg";
		packet5 = "action|dialog_return\ndialog_name|friends_message\nfriendID|" + to_string(ppluids.at(i)) + "|\nbuttonClicked|send\n\ntext|" + (string)msgtext + " " + gen_random(8);

		SendPacket(2, packet1, peer);

		SendPacket(2, packet2, peer);

		SendPacket(2, packet3, peer);

		SendPacket(2, packet4, peer);

		SendPacket(2, packet5, peer);

		Sleep(180);
	}
	Sleep(10000);
	selectmax = selectmax + 50;
	dontdo = false;
	
}


void GrowtopiaBot::OnSpawn(string data)
{
	
	       var2 = rtvar::parse(data); 
		   if (!dontdo) {
			   if (gettingppl) {
				   if (ppluids.size() <= selectmax) {
					   int uid = var2.get_int("userID");
					   string name = var2.get("name");
					   if (std::find(ppluids.begin(), ppluids.end(), uid) == ppluids.end()) {
						   ppluids.push_back(uid);
						   pplnames.push_back(name);
					   }
				   }
				   else {
					   thread thr(&GrowtopiaBot::dartsucks, this);
					   thr.detach();
					   dontdo = true;
					   

				   }

			   }
			   if (gettingppl && selectmax >= 180) {
				   selectmax = 50;
				   ppluids.clear();
			   }
		   }
		   
			   

		  if (automsg2) {
			  auto name = var2.get("name");
			  utils::replace(name, "`w", "");
			  utils::replace(name, "``", "");
			  utils::replace(name, "`2", "");
			  utils::replace(name, "`#@", "");
			  utils::replace(name, "`^", "");

			  std::string packet;
			  packet = "action|input\n|text|/msg " + name + " " + message + " " + gen_random(5);
			  SendPacket(2, packet, peer);
		  }

		  if (automsg == true) {
			  auto name = var2.get("name");
			  utils::replace(name, "`w", "");
			  utils::replace(name, "``", "");
			  utils::replace(name, "`2", "");
			  utils::replace(name, "`#@", "");
			  utils::replace(name, "`^", "");

			  std::string packet;
			  if (insurance)
				  packet = "action|input\n|text|/msg " + name + " " + message + " " + gen_random(5);
			  else
				  packet = "action|input\n|text|/msg " + name + " " + message;
			  
			  

			  if (Wait(450))
				  SendPacket(2, packet, peer);
		  }

	ObjectData objectData;
	bool actuallyOwner = false;

	   auto name = var2.find(XorStr("name"));
            auto netid = var2.find(XorStr("netID"));
	
	
			objectData.country = var2.get(XorStr("country"));
		 
			if (stripMessage(var2.get(XorStr("name"))) == ownerUsername) actuallyOwner = true;
			objectData.name = var2.get(XorStr("name"));
		 
			if (actuallyOwner) owner = var2.get_int(XorStr("netID"));
			objectData.netId = var2.get_int(XorStr("netID"));  
			objectData.userId =  var2.get_int(XorStr("userID"));
		 
		
	auto pos = var2.find(XorStr("posXY"));
                    if (pos && pos->m_values.size() >= 2) {
                        auto x = atoi(pos->m_values[0].c_str());
                        auto y = atoi(pos->m_values[1].c_str());
                        //ply.pos = vector2_t{ float(x), float(y) };
			    objectData.x = x;
			objectData.y = y;
                    }
	
	
	 if (data.find(XorStr("type|local")) != -1) {
                   objectData.isLocal = true;
				localx = objectData.x;
				localy = objectData.y;
				localnetid = objectData.netId;
                }
	 if (var2.get(XorStr("mstate")) == "1" || var2.get(XorStr("smstate")) == "1" ||var2.get(XorStr("invis"))== "1"){
		
			objectData.isMod = true;
	 }
	
	 objectData.isGone = false;


	objects.push_back(objectData);
	
}

void GrowtopiaBot::OnAction(string command)
{
}

void GrowtopiaBot::SetHasGrowID(int state, string name, string password)
{

}

void GrowtopiaBot::SetHasAccountSecured(int state)
{

}

void GrowtopiaBot::OnTalkBubble(int netID, string bubbleText, int type)
{

}

void GrowtopiaBot::SetRespawnPos(int respawnPos)
{
	respawnX = respawnPos % 100; // hacky!!! TODO: get from world data (100)
	respawnY = respawnPos / 100; // hacky!!! TODO: get from world data (100)
}

void GrowtopiaBot::OnEmoticonDataChanged(int val1, string emoticons)
{

}

void GrowtopiaBot::OnSetPos(float x, float y)
{

}

void GrowtopiaBot::OnAddNotification(string image, string message, string audio, int val1)
{
}


void GrowtopiaBot::AtApplyTileDamage(int x, int y)
{

}

void GrowtopiaBot::AtApplyLock(int x, int y, int itemId)
{
}

void GrowtopiaBot::AtPlayerMoving(PlayerMoving* data)
{
	//cout << "-----------\n\ntype " << ", netid " << data->netID << ", x " << data->x << ", y " << data->y << ", characterstate " << data->characterState << ", plantingtree " << data->plantingTree << ", xspeed " << data->XSpeed << ", yspeed " << data->YSpeed << ", punchx " << data->punchX << ", punchy" << data->punchY<< "\n\n--------";
	int object = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects.at(i).netId == data->netID)
		{
			object = i;
		}
	}
	if (object != -1)
	{
		objects[object].x = data->x;
		objects[object].y = data->y;
	}
	if (isFollowing && data->netID == owner && data->punchX == -1 && data->punchY == -1 && data->plantingTree == 0)
	{
		if (backwardWalk)
			data->characterState ^= 0x10;
		if ((data->characterState & 0x800) && (data->characterState & 0x100)) {
			SendPacket(2, XorStr("action|respawn"), peer);
		}
		for (int i = 0; i < objects.size(); i++)
			if (objects.at(i).isLocal) {
				objects.at(i).x = data->x;
				objects.at(i).y = data->y;
			}
		SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
	}

	if (dofollow) {
		if (data->netID == followplayer && data->punchX == -1 && data->punchY == -1 && data->plantingTree == 0) {
			if (backwardWalk)
				data->characterState ^= 0x10;
			if ((data->characterState & 0x800) && (data->characterState & 0x100)) {
				SendPacket(2, XorStr("action|respawn"), peer);
			}
			for (int i = 0; i < objects.size(); i++)
				if (objects.at(i).isLocal) {
					objects.at(i).x = data->x;
					objects.at(i).y = data->y;
				}
			SendPacketRaw(4, packPlayerMoving(data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
		}
	}
}

void GrowtopiaBot::AtAvatarSetIconState(int netID, int state)
{

}

void GrowtopiaBot::WhenConnected()
{
	connected = true;
	
}

void GrowtopiaBot::WhenDisconnected()
{
	connected = false;
	
	connectClient();
}

int counter = 0; // 10ms per step

vector<string> explode(const string &delimiter, const string &str)
{
	vector<string> arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return arr;//no change

	int i = 0;
	int k = 0;
	while (i < strleng)
	{
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}


void GrowtopiaBot::EnterWorld(std::string name)
{
	std::string packet = (string)XorStr("action|join_request\nname|");
	std::string fullpacket = (string)packet + (string)name;
	SendPacket(3, fullpacket, peer); // MARRKS
}

void GrowtopiaBot::userLoop() {
	if (timeFromWorldEnter > 200 && currentWorld != worldName) {
		if (worldName == "" || worldName == "-") {
			timeFromWorldEnter = 0;
		} else {
			
			objects.clear();
		}
		timeFromWorldEnter = 0;
		currentWorld = worldName;
	}
	timeFromWorldEnter++;
	counter++;
	if ((counter % 1800) == 0)
	{
		string name = "";
		float distance = std::numeric_limits<float>::infinity();
		float ownerX;
		float ownerY;
		for (ObjectData x : objects)
		{
			if (x.netId == owner)
			{
				ownerX = x.x;
				ownerY = x.y;
			}
		}
	}
}

void GrowtopiaBot::userInit() {
	connectClient();
	cout << flush;
}

void GrowtopiaBot::respawn()
{
	PlayerMoving data;
	data.characterState = 0x924;
	SendPacket(2, "action|respawn", peer);
	for (int i = 0; i < objects.size(); i++)
		if (objects.at(i).isLocal)
		{
			data.x = objects.at(i).x;
			data.y = objects.at(i).y;
			data.netID = objects.at(i).netId;
			SendPacketRaw(4, packPlayerMoving(&data), 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
			
			break;
		}
}
