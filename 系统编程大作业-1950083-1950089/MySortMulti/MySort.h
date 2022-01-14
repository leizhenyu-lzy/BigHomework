/* 1950083 刘智宇 自动化*/
#pragma once

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <chrono>
#include <thread>
#include <vector>
#include <omp.h>

using namespace std;

#define MAX_THREADS 64
#define SUBDATANUM 1000000//2000000
#define DATANUM (SUBDATANUM * MAX_THREADS)   /*这个数值是总数据量*/
#define BATCHSIZE 100000

#define FloatPrecision 4
#define FloatWidth 6

#define MinMergeLen 16//当数组长度小于此值，将使用插入排序替换归并排序
#define QuickSortMinPatch 500000//当快速排序长度小于此值时，不再进行线程划分


const char STARS[11] = "**********";
const char HALFSTARS[6] = "*****";
const int RandSeed = 1950083;
unsigned long useless=0UL;

//为了模拟任务：每次访问数据时，用log(sqrt(rawFloatData[i]))进行访问！
//就是说比如计算加法 用 sum += log(sqrt(rawFloatData[i])), 而不是sum += rawFloatData[i] !!。
//这里计算结果和存储精度之间有损失，但你们机器的指令集限制，
//如果使用SSE中的double型的话，单指令只能处理4个double，如果是float则可以8个。所以用float加速比会更大。
float AccessData(float rawdata);
int RandInitial(float* list[], const int numoflists = 1, const int datalen = DATANUM, const unsigned int RandSeed = 1950083);
int DispList(const float* list, const int len = 0, const char* prompt = "List Data:");
int CompTwoLists(const float* list1, const float* list2, const int len = 0);//返回不同的数的个数，没问题则返回0
int SerialMergeSort(float* Slist, const int left = 0, const int right = 0);
int SerialMerge(float* Slist, const int left = 0, const int right = 0);
int SerialInsertSort(float* Slist, const int left = 0, const int right = 0);
int SerialBubbleSort(float* Slist, const int left = 0, const int right = 0);
int ParallelMergeSort(float* Slist, const int left = 0, const int right = 0);
int ParallelMerge(float* Slist, const int left = 0, const int right = 0);
int SerialQuickSort(float* list, const int left = 0, const int right = 0);
int ParallelQuickSort(float* list, const int left = 0, const int right = 0);
int QuickSortPartition(float* list, const int left = 0, const int right = 0);
int CheckAllResult(float* list, const int len = 0);

//待测试数据定义为：
//float rawFloatData[DATANUM];

//参照下列数据初始化代码：两台计算机可以分开初始化，减少传输代价
//for (size_t i = 0; i < DATANUM; i++)//数据初始化
//{
//	rawFloatData[i] = float(i + 1);
//}

//不加速版本，为同样的数据量在两台计算机上各自运行的时间。算法一致，只是不采用任何加速手段（SSE，多线程或者OPENMP)
float SerialSum(const float data[], const int len); //data是原始数据，len为长度。结果通过函数返回
float SerialMax(const float data[], const int len);//data是原始数据，len为长度。结果通过函数返回
float SerialSort(const float data[], const int len, float  result[]);//data是原始数据，len为长度。排序结果在result中。

//双机加速版本
float ParallelSum(const float data[], const int len); //data是原始数据，len为长度。结果通过函数返回
float ParallelMax(const float data[], const int len);//data是原始数据，len为长度。结果通过函数返回
float ParallelSort(const float data[], const int len, float result[]);//data是原始数据，len为长度。排序结果在result中。
//加速中如果使用SSE，特别注意SSE的指令和数据长度有关，单精度后缀ps, 双精度后缀pd。

//测试速度的代码框架为：
//QueryPerformanceCounter(&start);//start  
//你的函数(...);//包括任务启动，结果回传，收集和综合
//QueryPerformanceCounter(&end);//end
//std::cout << "Time Consumed:" << (end.QuadPart - start.QuadPart) << endl;
//cout << 输出求和结果 << endl;
//cout << 输出最大值 << endl;
//cout << 输出排序是否正确 << endl;

//如果单机上那么大数据量无法计算，可以只算一半。
//修改 #define SUBDATANUM 2000000 为 #define SUBDATANUM 1000000做单机计算。
//双机上每个计算机都申请#define SUBDATANUM 1000000大的数据，即实现#define SUBDATANUM 2000000的运算。

//函数实现

//left有数，right没有数(超索引范围)，左闭右开
int SerialMergeSort(float* Slist, const int left, const int right)
{
	if (right - left > 1)//MinMergeLen)
	{
		int middle = (left + right + 1) / 2;
		SerialMergeSort(Slist, left, middle);
		SerialMergeSort(Slist, middle, right);
		SerialMerge(Slist, left, right);
	}
	//else
	//{
	//	SerialBubbleSort(Slist, left, right);//冒泡排序不用merge
	//}
	return 0;
}

