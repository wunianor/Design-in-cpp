//插件使用方法:
    //方法声明:void operator()(int level,const char* format,...);
    //作用:
    //  输出日志
    //参数:
    //  level:日志级别
    //  format,...:与printf()相同


    //方法声明:void setPrintStyle(int printStyle);
    //作用:
    //  设置文件输出方式
    //参数:
    //  printStyle:输出方式选项,一共有三个选项
    //    选项1:SCREEN          //日志输出方式:屏幕
    //    选项2:ONE_FILE        //日志输出方式:单个文件
    //    选项3:CLASSIFY_FILE   //日志输出方式:根据日志类型分类,输出到多个文件

#pragma once


#include <iostream>

#include <ctime>
#include <cstdarg>
#include <unistd.h>
#include <fcntl.h>

#define INFO    1
#define DEBUG   2
#define WARNING 3
#define ERROR   4
#define FATAL   5

#define SCREEN        1  //日志输出方式:屏幕
#define ONE_FILE      2  //日志输出方式:单个文件
#define CLASSIFY_FILE 3  //日志输出方式:根据日志类型分类,输出到多个文件

#define BUFFER_SIZE 1024 //缓冲区最小大小



class Log
{
    int _printStyle; //输出方式
    std::string _filePath; //若将日志输出到文件中,文件的路径
    mode_t _mode; //文件的权限

    //将数字形式的等级转为字符串形式的等级
    std::string levelToStr(int level)
    {
        switch (level)
        {
            case INFO : return "INFO";
            case DEBUG : return "DEBUG";
            case WARNING : return "WANRING";
            case ERROR : return "ERROR";
            case FATAL : return "FATAL";
            default: 
                break;
        }
        return "NULL";
    }

    //将日志写入文件
    void printLogMessageToFile(int level,const std::string& logMessage)
    {
        std::string pathName=_filePath;
        if(level == INFO || level == DEBUG || level == WARNING || level == ERROR || level == FATAL)
        {
            pathName += "."+levelToStr(level);
        }

        int fd=open(pathName.c_str(),O_WRONLY|O_CREAT|O_APPEND,_mode);
        if(fd<0)
        {
            perror("log open file error");
            exit(errno);
        }

        write(fd,logMessage.c_str(),logMessage.size());
    }

    //输出日志内容
    void printLogMessage(int level,const std::string& logMessage)
    {
        switch (_printStyle)
        {
        case SCREEN:
            std::cout<<logMessage; //将日志输出到屏幕上
            break;
        case ONE_FILE:
            printLogMessageToFile(INFO+DEBUG+WARNING+ERROR+FATAL,logMessage); //将日志输出到一个文件中
            break;
        case CLASSIFY_FILE:
            printLogMessageToFile(level,logMessage); //将日志分类输出到多个文件中
            break;
        default:
            break;
        }
    }

    

public:
    Log()
    {
        _printStyle=SCREEN;
        _filePath="./log/log.txt";
        _mode=0666;
    }

    Log(const Log& other)=delete;
    Log& operator=(const Log& other)=delete;

    //方法名:operator()
    //作用:
    //  输出日志
    //参数:
    //  level:日志级别
    //  format,...:与printf()相同
    void operator()(int level,const char* format,...)
    {
        //获取字符串形式的等级
        std::string levelStr=levelToStr(level);

        //获取时间,存入时间缓冲区
        time_t curTime=time(nullptr);
        struct tm* tm=localtime(&curTime);
        char timeBuffer[BUFFER_SIZE];
        snprintf(timeBuffer,sizeof(timeBuffer),\
                "%04d-%02d-%02d %02d:%02d:%02d",\
                tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,\
                tm->tm_hour,tm->tm_min,tm->tm_sec \
                );
        
        //将传入的信息,存入信息缓冲区
        char messageBuffer[BUFFER_SIZE];
        va_list s; //va_list 本质是char*
        va_start(s,format); //本质是s=&format+1(指针+1并非值加1,与指针类型有关)
        vsnprintf(messageBuffer,sizeof(messageBuffer),format,s);
        va_end(s); //将s置为NULL
        
        //将字符串形式的等级,时间缓冲区,信息缓冲区的内容,整合写入到日志缓冲区
        char logMessageBuffer[BUFFER_SIZE*2];
        snprintf(logMessageBuffer,sizeof(logMessageBuffer),"[%s][%s]%s\n",levelStr.c_str(),timeBuffer,messageBuffer);

        //打印日志
        printLogMessage(level,logMessageBuffer);
    }

    //方法名:setPrintStyle
    //作用:
    //  设置文件输出方式
    //参数:
    //  printStyle:输出方式选项,一共有三个选项
    //    选项1:SCREEN          //日志输出方式:屏幕
    //    选项2:ONE_FILE        //日志输出方式:单个文件
    //    选项3:CLASSIFY_FILE   //日志输出方式:根据日志类型分类,输出到多个文件
    void setPrintStyle(int printStyle)
    {
        _printStyle=printStyle;
    }
};


