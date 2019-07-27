#pragma once
#ifndef CHESS_CLI_H_INCLUDED
#define CHESS_CLI_H_INCLUDED
#include <WINSOCK2.H>
#include <stdio.h>
//#include "WuziqiView.h"


//=========================���ݽṹ==================
typedef void fp(int, int,int,char *);
enum Status { IDLE, MATCH, POSITION_,GAMEOVER ,regret,no_regret, agree_regret_num , new_game};  //״̬��


//=========================ȫ�ֳ���==================
#pragma comment(lib,"ws2_32.lib")
const int BUF_SIZE = 2048;
const int port = 12247;
const int DEAF_RECV_TIME = 0;  //Ĭ�ϳ�ʱʱ��
const int RECV_TIME = 10 * 1000;    //���ܳ�ʱʱ��3s

const char info_fomat[] = "%02d%s";

const int tried = 3000;
const int pad = 10;

extern SOCKET sockCli;
extern char pos_buf[BUF_SIZE];
extern char servIPP[] ;

//========================��������==================

bool create_connect();      //���Ӻ���
int match();   //����ƥ��
bool send_pos(int &x, int &y);     //�������ӵ�λ��
bool get_pos(int &x, int &y);
bool send_rst();
void aaa(LPVOID lp);
bool send_regret();
bool send_no_regret();
bool agree_regret();
bool send_new_gmae();
bool send_msg(char ch[100]);



//���̲߳���
DWORD WINAPI recvMsgThread(LPVOID lpParamter);  //������Ϣ
//DWORD WINAPI wait_pos(LPVOID lpParamter);  //������Ϣ




/*
Ҫ��˵��
״̬�룻
0:����ƥ��
1:��������λ��

ͷ����Ϣ��ʽ��%2d%s
*/

#endif // CHESS_CLI_H_INCLUDED
