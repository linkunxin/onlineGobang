#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <WINSOCK2.H>
#include <queue>
using namespace std;

//=========================数据结构==================
enum Status { IDLE, MATCH, POSITION,GAMEOVER ,regret,no_regret,agree_regret , new_game};  //状态码

//=========================全局常量==================
const int BUF_SIZE = 2048;  //缓冲区大小
const int MAX_LINK_NUM = 10;    //最大连接数
const int WAIT_TIME = 10; //客户端等待事件时间，毫秒
const int RECV_TIME = 10; //接受超时时间，毫秒
const int SEND_TIME = 10; //发送超时时间，毫秒
const int port = 12247;
char servIP[] = "";//192.168.1.154
int addrLen = sizeof(SOCKADDR);
const int MATCH_TIME = 10;   //
const int MATCH_TIME_D = 1000;
const char info_fomat[] = "%02d%s";
//=========================全局变量==================

bool m_status =FALSE;
int MATCH_TRIED =MATCH_TIME_D;
int MARCH_RST;  //返回匹配结果
//int chessboard[BUFSIZ][BUFSIZ]; //棋盘


//匹配信息
int match[BUF_SIZE] = {0};    //匹配的客户端
queue <int> match_q;


char sendbur[BUF_SIZE];
char inputbuf[BUF_SIZE];
char recvbuf[BUF_SIZE];

//客户端套接字、时间、地址数组
SOCKET cliSock[MAX_LINK_NUM];   //客户端Socket
WSAEVENT cliEvent[MAX_LINK_NUM];    //客户端事件
SOCKADDR_IN cliAddr[MAX_LINK_NUM];  //客户端地址
int total = 0; //当前已连接客户端数

//========================函数声明==================
//连接和断开、请求对战、更新棋局信息、胜负判定
DWORD WINAPI servEventThread(LPVOID lpParameter);
DWORD WINAPI startMatch(LPVOID lpParameter) ;



int main()
{
    //加载socket库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)  //Socket版本、系统通过它获取版本信息
    {
        cout <<"加载socket库失败"<<endl;
        system("pause");
    }

    //创建Socket
    SOCKET sockSer = socket(AF_INET,SOCK_STREAM,0);    //地址类型、socket类型、协议


    //获取本地IP
	gethostname(servIP, 30);
	HOSTENT *host = gethostbyname(servIP);
	in_addr PcAddr;
	for (int i = 0;;i++)
	{
		char* p = host->h_addr_list[i];
		if (NULL == p)
		{
			break;

		}
		memcpy(&(PcAddr.S_un.S_addr), p, host->h_length);
		strcpy(servIP, inet_ntoa(PcAddr));
	}


    //初始化地址包
    SOCKADDR_IN addrSer = {0}; //结构体
    addrSer.sin_addr.S_un.S_addr = inet_addr(servIP); //这里需要修改
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(port);    //服务和客户端口一致

    //绑定Socket
    bind(sockSer,(SOCKADDR*)&addrSer,sizeof(addrSer));

    //服务段socket绑定事件对象
    WSAEVENT servEvent = WSACreateEvent();
    WSAEventSelect(sockSer,servEvent,FD_ALL_EVENTS);  //所有事件

    cliSock[0] = sockSer;
    cliEvent[0] = servEvent;

    //监听
    listen(sockSer,MAX_LINK_NUM);

    //创建线程
    CloseHandle(CreateThread(NULL,0,servEventThread,(LPVOID)&sockSer,0,0));

    cout <<"服务端开启"<<endl;


    while (1);
    closesocket(sockSer);
    WSACleanup();

}

