#ifndef LST_TIMER
#define LST_TIMER

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <time.h>
#include "../log/log.h"

/**
 * @struct client_data
 * @brief 保存客户端数据的结构体
 */
struct client_data {
    sockaddr_in address; ///< 客户端地址
    int sockfd;          ///< 客户端socket文件描述符
    class util_timer *timer; ///< 定时器
};

/**
 * @class util_timer
 * @brief 定时器类
 */
class util_timer {
public:
    util_timer() : prev(NULL), next(NULL) {}

public:
    time_t expire; ///< 定时器生效的绝对时间

    // 定时器回调函数
    void (*cb_func)(client_data *);
    client_data *user_data; ///< 客户数据
    util_timer *prev; ///< 前一个定时器
    util_timer *next; ///< 后一个定时器
};

/**
 * @class sort_timer_lst
 * @brief 升序定时器链表
 */
class sort_timer_lst {
public:
    sort_timer_lst();
    ~sort_timer_lst();

    void add_timer(util_timer *timer);
    void adjust_timer(util_timer *timer);
    void del_timer(util_timer *timer);
    void tick();

private:
    void add_timer(util_timer *timer, util_timer *lst_head);

    util_timer *head; ///< 链表头节点
    util_timer *tail; ///< 链表尾节点
};

/**
 * @class Utils
 * @brief 工具类，包含一些常用函数和变量
 */
class Utils {
public:
    Utils() {}
    ~Utils() {}

    void init(int timeslot);

    // 对文件描述符设置非阻塞
    int setnonblocking(int fd);

    // 将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);

    // 信号处理函数
    static void sig_handler(int sig);

    // 设置信号函数
    void addsig(int sig, void(handler)(int), bool restart = true);

    // 定时处理任务，重新定时以不断触发SIGALRM信号
    void timer_handler();

    void show_error(int connfd, const char *info);

public:
    static int *u_pipefd; ///< 管道文件描述符
    sort_timer_lst m_timer_lst; ///< 定时器链表
    static int u_epollfd; ///< epoll文件描述符
    int m_TIMESLOT; ///< 定时时间片
};

// 定时器回调函数
void cb_func(client_data *user_data);

#endif // LST_TIMER
