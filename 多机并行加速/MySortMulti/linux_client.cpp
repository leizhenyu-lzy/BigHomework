#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"omp.h"
#include<arpa/inet.h>
#include<sys/socket.h>
#include"iostream"
#include "MySort.h"
using namespace std;

int main(int argv, char* argc[])
{
    a = new float [DATANUM];
    RandInitial(&a);
    ParallelQuickSort(a,0,DATANUM-1);//闭区间
    cout<<"sort completed"<<endl;
#pragma omp parallel for 
	for(int i=0;i<DATANUM/BATCHSIZE;i++)//每次传输BATCHSIZE个float
	{
        //创建套接字
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        //服务器的ip为本地，端口号8888
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("192.168.43.215");
        serv_addr.sin_port = htons(8888);

        char toSend='f';
        write(sock, (char*)&toSend, 1, 0);    
        char message;
        read(sock, (char*)&message, 4, 0);	
        if(message=='Y')
        {
            //创建套接字
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in serv_addr;
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = inet_addr("192.168.43.215");
            serv_addr.sin_port = htons(8889+i);
            //向服务器发送数据
            connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));         
            char *str=(char*)(a+i*BATCHSIZE);
            write(sock, str, BATCHSIZE*sizeof(int)); 
        }
	}
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.43.215");
    serv_addr.sin_port = htons(8888);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));         

    //传输完成message
    char toSend='p';
    write(clntSock, (char*)&toSend, 1, 0);   
    delete[] a;

	return 0;
}