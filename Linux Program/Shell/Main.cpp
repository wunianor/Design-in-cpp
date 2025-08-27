#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <functional>

#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "BuiltInCommand.hpp"
#include "CommonDefine.hpp"

using namespace std;

//输出命令行提示符
void printPrompt()
{
	//获取用户名
	uid_t uid=getuid();
	passwd* pw=getpwuid(uid);
	if(pw == nullptr)
	{
		perror("获取用户名失败");
		exit(errno);
	}

	//获取主机名
	char hostName[256];
	if(gethostname(hostName,sizeof(hostName))!=0)
	{
		perror("获取主机名失败");
		exit(errno);
	}

	//获取工作路径
	char buf[1024];
	if(getcwd(buf,sizeof(buf))==nullptr)
	{
		perror("获取工作路径失败");
		exit(errno);
	}

	//获取权限符
	char permissionSymbol= uid==0 ? '#' : '$';
	
	//打印命令行提示符	
	cout<<LEFT
		<<pw->pw_name
		<<"@"
		<<hostName
		<<" "
		<<buf 
		<<RIGHT
		<<permissionSymbol
		<<" ";
}

//命令行输入
void input(string& commandLine)
{
	commandLine.clear();
	getline(cin,commandLine);
}

//分割命令行输入
vector<string> split(string& commandLine)
{
	vector<string> tokens;
	stringstream ss(commandLine);
	string token;
	while(ss>>token)
	{
		tokens.emplace_back(token);
	}

	return tokens;
}

//对于一些命令增加/修改一些选项	
void changeTokens(vector<string>& tokens)
{
	if(tokens.size()==0)
	{
		return ;
	}

	if(tokens[0]=="ls")
	{
		//对ls指令的结果增加颜色
		tokens.emplace_back("--color");
	}
	else if(tokens[0]=="ll")
	{
		tokens[0]="ls";
		tokens.emplace_back("-l");
		tokens.emplace_back("--color"); //对ls指令的结果增加颜色
	}
}

//解析命令行参数
void getArgv(vector<string>& tokens,char* argv[])
{
	size_t i=0;
	for(;i<tokens.size();++i)
	{
		argv[i]=const_cast<char*>(tokens[i].c_str());
	}

	//exec*函数参数argv数组的最后一个元素必须为nullptr
	argv[i]=nullptr;
}

//执行普通命令
void executeNormalCommand(char* argv[],int& lastExitCode)
{

	pid_t id=fork();
	if(id>0)
	{
		//父进程
		int status=0;
		pid_t ret=waitpid(id,&status,0);
		if(ret>0)
		{
			lastExitCode=WEXITSTATUS(status);
			//cout<<"等待子进程成功,lastExitCode="<<lastExitCode<<endl;
		}
		else
		{
			perror("等待子进程失败");
		}
	}
	else if(id==0)
	{
		//子进程
		if(execvp(argv[0],argv)==-1)
		{
			perror("子进程替换失败");
		}
		//替换成功后不会执行这一句
		exit(errno);
	}
	else
	{
		perror("创建子进程失败");
		return ;
	}
}


int main()
{
	//最后一次运行的进程的退出码
	int lastExitCode=0;

	//保存通过export新增的环境变量
	vector<string> newEnv;
	newEnv.reserve(MAX_NEW_ENV_COUNT);//防止扩容导致新增的环境变量的字符串的地址发生改变,与putenv()的原理有关

	//内建命令及其对应执行函数 表
	unordered_map<string,function<void(vector<string>&)>> builtInCommand={
		{
			"cd",
			execute_cd 
		},
		{
			"export",
			bind(execute_export,placeholders::_1,ref(newEnv))
		},
		{
			"echo",
			bind(execute_echo,placeholders::_1,ref(lastExitCode))
		}

	};


	while(1)
	{
		//打印命令行提示符
		printPrompt();

		//输入命令行
		string commandLine;
		input(commandLine);

		//对字符串进行分割
		vector<string> tokens=split(commandLine);
		if(tokens.size()==0) continue; //如果分割后tokens为空

		//对于一些命令增加一些选项
		changeTokens(tokens);

		//解析输入的命令和选项
		char* argv[tokens.size()+1]{nullptr};
		getArgv(tokens,argv);
		
		//如果是内建命令,由shell亲自执行
		if(builtInCommand.count(tokens[0]))
		{
			builtInCommand[tokens[0]](tokens);
			lastExitCode=0;
			continue;
		}

		//执行普通命令(通过创建进程,替换进程,等待进程的方式执行)
		executeNormalCommand(argv,lastExitCode);
	}

	return 0;
}

