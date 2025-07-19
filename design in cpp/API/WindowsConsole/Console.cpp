#include "console.h"





void go_to_position(int line, int column) //�ƶ���굽��line��,��column��
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { column,line };//COORD��XΪ��,YΪ��
	SetConsoleCursorPosition(hOut, pos);///�ƶ����
}




void clear(int line, int column,bool way)//�������̨�ϵ��ַ�,����������ʽ
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ���
	COORD  pos = { column,line };
	CONSOLE_SCREEN_BUFFER_INFO csbi;//�洢����̨����Ϣ
	DWORD count_chars_written;//DWORD->unsigned long
	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		return;
	}
	SetConsoleCursorPosition(hOut, pos);

	DWORD size = csbi.dwSize.X - column;//(�ӵ�line�еĵ�column�п�ʼ,��line�е��ַ�����)
	if (way == false)//(�ӵ�line�еĵ�column�п�ʼ,����֮�����е��ַ�)
	{
		size += ((csbi.dwSize.Y - (line + 1 + 1) * csbi.dwSize.X));//dwSize��һ��COORD����,XΪ��,YΪ��
	}


	if (!FillConsoleOutputCharacter(
		hOut,//���
		' ', //Ҫ�����ַ�
		size,//����ַ��ĸ���
		pos, //��posλ�ÿ�ʼ
		&count_chars_written //��¼ʵ������˶��ٸ��ַ�
	))
	{
		return;
	}

	SetConsoleCursorPosition(hOut, pos);
}
