#pragma once
#include <string>
#include <winsock2.h>
using std::string;
//保存配置信息
class Config
{
public:
	static const int MAXCONNECTION;		//最大连接数
	static const int BUFFERLENGTH;		//缓冲区大小
	static string SERVERADDRESS;  //服务器地址
	static int PORT;				//服务器端口
	static const u_long BLOCKMODE;			//SOCKET阻塞模式
	static string HTMLDIR;			//主目录
private:
	Config(void);
	~Config(void);
};
