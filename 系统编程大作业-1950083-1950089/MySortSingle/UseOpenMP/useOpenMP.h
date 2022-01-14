/* 1950083 ������ �Զ���*/
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

using namespace std;

#define MAX_THREADS 64
#define SUBDATANUM 1000000//2000000
#define DATANUM (SUBDATANUM * MAX_THREADS)   /*�����ֵ����������*/

#define FloatPrecision 4
#define FloatWidth 6

#define MinMergeLen 16//�����鳤��С�ڴ�ֵ����ʹ�ò��������滻�鲢����
#define QuickSortMinPatch 500000//���������򳤶�С�ڴ�ֵʱ�����ٽ����̻߳���


const char STARS[11] = "**********";
const char HALFSTARS[6] = "*****";
const int RandSeed = 1950083;

//Ϊ��ģ������ÿ�η�������ʱ����log(sqrt(rawFloatData[i]))���з��ʣ�
//����˵�������ӷ� �� sum += log(sqrt(rawFloatData[i])), ������sum += rawFloatData[i] !!��
//����������ʹ洢����֮������ʧ�������ǻ�����ָ����ƣ�
//���ʹ��SSE�е�double�͵Ļ�����ָ��ֻ�ܴ���4��double�������float�����8����������float���ٱȻ����
float AccessData(float rawdata);
int RandInitial(float* list[], const int numoflists = 1, const int datalen = DATANUM, const unsigned int RandSeed = 1950083);
int DispList(const float* list, const int len = 0, const char* prompt = "List Data:");
int CompTwoLists(const float* list1, const float* list2, const int len = 0);//���ز�ͬ�����ĸ�����û�����򷵻�0
int SerialMergeSort(float* Slist, const int left = 0, const int right = 0);
int SerialMerge(float* Slist, const int left = 0, const int right = 0);
int SerialInsertSort(float* Slist, const int left = 0, const int right = 0);
int SerialBubbleSort(float* Slist, const int left = 0, const int right = 0);
int ParallelMergeSort(float* Slist, const int left = 0, const int right = 0);
int ParallelMerge(float* Slist, const int left = 0, const int right = 0);
int SerialQuickSort(float* list, const int left = 0, const int right = 0);
int ParallelQuickSort(float* list, const int left = 0, const int right = 0);
int QuickSortPartition(float* list, const int left = 0, const int right = 0);

//���������ݶ���Ϊ��
//float rawFloatData[DATANUM];

//�����������ݳ�ʼ�����룺��̨��������Էֿ���ʼ�������ٴ������
//for (size_t i = 0; i < DATANUM; i++)//���ݳ�ʼ��
//{
//	rawFloatData[i] = float(i + 1);
//}

//�����ٰ汾��Ϊͬ��������������̨������ϸ������е�ʱ�䡣�㷨һ�£�ֻ�ǲ������κμ����ֶΣ�SSE�����̻߳���OPENMP)
float SerialSum(const float data[], const int len); //data��ԭʼ���ݣ�lenΪ���ȡ����ͨ����������
float SerialMax(const float data[], const int len);//data��ԭʼ���ݣ�lenΪ���ȡ����ͨ����������
float SerialSort(const float data[], const int len, float  result[]);//data��ԭʼ���ݣ�lenΪ���ȡ���������result�С�

//˫�����ٰ汾
float ParallelSum(const float data[], const int len); //data��ԭʼ���ݣ�lenΪ���ȡ����ͨ����������
float ParallelMax(const float data[], const int len);//data��ԭʼ���ݣ�lenΪ���ȡ����ͨ����������
float ParallelSort(const float data[], const int len, float result[]);//data��ԭʼ���ݣ�lenΪ���ȡ���������result�С�
//���������ʹ��SSE���ر�ע��SSE��ָ������ݳ����йأ������Ⱥ�׺ps, ˫���Ⱥ�׺pd��

//�����ٶȵĴ�����Ϊ��
//QueryPerformanceCounter(&start);//start  
//��ĺ���(...);//������������������ش����ռ����ۺ�
//QueryPerformanceCounter(&end);//end
//std::cout << "Time Consumed:" << (end.QuadPart - start.QuadPart) << endl;
//cout << �����ͽ�� << endl;
//cout << ������ֵ << endl;
//cout << ��������Ƿ���ȷ << endl;

//�����������ô���������޷����㣬����ֻ��һ�롣
//�޸� #define SUBDATANUM 2000000 Ϊ #define SUBDATANUM 1000000���������㡣
//˫����ÿ�������������#define SUBDATANUM 1000000������ݣ���ʵ��#define SUBDATANUM 2000000�����㡣

//����ʵ��

//left������rightû����(��������Χ)������ҿ�
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
	//	SerialBubbleSort(Slist, left, right);//ð��������merge
	//}
	return 0;
}

int ParallelMergeSort(float* Slist, const int left, const int right)
{
	if (right - left > MinMergeLen)
	{
		int middle = (left + right + 1) / 2;
#pragma omp parallel sections shared(Slist,left,right,middle)
		{
#pragma omp section
			ParallelMergeSort(Slist, left, middle);
#pragma omp section
			ParallelMergeSort(Slist, middle, right);
		}
		ParallelMerge(Slist, left, right);
	}
	else
	{
		SerialBubbleSort(Slist, left, right);//ð��������merge
	}
	return 0;
}

//return -1˵������ʧ�ܣ�return 0��ʾû����
int SerialMerge(float* Slist, const int left, const int right)
{
	int middle = (left + right + 1) / 2 - left;
	int LP = 0;
	int RP = middle;
	int store_pos = left;
	int len = right - left;
	float leftvalue = 0.0f, rightvalue = 0.0f;
	float* Olist = new(nothrow) float[len];
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

int ParallelMerge(float* Slist, const int left, const int right)
{
	int middle = (left + right + 1) / 2 - left;
	int LP = 0;
	int RP = middle;
	int store_pos = left;
	int len = right - left;
	float leftvalue = 0.0f, rightvalue = 0.0f;
	float* Olist = new(nothrow) float[len];
	if (Olist == NULL)
	{
		cout << "Bad Allocate : TempList." << endl;
		return -1;
	}

#pragma omp parallel for schedule(dynamic,16)
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

//��ʽ���������б������
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

//ģ���ȡdata�Ĺ���
float AccessData(const float rawdata)
{
	return log(sqrt(rawdata));
}

//�����ʼ������
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

//�������б���бȽϣ�return��ͬ��������
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


//��������
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


