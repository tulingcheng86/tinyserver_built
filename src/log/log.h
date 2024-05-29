#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include "block_queue.h"

using namespace std;

/**
 * @class Log
 * @brief 单例日志类，提供异步/同步日志记录功能
 */
class Log
{
public:
    /**
     * @brief 获取单例实例
     * @return 日志类的单例实例
     */
    static Log *get_instance()
    {
        static Log instance;
        return &instance;
    }

    /**
     * @brief 异步写日志的线程函数
     * @param args 参数
     * @return 无
     */
    static void *flush_log_thread(void *args)
    {
        Log::get_instance()->async_write_log();
        return nullptr;
    }

    /**
     * @brief 初始化日志
     * @param file_name 日志文件名
     * @param close_log 是否关闭日志
     * @param log_buf_size 日志缓冲区大小
     * @param split_lines 最大行数
     * @param max_queue_size 队列最大长度
     * @return 成功返回true，失败返回false
     */
    bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

    /**
     * @brief 写日志
     * @param level 日志级别
     * @param format 格式
     * @param ... 可变参数
     */
    void write_log(int level, const char *format, ...);

    /**
     * @brief 刷新日志缓冲区
     */
    void flush(void);

private:
    Log();
    virtual ~Log();

    /**
     * @brief 异步写日志
     */
    void *async_write_log()
    {
        string single_log;
        // 从阻塞队列中取出一个日志string，写入文件
        while (m_log_queue->pop(single_log))
        {
            m_mutex.lock();
            fputs(single_log.c_str(), m_fp);
            m_mutex.unlock();
        }
        return nullptr;
    }

private:
    char dir_name[128]; ///< 路径名
    char log_name[128]; ///< log文件名
    int m_split_lines;  ///< 日志最大行数
    int m_log_buf_size; ///< 日志缓冲区大小
    long long m_count;  ///< 日志行数记录
    int m_today;        ///< 记录当前时间是那一天
    FILE *m_fp;         ///< 打开log的文件指针
    char *m_buf;        ///< 日志缓冲区
    block_queue<string> *m_log_queue; ///< 阻塞队列
    bool m_is_async;    ///< 是否异步标志
    locker m_mutex;     ///< 互斥锁
    int m_close_log;    ///< 是否关闭日志标志
};

// 日志级别宏定义
#define LOG_DEBUG(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(0, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(1, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(2, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(3, format, ##__VA_ARGS__); Log::get_instance()->flush();}

#endif // LOG_H
