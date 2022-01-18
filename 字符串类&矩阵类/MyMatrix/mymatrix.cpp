/* 1950083 刘智宇 自动化 */
#include <iostream>
#include "mymatrix.h"

using namespace std;

const char stars[8] = "*******";

int main()
{
	cout << stars << stars << "测试开始" << stars << stars << endl << endl;

#pragma region <test1>
	//测试构造函数
	cout << stars << "测试构造函数" << stars << endl << endl;
	cout << "测试默认构造函数：" << endl << endl;

	MyMatrix<double>tempdefaultdouble;
	tempdefaultdouble.DisplayMat();
	cout << endl;

	MyMatrix<int>tempdefaultint;
	tempdefaultint.DisplayMat();
	cout << endl;

	cout << "测试一般二维矩阵构造函数：" << endl << endl;

	MyMatrix<double>tempdouble(1, 2);
	tempdouble.DisplayMat();
	cout << endl;

	MyMatrix<int>tempint(2, 3);
	tempint.DisplayMat();
	cout << endl;

	cout << "测试二维方阵构造函数：" << endl << endl;

	MyMatrix<double>tempsquarefloat(4);
	tempsquarefloat.DisplayMat();
	cout << endl;

	MyMatrix<int>tempsquareint(2);
	tempsquareint.DisplayMat();
	cout << endl;

	cout << "测试Buffer构造函数：" << endl << endl;
	int intbuffer[10] = { 0,1,2,3,4,
							5,6,7,8,9 };
	float floatbuffer[3] = { 1.3f,
								4.4f,
								889.3f };
	double doublebuffer1[8] = { 0.1,1.2,2.3,3.4,
								4.5,5.6,6.7,7.8 };
	double doublebuffer2[10] = { 0.1,1.2,2.3,3.4,4.5,
								5.6,6.7,7.8,8.9,9.0 };

	MyMatrix<int>tempbufferint1(intbuffer, 2, 5);
	MyMatrix<int>tempbufferint2(intbuffer, 5, 2);
	tempbufferint1.DisplayMat();
	cout << endl;

	MyMatrix<float>tempbufferfloat(floatbuffer, 3, 1);
	tempbufferfloat.DisplayMat();
	cout << endl;

	MyMatrix<double>tempbufferdouble(doublebuffer1, 2, 4);
	tempbufferdouble.DisplayMat();
	cout << endl;

	cout << "测试拷贝构造函数(用上面的buffer创建的)：" << endl << endl;
	MyMatrix<int>tempcopyint(tempbufferint1);
	tempcopyint.DisplayMat();
	cout << endl;

	MyMatrix<float>tempcopyfloat(tempbufferfloat);
	tempcopyfloat.DisplayMat();
	cout << endl;

	MyMatrix<double>tempcopydouble(tempbufferdouble);
	tempcopydouble.DisplayMat();
	cout << endl;

#pragma endregion

#pragma region <test2>
	//测试拷贝赋值函数
	cout << stars << "测试拷贝赋值函数(用上面的buffer进行赋值)" << stars << endl << endl;
	MyMatrix<int>tempassignint = tempbufferint1;
	tempassignint.DisplayMat();
	cout << endl;

	MyMatrix<float>tempassignfloat = tempbufferfloat;
	tempassignfloat.DisplayMat();
	cout << endl;

	MyMatrix<double>tempassigndouble = tempbufferdouble;
	tempassigndouble.DisplayMat();
	cout << endl;
#pragma endregion

	//测试算数运算
	cout << stars << "测试算数运算" << stars << endl << endl;
	cout << "测试矩阵加法+=" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble += tempbufferdouble;
	cout << "+=操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;

	cout << "测试矩阵加法+" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble = tempbufferdouble + tempbufferdouble;
	cout << "+操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;


	cout << "测试矩阵减法-=" << endl << endl;
	cout << "原来的矩阵：" << endl;
	MyMatrix<double>tempbufferdouble1(tempbufferdouble);
	tempbufferdouble1.DisplayMat();
	tempbufferdouble1 -= tempbufferdouble1;
	cout << "-=操作后的矩阵：" << endl;
	tempbufferdouble1.DisplayMat();
	cout << endl;

	cout << "测试矩阵减法-" << endl << endl;
	cout << "原来的矩阵：" << endl;
	MyMatrix<double>tempbufferdouble2(tempbufferdouble);
	tempbufferdouble2.DisplayMat();
	tempbufferdouble2 = tempbufferdouble2 - tempbufferdouble2;
	cout << "-操作后的矩阵：" << endl;
	tempbufferdouble2.DisplayMat();
	cout << endl;

	cout << "测试矩阵乘法" << endl << endl;
	//buffer1可以乘buffer2，buffer2不能乘buffer1
	int multibuffer1[6] = { 1,2,3,
							4,5,6 };//2*3
	int multibuffer2[9] = { 9,8,7,
							6,5,4,
							3,2,1 };//3*3
	MyMatrix<int>multiMat1(multibuffer1, 2, 3);
	MyMatrix<int>multiMat2(multibuffer2, 3, 3);
	MyMatrix<int>multiMat3(multibuffer1, 2, 3);
	MyMatrix<int>multiMat4(multibuffer2, 3, 3);
	int multiresult = 0;
	cout << "测试不能相乘" << endl;
	multiresult = (multiMat2 *= multiMat1);
	cout << "结果：" << multiresult << " 。(-1表示不能相乘，1表示可以相乘。)" << endl << endl;
	cout << "测试可以相乘" << endl;
	multiresult = (multiMat1 *= multiMat2);
	cout << "结果：" << multiresult << " 。(-1表示不能相乘，1表示可以相乘。)" << endl;
	if (multiresult == 1)
		multiMat1.DisplayMat();
	cout << "经过检验，结果正确" << endl << endl;


	cout << "测试矩阵加数字+=" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble += 3.0;
	cout << "+=操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;

	cout << "测试矩阵加数字+" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble = tempbufferdouble + 3.0;
	cout << "+操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;

	cout << "测试矩阵减数字-=" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble -= 3.0;
	cout << "-=操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;

	cout << "测试矩阵减数字-" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble = tempbufferdouble - 3.0;
	cout << "-操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;
	

	cout << "测试矩阵乘数字" << endl << endl;
	cout << "测试矩阵乘数字*=" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble *= 2.0;
	cout << "*=操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;

	cout << "测试矩阵减数字*" << endl << endl;
	cout << "原来的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	tempbufferdouble = tempbufferdouble * 2.0;
	cout << "*操作后的矩阵：" << endl;
	tempbufferdouble.DisplayMat();
	cout << endl;


	//测试流插入运算符
	cout << stars << "测试流插入运算符\'<<\'" << stars << endl << endl;

	cout << tempbufferdouble << endl;

	//测试其他方法
	cout << stars << "测试其他方法" << stars << endl << endl;
	cout << "测试输出行数" << endl << endl;
	cout << "矩阵实际行数：" << 2 << "，成员函数输出结果：" << tempbufferint1.GetRow() << endl << endl;

	cout << "测试输出列数" << endl << endl;
	cout << "矩阵实际列数：" << 5 << "，成员函数输出结果：" << tempbufferint1.GetCol() << endl << endl;

	cout << "测试输出矩阵元素值" << endl << endl;
	cout << "实际元素值：" << 7 << "，成员函数输出结果：" << tempbufferint1.GetVal(1, 2) << endl << endl;

	cout << "测试修改矩阵元素值" << endl << endl;
	cout << "原来的元素值：" << tempbufferint1.GetVal(1, 2) << endl;
	tempbufferint1.ChangeVal(123, 1, 2);
	cout << "修改后的元素值：" << tempbufferint1.GetVal(1, 2) << endl;


	cout << endl << endl;
	cout << stars << stars << "测试结束" << stars << stars << endl << endl;

	return 0;
}


