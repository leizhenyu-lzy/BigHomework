/* 1950083 刘智宇 自动化 */
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
	cout << STARS << "测试开始" << STARS << endl << endl;

	cout << "数据量：" << DATANUM << endl << endl;

	//记录时间、加速比等变量
	double QuickSortAccelerate = 0.0;
	double MergeSortAccelerate = 0.0;

	clock_t ClockStart, ClockEnd;

	double MergeSortSerialUsage = 0.0;
	double MergeSortParallelUsage = 0.0;
	double QuickSortSerialUsage = 0.0;
	double QuickSortParallelUsage = 0.0;

	//动态申请Lists
	cout << "动态内存申请" << endl << endl;
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
	cout << "动态内存申请完成" << endl;

	//用相同Seed初始化所有数组
#pragma region <RandInitial Lists>
	//构建指针数组方便初始化
	float* Lists[5] = { RawList,MergeSortSerialList,MergeSortParallelList,QuickSortSerialList,QuickSortParallelList };
	char ListNames[5][25] = { "RawList","MergeSortSerialList","MergeSortParallelList","QuickSortSerialList","QuickSortParallelList"};

	//将n个list同时初始化（初始化结果相同）
	int listsize = sizeof(Lists) / sizeof(float*);
	RandInitial(Lists, listsize, DATANUM, RandSeed);
	//DispList(RawList, DATANUM, "RawList :");
	//DispList(SerialList, DATANUM, "SerialList :");
	//DispList(ParallelList, DATANUM, "ParallelList :");
#pragma endregion


#pragma region<MergeSortSerial>
	cout << endl;
	cout << HALFSTARS << "串行归并排序测试开始" << HALFSTARS << endl;
	ClockStart = clock();

	SerialMergeSort(MergeSortSerialList, 0, DATANUM);
	/*DispList(SerialList, DATANUM, "SerialList After Sort: ");*/

	ClockEnd = clock();
	MergeSortSerialUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "串行归并排序花费时长：" << MergeSortSerialUsage << "ms" << endl << endl;
	cout << HALFSTARS << "串行归并排序测试结束" << HALFSTARS << endl << endl;
#pragma endregion

#pragma region<MergeSortParallel>
	cout << endl;
	cout << HALFSTARS << "并行归并排序测试开始" << HALFSTARS << endl;
	ClockStart = clock();

	ParallelMergeSort(MergeSortParallelList, 0, DATANUM);
	/*DispList(ParallelList, DATANUM, "ParallelList After Sort: ");*/

	ClockEnd = clock();
	MergeSortParallelUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "并行归并排序花费时长：" << MergeSortParallelUsage << "ms" << endl << endl;
	cout << HALFSTARS << "并行归并排序测试结束" << HALFSTARS << endl << endl;
#pragma endregion

#pragma region<QuickSortSerial>
	cout << endl;
	cout << HALFSTARS << "串行快速排序测试开始" << HALFSTARS << endl;
	ClockStart = clock();

	SerialQuickSort(QuickSortSerialList, 0, DATANUM - 1);

	ClockEnd = clock();
	QuickSortSerialUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "串行快速排序花费时长：" << QuickSortSerialUsage << "ms" << endl << endl;
	cout << HALFSTARS << "串行快速排序测试结束" << HALFSTARS << endl << endl;
#pragma endregion

#pragma region<QuickSortParallel>
	cout << endl;
	cout << HALFSTARS << "并行快速排序测试开始" << HALFSTARS << endl;
	ClockStart = clock();

	ParallelQuickSort(QuickSortParallelList, 0, DATANUM - 1);

	ClockEnd = clock();
	QuickSortParallelUsage = -(static_cast<double>(ClockStart) - static_cast<double>(ClockEnd)) / CLOCKS_PER_SEC * 1000;
	cout << endl << "并行快速排序花费时长：" << QuickSortParallelUsage << "ms" << endl << endl;
	cout << HALFSTARS << "并行快速排序测试结束" << HALFSTARS << endl << endl;
#pragma endregion

	//检验排序结果并输出测试结论
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
		cout << "快速排序和归并排序的串行、并行结果完全一致。加速比如下：" << endl;
		cout << "归并排序加速比为：" << MergeSortAccelerate << endl;
		cout << "快速排序加速比为：" << QuickSortAccelerate << endl;
	}
	else
	{
		cout << "快速排序和归并排序的串行、并行结果不一致，请检查排序代码并重新测试。" << endl;
	}
	cout << endl;

	//for (int i = 0; i < listsize; i++)
	//{
	//	DispList(Lists[i], 20, ListNames[i]);
	//}

#pragma endregion

	//申请的堆空间进行释放
	cout << endl << "动态内存申请的空间释放" << endl;
#pragma region <ReleaseDyanmicAllocate>
	delete[] RawList;
	delete[] MergeSortSerialList;
	delete[] MergeSortParallelList;
	delete[] QuickSortSerialList;
	delete[] QuickSortParallelList;
#pragma endregion
	cout << endl << "动态内存申请的空间释放完成" << endl << endl;

	cout << STARS << "测试结束" << STARS << endl << endl;

	system("pause");

	return 0;
}

//RawList
//MergeSortSerialList
//MergeSortParallelList
//QuickSortSerialList
//QuickSortParallelList

