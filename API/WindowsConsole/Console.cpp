#include "console.h"





void go_to_position(int line, int column) //移动光标到第line行,第column列
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { column,line };//COORD的X为列,Y为行
	SetConsoleCursorPosition(hOut, pos);///移动光标
}




void clear(int line, int column,bool way)//清理控制台上的字符,有两种清理方式
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//获取句柄
	COORD  pos = { column,line };
	CONSOLE_SCREEN_BUFFER_INFO csbi;//存储控制台的信息
	DWORD count_chars_written;//DWORD->unsigned long
	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		return;
	}
	SetConsoleCursorPosition(hOut, pos);

	DWORD size = csbi.dwSize.X - column;//(从第line行的第column列开始,第line行的字符个数)
	if (way == false)//(从第line行的第column列开始,清理之后所有的字符)
	{
		size += ((csbi.dwSize.Y - (line + 1 + 1) * csbi.dwSize.X));//dwSize是一个COORD对象,X为列,Y为行
	}


	if (!FillConsoleOutputCharacter(
		hOut,//句柄
		' ', //要填充的字符
		size,//填充字符的个数
		pos, //从pos位置开始
		&count_chars_written //记录实际填充了多少个字符
	))
	{
		return;
	}

	SetConsoleCursorPosition(hOut, pos);
}
