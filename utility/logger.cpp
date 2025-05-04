#include "ctime" 
#include <string>
#include <stdexcept> // for runtime_error
#include <iostream> // for cout, endl
#include <cstdarg> // for va_list, va_start, va_end
#include "logger.h"
using std::cout;
using std::endl;
using std::runtime_error;
using namespace wuah::logger;

const char* Logger::m_level[LEVEL_COUNT] = 
{ 
    "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
};

Logger::Logger() : m_loglevel(DEBUG), m_maxlogfilesize(0), m_curlogfilesize(0)
{

}

Logger::~Logger()
{
    close();
}

void Logger::open(const string& filename)
{
    m_filename = filename; // 将传入的文件名赋值给成员变量 m_filename

    m_fileoutput.open(filename, std::fstream::out | std::fstream::app);
    if(m_fileoutput.fail()) // 检查文件是否打开失败
    {
        throw runtime_error("Failed to open log file for writing"); // 如果打开失败，抛出一个运行时错误
    }
    cout<<"open log file success"<<endl;
    m_fileoutput.seekp(0, std::fstream::end); // 将文件输出位置指针（put pointer）移动到文件末尾
    m_curlogfilesize = m_fileoutput.tellp(); // 获取当前文件输出位置指针的位置，并赋值给 m_curlogfilesize
}

void Logger::close()
{
    m_fileoutput.close();
    cout << "close log file success" << endl;
}

void Logger::log(Level level, const char* filename, int line, const char* format, ...)
{
    // 如果当前日志级别大于传入的日志级别，则不记录日志
    if(m_loglevel > level)
    {
        return;
    }

    // 检查文件流是否处于失败状态
    if(m_fileoutput.fail())
    {
        // 如果文件流失败，抛出运行时错误
        throw runtime_error("Failed to open log file " + m_filename + " for writing");
    }

    // 获取当前时间
    time_t now = time(nullptr);
    // 将时间转换为本地时间
    struct tm* timePtr = localtime(&now);
    char timeStamp[32];
    // 初始化时间戳数组
    memset(timeStamp, 0, sizeof(timeStamp));
    // 格式化时间戳为 "YYYY-MM-DD HH:MM:SS"
    strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", timePtr);

    // 定义日志格式字符串
    const char* ftm = "%s %s:%d %s";
    // 计算格式化后的日志消息的长度（不包括终止符）
    int size = snprintf(nullptr, 0, ftm, timeStamp, filename, line, m_level[level]);
    if(size > 0)
    {
        // 分配足够的空间存储格式化后的日志消息
        char* buffer = new char[size+1];
        // 初始化缓冲区
        memset(buffer, 0, size+1);
        
        // 格式化日志消息
        snprintf(buffer, size+1, ftm, timeStamp, filename, line, m_level[level]);
        // 确保字符串以空字符结尾
        buffer[size] = '\0';
        // 输出日志消息到标准输出
        cout << buffer << endl;
        // 输出日志消息到文件
        m_fileoutput << buffer;

        // 更新当前日志文件的大小
        m_curlogfilesize += size;
        // 释放分配的缓冲区
        delete[] buffer;
    }

    // 获取可变参数列表
    va_list args;
    va_start(args, format);
    // 计算格式化后的日志内容的长度（不包括终止符）
    size = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if(size > 0)
    {
        // 分配足够的空间存储格式化后的日志内容
        char* content = new char[size+1];
        // 初始化缓冲区
        memset(content, 0, size+1);

        // 获取可变参数列表
        va_start(args, format);
        // 格式化日志内容
        vsnprintf(content, size+1, format, args);
        va_end(args);

        // 确保字符串以空字符结尾
        content[size] = '\0';
        // 输出日志内容到标准输出
        cout << content << endl;
        // 输出日志内容到文件
        m_fileoutput << content;

        // 更新当前日志文件的大小
        m_curlogfilesize += size;
        // 释放分配的缓冲区
        delete[] content;
    }
    
    m_fileoutput << endl; // 输出换行符
    // 刷新文件输出缓冲区，确保所有数据被写入文件
    m_fileoutput.flush();

    // 如果当前日志文件大小超过最大允许大小，则进行日志文件轮转
    if(m_curlogfilesize > m_maxlogfilesize && m_maxlogfilesize > 0)
    {
        rotate();
    }
}

void Logger::rotate()
{
    close();
    time_t now = time(nullptr);
    struct tm* timePtr = localtime(&now);
    char timeStamp[32];
    memset(timeStamp, 0, sizeof(timeStamp));
    strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d_%H-%M-%S", timePtr);
    string newFileName = m_filename + "." + timeStamp;
    if(rename(m_filename.c_str(), newFileName.c_str()) != 0)
    {
        throw runtime_error("Failed to rename log file " + m_filename + " to " + newFileName);
    }
    open(m_filename);
}

void Logger::set_log_level(Level level)
{
    m_loglevel = level;
}

void Logger::set_maxlogfile_size(int size)
{
    m_maxlogfilesize = size;
}