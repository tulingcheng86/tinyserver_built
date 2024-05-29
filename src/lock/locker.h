#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/**
 * @class sem
 * @brief 信号量类，提供同步机制
 */
class sem
{
private:
    sem_t m_sem;  ///< POSIX信号量
public:

    /**
     * @brief 默认构造函数，初始化信号量为0
     * @throw std::exception 如果初始化失败
     */
    sem() {
        if (sem_init(&m_sem, 0, 0) != 0) {
            throw std::exception();
        }
    }

    /**
     * @brief 带参数的构造函数，初始化信号量为num
     * @param num 信号量初始值
     * @throw std::exception 如果初始化失败
     */
    sem(int num) {
        if (sem_init(&m_sem, 0, num) != 0) {
            throw std::exception();
        }
    }

    /**
     * @brief 析构函数，销毁信号量
     */
    ~sem() {
        sem_destroy(&m_sem);
    }

public:
    /**
     * @brief 等待信号量
     * @return 成功返回true，失败返回false
     */
    bool wait() {
        return sem_wait(&m_sem) == 0;
    }

    /**
     * @brief 增加信号量
     * @return 成功返回true，失败返回false
     */
    bool post() {
        return sem_post(&m_sem) == 0;
    }

};


/**
 * @class locker
 * @brief 互斥锁类，提供线程间互斥机制
 */
class locker
{
private:
    pthread_mutex_t m_mutex;  ///< POSIX互斥锁

public:
    /**
     * @brief 构造函数，初始化互斥锁
     * @throw std::exception 如果初始化失败
     */
    locker() {
        if (pthread_mutex_init(&m_mutex, NULL) != 0) {
            throw std::exception();
        }
    }

    /**
     * @brief 析构函数，销毁互斥锁
     */
    ~locker() {
        pthread_mutex_destroy(&m_mutex);
    }

public:
    /**
     * @brief 上锁
     * @return 成功返回true，失败返回false
     */
    bool lock() {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    /**
     * @brief 解锁
     * @return 成功返回true，失败返回false
     */
    bool unlock() {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    /**
     * @brief 获取互斥锁
     * @return 互斥锁指针
     */
    pthread_mutex_t *get() {
        return &m_mutex;
    }
};


/**
 * @class cond
 * @brief 条件变量类，提供线程间同步机制
 */
class cond
{
private:
    pthread_cond_t m_cond;  ///< POSIX条件变量

public:
    /**
     * @brief 构造函数，初始化条件变量
     * @throw std::exception 如果初始化失败
     */
    cond() {
        if (pthread_cond_init(&m_cond, NULL) != 0) {
            throw std::exception();
        }
    }
    /**
     * @brief 析构函数，销毁条件变量
     */
    ~cond() {
        pthread_cond_destroy(&m_cond);
    }

public:
    /**
     * @brief 等待条件变量
     * @param m_mutex 互斥锁
     * @return 成功返回true，失败返回false
     */
    bool wait(pthread_mutex_t *m_mutex) {
        return pthread_cond_wait(&m_cond, m_mutex) == 0;
    }

    /**
     * @brief 定时等待条件变量
     * @param m_mutex 互斥锁
     * @param t 等待时间
     * @return 成功返回true，失败返回false
     */
    bool timewait(pthread_mutex_t *m_mutex, struct timespec t) {
        return pthread_cond_timedwait(&m_cond, m_mutex, &t) == 0;
    }

    /**
     * @brief 发送信号唤醒一个等待线程
     * @return 成功返回true，失败返回false
     */
    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }

    /**
     * @brief 广播信号唤醒所有等待线程
     * @return 成功返回true，失败返回false
     */
    bool broadcast() {
        return pthread_cond_broadcast(&m_cond) == 0;
    }
};

#endif // LOCKER_H




