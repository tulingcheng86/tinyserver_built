#include "lst_timer.h"
#include "../http/http_conn.h"

/**
 * @brief 构造函数，初始化定时器链表
 */
sort_timer_lst::sort_timer_lst()
{
    head = NULL;
    tail = NULL;
}

/**
 * @brief 析构函数，销毁定时器链表中的所有定时器
 */
sort_timer_lst::~sort_timer_lst()
{
    util_timer *tmp = head;
    while (tmp)
    {
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
}

/**
 * @brief 添加定时器到链表中，保持链表有序
 * @param timer 要添加的定时器
 */
void sort_timer_lst::add_timer(util_timer *timer)
{
    if (!timer)
    {
        return;
    }
    if (!head)
    {
        head = tail = timer;
        return;
    }
    if (timer->expire < head->expire)
    {
        timer->next = head;
        head->prev = timer;
        head = timer;
        return;
    }
    add_timer(timer, head);
}

/**
 * @brief 调整定时器在链表中的位置
 * @param timer 要调整的定时器
 */
void sort_timer_lst::adjust_timer(util_timer *timer)
{
    if (!timer)
    {
        return;
    }
    util_timer *tmp = timer->next;
    if (!tmp || (timer->expire < tmp->expire))
    {
        return;
    }
    if (timer == head)
    {
        head = head->next;
        head->prev = NULL;
        timer->next = NULL;
        add_timer(timer, head);
    }
    else
    {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer, timer->next);
    }
}

/**
 * @brief 删除定时器
 * @param timer 要删除的定时器
 */
void sort_timer_lst::del_timer(util_timer *timer)
{
    if (!timer)
    {
        return;
    }
    if ((timer == head) && (timer == tail))
    {
        delete timer;
        head = NULL;
        tail = NULL;
        return;
    }
    if (timer == head)
    {
        head = head->next;
        head->prev = NULL;
        delete timer;
        return;
    }
    if (timer == tail)
    {
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
        return;
    }
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
}

/**
 * @brief 处理定时器链表中的定时事件
 */
void sort_timer_lst::tick()
{
    if (!head)
    {
        return;
    }
    
    time_t cur = time(NULL);
    util_timer *tmp = head;
    while (tmp)
    {
        if (cur < tmp->expire)
        {
            break;
        }
        tmp->cb_func(tmp->user_data);
        head = tmp->next;
        if (head)
        {
            head->prev = NULL;
        }
        delete tmp;
        tmp = head;
    }
}

/**
 * @brief 添加定时器到链表的内部实现，保持链表有序
 * @param timer 要添加的定时器
 * @param lst_head 链表头节点
 */
void sort_timer_lst::add_timer(util_timer *timer, util_timer *lst_head)
{
    util_timer *prev = lst_head;
    util_timer *tmp = prev->next;
    while (tmp)
    {
        if (timer->expire < tmp->expire)
        {
            prev->next = timer;
            timer->next = tmp;
            tmp->prev = timer;
            timer->prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp)
    {
        prev->next = timer;
        timer->prev = prev;
        timer->next = NULL;
        tail = timer;
    }
}

/**
 * @brief 初始化工具类
 * @param timeslot 定时器时间间隔
 */
void Utils::init(int timeslot)
{
    m_TIMESLOT = timeslot;
}

/**
 * @brief 对文件描述符设置非阻塞
 * @param fd 文件描述符
 * @return 旧的文件状态标志
 */
int Utils::setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/**
 * @brief 将文件描述符添加到epoll中
 * @param epollfd epoll文件描述符
 * @param fd 要添加的文件描述符
 * @param one_shot 是否启用EPOLLONESHOT
 * @param TRIGMode 触发模式
 */
void Utils::addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
    epoll_event event;
    event.data.fd = fd;

    if (1 == TRIGMode)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

/**
 * @brief 信号处理函数
 * @param sig 信号值
 */
void Utils::sig_handler(int sig)
{
    // 为保证函数的可重入性，保留原来的errno
    int save_errno = errno;
    int msg = sig;
    send(u_pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

/**
 * @brief 设置信号处理函数
 * @param sig 信号值
 * @param handler 信号处理函数
 * @param restart 是否重启被该信号终止的系统调用
 */
void Utils::addsig(int sig, void(handler)(int), bool restart)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart)
        sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

/**
 * @brief 定时处理任务，重新定时以不断触发SIGALRM信号
 */
void Utils::timer_handler()
{
    m_timer_lst.tick();
    alarm(m_TIMESLOT);
}

/**
 * @brief 显示错误信息
 * @param connfd 连接文件描述符
 * @param info 错误信息
 */
void Utils::show_error(int connfd, const char *info)
{
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

// 初始化静态成员变量
int *Utils::u_pipefd = 0;
int Utils::u_epollfd = 0;

/**
 * @brief 定时器回调函数，处理定时器事件
 * @param user_data 客户端数据
 */
void cb_func(client_data *user_data)
{
    epoll_ctl(Utils::u_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count--;
}
