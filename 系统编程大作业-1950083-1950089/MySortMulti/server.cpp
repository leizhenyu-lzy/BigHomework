#include <WinSock2.h>
#include "MySort.h"

using namespace std;  

#pragma comment(lib,"ws2_32.lib")		//加载ws2_32.dll

float *ClientData = NULL;
float *ServerData = NULL;
float *AllData = NULL;
int ClientFinishFlag=0;
int Port=8887;

clock_t ClockStart, ClockEnd;
double SortAccelerate=0.0;
double ParallelSortUsage=0.0;
double SerialSortUsage=0.0;

void createNewSock(int port)
{
	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//绑定套接字
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));		//每个字节用0填充
	sockAddr.sin_family = PF_INET;				//使用ipv4
	//sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.215");
	sockAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(port);			//端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	listen(servSock, 20);

	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	int arrPos=port-8889;
	int tag=0;
	while(tag==0)
	{
		tag=recv(clntSock, (char*)(ServerData+arrPos*BATCHSIZE), 4*BATCHSIZE, 0);
		//send(clntSock,(char*)(&i),sizeof(int),0);	
	}
	//cout<<"port "<<port<<" end:"<<"last num:"<<*(float*)(ServerData+arrPos*1000+999)<<endl;	
	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);
}

void createMainSock(int port)
{
	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//绑定套接字
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));		//每个字节用0填充
	sockAddr.sin_family = PF_INET;				//使用ipv4
	//sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.215");
	sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(port);			//端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	listen(servSock, 20);

	while(true)
	{
		SOCKADDR clntAddr;
		int nSize = sizeof(SOCKADDR);
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		char signal;
		recv(clntSock, (char*)&signal, 1, 0);
		if(signal=='f')
		{
    		thread task(createNewSock,++Port);  
			task.detach();  
			char message='Y';
			//if(Port/2000==0)
			cout<<"port "<<Port<<" created"<<endl;
			send(clntSock,(char*)(&message),5,0);			
		}	
		else
		{
			// char message='N';
			// send(clntSock,(char*)(&message),5,0);
			ClientFinishFlag=1;//标志传输结束
			break;		
		}
		//关闭套接字
		closesocket(clntSock);	
	}
}

int main()  
{  
	cout << HALFSTARS << "MultiPC Sort Test Start" << HALFSTARS << endl << endl;

	ClientData=new float[DATANUM]{0};
	ServerData=new float[DATANUM]{0};
	AllData=new float[2*DATANUM]{0};
	RandInitial(&ServerData);//只初始化一半
	ClockStart=clock();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	thread mainTask(createMainSock,++Port);
	mainTask.detach();
	ParallelQuickSort(ServerData,0,DATANUM-1);//闭区间

	cout<<"Server quick sort finished"<<endl<<endl;

	while(!ClientFinishFlag);//在客户端没有将结果传完之前不进行合并工作
	cout<<"Receive all data from client."<<endl<<endl;

	ClockEnd=clock();
	ParallelSortUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;

	ServerClientMerge(AllData,ServerData,ClientData,DATANUM,DATANUM);
	int Check=CheckAllResult(AllData,2*DATANUM);


	if(Check==1)
	{
		cout<<"Correct Sort Result"<<endl;
		cout << endl << "MultiPC Sort Usage:" << ParallelSortUsage << "ms" << endl << endl;
	}
	else
	{
		cout<<"Wrong Sort Result"<<endl<<endl;
	}

	cout << HALFSTARS << "MultiPC Sort Test Finished." << HALFSTARS << endl << endl;

	//终止dll使用
	WSACleanup();
	delete[] ClientData;
	delete[] ServerData;
	delete[] AllData;

	system("pause");
    return 0;
}  
// g++ server.cpp -lwsock32 -o server