//return -1说明申请失败，return 0表示没问题
int SerialMerge(float* Slist, const int left, const int right)
{
	int middle = (left + right + 1) / 2 - left;
	int LP = 0;
	int RP = middle;
	int store_pos = left;
	int len = right - left;
	float leftvalue = 0.0f, rightvalue = 0.0f;
	float* Olist = new float[len];
	if (Olist == NULL)
	{
		cout << "Bad Allocate : TempList." << endl;
		return -1;
	}

	for (int cnt = 0; cnt < len; cnt++)
	{
		Olist[cnt] = Slist[left + cnt];
	}

	while (LP < middle || RP < len)
	{
		if (RP >= len)
		{
			Slist[store_pos++] = Olist[LP++];
		}
		else if (LP >= middle)
		{
			Slist[store_pos++] = Olist[RP++];
		}
		else if (LP < middle && RP < len)
		{
			leftvalue = AccessData(Olist[LP]);
			rightvalue = AccessData(Olist[RP]);
			if (leftvalue < rightvalue)
				Slist[store_pos++] = Olist[LP++];
			else //(rightvalue < leftvalue)
				Slist[store_pos++] = Olist[RP++];
		}
	}

	delete[] Olist;
	return 0;
}


int SerialInsertSort(float* Slist, const int left, const int right)
{
	return 0;
}

int SerialBubbleSort(float* Slist, const int left, const int right)
{
	const int len = right - left;
	float tempfloat = 0.0f;
	for (int cnti = 0; cnti < len - 1; cnti++)
	{
		for (int cntj = left; cntj < right - 1 - cnti; cntj++)
		{
			if (AccessData(Slist[cntj]) > AccessData(Slist[cntj + 1]))
			{
				tempfloat = Slist[cntj];
				Slist[cntj] = Slist[cntj + 1];
				Slist[cntj + 1] = tempfloat;
			}
		}
	}
	return 0;
}

//格式化输出输出列表的数据
int DispList(const float* list, const int len, const char* prompt)
{
	cout << prompt;
	for (int cnt = 0; cnt < len; cnt++)
	{
		if (cnt % 10 == 0)
			cout << endl;
		cout << setiosflags(ios::left) << setw(FloatWidth) << setprecision(FloatPrecision) << list[cnt] << " ";
	}
	cout << endl;
	return len;
}

//模拟获取data的过程
float AccessData(const float rawdata)
{
	return log(sqrt(rawdata));
}

//随机初始化数组
int RandInitial(float* list[], const int numoflists, const int len, const unsigned int RandSeed)
{
	float tempnum = 0.0f;
	srand(RandSeed);
	for (int cnt = 0; cnt < len; cnt++)
	{
		tempnum = 1.0f + static_cast<float>(rand() % 100) + static_cast<float>(rand()) / (0.0f + RAND_MAX);
		for (int SetNo = 0; SetNo < numoflists; SetNo++)
		{
			list[SetNo][cnt] = tempnum;
		}
	}
	return RandSeed;
}

//将两个列表进行比较，return不同数的数量
int CompTwoLists(const float* list1, const float* list2, const int len)
{
	int diff_num = 0;
	for (int cnt = 0; cnt < len; cnt++)
	{
		if (AccessData(list1[cnt]) != AccessData(list2[cnt]))
			diff_num++;
	}
	return diff_num;
}


//快速排序
int QuickSortPartition(float* list, const int left, const int right)
{
	int i = left;
	int j = right + 1;
	float x = list[left];
	float Datax = AccessData(x);

	while (1)
	{
		while (AccessData(list[++i]) < Datax)
		{
			if (i >= right)
				break;
		}
		while (AccessData(list[--j]) > Datax);
		if (i >= j)
			break;
		float temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}
	list[left] = list[j];
	list[j] = x;
	return j;
}

int SerialQuickSort(float* list, const int left, const int right)
{
	if (left >= right)
		return -1;
	int middle = QuickSortPartition(list, left, right);
	SerialQuickSort(list, left, middle - 1);
	SerialQuickSort(list, middle + 1, right);
	return middle;
}

int ParallelQuickSort(float* list, const int left, const int right)
{
	if (left >= right)
		return -1;
	int middle = QuickSortPartition(list, left, right);
	vector<thread> POOL;
	if (middle - 1 - left + 1 >= QuickSortMinPatch)
	{
		POOL.push_back(thread(ParallelQuickSort, list, left, middle - 1));
	}
	else
	{
		SerialQuickSort(list, left, middle - 1);
	}

	if ((right - (middle + 1) + 1) >= QuickSortMinPatch)
	{
		POOL.push_back(thread(ParallelQuickSort, list, middle + 1, right));
	}
	else
	{
		SerialQuickSort(list, middle + 1, right);
	}

	for (unsigned i = 0; i < POOL.size(); ++i)
	{
		POOL[i].join();
	}
	return middle;
}

int CheckAllResult(float* list, const int len)
{
    int result=1;
    for(int i=0;i<len-1;i++)
    {
        if(AccessData(list[i])>AccessData(list[i+1]))
        {
            result=0;
            break;
        }
    }
    return result;
}

int ServerClientMerge(float* all,float* server,float* client,int serverlen,int clientlen)
{
    int allptr=0;
    int serverptr=0;
    int clientptr=0;
    
    while(serverptr<serverlen||clientptr<clientlen)
    {
        if(serverptr==serverlen)
        {
            all[allptr++]=client[clientptr++];
        }
        else if(clientptr==clientlen)
        {
            all[allptr++]=server[serverptr++];
        }
        else
        {
            float serverdata=AccessData(server[serverptr]);
            float clientdata=AccessData(client[clientptr]);
            if(serverdata<=clientdata)
            {
                all[allptr++]=server[serverptr++];
            }
            else
            {
                all[allptr++]=client[clientptr++];
            }
        }
    }
    return 0;
}
