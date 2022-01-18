/* 1950083 刘智宇 自动化 */
#pragma once

#ifndef __mymatrix__
#define __mymatrix__

#include <iostream>
#include <cmath>
#include <iomanip>


using namespace std;

template<class T>
class MyMatrix
{
private:
	int matrow = 0;
	int matcol = 0;
	T* mat = nullptr;
public:
	//构造函数
	MyMatrix(int row, int col);
	MyMatrix(int squarelen = 1);
	MyMatrix(T* buffer, int row, int col);
	//拷贝构造
	MyMatrix(const MyMatrix<T> &AnotherMat);
	//拷贝赋值
	MyMatrix<T>& operator =(const MyMatrix<T>& AnotherMat);
	//析构函数
	~MyMatrix();
	//算数操作
	MyMatrix<T>& operator +=(const MyMatrix<T>& AnotherMat);
	MyMatrix<T> operator +(const MyMatrix<T>& AnotherMat)const;
	MyMatrix<T>& operator +=(T number);
	MyMatrix<T> operator +(T number)const;
	MyMatrix<T>& operator -=(const MyMatrix<T>& AnotherMat);
	MyMatrix<T> operator -(const MyMatrix<T>& AnotherMat)const;
	MyMatrix<T>& operator -=(T number);
	MyMatrix<T> operator -(T number)const;
	int operator *=(const MyMatrix<T>& AnotherMat);
	MyMatrix<T>& operator *=(T number);
	MyMatrix<T> operator *(T number)const;

	//其他函数
	int DisplayMat()const;
	int CopyBuffer(T* buffer, int row, int col);
	//获取数据、修改数据
	int GetRow()const { return matrow; }
	int GetCol()const { return matcol; }
	T* GetMat()const { return mat; }
	T ChangeVal(T NewValue, int row, int col);
	T GetVal(int row, int col)const;
};



template<class T>
inline 
MyMatrix<T>::MyMatrix(int row, int col) :matrow(row), matcol(col)
{
	if (row == 0)
		matrow = 1;
	if (col == 0)
		matcol = 1;
	mat = new T[matrow * matcol]{ 0 };
}

template<class T>
inline
MyMatrix<T>::MyMatrix(int squarelen) :matrow(squarelen), matcol(squarelen)
{
	if (squarelen == 0)
	{
		matrow = 1;
		matcol = 1;
	}
	mat = new T[matrow * matcol]{ 0 };
}

template<class T>
inline
MyMatrix<T>::MyMatrix(T* buffer,int row,int col) :matrow(row), matcol(col)
{
	if (row == 0)
		matrow = 1;
	if (col == 0)
		matcol = 1;
	mat = new T[matrow * matcol]{ 0 };
	CopyBuffer(buffer, row, col);
}

template<class T>
inline
MyMatrix<T>::MyMatrix(const MyMatrix<T>& AnotherMat)
{
	matrow = AnotherMat.matrow;
	matcol = AnotherMat.matcol;
	mat = new T[matrow * matcol]{ 0 };
	CopyBuffer(AnotherMat.mat, matrow, matcol);
}

template<class T>
inline
MyMatrix<T>& MyMatrix<T>::operator =(const MyMatrix<T>& AnotherMat)
{
	if (this != &AnotherMat)
	{
		delete[] mat;
		matrow = AnotherMat.matrow;
		matcol = AnotherMat.matcol;
		mat = new T[matrow * matcol]{ 0 };
		CopyBuffer(AnotherMat.mat, matrow, matcol);
	}
	return *this;
}

template<class T>
inline
MyMatrix<T>& MyMatrix<T>::operator +=(const MyMatrix<T>& AnotherMat)
{
	int pos = 0;
	if (matrow != AnotherMat.matrow || matcol != AnotherMat.matcol)
		cout << "Size of the two matrices can not match!" << endl;
	else
	{
		for (int i = 0; i < matrow; i++)
		{
			for (int j = 0; j < matcol; j++)
			{
				pos = i * matcol + j;
				mat[pos] += (AnotherMat.mat)[pos];
			}
		}
	}
	return *this;
}

template<class T>
inline
MyMatrix<T>& MyMatrix<T>::operator +=(T number)
{
	int pos = 0;
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			pos = i * matcol + j;
			mat[pos] += number;
		}
	}
	return *this;
}


template<class T>
inline
MyMatrix<T> MyMatrix<T>::operator +(const MyMatrix<T>& AnotherMat) const
{
	MyMatrix<T>temp(matrow,matcol);
	int pos = 0;
	if (matrow != AnotherMat.matrow || matcol != AnotherMat.matcol)
		cout << "Size of the two matrices can not match!" << endl;
	else
	{
		for (int i = 0; i < matrow; i++)
		{
			for (int j = 0; j < matcol; j++)
			{
				pos = i * matcol + j;
				temp.mat[pos] = mat[pos] + AnotherMat.mat[pos];
			}
		}
	}
	return temp;
}

