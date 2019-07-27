#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <WINSOCK2.H>
#include <queue>
using namespace std;

//=========================���ݽṹ==================
enum Status { IDLE, MATCH, POSITION,GAMEOVER ,regret,no_regret,agree_regret , new_game};  //״̬��

//=========================ȫ�ֳ���==================
const int BUF_SIZE = 2048;  //��������С
const int MAX_LINK_NUM = 10;    //���������
const int WAIT_TIME = 10; //�ͻ��˵ȴ��¼�ʱ�䣬����
const int RECV_TIME = 10; //���ܳ�ʱʱ�䣬����
const int SEND_TIME = 10; //���ͳ�ʱʱ�䣬����
const int port = 12247;
char servIP[] = "";//192.168.1.154
int addrLen = sizeof(SOCKADDR);
const int MATCH_TIME = 10;   //
const int MATCH_TIME_D = 1000;
const char info_fomat[] = "%02d%s";
//=========================ȫ�ֱ���==================

bool m_status =FALSE;
int MATCH_TRIED =MATCH_TIME_D;
int MARCH_RST;  //����ƥ����
//int chessboard[BUFSIZ][BUFSIZ]; //����


//ƥ����Ϣ
int match[BUF_SIZE] = {0};    //ƥ��Ŀͻ���
queue <int> match_q;


char sendbur[BUF_SIZE];
char inputbuf[BUF_SIZE];
char recvbuf[BUF_SIZE];

//�ͻ����׽��֡�ʱ�䡢��ַ����
SOCKET cliSock[MAX_LINK_NUM];   //�ͻ���Socket
WSAEVENT cliEvent[MAX_LINK_NUM];    //�ͻ����¼�
SOCKADDR_IN cliAddr[MAX_LINK_NUM];  //�ͻ��˵�ַ
int total = 0; //��ǰ�����ӿͻ�����

//========================��������==================
//���ӺͶϿ��������ս�����������Ϣ��ʤ���ж�
DWORD WINAPI servEventThread(LPVOID lpParameter);
DWORD WINAPI startMatch(LPVOID lpParameter) ;



int main()
{
    //����socket��
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)  //Socket�汾��ϵͳͨ������ȡ�汾��Ϣ
    {
        cout <<"����socket��ʧ��"<<endl;
        system("pause");
    }

    //����Socket
    SOCKET sockSer = socket(AF_INET,SOCK_STREAM,0);    //��ַ���͡�socket���͡�Э��


    //��ȡ����IP
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


    //��ʼ����ַ��
    SOCKADDR_IN addrSer = {0}; //�ṹ��
    addrSer.sin_addr.S_un.S_addr = inet_addr(servIP); //������Ҫ�޸�
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(port);    //����Ϳͻ��˿�һ��

    //��Socket
    bind(sockSer,(SOCKADDR*)&addrSer,sizeof(addrSer));

    //�����socket���¼�����
    WSAEVENT servEvent = WSACreateEvent();
    WSAEventSelect(sockSer,servEvent,FD_ALL_EVENTS);  //�����¼�

    cliSock[0] = sockSer;
    cliEvent[0] = servEvent;

    //����
    listen(sockSer,MAX_LINK_NUM);

    //�����߳�
    CloseHandle(CreateThread(NULL,0,servEventThread,(LPVOID)&sockSer,0,0));

    cout <<"����˿���"<<endl;


    while (1);
    closesocket(sockSer);
    WSACleanup();

}

