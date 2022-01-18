/* 1950083 刘智宇 自动化 */
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#ifndef __MyString__
#define __MyString__

#include <cstring>
#include <iostream>
#include <cmath>

using namespace std;

#define BufferLen 128       //临时字符数组统一大小
#define SystemPrompt 1   //系统提示

//全局函数声明
int KMP(const char* const main, const char* const sub);


//类声明
class MyString
{
private:
	char* mydata;
	int length;
public:
	//Key Functions
	//构造函数
	MyString(const char* tempstr = NULL); //默认构造
	MyString(const int& tempint);//整数构造
	MyString(const double& tempdouble);//双精度浮点数构造
	MyString(const float& tempfloat);//单精度浮点数构造
	//拷贝构造函数
	MyString(const MyString& tempstr);
	//拷贝赋值函数，加&用于连续赋值
	MyString& operator = (const MyString& tempstr);
	//析构函数
	~MyString();


	//Other Functions
	char* get_mydata()const {return mydata;}
	int get_length()const { return length; }
	MyString operator + (const MyString& right)const;
	MyString& operator += (const MyString& right);
	MyString& operator += (const int right);
	MyString& operator += (const float right);
	MyString& operator += (const double right);
	MyString& operator -= (const MyString& right);
	MyString& operator -= (const int right);
	MyString& operator -= (const char* const right);
	MyString& operator -= (const float right);
	MyString& operator -= (const double right);

	MyString subString(int m, int n)const;
	int find(const MyString& sub)const;
	int toInt()const;
	float toFloat()const;
	int ChangeData(int pos, char temp);
};

//关键成员函数--------------------------------------------------------------------------------
//构造函数-------------------------------------------------------------------
//①默认&一般构造函数
inline
MyString::MyString(const char* tempstr)//定义函数，不写默认参数
{
	if (tempstr == NULL)
	{
		length = 0;
		mydata = new char[length + 1];
		mydata[0] = '\0';//用数组方式，方便写析构
	}
	else//(tempstr!=NULL)
	{
		length = strlen(tempstr);
		mydata = new char[length + 1];//+1是为了方便放尾零
		strcpy(mydata, tempstr);//将原数据拷贝到新空间，strcpy会补尾零
	}
}
//②整数构造
inline
MyString::MyString(const int& tempint) 
{
	char tempbuf[BufferLen] = { 0, };
	length = sprintf(tempbuf, "%d", tempint);//没有尾零

	mydata = new char[length+1];
	strcpy(mydata, tempbuf);
}
//③浮点数构造
inline
MyString::MyString(const double& tempdouble)
{
	char tempbuf[BufferLen] = { 0, };
	length = sprintf(tempbuf, "%.1f", tempdouble-0.05);//没有尾零

	mydata = new char[length+1];
	strcpy(mydata, tempbuf);
}
inline
MyString::MyString(const float& tempfloat)
{
	char tempbuf[BufferLen] = { 0, };
	length = sprintf(tempbuf, "%.1f", float(tempfloat-0.05));//没有尾零

	mydata = new char[length + 1];
	strcpy(mydata, tempbuf);
}
//⑤拷贝构造
inline
MyString::MyString(const MyString& tempstr) 
{
	length = strlen(tempstr.mydata);//没算上尾零
	mydata = new char[length + 1];//+1是为了存放尾零
	strcpy(mydata, tempstr.mydata);//strcpy会补尾零
}

//拷贝赋值函数
inline
MyString& MyString:: operator = (const MyString& tempstr)
{
	//判断是否是自己，否则可能会将自身删除
	if (mydata == tempstr.mydata)//是自身
	{
		return *this;
	}
	else //(mydata == tempstr.mydata)//不是自身
	{
		delete[] mydata;//先将自己清空
		length = strlen(tempstr.mydata);//没算上尾零
		mydata = new char[length + 1];//+1是为了存放尾零
		strcpy(mydata, tempstr.mydata);//strcpy会补尾零
		return *this;
	}
}

//析构
inline
MyString::~MyString()
{
	delete[] mydata;
}

//其他成员函数--------------------------------------------------------------------------------
//包括 + += -= .subString .find .toInt .toFloat .length
//+
MyString MyString::operator + (const MyString& right)const
{
	int len1 = 0, len2 = 0;
	len1 = length;
	len2 = right.length;
	int newlen = len1 + len2;

	MyString temp;
	temp.mydata = new char[newlen + 1];//+1为了放尾零
	temp.length = newlen;
	strcpy(temp.mydata, mydata);
	strcat(temp.mydata, right.mydata);

	return temp;
}
//+=
MyString& MyString::operator += (const MyString& right)
{
	int len1 = 0, len2 = 0;
	len1 = strlen(mydata);
	len2 = strlen(right.mydata);
	length = len1 + len2;

	char* temp;//不能设为MyString类，否则生命期结束，空间会被自动释放
	temp = new char[length + 1];//+1为了放尾零
	strcpy(temp, mydata);
	strcat(temp, right.mydata);
	delete[] mydata;
	mydata = temp;

	return *this;
}
MyString& MyString::operator += (const int right)
{
	MyString rightstring(right);
	return *this += rightstring;
}
MyString& MyString::operator += (const float right)
{
	MyString rightstring(right);
	return *this += rightstring;
}
MyString& MyString::operator += (const double right)
{
	MyString rightstring(right);
	return *this += rightstring;
}
//-=
MyString& MyString::operator-= (const MyString& sub)
{
	int subpos = 0;
	while (true)
	{
		subpos = this->find(sub);
		if (subpos < 0)
			break;
		else
		{
			for (int i = subpos; i < length - sub.length + 1; i++)
			{
				mydata[i] = mydata[i + sub.length];
			}
			length -= sub.length;//长度缩减
		}
	}

	return *this;
}

