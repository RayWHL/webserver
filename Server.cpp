#pragma once
#include "stdafx.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <algorithm>
#include <time.h>
#include "Server.h"
#include "Config.h"
#include "FileType.h"
#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable:4996)

#define SERVERNAME   "Apache/2.2.3"

using std::cout;
using std::cin;
using std::endl;
std::mutex m;


Server::Server(void)
{
	//memset(this->recvBuf, '\0', Config::BUFFERLENGTH);
	conns_max = Config::MAXCONNECTION;
	this->clientAddrMaps = new map<SOCKET, string>();
}

Server::~Server(void)
{
	/*
	//释放接受缓冲区
	if (this->recvBuf != NULL) {
	delete this->recvBuf;
	this->recvBuf = NULL;
	}
	*/

	//关闭server socket
	if (this->srvSocket != NULL) {
		closesocket(this->srvSocket);
		this->srvSocket = NULL;
	}

	//释放客户端地址列表
	if (this->clientAddrMaps != NULL) {
		this->clientAddrMaps->clear();
		delete this->clientAddrMaps;
		this->clientAddrMaps = NULL;
	}

	WSACleanup(); //清理winsock 运行环境
}

//初始化Winsock
int Server::WinsockStartup() {
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());

	//Set_Server();
	WSADATA wsaData;//用于返回Winsock的环境信息   
	int rtn;		//Winsock初始化函数的返回值
	WORD  wVersionRequested = MAKEWORD(1, 1); //Winsock版本号，高字节次版本，低字节主版本号

	rtn = WSAStartup(wVersionRequested, &wsaData);

	if (rtn) { //如果返回值不是0，则Winsock初始化失败
		dlg->C_List.AddString(CString("Winsock startup error!\n"));

		return -1;
	}
	//判断返回的Winsock版本号
	if ((wsaData.wVersion) != 0x0101) { //如果版本不对

		WSACleanup();  //清除Winsock
		dlg->C_List.AddString(CString("Winsock version error!\n"));


		return -1;
	}
	dlg->C_List.AddString(CString("Winsock startup ok!\n"));

	return 0;

}

//初始化Server，包括创建sockect，绑定到IP和PORT
int Server::ServerStartup() {
	//创建 TCP socket
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());

	this->srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->srvSocket == INVALID_SOCKET) {
		dlg->C_List.AddString(CString("Server socket creare error !\n"));

		WSACleanup();
		return -1;
	}
	//dlg->m_listbox.AddString(CString("Server socket creare ok !\n"));


	//设置服务器IP地址和端口号
	this->srvAddr.sin_family = AF_INET;
	this->srvAddr.sin_port = htons(Config::PORT);
	//this->srvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//会自动找到服务器合适的IP地址
	this->srvAddr.sin_addr.S_un.S_addr = inet_addr(Config::SERVERADDRESS.c_str()); //这是另外一种设置IP地址的方法
																				   //绑定 socket to Server's IP and port

	int rtn = bind(this->srvSocket, (LPSOCKADDR)&(this->srvAddr), sizeof(this->srvAddr));
	if (rtn == SOCKET_ERROR) {
		dlg->C_List.AddString(CString("IP or PORT IS ERROR!\n"));

		closesocket(this->srvSocket);
		WSACleanup();
		return -1;
	}
	dlg->C_List.AddString(CString("Server socket bind ok !\n"));


	return 0;
}

//开始监听,等待客户的连接请求
int Server::ListenStartup() {
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());

	int rtn = listen(this->srvSocket, Config::MAXCONNECTION);
	if (rtn == SOCKET_ERROR) {
		dlg->C_List.AddString(CString("Server socket listen error!\n"));

		closesocket(this->srvSocket);
		WSACleanup();
		return -1;
	}
	dlg->C_List.AddString(CString("Server socket listen ok!\n"));

	return 0;
}


//得到客户端IP地址
string  Server::GetClientAddress(SOCKET s) {
	string clientAddress; //clientAddress是个空字符串， clientAddress.empty() is true
	sockaddr_in clientAddr;
	int nameLen, rtn;

	nameLen = sizeof(clientAddr);
	rtn = getsockname(s, (LPSOCKADDR)&clientAddr, &nameLen);
	if (rtn != SOCKET_ERROR) {
		clientAddress += inet_ntoa(clientAddr.sin_addr);
	}

	return clientAddress;
}