DWORD WINAPI servEventThread(LPVOID lpParameter)
{
    //�Դ���Ĳ�����ԭ
    SOCKET servSock = *(SOCKET*)lpParameter;
    //ת����SOCKET*�����ٽ�����
    while(1)
    {
        for (int i = 0; i < total+1; i++)   //i�ǿͻ�������
        {
            int index = WSAWaitForMultipleEvents(1,&cliEvent[i],false,WAIT_TIME,0);
            index -= WSA_WAIT_EVENT_0;  //�����¼����ն��±�
            if (index == WSA_WAIT_TIMEOUT || index == WSA_WAIT_FAILED)
                continue;
            else if (index == 0) //�����¼�
            {
                WSANETWORKEVENTS networkEvent;  //�ṹ��
                WSAEnumNetworkEvents(cliSock[i],cliEvent[i],&networkEvent); //�鿴��ʲô�¼�
    //�¼�ѡ��===========================================
                if (networkEvent.lNetworkEvents & FD_ACCEPT) //��λ������������㣬������acceot�¼�
                {
                    if (networkEvent.iErrorCode[FD_ACCEPT_BIT] != 0)
                    {
                        cout <<"����ʱ���󣬴��룺"<<networkEvent.iErrorCode[FD_ACCEPT_BIT] <<endl;
                        continue;
                    }
                    if(total +1 < MAX_LINK_NUM)
                    {
                        int newIndex = total + 1; //������¿ͻ����±�
                        SOCKET sockCli = accept(servSock,(SOCKADDR*)&cliAddr[newIndex],&addrLen);
                        if (sockCli != INVALID_SOCKET)
                        {
                            //���÷��ͺͽ���ʱ��
                            //setsockopt(sockCli,SOL_SOCKET,SO_SNDTIMEO)

                            //���¿ͻ���socket����
                            cliSock[newIndex] = sockCli;

                            //�����µ��¼�����
                            WSAEVENT newEvent = WSACreateEvent();
                            WSAEventSelect(sockCli,newEvent,FD_CLOSE | FD_READ | FD_WRITE); //λ���룬λ����ķ�ʽ
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

                    //˳��ɾ��Ԫ��
                    for (int j=i;j<=total;j++)
                    {
                        cliSock[j] = cliSock[j+1];
                        cliEvent[j] = cliEvent[j+1];
                        cliAddr[j] = cliAddr[j+1];
                    }
                    //������ʾ��Ϣ
                    cout<<"���"<<i<<"�˳���Ϸ\n";
                    if (match[i]!=0)
                    {

                        match[match[i]] = 0;    //���
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
                        int nrecv = recv(cliSock[j],buf,sizeof(buf),0); //nrecv���ֽ���
                        if (nrecv > 0)
                        {
                            //���¼�������֪������������״̬�뻹�Ǿ������� === ͷ����һ����Ϣ
                            //**�򵥴���
                            sscanf(buf,info_fomat,&status,str);
                            if (MATCH == status)
                            {
                                cout <<"���"<<j<<"    ����ƥ��" <<endl;
                                int k = j;
                                CloseHandle(CreateThread(NULL,0,startMatch,(LPVOID)&k,0,0));
                            }
                            else if (POSITION == status)
                            {
                                //ת��λ����Ϣ
                                cout <<"���"<<j<<"    ����" <<buf <<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if (GAMEOVER == status)
                            {
                                cout <<"���"<<j<<"    ��Ϸ����" <<endl;
                            }
                            else if (regret==status)
                            {
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                                cout<<"���"<<j<<"    �������"<<endl;
                            }
                            else if (status == no_regret)
                            {
                                cout<< "���"<<match[j]<<" �ܾ��Է�����"<<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if (status == agree_regret)
                            {
                                cout<< "���"<<match[j]<<" ͬ��Է�����"<<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if(status == new_game)
                            {
                                cout<<"���"<<j<<"  �¿�һ��"<<endl;
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                            }
                            else if(status== 8)
                            {
                                send(cliSock[match[j]],buf,sizeof(buf),0);
                                cout<<"���"<<j<<":  "<<str<<endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

DWORD WINAPI startMatch(LPVOID lpParameter)  //ƥ��
{                                //**ƥ�亯��
    int j =*(int*)lpParameter;
    char buf[3];
    if (match_q.size()>0)
    {
        match[j] = match_q.front();
        match[match_q.front()] = j;
        //���ͳɹ���Ϣ
        m_status = TRUE;
        MARCH_RST = 1;
        sprintf(buf,"%d",MARCH_RST);
        send(cliSock[j],buf,sizeof(buf),0);
        cout <<"���"<<j<<"   ƥ��ɹ�" <<endl;

        MARCH_RST = -1;
        sprintf(buf,"%d",MARCH_RST);
        send(cliSock[match[j]],buf,sizeof(buf),0);
        cout <<"���"<<match[j]<<"   ƥ��ɹ�" <<endl;
    }
    else
    {
        match_q.push(j); //�������
        while (!m_status && MATCH_TRIED--)
        {
            if(match[j] != IDLE)
            {
                break;
            }
            Sleep(MATCH_TIME);
        }
        m_status = FALSE;   //���Ĭ��״̬
        MATCH_TRIED = MATCH_TIME_D;     //���Ĭ��
        match_q.pop();  //��������ƥ�䣬��Ҫ�˳�����
    }
}