MyString& MyString::operator-= (const int subint)
{
	MyString Sub(subint);
	return (*this) -= Sub;
}

MyString& MyString::operator-= (const float subfloat)
{
	MyString Sub(subfloat);
	return (*this) -= Sub;
}

MyString& MyString::operator-= (const double subdouble)
{
	MyString Sub(subdouble);
	return (*this) -= Sub;
}

MyString& MyString::operator-= (const char* const substr)
{
	MyString Sub(substr);
	return (*this) -= Sub;
}

//find，利用KMP算法，KMP实现在最后的全局函数部分
int MyString:: find(const MyString& sub)const
{
	int position = KMP(this->mydata, sub.mydata);
	return position;
}

//subString
MyString MyString::subString(int m = 0, int n = 0) const
{
	MyString sub;
	
	if (m < 0)
		m = 0;
	if (m > length)
		m = length;
	if (n < 0)
		n = 0;
	if (m + n > length)
		n = length - m;

	delete[] sub.mydata;
	sub.mydata = new char[n + 1];

	for (int i = 0; i < n; i++)
	{
		sub.mydata[i] = mydata[m + i];
	}
	sub.mydata[n] = '\0';

	return sub;
}

//toInt
int MyString::toInt()const
{
	int numberlen = 0;
	int temp = 0;
	int neg_flag = 0;//判断是否为负
	if (mydata[0] == '-')
		neg_flag = 1;
	for (int cnt = 0+neg_flag; cnt < length; cnt++)//如果是负数，跳过第一个检查
	{
		if (mydata[cnt] < '0' || mydata[cnt]>'9')
			break;
		else
		{
			temp = temp * 10 + (mydata[cnt] - '0');
			numberlen++;
		}
	}
	if (numberlen == 0)
		return -2;
	if (neg_flag == 1)
		return temp * (-1);
	else
		return temp;
}


//ChangeData
int MyString::ChangeData(int pos, char temp)
{
	if (pos >= length)
		return -1;//表示不能修改
	else
		mydata[pos] = temp;
	return 1;
}

//toFloat
float MyString::toFloat()const
{
	int numberlen = 0;
	float temp = 0.0f;
	int num_of_dot = 0;//只能有一个小数点
	int dot_pos = 0;//小数点位置
	int neg_flag = 0;
	if (mydata[0] == '-')
		neg_flag = 1;
	for (int cnt = 0 + neg_flag; cnt < length; cnt++)
	{
		if (mydata[cnt] != '.' && (mydata[cnt] < '0' || mydata[cnt]>'9'))
			break;
		if (mydata[cnt] == '.')//遇到点
		{
			if (num_of_dot >= 1)//之前已经有点
				return -2;
			num_of_dot++;
			dot_pos = cnt;
		}
		else//遇到数字
		{
			temp = temp * 10 + (mydata[cnt] - '0');
			numberlen++;
		}
		
	}

	if (numberlen == 0)
		return -2;

	if(neg_flag==1)
		return float(temp / (pow(10, length - dot_pos - 1)))*(-1);
	else
		return float(temp / (pow(10, length - dot_pos - 1)));
	
}



//其他全局函数--------------------------------------------------------------------------------
//流插入运算符重载
ostream& operator << (ostream& os, const MyString& temp)
{
	if (SystemPrompt == 1&& (temp.get_mydata())[0] == '\0')
	{
		return os << "System Prompt: Empty String.";
	}
	return os << temp.get_mydata();
}

//KMP字符串匹配算法
int KMP(const char* const main, const char* const sub)
{
	int len_main = strlen(main);
	int len_sub = strlen(sub);

	if (len_main < len_sub)//字串更长，直接结束
		return -1;

	//prefix，仅使用sub的相关信息
	int* prefix = new int[len_sub];//申请prefix空间

	for (int cnt = 0; cnt < len_sub; cnt++)//初始化prefix
		prefix[cnt] = 0;
	
	int subFormer = 1, subLatter = 0;//former在latter右边

	while (subFormer < len_sub)
	{
		if (sub[subFormer] == sub[subLatter])
		{
			prefix[subFormer++] = ++subLatter;
		}
		else
		{
			if (subLatter > 0)
				subLatter = prefix[subLatter - 1];
			else
				prefix[subFormer++] = 0;
		}
	}
	//match
	int mainPointer = 0, subPointer = 0;
	bool flagFound = 0;//判断是否找到的标志

	while (mainPointer < len_main)
	{
		if (main[mainPointer] == sub[subPointer])
		{
			mainPointer++, subPointer++;
			if (subPointer == len_sub)
			{
				flagFound = static_cast<int>(1);
				break;
			}
		}
		else
		{
			if (subPointer > 0)
				subPointer = prefix[subPointer - 1];
			else
				mainPointer++;
		}
	}

	delete[] prefix;//回收申请的prefix数组空间
	if (flagFound == static_cast<int>(0))
		return -1;
	else
		return mainPointer - subPointer;
}


#endif //__MyString__
