#pragma once
#include <string>
#include <winsock2.h>
using std::string;
//����������Ϣ
class Config
{
public:
	static const int MAXCONNECTION;		//���������
	static const int BUFFERLENGTH;		//��������С
	static string SERVERADDRESS;  //��������ַ
	static int PORT;				//�������˿�
	static const u_long BLOCKMODE;			//SOCKET����ģʽ
	static string HTMLDIR;			//��Ŀ¼
private:
	Config(void);
	~Config(void);
};