DWORD WINAPI servEventThread(LPVOID lpParameter)
{
    //对传入的参数还原
    SOCKET servSock = *(SOCKET*)lpParameter;
    //转换成SOCKET*类型再解引用
    while(1)
    {
        for (int i = 0; i < total+1; i++)   //i是客户端索引
        {
            int index = WSAWaitForMultipleEvents(1,&cliEvent[i],false,WAIT_TIME,0);
            index -= WSA_WAIT_EVENT_0;  //发生事件的终端下标
            if (index == WSA_WAIT_TIMEOUT || index == WSA_WAIT_FAILED)
                continue;
            else if (index == 0) //发生事件
            {
                WSANETWORKEVENTS networkEvent;  //结构体
                WSAEnumNetworkEvents(cliSock[i],cliEvent[i],&networkEvent); //查看是什么事件
    //事件选择===========================================
                if (networkEvent.lNetworkEvents & FD_ACCEPT) //与位掩码进行与运算，若产生acceot事件
                {
                    if (networkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
                    {
                        cout <<"连接时错误，代码："<<networkEvent.iErrorCode[FD_ACCEPT_BIT] <<endl;
                        continue;
                    }
                    if(total +1 < MAX_LINK_NUM)
                    {
                        int newIndex = total + 1; //分配给新客户的下标
                        SOCKET sockCli = accept(servSock,(SOCKADDR*)&cliAddr[newIndex],&addrLen);
                        if (sockCli != INVALID_SOCKET)
                        {
                            //设置发送和接受时限
                            //setsockopt(sockCli,SOL_SOCKET,SO_SNDTIMEO)

                            //将新客户端socket加入
                            cliSock[newIndex] = sockCli;

                            //创建新的事件并绑定
                            WSAEVENT newEvent = WSACreateEvent();
                            WSAEventSelect(sockCli,newEvent,FD_CLOSE | FD_READ | FD_WRITE); //位掩码，位运算的方式
                            cliEvent[newIndex] = newEvent;


                            total++;
                            cout<<newIndex<<"IP:"<<inet_ntoa(cliAddr[newIndex].sin_addr)<<endl;

                        }
                    }
                }
                else if (networkEvent.lNetworkEvents & FD_CLOSE)
                {
                    total--;
                    closesocket(cliSock[i]);
                    WSACloseEvent(cliEvent[i]);

                    //顺序删除元素
                    for (int j=i;j<=total;j++)
                    {
                        cliSock[j] = cliSock[j+1];
                        cliEvent[j] = cliEvent[j+1];
                        cliAddr[j] = cliAddr[j+1];
                    }
                    //发送提示消息
                    cout<<"玩家"<<i<<"退出游戏\n";
                    if (match[i]!=0)
                    {

                        match[match[i]] = 0;    //清除
                        match[i] = 0;
                    }
                }
                else if (networkEvent.lNetworkEvents & FD_READ)
                {
                    char buf[BUF_SIZE] = {0};
                    char str[BUF_SIZE] = {0};
                    int status;
                    for (int j=1;j<=total;j++)
                    {
                        int nrecv = recv(cliSock[j],buf,sizeof(buf),0); //nrecv是字节数
                        if (nrecv > 0)
                        {
                            //分事件处理，不知道发过来的是状态码还是具体内容 === 头部加一点信息
                            //**简单处理
                            sscanf(buf,info_fomat,&status,str);
                            if (MATCH == status)
                            {
                                cout <<"玩家"<<j<<"    请求匹配" <<endl;
                                int k = j;
                                CloseHandle(CreateThread(NULL,0,startMatch,(LPVOID)&k,0,0));
                            }
                            else if (POSITION == status)
                            {
                                //转发位置信息
                                cout <<"玩家"<<j<<"    坐标" <<buf <<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if (GAMEOVER == status)
                            {
                                cout <<"玩家"<<j<<"    游戏结束" <<endl;
                            }
                            else if (regret==status)
                            {
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                                cout<<"玩家"<<j<<"    请求悔棋"<<endl;
                            }
                            else if (status == no_regret)
                            {
                                cout<< "玩家"<<match[j]<<" 拒绝对方悔棋"<<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if (status == agree_regret)
                            {
                                cout<< "玩家"<<match[j]<<" 同意对方悔棋"<<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if(status == new_game)
                            {
                                cout<<"玩家"<<j<<"  新开一局"<<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if(status== 8)
                            {
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                                cout<<"玩家"<<j<<":  "<<str<<endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

DWORD WINAPI startMatch(LPVOID lpParameter)  //匹配
{                                //**匹配函数
    int j =*(int*)lpParameter;
    char buf[3];
    if (match_q.size()>0)
    {
        match[j] = match_q.front();
        match[match_q.front()] = j;
        //发送成功消息
        m_status = TRUE;
        MARCH_RST = 1;
        sprintf(buf,"%d",MARCH_RST);
        send(cliSock[j],buf,sizeof(buf),0);
        cout <<"玩家"<<j<<"   匹配成功" <<endl;

        MARCH_RST = -1;
        sprintf(buf,"%d",MARCH_RST);
        send(cliSock[match[j]],buf,sizeof(buf),0);
        cout <<"玩家"<<match[j]<<"   匹配成功" <<endl;
    }
    else
    {
        match_q.push(j); //加入队列
        while (!m_status && MATCH_TRIED--)
        {
            if(match[j] != IDLE)
            {
                break;
            }
            Sleep(MATCH_TIME);
        }
        m_status = FALSE;   //变成默认状态
        MATCH_TRIED = MATCH_TIME_D;     //变成默认
        match_q.pop();  //不管有无匹配，都要退出队列
    }
}
