#pragma once

#include <iostream>
#include <windows.h>
using namespace std;



void go_to_position(int line, int column);  //移动光标到第line行,第column列
void clear(int line, int column,bool way = false); //清理控制台上的字符,有两种清理方式