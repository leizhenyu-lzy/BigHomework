#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <omp.h>
#include "MySort.h"

using namespace std;
#pragma comment(lib,"ws2_32.lib")
float* a=NULL;

int main(int argv, char* argc[])
{
    a = new float [DATANUM];
    RandInitial(&a);

	//初始化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

    ParallelQuickSort(a,0,DATANUM-1);//闭区间
    cout<<"sort completed"<<endl;
#pragma omp parallel for 
	for(int i=0;i<DATANUM/BATCHSIZE;i++)//每次传输BTACHSIZE个float
	{
        //创建套接字
        SOCKET clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        //向服务器发送消息
        struct sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));			//每个字节都用0填充
        sockAddr.sin_family = PF_INET;	
        sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        //sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.215");

        sockAddr.sin_port = htons(8888);
        connect(clntSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));

        char toSend='f';
        send(clntSock, (char*)&toSend, 1, 0);    
        char message;
        recv(clntSock, (char*)&message, 4, 0);	
        if(message=='Y')
        {
            //创建套接字
            SOCKET clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

            //向服务器发送消息
            struct sockaddr_in sockAddr;
            memset(&sockAddr, 0, sizeof(sockAddr));			//每个字节都用0填充
            sockAddr.sin_family = PF_INET;	
            sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
            //sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.215");

            sockAddr.sin_port = htons(8889+i);
            connect(clntSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));            
            char *str=(char*)(a+i*BATCHSIZE);
            send(clntSock, str, BATCHSIZE*sizeof(float), 0);   
        }
	}
    //创建套接字
    SOCKET clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发送消息
    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));			//每个字节都用0填充
    sockAddr.sin_family = PF_INET;	
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.215");

    sockAddr.sin_port = htons(8888);
    connect(clntSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));

    char toSend='p';
    send(clntSock, (char*)&toSend, 1, 0);   

	system("pause");

    delete[] a;

	return 0;
}
// g++ client.cpp -lwsock32 -o client