void Server::AcceptConnection()					//连接会话
{
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());

	while (true)
	{
		sockaddr ConnAddrs;
		int nAddrLen = sizeof(sockaddr_in);
		//每当收到客户端连接请求，创建新的会话SOCKET，保存在/	//conns数组中
		//客户端地址保存在ConnAddrs数组中
		SOCKET rtn;
		//cout << "waiting...\n";
		rtn = accept(srvSocket,
			&ConnAddrs, &nAddrLen);

		{
			//nSockErr = WSAGetLastError();
			//创建会话SOCKET出错处理
			//dlg->C_List.AddString(CString("Server sock accept error!\n"));
		}
		if (rtn != INVALID_SOCKET)
		{
			m.lock();
			//dlg->C_List.AddString(CString("Server sock accept ok!\n"));

			sockaddr_in *pSin = (sockaddr_in *)&ConnAddrs;
			dlg->C_List.AddString(CString(" "));
			dlg->C_List.AddString(CString("IP:"));
			dlg->C_List.AddString(CString(inet_ntoa(pSin->sin_addr)));
			dlg->C_List.AddString(CString("PORT:"));
			string str;
			str = std::to_string((pSin->sin_port));
			CString Cstr(str.c_str());
			dlg->C_List.AddString(Cstr);
			m.unlock();
			//cout << "IP:" << inet_ntoa(pSin->sin_addr) << endl;
			//cout << "PORT:" << pSin->sin_port << endl;

			//conns.push_back(rtn);
			//this->clientAddrMaps->insert(map<SOCKET, string>::value_type(rtn, this->GetClientAddress(rtn)));//保存地址
			//sockt_run = conns.size() - 1;
			//std::thread LoopThread(&Server::Loop, this,rtn);
			//std::thread LoopThread(&Server::handle_accept, this, rtn);
			std::thread LoopThread(&Server::ParseMessage, this,rtn);

			LoopThread.detach();	
			//dlg->C_List.AddString(CString(""));
			Sleep(10);
		}
	}

}


int Server::Loop(SOCKET rtn) {
	//CServerGUIDlg *dlg = (CServerGUIDlg *)(AfxGetApp()->GetMainWnd());
	//std::thread t1(&Server::AcceptConnection);
	char *recvBuf = new char[Config::BUFFERLENGTH]; //初始化接受缓冲区
													//int i = sockt_run;
													//while(true)
	{
		//std::thread AcceptThreadHandle(&Server::AcceptConnection());
		//待判断
		int receivedBytes = recv(rtn, recvBuf, Config::BUFFERLENGTH, 0);
		//dlg->m_listbox.AddString(CString(recvBuf));
		//cout << recvBuf;

		ParseRequest(rtn, recvBuf);

	}
	delete[] recvBuf;
	return 0;


}

