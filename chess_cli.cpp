#include "chess_cli.h"
#include "stdafx.h"
#include <iostream>
using namespace std;

//=========================ȫ�ֱ�����==================
SOCKET sockCli;
char servIPP[] = { 0 };
char pos_buf[BUF_SIZE] = { 0 };
char rcv_buf[BUF_SIZE] = { 0 };
char send_buf[BUF_SIZE] = { 0 };
int status;
//int x, y;    //λ������
int pos[3];

bool create_connect()   //��������
{
	//����socket��
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)  //Socket�汾��ϵͳͨ������ȡ�汾��Ϣ
	{
		system("pause");
	}

	//����Socket
	sockCli = socket(AF_INET, SOCK_STREAM, 0);    //��ַ���͡�socket���͡�Э��

	//��ȡ����IP
	gethostname(servIPP, 30);
	HOSTENT *host = gethostbyname(servIPP);
	in_addr PcAddr;
	for (int i = 0;;i++)
	{
		char* p = host->h_addr_list[i];
		if (NULL == p)
		{
			break;

		}
		memcpy(&(PcAddr.S_un.S_addr), p, host->h_length);
		strcpy(servIPP, inet_ntoa(PcAddr));
	}



	//��ʼ����������ַ
	SOCKADDR_IN addrSer = { 0 };
	addrSer.sin_addr.S_un.S_addr = inet_addr(servIPP); //������Ҫ�޸�
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(port);    //����Ϳͻ��˿�һ��

	//���ӷ�����
	int tried = 3;
	while (tried--)
	{
		if (connect(sockCli, (SOCKADDR*)&addrSer, sizeof(addrSer)) != SOCKET_ERROR)
			return true;

	}
	return false;
}

int match()   //����ƥ��
{
	setsockopt(sockCli, SOL_SOCKET, SO_RCVTIMEO, (char*)&RECV_TIME, sizeof(RECV_TIME));
	char buff[3], buff_rcv[3];	//����-1��Ҫ�����ֽ�
	sprintf(buff, "%d", MATCH);  //���δ�ŵ��ַ���
	send(sockCli, buff, sizeof(buff), 0);
	int c = recv(sockCli, buff_rcv, sizeof(buff_rcv), 0);
	setsockopt(sockCli, SOL_SOCKET, SO_RCVTIMEO, (char*)&DEAF_RECV_TIME, sizeof(DEAF_RECV_TIME));   //����ʱ
	if (c > 0)
		if (!strcmp(buff_rcv, "1"))
		{
			//cout << "���ӳɹ�";
			return 1;//����
		}
		else if (!strcmp(buff_rcv, "-1"))
		{
			return -1;//����
		}
	return 0;   //ƥ��ʧ��
}


DWORD WINAPI recvMsgThread(LPVOID lpParamter)
{
	int x, y;    //λ������,�᲻������Ϊʲô��������£����Ҳ������ꣿ��
	fp* draw_pos;
	draw_pos = (fp*)lpParamter;
	while (1)
	{
		int nrecv = recv(sockCli, rcv_buf, sizeof(rcv_buf), 0);
		if (nrecv > 0)
		{
			sscanf(rcv_buf, info_fomat, &status, pos_buf);
			if (status == POSITION_)
			{
				sscanf(pos_buf, "%04d%04d", &x, &y);
               /*if (pos[0] == 0)
				{
					pos[0] = 1;
					pos[1] = x;
					pos[2] = y;
				}*/	
				//Sleep(30000);
				draw_pos(x, y,0, pos_buf);	//������ǻ�ͼ�ģ�����Ӧ�þ�ը��				
			}
			else if (status == regret)
			{
				draw_pos(x, y, regret, pos_buf);//����
			}
			else if (status == no_regret)
			{
				draw_pos(x, y, no_regret, pos_buf);
			}
			else if (status == agree_regret_num)
			{
				draw_pos(x, y, agree_regret_num, pos_buf);
			}
			else if (status == new_game)
			{
				draw_pos(x, y, new_game, pos_buf);
			}
			else if (status == 8)
			{
				draw_pos(1500, 1500, 8, pos_buf);
			}

		}
		else if (nrecv < 0 && GetLastError() == 10060)    //&&(errno == EAGAIN||errno == EWOULDBLOCK||errno == EINTR) **�����Ժ�
		{
			//cout << nrecv<<"��������Ͽ�����" << endl;
			break;
		}
	}
	return 0;
}

bool send_pos(int &x, int &y)    //�������ӵ�λ��
{
	sprintf(send_buf, "%02d%04d%04d", POSITION_, x, y);
	send(sockCli, send_buf, sizeof(send_buf), 0);
	return true;
}


bool get_pos(int &x, int &y)
{
	int tired = tried;
	while (tired--)
	{
		if (pos[0] == 1)
		{
			x = pos[1];
			y = pos[2];
			pos[0] = 0;
			break;
		}
		Sleep(pad); //��10����
	}
	return TRUE;
}



bool send_rst()
{
	char buff[3], buff_rcv[3];	//����-1��Ҫ�����ֽ�
	sprintf(buff, "%d", GAMEOVER);  //���δ�ŵ��ַ���
	send(sockCli, buff, sizeof(buff), 0);
	return 0;
}

bool send_regret()
{
	char buf_regret[3];
	sprintf(buf_regret, "%02d", regret);
	send(sockCli, buf_regret, sizeof(buf_regret), 0);
	return 0;
}

bool send_no_regret()
{
	char buf_no_regret[3];
	sprintf(buf_no_regret, "%02d", no_regret);
	send(sockCli, buf_no_regret, sizeof(buf_no_regret), 0);
	return 0;
}

bool agree_regret()
{
	char buf_no_regret[3];
	sprintf(buf_no_regret, "%02d", agree_regret_num);
	send(sockCli, buf_no_regret, sizeof(buf_no_regret), 0);
	return 0;
}


bool send_new_gmae()
{
	char buf_new_game[3];
	//char* buf_new_game="";

	sprintf(buf_new_game, "%02d", new_game);
	send(sockCli, buf_new_game, sizeof(buf_new_game), 0);

	return 0;
}

bool send_msg(char ch[100])
{
	//char* buff="";
	char buff[1000];
	int a = 8;
	sprintf(buff, "%2d%s", a,ch);
	send(sockCli, buff, sizeof(buff), 0);

	return 0;
}