/*************************************************************
* 循环数组实现的阻塞队列，m_back = (m_back + 1) % m_max_size;  
* 线程安全，每个操作前都要先加互斥锁，操作完后，再解锁
**************************************************************/

#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "../lock/locker.h"

using namespace std;

/**
 * @class block_queue
 * @brief 使用循环数组实现的线程安全阻塞队列
 */
template <class T>
class block_queue
{
public:
    /**
     * @brief 构造函数
     * @param max_size 队列的最大容量，默认为1000
     */
    block_queue(int max_size = 1000)
    {
        if (max_size <= 0)
        {
            exit(-1);
        }

        m_max_size = max_size;
        m_array = new T[max_size];
        m_size = 0;
        m_front = -1;
        m_back = -1;
    }

    /**
     * @brief 清空队列
     */
    void clear()
    {
        m_mutex.lock();
        m_size = 0;
        m_front = -1;
        m_back = -1;
        m_mutex.unlock();
    }

    /**
     * @brief 析构函数
     */
    ~block_queue()
    {
        m_mutex.lock();
        if (m_array != NULL)
            delete [] m_array;

        m_mutex.unlock();
    }

    /**
     * @brief 判断队列是否满
     * @return 队列满返回true，否则返回false
     */
    bool full() 
    {
        m_mutex.lock();
        if (m_size >= m_max_size)
        {
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }

    /**
     * @brief 判断队列是否为空
     * @return 队列空返回true，否则返回false
     */
    bool empty() 
    {
        m_mutex.lock();
        if (0 == m_size)
        {
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }

    /**
     * @brief 返回队首元素
     * @param value 队首元素
     * @return 成功返回true，失败返回false
     */
    bool front(T &value) 
    {
        m_mutex.lock();
        if (0 == m_size)
        {
            m_mutex.unlock();
            return false;
        }
        value = m_array[m_front];
        m_mutex.unlock();
        return true;
    }

    /**
     * @brief 返回队尾元素
     * @param value 队尾元素
     * @return 成功返回true，失败返回false
     */
    bool back(T &value) 
    {
        m_mutex.lock();
        if (0 == m_size)
        {
            m_mutex.unlock();
            return false;
        }
        value = m_array[m_back];
        m_mutex.unlock();
        return true;
    }

    /**
     * @brief 返回队列中元素个数
     * @return 队列中元素个数
     */
    int size() 
    {
        int tmp = 0;

        m_mutex.lock();
        tmp = m_size;
        m_mutex.unlock();
        return tmp;
    }

    /**
     * @brief 返回队列的最大容量
     * @return 队列的最大容量
     */
    int max_size()
    {
        int tmp = 0;

        m_mutex.lock();
        tmp = m_max_size;
        m_mutex.unlock();
        return tmp;
    }

    /**
     * @brief 往队列添加元素
     * @param item 待添加的元素
     * @return 成功返回true，失败返回false
     */
    bool push(const T &item)
    {
        m_mutex.lock();
        if (m_size >= m_max_size)
        {
            m_cond.broadcast();
            m_mutex.unlock();
            return false;
        }

        m_back = (m_back + 1) % m_max_size;
        m_array[m_back] = item;
        m_size++;
        m_cond.broadcast();
        m_mutex.unlock();
        return true;
    }

    /**
     * @brief 从队列中取出元素，如果队列为空，则等待
     * @param item 取出的元素
     * @return 成功返回true，失败返回false
     */
    bool pop(T &item)
    {
        m_mutex.lock();
        while (m_size <= 0)
        {
            if (!m_cond.wait(m_mutex.get()))
            {
                m_mutex.unlock();
                return false;
            }
        }

        m_front = (m_front + 1) % m_max_size;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();
        return true;
    }

    /**
     * @brief 从队列中取出元素，增加了超时处理
     * @param item 取出的元素
     * @param ms_timeout 超时时间（毫秒）
     * @return 成功返回true，失败返回false
     */
    bool pop(T &item, int ms_timeout)
    {
        struct timespec t = {0, 0};
        struct timeval now = {0, 0};
        gettimeofday(&now, NULL);
        m_mutex.lock();
        if (m_size <= 0)
        {
            t.tv_sec = now.tv_sec + ms_timeout / 1000;
            t.tv_nsec = (ms_timeout % 1000) * 1000;
            if (!m_cond.timewait(m_mutex.get(), t))
            {
                m_mutex.unlock();
                return false;
            }
        }

        if (m_size <= 0)
        {
            m_mutex.unlock();
            return false;
        }

        m_front = (m_front + 1) % m_max_size;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();
        return true;
    }

private:
    locker m_mutex; ///< 互斥锁
    cond m_cond;    ///< 条件变量

    T *m_array;     ///< 队列数组
    int m_size;     ///< 当前元素数量
    int m_max_size; ///< 队列最大容量
    int m_front;    ///< 队首索引
    int m_back;     ///< 队尾索引
};

#endif // BLOCK_QUEUE_H
