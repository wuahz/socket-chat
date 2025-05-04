#pragma once

#include <cstring> // for memset
#include <fstream> // for ofstream
#include <singleton.hpp> // for Singleton
using std::string;
using std::ofstream;
using namespace wuah::singleton;

namespace wuah
{
namespace logger
{

#define LOG_DEBUG(format, ...) Singleton<Logger>::get_instance().log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Singleton<Logger>::get_instance().log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Singleton<Logger>::get_instance().log(Logger::WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Singleton<Logger>::get_instance().log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) Singleton<Logger>::get_instance().log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

class Logger
{
    friend class Singleton<Logger>; /// 声明友元类，以便单例对象可以调用私有方法

public:
    enum Level
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };

    static Singleton<Logger>& get_instance(); // 获取单例对象

    void open(const string& fileName); // 打开日志文件
    void close(); // 关闭日志文件
    void log(Level level, const char* fileName, int line, const char* format, ...); // 日志输出
    
    void rotate(); // 日志文件切割
    void set_log_level(Level level); // 设置日志级别
    void set_maxlogfile_size(int size); // 设置日志文件最大大小

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    ~Logger();

private:
    string m_filename; // 日志文件名
    ofstream m_fileoutput; // 日志文件输出流
    Level m_loglevel; // 日志级别
    int m_maxlogfilesize; // 日志文件最大大小
    int m_curlogfilesize; // 当前日志文件大小
    static const char* m_level[LEVEL_COUNT]; // 日志级别名称
};

} // logger
} // wuah