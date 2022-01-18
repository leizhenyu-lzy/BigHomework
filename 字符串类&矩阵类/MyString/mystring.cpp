/* 1950083 刘智宇 自动化 */
#include "mystring.h"
#include <iostream>

using namespace std;

const char stars[9] = "********";

int main()
{
	cout << stars << stars << "测试开始" << stars << stars << endl << endl;
#pragma region <test1>
	//测试构造函数
	cout << stars << "测试构造函数" << stars << endl << endl;

	cout << "测试默认构造函数（若为空会有系统提醒）" << endl;
	MyString defaultconstruct;
	cout << "默认构造函数输出内容为：" << defaultconstruct << endl << endl;

	cout << "测试整数、浮点数构造（浮点数构造函数精确到小数点后一位）" << endl;
	MyString intconstruct(12345);
	cout << "整数构造函数输出内容为：" << intconstruct << endl << endl;
	MyString floatconstruct(12345.6789f);
	cout << "单精度浮点数构造函数输出内容为：" << floatconstruct << endl << endl;
	MyString doubleconstruct(9876.54321);
	cout << "双精度浮点数构造函数输出内容为：" << doubleconstruct << endl << endl;

	cout << "测试一般构造函数" << endl;
	char temptestchar[10] = "abcdefghi";
	MyString normalconstruct(temptestchar);
	cout << "一般构造函数输出内容为：" << normalconstruct << endl << endl;

	cout << "测试拷贝构造函数" << endl;
	MyString copyconstruct(normalconstruct);
	cout << "拷贝构造函数输出内容为：" << copyconstruct << endl << endl;


#pragma endregion

#pragma region <test2>
	//测试运算符重载
	MyString test1("abc");
	MyString test2("def");
	MyString test3("ghi");

	cout << stars << "运算符重载" << stars << endl << endl;

	cout << "测试加法+" << endl;
	cout << test1 << " + " << test2 << " + " << test3 << " = " << (test1 + (test2 + test3)) << endl;
	MyString temp("abc");
	cout << "测试加法+=，加浮点数（保留一位小数），输出结果为：" << (temp += 123.45) << endl;
	cout << "测试加法+=，加字母，输出结果为：" << (temp += "ojbk") << endl << endl;

	cout << "测试减法-" << endl;

	cout << "测试减法-=" << endl;
	MyString testtobeSubchar("caababbc");
	MyString testtobeSubcopychar(testtobeSubchar);
	MyString testSubchar("ab");
	cout << "从" << testtobeSubchar << "中删除" << testSubchar << "，输出结果为" << (testtobeSubcopychar -= testSubchar) << endl;
	MyString testtobeSubint("123123");
	MyString testtobeSubcopyint(testtobeSubint);
	MyString testSubint("23");
	cout << "从" << testtobeSubint << "中删除" << testSubint << "，输出结果为" << (testtobeSubcopyint -= testSubint) << endl;
	cout << endl;


	cout << "测试拷贝赋值函数（对于=的重载）" << endl;
	MyString copyassign = normalconstruct;
	cout << "对于非自我赋值，拷贝构造赋值输出内容为：" << copyassign << endl;
	copyassign = copyassign;
	cout << "对于自我赋值，拷贝构造赋值输出内容为：" << copyassign << endl << endl;

	cout << "测试流插入运算符<<重载" << endl;
	cout << "上述输出均是通过流插入运算符号<<输出的，这里就不再赘述" << endl << endl;
#pragma endregion

#pragma region <test3>
	//测试其他方法
	cout << stars << "其他方法" << stars << endl << endl;
	MyString Othermethod("abcdefghi");
	cout << "测试获取成员" << endl;
	cout << "数据实际length为9，成员函数返回结果为：" << Othermethod.get_length() << endl;
	cout << "数据为：abcdefghi，成员函数返回结果为：" << Othermethod.get_mydata() << endl;
	cout << endl;

	cout << "测试获取修改成员" << endl;
	MyString ChangeValueTest("0001000");
	cout << "对数据进行修改，修改是否成功（1表示成功，-1表示成功）:" << ChangeValueTest.ChangeData(3, '0') << " 。修改结果为：" << ChangeValueTest << endl << endl;

	cout << "测试动态内存分配" << endl;
	cout << "上述所有的构造函数、赋值函数几乎都用到了动态内存分配，这里不再赘述" << endl << endl;

#pragma endregion

#pragma region <test4>
	//测试特殊函数
	cout << stars << "特殊函数" << stars << endl << endl;
	cout << "测试subString函数" << endl;
	MyString testsubString("abcdefg");
	MyString testsub = testsubString.subString(2, 3);
	cout << "subString的结果为：" << testsub << endl;
	cout << "subString对于超出范围的处理结果：" << testsubString.subString(-1, 8) << endl << endl;

	cout << "测试toInt、toFloat函数" << endl;
	cout << "测试对正数的处理" << endl;
	cout << MyString("123.45").toInt() << endl;
	cout << MyString("123.45").toFloat() << endl;
	cout << "测试对负数的处理" << endl;
	cout << MyString("-123.45").toInt() << endl;
	cout << MyString("-123.45").toFloat() << endl;
	cout << "测试对无法转换的数的处理（-2表示失败，无法转换）" << endl;
	cout << MyString("abc-123.45").toInt() << endl;
	cout << MyString("abc-123.45").toFloat() << endl;
	cout << endl;

	cout << "测试find函数（-1表示找不到）" << endl;
	const char tobefind[] = "casdfafu18uddf7a81 3yrfwehfadsf";
	MyString tempFind(tobefind);
	//0         1         2         3         4
	//01234567890123456789012345678901234567890
	//casdfafu18uddf7a81 3yrfwehfadsf
	cout << "在 " << tempFind << " 中寻找" << endl;
	cout << "寻找\"1 3\"，预期位置17，成员函数实际返回的位置: " << tempFind.find("1 3") << endl;//17
	cout << "寻找\"   \"，预期位置-1，成员函数实际返回的位置: " << tempFind.find("   ") << endl;//-1
	cout << "寻找\"dsf\"，预期位置28，成员函数实际返回的位置: " << tempFind.find("dsf") << endl;//28
	cout << "寻找\"cas\"，预期位置0，成员函数实际返回的位置: " << tempFind.find("cas") << endl;//0
	cout << endl;

#pragma endregion


	cout << stars << stars << "测试结束" << stars << stars << endl << endl;

	return 0;
}

