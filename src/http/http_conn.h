#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

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
#include <map>
#include <string>

#include "../lock/locker.h"
#include "../CGImysql/sql_connection_pool.h"
#include "../timer/lst_timer.h"
#include "../log/log.h"

/**
 * @class http_conn
 * @brief 处理HTTP连接的类
 */
class http_conn {
public:
    static const int FILENAME_LEN = 200; ///< 文件名的最大长度
    static const int READ_BUFFER_SIZE = 2048; ///< 读缓冲区大小
    static const int WRITE_BUFFER_SIZE = 1024; ///< 写缓冲区大小

    // HTTP请求方法
    enum METHOD {
        GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATH
    };

    // 主状态机的状态
    enum CHECK_STATE {
        CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT
    };

    // HTTP响应状态码
    enum HTTP_CODE {
        NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION
    };

    // 从状态机的状态
    enum LINE_STATUS {
        LINE_OK = 0, LINE_BAD, LINE_OPEN
    };

public:
    http_conn() {}
    ~http_conn() {}

    void init(int sockfd, const sockaddr_in &addr, char *, int, int, std::string user, std::string passwd, std::string sqlname);
    void close_conn(bool real_close = true);
    void process();
    bool read_once();
    bool write();
    sockaddr_in *get_address() {
        return &m_address;
    }
    void initmysql_result(connection_pool *connPool);
    int timer_flag;
    int improv;

private:
    void init();
    HTTP_CODE process_read();
    bool process_write(HTTP_CODE ret);
    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();
    char *get_line() { return m_read_buf + m_start_line; };
    LINE_STATUS parse_line();
    void unmap();
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_type();
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();

public:
    static int m_epollfd; ///< epoll文件描述符
    static int m_user_count; ///< 用户数量
    MYSQL *mysql; ///< MySQL连接
    int m_state; ///< 读为0, 写为1

private:
    int m_sockfd; ///< 套接字描述符
    sockaddr_in m_address; ///< 客户端地址
    char m_read_buf[READ_BUFFER_SIZE]; ///< 读缓冲区
    long m_read_idx; ///< 标记读缓冲区中已经读入客户端数据的最后一个字节的下一个位置
    long m_checked_idx; ///< 当前正在分析的字符在读缓冲区中的位置
    int m_start_line; ///< 当前正在解析的行的起始位置
    char m_write_buf[WRITE_BUFFER_SIZE]; ///< 写缓冲区
    int m_write_idx; ///< 写缓冲区中待发送的字节数
    CHECK_STATE m_check_state; ///< 主状态机当前所处的状态
    METHOD m_method; ///< 请求方法

    char m_real_file[FILENAME_LEN]; ///< 客户请求的目标文件的完整路径，其内容等于doc_root + m_url，doc_root是网站根目录
    char *m_url; ///< 客户请求的目标文件的文件名
    char *m_version; ///< HTTP协议版本号，仅支持HTTP/1.1
    char *m_host; ///< 主机名
    long m_content_length; ///< HTTP请求的消息总长度
    bool m_linger; ///< HTTP请求是否要求保持连接
    char *m_file_address; ///< 客户请求的目标文件被mmap到内存中的起始位置
    struct stat m_file_stat; ///< 目标文件的状态，通过它我们可以判断文件是否存在、是否为目录、是否可读，并获取文件大小等信息
    struct iovec m_iv[2]; ///< 我们将采用writev来执行写操作，m_iv_count表示被写内存块的数量
    int m_iv_count; ///< 内存块数量
    int cgi; ///< 是否启用的POST
    char *m_string; ///< 存储请求头数据
    int bytes_to_send; ///< 将要发送的数据的字节数
    int bytes_have_send; ///< 已经发送的字节数
    char *doc_root; ///< 网站根目录

    std::map<std::string, std::string> m_users; ///< 用户名到密码的映射表
    int m_TRIGMode; ///< 触发模式
    int m_close_log; ///< 是否关闭日志

    char sql_user[100]; ///< 数据库用户名
    char sql_passwd[100]; ///< 数据库密码
    char sql_name[100]; ///< 数据库名称
};

#endif // HTTPCONNECTION_H
