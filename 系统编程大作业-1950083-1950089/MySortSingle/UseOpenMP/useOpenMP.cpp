/* 1950083 ������ �Զ��� */
#include <iostream>
#include <omp.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cstdlib>
#include "useOpenMP.h"

using namespace std;

int main()
{
	cout << STARS << "���Կ�ʼ" << STARS << endl << endl;

	cout << "��������" << DATANUM << endl << endl;

	//��¼ʱ�䡢���ٱȵȱ���
	double QuickSortAccelerate = 0.0;
	double MergeSortAccelerate = 0.0;

	clock_t ClockStart, ClockEnd;

	double MergeSortSerialUsage = 0.0;
	double MergeSortParallelUsage = 0.0;
	double QuickSortSerialUsage = 0.0;
	double QuickSortParallelUsage = 0.0;

	//��̬����Lists
	cout << "��̬�ڴ�����" << endl << endl;
#pragma region <DyanmicAllocate>
	float* RawList = new(nothrow) float[DATANUM];
	if (RawList == NULL)
	{
		cout << "Bad Allocate : RawData" << endl;
		return -1;
	}
	float* MergeSortSerialList = new(nothrow) float[DATANUM];
	if (MergeSortSerialList == NULL)
	{
		cout << "Bad Allocate : SerialData" << endl;
		delete[] RawList;
		return -1;
	}
	float* MergeSortParallelList = new(nothrow) float[DATANUM];
	if (MergeSortParallelList == NULL)
	{
		cout << "Bad Allocate : ParallelData" << endl;
		delete[] RawList;
		delete[] MergeSortSerialList;
		return -1;
	}
	float* QuickSortSerialList = new(nothrow) float[DATANUM];
	if (QuickSortSerialList == NULL)
	{
		cout << "Bad Allocate : SerialData" << endl;
		delete[] RawList;
		delete[] MergeSortSerialList;
		delete[] MergeSortParallelList;
		return -1;
	}
	float* QuickSortParallelList = new(nothrow) float[DATANUM];
	if (QuickSortParallelList == NULL)
	{
		cout << "Bad Allocate : ParallelData" << endl;
		delete[] RawList;
		delete[] MergeSortSerialList;
		delete[] MergeSortParallelList;
		delete[] QuickSortSerialList;
		return -1;
	}
#pragma endregion
	cout << "��̬�ڴ��������" << endl;

	//����ͬSeed��ʼ����������
#pragma region <RandInitial Lists>
	//����ָ�����鷽���ʼ��
	float* Lists[5] = { RawList,MergeSortSerialList,MergeSortParallelList,QuickSortSerialList,QuickSortParallelList };
	char ListNames[5][25] = { "RawList","MergeSortSerialList","MergeSortParallelList","QuickSortSerialList","QuickSortParallelList"};

	//��n��listͬʱ��ʼ������ʼ�������ͬ��
	int listsize = sizeof(Lists) / sizeof(float*);
	RandInitial(Lists, listsize, DATANUM, RandSeed);
	//DispList(RawList, DATANUM, "RawList :");
	//DispList(SerialList, DATANUM, "SerialList :");
	//DispList(ParallelList, DATANUM, "ParallelList :");
#pragma endregion


#pragma region<MergeSortSerial>
	cout << endl;
	cout << HALFSTARS << "���й鲢������Կ�ʼ" << HALFSTARS << endl;
	ClockStart = clock();

	SerialMergeSort(MergeSortSerialList, 0, DATANUM);
	/*DispList(SerialList, DATANUM, "SerialList After Sort: ");*/

	ClockEnd = clock();
	MergeSortSerialUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "���й鲢���򻨷�ʱ����" << MergeSortSerialUsage << "ms" << endl << endl;
	cout << HALFSTARS << "���й鲢������Խ���" << HALFSTARS << endl << endl;
#pragma endregion

#pragma region<MergeSortParallel>
	cout << endl;
	cout << HALFSTARS << "���й鲢������Կ�ʼ" << HALFSTARS << endl;
	ClockStart = clock();

	ParallelMergeSort(MergeSortParallelList, 0, DATANUM);
	/*DispList(ParallelList, DATANUM, "ParallelList After Sort: ");*/

	ClockEnd = clock();
	MergeSortParallelUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "���й鲢���򻨷�ʱ����" << MergeSortParallelUsage << "ms" << endl << endl;
	cout << HALFSTARS << "���й鲢������Խ���" << HALFSTARS << endl << endl;
#pragma endregion

#pragma region<QuickSortSerial>
	cout << endl;
	cout << HALFSTARS << "���п���������Կ�ʼ" << HALFSTARS << endl;
	ClockStart = clock();

	SerialQuickSort(QuickSortSerialList, 0, DATANUM - 1);

	ClockEnd = clock();
	QuickSortSerialUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "���п������򻨷�ʱ����" << QuickSortSerialUsage << "ms" << endl << endl;
	cout << HALFSTARS << "���п���������Խ���" << HALFSTARS << endl << endl;
#pragma endregion

#pragma region<QuickSortParallel>
	cout << endl;
	cout << HALFSTARS << "���п���������Կ�ʼ" << HALFSTARS << endl;
	ClockStart = clock();

	ParallelQuickSort(QuickSortParallelList, 0, DATANUM - 1);

	ClockEnd = clock();
	QuickSortParallelUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "���п������򻨷�ʱ����" << QuickSortParallelUsage << "ms" << endl << endl;
	cout << HALFSTARS << "���п���������Խ���" << HALFSTARS << endl << endl;
#pragma endregion

	//������������������Խ���
#pragma region <checkresults>
	int DiffQuickParallelSerial = 0;
	int DiffMergeParallelSerial = 0;
	int DiffQuickMergeParallel = 0;
	int DiffQuickMergeSerial = 0;
	int SumError = 0;

	DiffMergeParallelSerial = CompTwoLists(MergeSortParallelList, MergeSortSerialList, DATANUM);
	DiffQuickParallelSerial = CompTwoLists(QuickSortParallelList, QuickSortSerialList, DATANUM);
	DiffQuickMergeParallel = CompTwoLists(MergeSortParallelList, QuickSortParallelList, DATANUM);
	DiffQuickMergeSerial = CompTwoLists(MergeSortSerialList, QuickSortSerialList, DATANUM);

	SumError = DiffMergeParallelSerial + DiffQuickParallelSerial + DiffQuickMergeParallel + DiffQuickMergeSerial;

	QuickSortAccelerate = QuickSortSerialUsage / QuickSortParallelUsage;
	MergeSortAccelerate = MergeSortSerialUsage / MergeSortParallelUsage;


	if (SumError == 0)
	{
		cout << "��������͹鲢����Ĵ��С����н����ȫһ�¡����ٱ����£�" << endl;
		cout << "�鲢������ٱ�Ϊ��" << MergeSortAccelerate << endl;
		cout << "����������ٱ�Ϊ��" << QuickSortAccelerate << endl;
	}
	else
	{
		cout << "��������͹鲢����Ĵ��С����н����һ�£�����������벢���²��ԡ�" << endl;
	}
	cout << endl;

	//for (int i = 0; i < listsize; i++)
	//{
	//	DispList(Lists[i], 20, ListNames[i]);
	//}

#pragma endregion

	//����Ķѿռ�����ͷ�
	cout << endl << "��̬�ڴ�����Ŀռ��ͷ�" << endl;
#pragma region <ReleaseDyanmicAllocate>
	delete[] RawList;
	delete[] MergeSortSerialList;
	delete[] MergeSortParallelList;
	delete[] QuickSortSerialList;
	delete[] QuickSortParallelList;
#pragma endregion
	cout << endl << "��̬�ڴ�����Ŀռ��ͷ����" << endl << endl;

	cout << STARS << "���Խ���" << STARS << endl << endl;

	system("pause");

	return 0;
}

//RawList
//MergeSortSerialList
//MergeSortParallelList
//QuickSortSerialList
//QuickSortParallelList

