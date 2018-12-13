#include "stdafx.h"

#include <string>
#include "Config.h"
using namespace std;

Config::Config(void)
{
}

Config::~Config(void)
{
}

string Config::SERVERADDRESS = "192.168.1.103";	//服务器IP地址
const int Config::MAXCONNECTION = 50;				//最大连接数50
const int Config::BUFFERLENGTH = 512;				//缓冲区大小256字节
int Config::PORT = 80;						//服务器端口80
const u_long Config::BLOCKMODE = 1;					//SOCKET为阻塞模式 用不到
string Config::HTMLDIR = "D:/ray/Network/HTML";			//主目录

