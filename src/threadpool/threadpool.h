#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "../lock/locker.h"
#include "../CGImysql/sql_connection_pool.h"

/**
 * @class threadpool
 * @brief 线程池模板类，用于管理和调度线程处理任务
 */
template <typename T>
class threadpool {
public:
    /**
     * @brief 构造函数
     * @param actor_model 模型标志
     * @param connPool 数据库连接池
     * @param thread_number 线程数量
     * @param max_requests 请求队列最大长度
     * @throw std::exception 如果线程池初始化失败
     */
    threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_requests = 10000);

    /**
     * @brief 析构函数
     */
    ~threadpool();

    /**
     * @brief 添加任务到请求队列
     * @param request 请求任务
     * @param state 状态
     * @return 成功返回true，失败返回false
     */
    bool append(T *request, int state);

    /**
     * @brief 仅添加任务到请求队列
     * @param request 请求任务
     * @return 成功返回true，失败返回false
     */
    bool append_p(T *request);

private:
    /**
     * @brief 工作线程运行的函数，不断从工作队列中取出任务并执行
     * @param arg 线程池实例
     * @return NULL
     */
    static void *worker(void *arg);

    /**
     * @brief 线程池运行函数，执行具体任务
     */
    void run();

private:
    int m_thread_number;        ///< 线程池中的线程数量
    int m_max_requests;         ///< 请求队列中允许的最大请求数量
    pthread_t *m_threads;       ///< 描述线程池的数组，大小为m_thread_number
    std::list<T *> m_workqueue; ///< 请求队列
    locker m_queuelocker;       ///< 保护请求队列的互斥锁
    sem m_queuestat;            ///< 是否有任务需要处理的信号量
    connection_pool *m_connPool; ///< 数据库连接池
    int m_actor_model;          ///< 模型切换标志
};

template <typename T>
threadpool<T>::threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_requests)
    : m_actor_model(actor_model), m_thread_number(thread_number), m_max_requests(max_requests), m_threads(nullptr), m_connPool(connPool)
{
    if (thread_number <= 0 || max_requests <= 0)
        throw std::exception();

    m_threads = new pthread_t[m_thread_number];
    if (!m_threads)
        throw std::exception();

    for (int i = 0; i < thread_number; ++i)
    {
        if (pthread_create(m_threads + i, nullptr, worker, this) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }
        if (pthread_detach(m_threads[i]) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template <typename T>
threadpool<T>::~threadpool()
{
    delete[] m_threads;
}

template <typename T>
bool threadpool<T>::append(T *request, int state)
{
    m_queuelocker.lock();
    if (m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    request->m_state = state;
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template <typename T>
bool threadpool<T>::append_p(T *request)
{
    m_queuelocker.lock();
    if (m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template <typename T>
void *threadpool<T>::worker(void *arg)
{   
    // 将参数 arg 转换为 threadpool<T> 类型的指针
    threadpool *pool = static_cast<threadpool *>(arg);
    // 调用线程池实例的 run 方法
    pool->run();
    // 返回线程池的指针
    return pool;
}

template <typename T>
void threadpool<T>::run()
{
    // 无限循环，确保线程池持续运行
    while (true)
    {
        // 等待任务队列中有任务可处理
        m_queuestat.wait();

        // 锁定任务队列，以保证线程安全
        m_queuelocker.lock();

        // 如果任务队列为空，解锁并继续等待
        if (m_workqueue.empty())
        {
            m_queuelocker.unlock();
            continue;
        }

        // 从任务队列中取出第一个任务
        T *request = m_workqueue.front();
        m_workqueue.pop_front();

        // 解锁任务队列，以便其他线程能访问
        m_queuelocker.unlock();

        // 如果任务为空，继续下一次循环
        if (!request)
            continue;

        // 如果使用的是 Reactor 模型
        if (1 == m_actor_model)
        {
            // 根据请求的状态执行读或写操作
            if (0 == request->m_state)
            {
                // 处理读操作
                if (request->read_once())
                {
                    // 如果读取成功，设置改进标志，并处理请求
                    request->improv = 1;
                    connectionRAII mysqlcon(&request->mysql, m_connPool);
                    request->process();
                }
                else
                {
                    // 如果读取失败，设置定时器标志
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
            else
            {
                // 处理写操作
                if (request->write())
                {
                    // 如果写入成功，设置改进标志
                    request->improv = 1;
                }
                else
                {
                    // 如果写入失败，设置定时器标志
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
        }
        else
        {
            // 如果使用的是 Proactor 模型，直接处理请求
            connectionRAII mysqlcon(&request->mysql, m_connPool);
            request->process();
        }
    }
}

#endif // THREADPOOL_H
