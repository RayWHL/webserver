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

string Config::SERVERADDRESS = "192.168.1.103";	//������IP��ַ
const int Config::MAXCONNECTION = 50;				//���������50
const int Config::BUFFERLENGTH = 512;				//��������С256�ֽ�
int Config::PORT = 80;						//�������˿�80
const u_long Config::BLOCKMODE = 1;					//SOCKETΪ����ģʽ �ò���
string Config::HTMLDIR = "D:/ray/Network/HTML";			//��Ŀ¼