//分析接受到的数据并发送数据
int Server::ParseRequest(SOCKET sr, char *&recvBuf)
{
	char StatueLine[100] = { 0 };
	char HtmlDirP[256] = "D:/ray/Network/HTML";
	char szStatusCode[20] = { 0 };
	char szStatue[50] = { 0 };
	strcpy(szStatusCode, "200 OK");
	strcpy(HtmlDirP, Config::HTMLDIR.c_str());

	char *p = NULL;
	p = recvBuf;
	int nLen = strlen(recvBuf);		//	请求报文长度
	int n = 0;
	int KeepAlive = 0;

	char *pTemp = strstr(p, "\n");	//第一次换行符地址
	n = pTemp - p;
	memcpy(StatueLine, p, n);
	//cout << StatueLine << endl;

	pTemp = strstr(p, " ");		//	得到第一次出现空格地址
	n = pTemp - p;
	char szMode[100] = { 0 };
	char szFileName[30] = { 0 };
	memcpy(szMode, p, n);		//请求方法

	p = pTemp + 1;

	//	GET方法处理
	if (strcmp(szMode, "GET") == 0)
	{
		pTemp = strstr(p, " ");
		n = pTemp - p;
		memcpy(szFileName, p, n);
		p = pTemp + 1;
		if (strcmp(szFileName, "/") == 0)
		{
			strcpy(szFileName + 1, "index.html");
		}
		if (strstr(szFileName, ".jpg") != NULL || strstr(szFileName, ".jpeg") != NULL)
		{
			char *filename = szFileName;
			PostPicture(sr, filename);
			return KeepAlive;
		}
	}
	else
	{
		//cout << StatueLine << endl;
		closesocket(sr);
		return KeepAlive;
		//strcpy(szStatusCode, "404 Not Found");
	}
	//连接类型
	if (strstr(p, "Connection: keep-alive") != NULL)
		KeepAlive = 1;
	else
		KeepAlive = 0;

	//文本还是图片
	//if(strstr(p,""))

	//定义一个回显头
	char pResponseHeader[512] = { 0 };
	char szContentType[20] = { 0 };

	strcpy(szContentType, "text/html");

	strcpy(szStatue, "Connection: close");

	/*
	if (KeepAlive == 1)
	strcpy(szStatue, "Connection: keep-alive");
	else
	strcpy(szStatue, "Connection: close");
	*/
	char szDT[128];
	struct tm *newtime;
	time_t ltime;
	ltime = time(NULL);
	newtime = localtime(&ltime);
	strftime(szDT, 128, "%a, %d %b %Y %H:%M:%S GMT", newtime);
	//读取文件
	//定义一个文件流指针
	strcat(HtmlDirP, szFileName);
	FILE* fp = fopen(HtmlDirP, "rb");

	fpos_t lengthActual = 0;
	int length = 0;

	char* BufferTemp = NULL;

	if (fp != NULL)
	{
		// 获得文件大小
		fseek(fp, 0, SEEK_END);
		fgetpos(fp, &lengthActual);
		fseek(fp, 0, SEEK_SET);
		//计算出文件的大小后我们进行分配内存
		BufferTemp = (char*)malloc(sizeof(char)*((int)lengthActual) + 2);
		length = fread(BufferTemp, 1, (int)lengthActual, fp);
		//fclose(fp);
		// 返回响应
		sprintf(pResponseHeader, "HTTP/1.1 %s\r\n%s\r\nDate: %s\r\nServer: %s\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n",
			szStatusCode, szStatue, szDT, SERVERNAME, length, szContentType);   //响应报文
																				//sprintf(pResponseHeader, "HTTP/1.1 %s\r\n\r\n",szStatusCode);   //响应报文

		strcpy(szResponse, pResponseHeader);
		strcat(szResponse, BufferTemp);
		free(BufferTemp);

		send(sr, szResponse, strlen(szResponse), 0);
		fclose(fp);
		//cout << StatueLine << endl;

		closesocket(sr);
		return KeepAlive;
	}
	//Accept-Ranges: bytes\r\n
	else
	{
		//BufferTemp = NULL;
		//strcpy(szStatusCode, "404 Not Found");
		//sprintf(pResponseHeader, "HTTP/1.1 %s\r\n%s\r\nDate: %s\r\nServer: %s\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n",
		//	szStatusCode, szStatue, szDT, SERVERNAME, length, szContentType);   //响应报文
		//sprintf(pResponseHeader, "HTTP/1.1 %s\r\n\r\n", szStatusCode);   //响应报文

		//send(sr, pResponseHeader, strlen(pResponseHeader), 0);
		//cout << StatueLine << endl;

		closesocket(sr);
		return KeepAlive;
	}

}

//图片请求
int Server::PostPicture(SOCKET sr, char *&filename)
{
	char Response[10240];
	char pResponseHeader[1024] = { 0 };
	char PicDic[256] = { 0 };
	strcpy(PicDic, Config::HTMLDIR.c_str());
	strcat(PicDic, filename);
	FILE *fp = fopen(PicDic, "rb");

	fpos_t lengthActual = 0;
	int length = 0;



	char* BufferTemp = NULL;
	if (fp != NULL)
	{
		// 获得文件大小
		fseek(fp, 0, SEEK_END);
		fgetpos(fp, &lengthActual);
		fseek(fp, 0, SEEK_SET);
		//计算出文件的大小后我们进行分配内存
		BufferTemp = (char*)malloc(sizeof(char)*((int)lengthActual));
		length = fread(BufferTemp, 1, (int)lengthActual, fp);

		sprintf(pResponseHeader, "HTTP/1.1 200 OK\r\nConnection:close\r\nContent-Length:%d\r\nContent-Type: image/jpeg\r\n\r\n", length);   //响应报文
		strcpy(Response, pResponseHeader);
		strcat(Response, BufferTemp);
		//send(sr, pResponseHeader, strlen(pResponseHeader), 0);
		//send(sr, BufferTemp, strlen((char*)BufferTemp), 0);
		free(BufferTemp);

		send(sr, Response, strlen(Response), 0);
		fclose(fp);
		//cout << StatueLine << endl;

	}

	closesocket(sr);
	return  1;
}



void Server::Set_Server()
{
	cout << "配置IP：";
	cin >> Config::SERVERADDRESS;
	cout << "配置Port:";
	cin >> Config::PORT;
	cout << "配置主目录：";
	cin >> Config::HTMLDIR;
}



