#pragma once
#include <winsock2.h>
#include <vector>
#include <string>
#include <map>
#include "ServerGUI2Dlg.h"

using std::map;
using std::vector;
using std::string;
using std::endl;


//服务器

class Server
{
private:
	SOCKET srvSocket;			//服务器socket
								//char* recvBuf;				//接受缓冲区
	fd_set rfds;				//用于检查socket是否有数据到来的的文件描述符，用于socket非阻塞模式下等待网络事件通知（有数据到来）
	fd_set wfds;				//用于检查socket是否可以发送的文件描述符，用于socket非阻塞模式下等待网络事件通知（可以发送数据）
	sockaddr_in srvAddr;		//服务器端IP地址
	vector<SOCKET> conns;		//会话socket数组
	vector<int> conns_manage;	//	socket状态
	int conns_max;				//最大会话数
								//char HtmlDir[512];			//主目录
	char szResponse[256 * 10];   //相应报文
	int sockt_run;

	/*
	list<SOCKET> *sessions;							//当前的会话socket队列
	list<SOCKET> *closedSessions;					//所有已失效的会话socket队列
	list<string> *rcvedMessages;					//已接收到的客户端信息队列
	int numOfSocketSignaled;						//可读、写、有请求到来的socket总数
	*/
	map<SOCKET, string> *clientAddrMaps;				//客户端地址列表，地址以(key,value)对形式保存，key为SOCKET类型，value为string类型

protected:
	virtual string  GetClientAddress(SOCKET s);								//得到客户端IP地址


public:
	Server(void);
	virtual ~Server(void);
	virtual int WinsockStartup();		//初始化Winsock
	virtual int ServerStartup();		//初始化Server，包括创建SOCKET，绑定到IP和PORT
	virtual int ListenStartup();		//开始监听客户端请求

	int Loop(SOCKET rtn);					//循环执行"等待客户端请求"->“向其他客户转发信息”->"等待客户端消息"

public:
	void AcceptConnection();					//连接会话
	virtual int ParseRequest(SOCKET sr, char *&recvBuf);						//分析接受到的数据并发送数据
	virtual int PostPicture(SOCKET sr, char *&recvBuf);		//图片请求
	void Set_Server();					//配置更改

	//void handle_accept(SOCKET socket_feed);
	void Close();
	void ParseMessage(SOCKET sock);

};



