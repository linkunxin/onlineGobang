#include "chess_cli.h"
#include "stdafx.h"
#include <iostream>
using namespace std;

//=========================全局变量量==================
SOCKET sockCli;
char servIPP[] = { 0 };
char pos_buf[BUF_SIZE] = { 0 };
char rcv_buf[BUF_SIZE] = { 0 };
char send_buf[BUF_SIZE] = { 0 };
int status;
//int x, y;    //位置坐标
int pos[3];

bool create_connect()   //创建连接
{
	//加载socket库
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)  //Socket版本、系统通过它获取版本信息
	{
		system("pause");
	}

	//创建Socket
	sockCli = socket(AF_INET, SOCK_STREAM, 0);    //地址类型、socket类型、协议

	//获取本地IP
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



	//初始化服务器地址
	SOCKADDR_IN addrSer = { 0 };
	addrSer.sin_addr.S_un.S_addr = inet_addr(servIPP); //这里需要修改
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(port);    //服务和客户端口一致

	//连接服务器
	int tried = 3;
	while (tried--)
	{
		if (connect(sockCli, (SOCKADDR*)&addrSer, sizeof(addrSer)) != SOCKET_ERROR)
			return true;

	}
	return false;
}

int match()   //请求匹配
{
	setsockopt(sockCli, SOL_SOCKET, SO_RCVTIMEO, (char*)&RECV_TIME, sizeof(RECV_TIME));
	char buff[3], buff_rcv[3];	//可能-1需要三个字节
	sprintf(buff, "%d", MATCH);  //整形存放到字符串
	send(sockCli, buff, sizeof(buff), 0);
	int c = recv(sockCli, buff_rcv, sizeof(buff_rcv), 0);
	setsockopt(sockCli, SOL_SOCKET, SO_RCVTIMEO, (char*)&DEAF_RECV_TIME, sizeof(DEAF_RECV_TIME));   //不超时
	if (c > 0)
		if (!strcmp(buff_rcv, "1"))
		{
			//cout << "连接成功";
			return 1;//黑棋
		}
		else if (!strcmp(buff_rcv, "-1"))
		{
			return -1;//白棋
		}
	return 0;   //匹配失败
}


DWORD WINAPI recvMsgThread(LPVOID lpParamter)
{
	int x, y;    //位置坐标,会不会是这为什么关坐标的事，他找不到坐标？。
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
				draw_pos(x, y,0, pos_buf);	//这里就是画图的，这样应该就炸了				
			}
			else if (status == regret)
			{
				draw_pos(x, y, regret, pos_buf);//悔棋
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
		else if (nrecv < 0 && GetLastError() == 10060)    //&&(errno == EAGAIN||errno == EWOULDBLOCK||errno == EINTR) **留着以后
		{
			//cout << nrecv<<"与服务器断开连接" << endl;
			break;
		}
	}
	return 0;
}

bool send_pos(int &x, int &y)    //发送棋子的位置
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
		Sleep(pad); //等10毫秒
	}
	return TRUE;
}



bool send_rst()
{
	char buff[3], buff_rcv[3];	//可能-1需要三个字节
	sprintf(buff, "%d", GAMEOVER);  //整形存放到字符串
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