void Server::ParseMessage(SOCKET sock) {
	m.lock();
	CServerGUI2Dlg *dlg = (CServerGUI2Dlg *)(AfxGetApp()->GetMainWnd());

	SOCKET clientsocket = (SOCKET)sock;
	char msg[1024];
	int len = recv(clientsocket, msg, 1024, 0);
	msg[len] = 0;
	
	char ptemp[1024];
	strcpy(ptemp, msg);
	char *p;
	p = strtok(msg, " ");
	if (strcmp(p, "GET") == 0)
	{
		char* ptemp1 = strstr(ptemp, "\n");
		int n = ptemp1 - ptemp;
		char StatueLine[100] = { 0 };
		memcpy(StatueLine, ptemp, n);
		dlg->C_List.AddString(CString(StatueLine));

		std::string file;
		file = Config::HTMLDIR;
		p = strtok(NULL, " ");
		if (strcmp(p, "/") == 0)
		{
			file = file + "/index.html";
			strcpy(p, "index.html");
		}
		else if (p[0] == '/')
		{
			file = file + p;
		}
		std::ifstream fin(file, std::ios::binary);
		char sentence[2048] = { 0 };
		int flen, textflag = 0;
		if (fin.good())
		{
			char st[40];
			fin.seekg(0, std::ios::end);
			flen =(int) fin.tellg();
			fin.seekg(0, std::ios::beg);
			strcpy(sentence, "HTTP/1.1 200 OK\r\n");

			//相应报文状态行
			char szRseponse[100] = { 0 };
			strcpy(szRseponse, sentence);

			strcat(sentence, "Content-Length: "); //把两个字符串连接起来
			sprintf(st, "%d", flen);  //格式化输出字符串
			strcat(sentence, st);
			strcat(sentence, "\r\nContent-Type: ");
			std::string type;
			type = p;
			type = type.erase(0, type.find_last_of('.'));
			std::map<std::string, std::string>::iterator ftype;
			ftype = filetypemaps->find(type);
			if (ftype == filetypemaps->end())
			{
				dlg->C_List.AddString(CString("OPEN FILE ERROR!"));

				fin.close();
				return;
			}
			//显示响应报文状态行
			dlg->C_List.AddString(CString(szRseponse));
			m.unlock();

			type = ftype->second;
			strcpy(st, type.c_str());
			strcat(sentence, st);
			strcat(sentence, "\r\n");
			strcat(sentence, "\r\n");
			int s;
			s = send(clientsocket, sentence, strlen(sentence), 0);
			char ft[1024] = { 0 };
			if (textflag == 0)
			{
				while (!fin.eof())
				{
					fin.read(ft, sizeof(ft));
					send(clientsocket, ft, sizeof(ft), 0);
				}
			}
			else
			{
				while (fin.getline(ft, 1024))
				{
					send(clientsocket, ft, strlen(ft), 0);
				}
			}
		}
		else
		{
			string FileDir = "D:/ray/Network/HTML/404.html";
			std::ifstream fin404(FileDir, std::ios::binary);

				//char st[40];
			fin404.seekg(0, std::ios::end);
			flen = (int)fin404.tellg();
			fin404.seekg(0, std::ios::beg);
				//strcpy(sentence, "HTTP/1.1 404 Not Found\r\n");
			sprintf(sentence, "HTTP/1.1 404 Not Found\r\nContent-Length:%d\r\nContent-Type: text/html\r\n\r\n", flen);   //响应报文
			dlg->C_List.AddString(CString("HTTP/1.1 404 Not Found"));

			m.unlock();

			int s;
			s = send(clientsocket, sentence, strlen(sentence), 0);
			char ft[1024] = { 0 };
			if (textflag == 0)
			{
				while (!fin404.eof())
				{
					fin404.read(ft, sizeof(ft));
					send(clientsocket, ft, sizeof(ft), 0);
				}
			}
			else
			{
				while (fin404.getline(ft, 1024))
				{
					send(clientsocket, ft, strlen(ft), 0);
				}
			}
			fin404.close();

			//dlg->C_List.AddString(CString("OPEN FILE ERROR!"));

		}

		if (strstr(p, "Connection: keep-alive") == NULL) closesocket(clientsocket);
		fin.close();
	}
	else
	{
		m.unlock();
		closesocket(clientsocket);
	}
}

void Server::Close()
{
	closesocket(srvSocket);
	WSACleanup();
}