template<class T>
inline
MyMatrix<T> MyMatrix<T>::operator +(T number) const
{
	MyMatrix<T>temp(matrow, matcol);
	int pos = 0;
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			pos = i * matcol + j;
			temp.mat[pos] = mat[pos] + number;
		}
	}
	return temp;
}


template<class T>
inline
MyMatrix<T>& MyMatrix<T>::operator -=(const MyMatrix<T>& AnotherMat)
{
	int pos = 0;
	if (matrow != AnotherMat.matrow || matcol != AnotherMat.matcol)
		cout << "Size of the two matrices can not match!" << endl;
	else
	{
		for (int i = 0; i < matrow; i++)
		{
			for (int j = 0; j < matcol; j++)
			{
				pos = i * matcol + j;
				mat[pos] -= (AnotherMat.mat)[pos];
			}
		}
	}
	return *this;
}

template<class T>
inline
MyMatrix<T>& MyMatrix<T>::operator -=(T number)
{
	int pos = 0;
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			pos = i * matcol + j;
			mat[pos] -= number;
		}
	}
	return *this;
}

template<class T>
inline
MyMatrix<T> MyMatrix<T>::operator -(const MyMatrix<T>& AnotherMat) const
{
	MyMatrix<T>temp(matrow, matcol);
	int pos = 0;
	if (matrow != AnotherMat.matrow || matcol != AnotherMat.matcol)
		cout << "Size of the two matrices can not match!" << endl;
	else
	{
		for (int i = 0; i < matrow; i++)
		{
			for (int j = 0; j < matcol; j++)
			{
				pos = i * matcol + j;
				temp.mat[pos] = mat[pos] - AnotherMat.mat[pos];
			}
		}
	}
	return temp;
}

template<class T>
inline
MyMatrix<T> MyMatrix<T>::operator -(T number) const
{
	MyMatrix<T>temp(matrow, matcol);
	int pos = 0;
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			pos = i * matcol + j;
			temp.mat[pos] = mat[pos] - number;
		}
	}
	return temp;
}

template<class T>
inline
int MyMatrix<T>::operator *=(const MyMatrix<T>& AnotherMat)
{
	int temprow = matrow;
	int tempcol = AnotherMat.matcol;
	MyMatrix<T> temp(temprow,tempcol);
	int pos = 0;
	if (matcol != AnotherMat.matrow)
	{
		cout << "Size of the two matrices can not match!" << endl;
		return -1;
	}
	else
	{
		for (int i = 0; i < temprow; i++)
		{
			for (int j = 0; j < tempcol; j++)
			{
				for (int k = 0; k < matcol; k++)
				{
					temp.mat[i * tempcol + j] += (mat[i * matcol + k]) * (AnotherMat.mat[k * matcol + j]);
				}
			}
		}
		*this = temp;
		return 1;
	}
}


template<class T>
inline
MyMatrix<T>& MyMatrix<T>::operator *=(T number)
{
	int pos = 0;
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			pos = i * matcol + j;
			mat[pos] *= number;
		}
	}
	return *this;
}


template<class T>
inline
MyMatrix<T> MyMatrix<T>::operator *(T number) const
{
	MyMatrix<T>temp(matrow, matcol);
	int pos = 0;
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			pos = i * matcol + j;
			temp.mat[pos] = mat[pos] * number;
		}
	}
	return temp;
}

template<class T>
inline
MyMatrix<T>::~MyMatrix()
{
	delete[] mat;
}



//其他成员函数
template<class T>
inline
int MyMatrix<T>::DisplayMat() const
{
	for (int i = 0; i < matrow; i++)
	{
		for (int j = 0; j < matcol; j++)
		{
			cout << setw(5) << mat[i * matcol + j] << "  ";
		}
		cout << endl;
	}
	return matrow * matcol;
}

template <class T>
inline
int MyMatrix<T>:: CopyBuffer(T* buffer, int row, int col)
{
	int pos = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			pos = i * col + j;
			mat[pos] = buffer[pos];
		}
	}
	return 0;
}

template <class T>
inline
T MyMatrix<T>::ChangeVal(T NewValue, int row, int col)
{
	row = (row > matrow) ? matrow - 1 : row;
	col = (col > matcol) ? matcol - 1 : col;
	mat[row * matcol + col] = NewValue;
	return NewValue;
}

template <class T>
inline
T MyMatrix<T>::GetVal(int row, int col) const
{
	row = (row > matrow) ? matrow - 1 : row;
	col = (col > matcol) ? matcol - 1 : col;
	return mat[row * matcol + col];
}

template <class T>
ostream& operator<<(ostream& os, MyMatrix<T>outputmat)
{
	int row = outputmat.GetRow();
	int col = outputmat.GetCol();

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			os << setw(5) << (outputmat.GetMat())[i * col + j] << "  ";
		}
		os << endl;
	}
	return os;
}

#endif //__mymatrix__