/* 1950083 ������ �Զ��� */
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#ifndef __MyString__
#define __MyString__

#include <cstring>
#include <iostream>
#include <cmath>

using namespace std;

#define BufferLen 128       //��ʱ�ַ�����ͳһ��С
#define SystemPrompt 1   //ϵͳ��ʾ

//ȫ�ֺ�������
int KMP(const char* const main, const char* const sub);


//������
class MyString
{
private:
	char* mydata;
	int length;
public:
	//Key Functions
	//���캯��
	MyString(const char* tempstr = NULL); //Ĭ�Ϲ���
	MyString(const int& tempint);//��������
	MyString(const double& tempdouble);//˫���ȸ���������
	MyString(const float& tempfloat);//�����ȸ���������
	//�������캯��
	MyString(const MyString& tempstr);
	//������ֵ��������&����������ֵ
	MyString& operator = (const MyString& tempstr);
	//��������
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

//�ؼ���Ա����--------------------------------------------------------------------------------
//���캯��-------------------------------------------------------------------
//��Ĭ��&һ�㹹�캯��
inline
MyString::MyString(const char* tempstr)//���庯������дĬ�ϲ���
{
	if (tempstr == NULL)
	{
		length = 0;
		mydata = new char[length + 1];
		mydata[0] = '\0';//�����鷽ʽ������д����
	}
	else//(tempstr!=NULL)
	{
		length = strlen(tempstr);
		mydata = new char[length + 1];//+1��Ϊ�˷����β��
		strcpy(mydata, tempstr);//��ԭ���ݿ������¿ռ䣬strcpy�Ჹβ��
	}
}
//����������
inline
MyString::MyString(const int& tempint) 
{
	char tempbuf[BufferLen] = { 0, };
	length = sprintf(tempbuf, "%d", tempint);//û��β��

	mydata = new char[length+1];
	strcpy(mydata, tempbuf);
}
//�۸���������
inline
MyString::MyString(const double& tempdouble)
{
	char tempbuf[BufferLen] = { 0, };
	length = sprintf(tempbuf, "%.1f", tempdouble-0.05);//û��β��

	mydata = new char[length+1];
	strcpy(mydata, tempbuf);
}
inline
MyString::MyString(const float& tempfloat)
{
	char tempbuf[BufferLen] = { 0, };
	length = sprintf(tempbuf, "%.1f", float(tempfloat-0.05));//û��β��

	mydata = new char[length + 1];
	strcpy(mydata, tempbuf);
}
//�ݿ�������
inline
MyString::MyString(const MyString& tempstr) 
{
	length = strlen(tempstr.mydata);//û����β��
	mydata = new char[length + 1];//+1��Ϊ�˴��β��
	strcpy(mydata, tempstr.mydata);//strcpy�Ჹβ��
}

//������ֵ����
inline
MyString& MyString:: operator = (const MyString& tempstr)
{
	//�ж��Ƿ����Լ���������ܻὫ����ɾ��
	if (mydata == tempstr.mydata)//������
	{
		return *this;
	}
	else //(mydata == tempstr.mydata)//��������
	{
		delete[] mydata;//�Ƚ��Լ����
		length = strlen(tempstr.mydata);//û����β��
		mydata = new char[length + 1];//+1��Ϊ�˴��β��
		strcpy(mydata, tempstr.mydata);//strcpy�Ჹβ��
		return *this;
	}
}

//����
inline
MyString::~MyString()
{
	delete[] mydata;
}

//������Ա����--------------------------------------------------------------------------------
//���� + += -= .subString .find .toInt .toFloat .length
//+
MyString MyString::operator + (const MyString& right)const
{
	int len1 = 0, len2 = 0;
	len1 = length;
	len2 = right.length;
	int newlen = len1 + len2;

	MyString temp;
	temp.mydata = new char[newlen + 1];//+1Ϊ�˷�β��
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

	char* temp;//������ΪMyString�࣬���������ڽ������ռ�ᱻ�Զ��ͷ�
	temp = new char[length + 1];//+1Ϊ�˷�β��
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
			length -= sub.length;//��������
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

//find������KMP�㷨��KMPʵ��������ȫ�ֺ�������
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
	int neg_flag = 0;//�ж��Ƿ�Ϊ��
	if (mydata[0] == '-')
		neg_flag = 1;
	for (int cnt = 0+neg_flag; cnt < length; cnt++)//����Ǹ�����������һ�����
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
		return -1;//��ʾ�����޸�
	else
		mydata[pos] = temp;
	return 1;
}

//toFloat
float MyString::toFloat()const
{
	int numberlen = 0;
	float temp = 0.0f;
	int num_of_dot = 0;//ֻ����һ��С����
	int dot_pos = 0;//С����λ��
	int neg_flag = 0;
	if (mydata[0] == '-')
		neg_flag = 1;
	for (int cnt = 0 + neg_flag; cnt < length; cnt++)
	{
		if (mydata[cnt] != '.' && (mydata[cnt] < '0' || mydata[cnt]>'9'))
			break;
		if (mydata[cnt] == '.')//������
		{
			if (num_of_dot >= 1)//֮ǰ�Ѿ��е�
				return -2;
			num_of_dot++;
			dot_pos = cnt;
		}
		else//��������
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



//����ȫ�ֺ���--------------------------------------------------------------------------------
//���������������
ostream& operator << (ostream& os, const MyString& temp)
{
	if (SystemPrompt == 1&& (temp.get_mydata())[0] == '\0')
	{
		return os << "System Prompt: Empty String.";
	}
	return os << temp.get_mydata();
}

//KMP�ַ���ƥ���㷨
int KMP(const char* const main, const char* const sub)
{
	int len_main = strlen(main);
	int len_sub = strlen(sub);

	if (len_main < len_sub)//�ִ�������ֱ�ӽ���
		return -1;

	//prefix����ʹ��sub�������Ϣ
	int* prefix = new int[len_sub];//����prefix�ռ�

	for (int cnt = 0; cnt < len_sub; cnt++)//��ʼ��prefix
		prefix[cnt] = 0;
	
	int subFormer = 1, subLatter = 0;//former��latter�ұ�

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
	bool flagFound = 0;//�ж��Ƿ��ҵ��ı�־

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

	delete[] prefix;//���������prefix����ռ�
	if (flagFound == static_cast<int>(0))
		return -1;
	else
		return mainPointer - subPointer;
}


#endif //__MyString__