//1、字符串类
// //字符串类名：MyString
//a、支持不同长度的字符串
//b、构造方法：	(1)默认构造；									ok
//				(2)整数，浮点数构造；								ok
//				(3)一般构造； 									ok
//				(4)复制构造；									ok
//c、运算符重载：	(1)加法 + ；										ok
//				(2)减法 - ；										ok
//				(3)输出流运算符 <<								ok
//d、其他方法：	(1)获取成员；									ok
//				(2)修改成员；									ok
//				(3)动态内存分配									ok
//MyString temp;												ok
//MyString temp(567)）;//字符串内部存“567”						ok
//MyString temp(buffer);//把以0结尾的buffer转换成MyString			ok
//MyString  temp2(temp);										ok
//MyString  temp3;												ok
//temp = temp + temp2;											ok
//temp += 23.0;//给字符串后面增加字母“23.0”，保留小数后面1位小数。	ok
//temp -= 23；  //把字符串中的“23”子串删除						ok
//temp -= “23”；//把字符串中的“23”子串删除						ok
//temp.subString(m, n);//返回从m开始的n个字母；					ok
//temp.find(temp2);//返回temp2子串在temp中的位置。-1表示没有找到	ok
//temp.toInt();//数值串转换成整数，-2表示失败，无法转换；			ok
//temp.tofloat();//数值串转换成浮点数，-2表示失败，无法转换；		ok
//cout << temp << endl;//输出字符串到屏幕							ok
//cout << temp.length << endl;//输出字符串长度到屏幕				ok