//2、二维矩阵
//a、支持不同大小的矩阵，并可以放入不同的数据类型
//b、构造方法：	(1)默认构造；					ok
//				(2)方阵构造；					ok
//				(3)一般矩阵构造； 				ok
//				(4)复制构造；					ok
//c、运算符重载：	(1)加法 + ；						ok
//				(2)减法 - ；						ok
//				(3)矩阵数乘、矩阵相乘 * ；		ok
//				(4)复制赋值 ；					ok
//				(5)输出流运算符 <<				ok
//d、其他方法：	(1)获取成员；					ok
//				(2)修改成员；					ok
//				(3)动态内存分配					ok
//
//矩阵类名 : MyMatrix，支持一下使用方法
//MyMatrix<int> temp;										ok
//MyMatrix<double> temp(m, n)）;//构造m*n的空矩阵				ok
//MyMatrix<int> temp(buffer, m, n);//把buffer转换成m*n矩阵	ok
//
//MyMatrix<int> temp2(temp);								ok
//MyMatrix<int> temp3;										ok
//temp3 = temp;//所有元素都要拷贝过去							ok
//
//temp = temp + temp2;//同样大小的矩阵									ok
//temp2 = temp * 3;//每个元素乘以3										ok
//temp2 = temp - 3;//每个元素加3											ok
//int kk = (temp *= temp3);//支持矩阵乘法和矩阵向量乘，如果不能乘返回值		ok
//
//cout << temp << endl;//输出temp格式到屏幕					ok
