#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

// 初始化当前连接数和空闲连接数
connection_pool::connection_pool()
{
    m_CurConn = 0;
    m_FreeConn = 0;
}

// 获取连接池实例
connection_pool *connection_pool::GetInstance()
{
    static connection_pool connPool;
    return &connPool;
}

// 初始化连接池
void connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn, int close_log)
{
    // 保存数据库连接参数
    m_url = url;
    m_Port = Port;
    m_User = User;
    m_PassWord = PassWord;
    m_DatabaseName = DBName;
    m_close_log = close_log;

    // 创建指定数量的数据库连接
    for (int i = 0; i < MaxConn; i++)
    {
        MYSQL *con = NULL; // 定义MYSQL连接对象
        con = mysql_init(con); // 初始化MYSQL连接对象

        // 如果初始化失败，记录错误日志并退出
        if (con == NULL)
        {
            LOG_ERROR("MySQL Error");
            exit(1);
        }

        // 使用提供的参数连接数据库
        con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), Port, NULL, 0);

        // 如果连接失败，记录错误日志并退出
        if (con == NULL)
        {
            LOG_ERROR("MySQL Error");
            exit(1);
        }

        // 将成功连接的MYSQL对象添加到连接池
        connList.push_back(con);
        ++m_FreeConn; // 增加空闲连接计数
    }

    // 初始化信号量，初始值为空闲连接数量
    reserve = sem(m_FreeConn);

    // 设置最大连接数
    m_MaxConn = m_FreeConn;
}

// 从连接池中获取一个数据库连接
MYSQL *connection_pool::GetConnection()
{
    MYSQL *con = NULL;

    if (0 == connList.size())
        return NULL;

    reserve.wait();

    lock.lock();

    con = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    lock.unlock();
    return con;
}

// 释放当前使用的连接
bool connection_pool::ReleaseConnection(MYSQL *con)
{
    if (NULL == con)
        return false;

    lock.lock();

    connList.push_back(con);
    ++m_FreeConn;
    --m_CurConn;

    lock.unlock();

    reserve.post();
    return true;
}

// 销毁连接池
void connection_pool::DestroyPool()
{
    lock.lock();
    if (connList.size() > 0)
    {
        list<MYSQL *>::iterator it;
        for (it = connList.begin(); it != connList.end(); ++it)
        {
            MYSQL *con = *it;
            mysql_close(con);
        }
        m_CurConn = 0;
        m_FreeConn = 0;
        connList.clear();
    }
    lock.unlock();
}

// 获取当前空闲的连接数
int connection_pool::GetFreeConn()
{
    return this->m_FreeConn;
}

// 析构函数，销毁连接池
connection_pool::~connection_pool()
{
    DestroyPool();
}

// RAII机制获取数据库连接
connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool)
{
    *SQL = connPool->GetConnection();
    conRAII = *SQL;
    poolRAII = connPool;
}

// RAII机制释放数据库连接
connectionRAII::~connectionRAII()
{
    poolRAII->ReleaseConnection(conRAII);
}
