#pragma once
#ifndef CHESS_CLI_H_INCLUDED
#define CHESS_CLI_H_INCLUDED
#include <WINSOCK2.H>
#include <stdio.h>
//#include "WuziqiView.h"


//=========================数据结构==================
typedef void fp(int, int,int,char *);
enum Status { IDLE, MATCH, POSITION_,GAMEOVER ,regret,no_regret, agree_regret_num , new_game};  //状态码


//=========================全局常量==================
#pragma comment(lib,"ws2_32.lib")
const int BUF_SIZE = 2048;
const int port = 12247;
const int DEAF_RECV_TIME = 0;  //默认超时时间
const int RECV_TIME = 10 * 1000;    //接受超时时间3s

const char info_fomat[] = "%02d%s";

const int tried = 3000;
const int pad = 10;

extern SOCKET sockCli;
extern char pos_buf[BUF_SIZE];
extern char servIPP[] ;

//========================函数声明==================

bool create_connect();      //连接函数
int match();   //请求匹配
bool send_pos(int &x, int &y);     //发送棋子的位置
bool get_pos(int &x, int &y);
bool send_rst();
void aaa(LPVOID lp);
bool send_regret();
bool send_no_regret();
bool agree_regret();
bool send_new_gmae();
bool send_msg(char ch[100]);



//多线程部分
DWORD WINAPI recvMsgThread(LPVOID lpParamter);  //接受消息
//DWORD WINAPI wait_pos(LPVOID lpParamter);  //接受消息




/*
要点说明
状态码；
0:请求匹配
1:发送棋子位置

头部信息格式：%2d%s
*/

#endif // CHESS_CLI_H_INCLUDED
