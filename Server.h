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


//������

class Server
{
private:
	SOCKET srvSocket;			//������socket
								//char* recvBuf;				//���ܻ�����
	fd_set rfds;				//���ڼ��socket�Ƿ������ݵ����ĵ��ļ�������������socket������ģʽ�µȴ������¼�֪ͨ�������ݵ�����
	fd_set wfds;				//���ڼ��socket�Ƿ���Է��͵��ļ�������������socket������ģʽ�µȴ������¼�֪ͨ�����Է������ݣ�
	sockaddr_in srvAddr;		//��������IP��ַ
	vector<SOCKET> conns;		//�Ựsocket����
	vector<int> conns_manage;	//	socket״̬
	int conns_max;				//���Ự��
								//char HtmlDir[512];			//��Ŀ¼
	char szResponse[256 * 10];   //��Ӧ����
	int sockt_run;

	/*
	list<SOCKET> *sessions;							//��ǰ�ĻỰsocket����
	list<SOCKET> *closedSessions;					//������ʧЧ�ĻỰsocket����
	list<string> *rcvedMessages;					//�ѽ��յ��Ŀͻ�����Ϣ����
	int numOfSocketSignaled;						//�ɶ���д������������socket����
	*/
	map<SOCKET, string> *clientAddrMaps;				//�ͻ��˵�ַ�б���ַ��(key,value)����ʽ���棬keyΪSOCKET���ͣ�valueΪstring����

protected:
	virtual string  GetClientAddress(SOCKET s);								//�õ��ͻ���IP��ַ


public:
	Server(void);
	virtual ~Server(void);
	virtual int WinsockStartup();		//��ʼ��Winsock
	virtual int ServerStartup();		//��ʼ��Server����������SOCKET���󶨵�IP��PORT
	virtual int ListenStartup();		//��ʼ�����ͻ�������

	int Loop(SOCKET rtn);					//ѭ��ִ��"�ȴ��ͻ�������"->���������ͻ�ת����Ϣ��->"�ȴ��ͻ�����Ϣ"

public:
	void AcceptConnection();					//���ӻỰ
	virtual int ParseRequest(SOCKET sr, char *&recvBuf);						//�������ܵ������ݲ���������
	virtual int PostPicture(SOCKET sr, char *&recvBuf);		//ͼƬ����
	void Set_Server();					//���ø���

	//void handle_accept(SOCKET socket_feed);
	void Close();
	void ParseMessage(SOCKET sock);

};



