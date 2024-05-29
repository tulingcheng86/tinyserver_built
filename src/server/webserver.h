#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>

#include "../threadpool/threadpool.h"
#include "../http/http_conn.h"

const int MAX_FD = 65536;           ///< 最大文件描述符数
const int MAX_EVENT_NUMBER = 10000; ///< 最大事件数
const int TIMESLOT = 5;             ///< 最小超时单位

/**
 * @class WebServer
 * @brief Web服务器类，封装了服务器的初始化、配置、事件循环等功能
 */
class WebServer
{
public:
    WebServer();
    ~WebServer();

    /**
     * @brief 初始化服务器配置
     * @param port 服务器监听端口
     * @param user 数据库用户名
     * @param passWord 数据库密码
     * @param databaseName 数据库名称
     * @param log_write 是否写日志
     * @param opt_linger 是否优雅关闭连接
     * @param trigmode 触发模式
     * @param sql_num 数据库连接池中的连接数
     * @param thread_num 线程池中的线程数
     * @param close_log 是否关闭日志
     * @param actor_model 并发模型
     */
    void init(int port, string user, string passWord, string databaseName,
              int log_write, int opt_linger, int trigmode, int sql_num,
              int thread_num, int close_log, int actor_model);

    /**
     * @brief 初始化线程池
     */
    void thread_pool();

    /**
     * @brief 初始化数据库连接池
     */
    void sql_pool();

    /**
     * @brief 配置日志写入
     */
    void log_write();

    /**
     * @brief 设置 epoll 的触发模式
     */
    void trig_mode();

    /**
     * @brief 设置并启动监听
     */
    void eventListen();

    /**
     * @brief 执行主事件循环，处理网络事件
     */
    void eventLoop();

    /**
     * @brief 初始化客户端定时器
     * @param connfd 客户端文件描述符
     * @param client_address 客户端地址
     */
    void timer(int connfd, struct sockaddr_in client_address);

    /**
     * @brief 调整定时器
     * @param timer 定时器指针
     */
    void adjust_timer(util_timer *timer);

    /**
     * @brief 处理定时器超时事件
     * @param timer 定时器指针
     * @param sockfd 客户端文件描述符
     */
    void deal_timer(util_timer *timer, int sockfd);

    /**
     * @brief 处理客户端数据
     * @return 成功返回true，失败返回false
     */
    bool dealclientdata();

    /**
     * @brief 处理信号
     * @param timeout 超时标志
     * @param stop_server 停止服务器标志
     * @return 成功返回true，失败返回false
     */
    bool dealwithsignal(bool &timeout, bool &stop_server);

    /**
     * @brief 处理读事件
     * @param sockfd 客户端文件描述符
     */
    void dealwithread(int sockfd);

    /**
     * @brief 处理写事件
     * @param sockfd 客户端文件描述符
     */
    void dealwithwrite(int sockfd);

public:
    // 基础
    int m_port;             ///< 服务器监听端口
    char *m_root;           ///< 服务器根目录
    int m_log_write;        ///< 日志写入方式
    int m_close_log;        ///< 是否关闭日志
    int m_actormodel;       ///< 并发模型

    int m_pipefd[2];        ///< 管道文件描述符
    int m_epollfd;          ///< epoll文件描述符
    http_conn *users;       ///< 客户端连接数组

    // 数据库相关
    connection_pool *m_connPool; ///< 数据库连接池
    string m_user;               ///< 数据库用户名
    string m_passWord;           ///< 数据库密码
    string m_databaseName;       ///< 数据库名称
    int m_sql_num;               ///< 数据库连接池中的连接数

    // 线程池相关
    threadpool<http_conn> *m_pool; ///< 线程池
    int m_thread_num;              ///< 线程池中的线程数

    // epoll_event相关
    epoll_event events[MAX_EVENT_NUMBER]; ///< epoll事件数组

    int m_listenfd;      ///< 监听文件描述符
    int m_OPT_LINGER;    ///< 优雅关闭连接选项
    int m_TRIGMode;      ///< 触发模式
    int m_LISTENTrigmode; ///< 监听触发模式
    int m_CONNTrigmode;   ///< 连接触发模式

    // 定时器相关
    client_data *users_timer; ///< 客户端定时器数组
    Utils utils;              ///< 工具类实例
};

#endif // WEBSERVER_H

