#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <unistd.h>

#include "CommonDefine.hpp"

using namespace std;


//内建命令cd执行函数
void execute_cd(const vector<string>& tokens)
{
	if(tokens.size()<2)
	{
		return ;
	}
	else if(tokens.size()>2)
	{
		errno=TOO_MANY_ARGUMENTS;
		perror("cd指令参数过多");
		return ;
	}

	chdir(tokens.back().c_str());
}

//内建命令export执行函数
void execute_export(const vector<string>& tokens,vector<string>& newEnv)
{
	if(tokens.size()<2)
	{
		return ;
	}
	else if(tokens.size()>2)
	{
		errno=TOO_MANY_ARGUMENTS;
		perror("export参数过多");
		return ;
	}

	if(newEnv.size()>=MAX_NEW_ENV_COUNT)
	{
		errno=UNDIFINED_ERROR;
		perror("export能导入的环境变量已达到上限");
		return ;
	}

	newEnv.emplace_back(tokens.back());

	// //debug
	// for(int i=0;i<newEnv.size();++i)
	// {
	// 	cout<<"["<<i<<"]:"<<newEnv[i]<<endl;
	// }

	//cout<<"开始执行export:"<<tokens.back().c_str()<<endl;
	putenv(const_cast<char*>(newEnv.back().c_str()));
	//cout<<"执行export完毕"<<endl;
}

//内建命令echo执行函数
void execute_echo(const vector<string>& tokens,const int lastExitCode)
{
	if(tokens.size()<2)
	{
		return ;
	}
	else if(tokens.size()>2)
	{
		errno=TOO_MANY_ARGUMENTS;
		perror("echo参数过多");
		return ;
	}
	
	if(tokens.back()=="$?")
	{
		cout<<lastExitCode<<endl;
	}
	else if(tokens.back().find('$')==0)
	{
		char* value=getenv(tokens.back().c_str()+1);
		if(value)
		{
			cout<<value<<endl;
		}
		else{
			cout<<"没有此环境变量"<<endl;
		}
	}
	else
	{
		cout<<tokens.back()<<endl;
	}
}
