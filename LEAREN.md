# 项目结构

项目一般包含以下主要目录和文件：
- `src/`：源代码目录，包含服务器实现的各个模块。
- `root/`：网站的根目录，包含 HTML 文件、图片等静态资源。

### 1.主要模块

#### 1. `main.cpp`
- 启动服务器的入口点，初始化服务器并开始监听端口。

#### 2. `src/server/webserver.cpp` 和 `webserver.h`
- `WebServer` 类：负责服务器的初始化、资源加载、事件循环等核心功能。

#### 3. `src/http/http_conn.cpp` 和 `http_conn.h`
- `http_conn` 类：处理 HTTP 连接，包括解析请求、生成响应。

#### 4. `src/log/log.cpp` 和 `log.h`
- 日志系统：用于记录服务器运行时的信息、错误和调试信息。

#### 5. `src/lock/locker.h`
- 锁机制：实现线程间同步，防止资源竞争。

#### 6. `src/threadpool/threadpool.h`
- 线程池：管理工作线程，执行并发任务。

#### 7. `src/timer/lst_timer.cpp` 和 `lst_timer.h`
- 定时器：管理连接的超时处理，保持长连接的状态。

### 2. 代码解析

#### 1. `main.cpp`
```cpp
int main(int argc, char *argv[]) {
    WebServer server;
    server.init();  // 初始化服务器
    server.start(); // 启动服务器
    return 0;
}
```

#### 2. `WebServer` 类
```cpp
void WebServer::init() {
    // 初始化日志、线程池、数据库连接池等
}

void WebServer::start() {
    // 事件循环，处理客户端连接
    while (true) {
        int event_number = epoll_wait();
        // 处理每个事件，如读写事件、定时器事件
    }
}
```

#### 3. `http_conn` 类
```cpp
void http_conn::process() {
    // 解析请求行、请求头和请求体
    // 根据请求类型生成响应
}

bool http_conn::read() {
    // 从客户端读取数据
}

bool http_conn::write() {
    // 向客户端写入数据
}
```

#### 4. 日志系统
```cpp
void Log::init() {
    // 初始化日志文件
}

void Log::write_log() {
    // 将日志写入文件
}
```

### 3. 运行流程

1. 启动服务器：初始化日志、线程池、数据库连接池等资源。
2. 等待连接：使用 `epoll` 等机制等待客户端连接。
3. 处理请求：当有新连接时，创建 `http_conn` 实例处理请求。
4. 生成响应：根据请求类型生成相应的响应并返回给客户端。
5. 日志记录：记录运行中的重要信息和错误。





# 线程池

https://wangpengcheng.github.io/2019/05/17/cplusplus_theadpool/

- ==只有一个==，创建并管理多个工作线程，这些线程会在服务器运行期间一直存在，等待处理任务。
- ==维护一个任务队列==，将所有的客户端请求任务添加到队列中，工作线程从队列中取出任务进行处理。

**==线程池的主要组成有三个部分：==**

- 任务队列(Task Quene)

- 线程池(Thread Pool)

- 完成队列(Completed Tasks)

  

在线程池部分做几点解释，然后大家去看代码的时候就更容易看懂了：

- 所谓线程池，就是一个`pthread_t`类型的普通数组，通过`pthread_create()`函数创建`m_thread_number`个**线程**，用来执行`worker()`函数以执行每个请求处理函数（HTTP请求的`process`函数），通过`pthread_detach()`将线程设置成脱离态（detached）后，当这一线程运行结束时，它的资源会被系统自动回收，而不再需要在其它线程中对其进行 `pthread_join()` 操作。
- 操作工作队列一定要加锁（`locker`），因为它被所有线程共享。
- 我们用信号量来标识请求队列中的请求数，通过`m_queuestat.wait();`来等待一个请求队列中待处理的HTTP请求，然后交给线程池中的空闲线程来处理。



![image-20240531121052709](../../Desktop/dailywork/piccloud/image-20240531121052709.png)

![image-20240531121101459](../../Desktop/dailywork/piccloud/image-20240531121101459.png)

## threadpool.cpp

### 总体流程

threadpool-----

1、创建一个**描述线程池**的大小为 `m_thread_number` 的线程数组 `m_threads`

2、循环创建 `m_thread_number` 个线程，每个线程运行 `worker` 函数。



***

<u>==等一下，首先讲讲任务队列==</u>

多线程服务器中，**任务队列和线程池数组中的线程之间的关系**

- **任务队列**

  - 任务队列 (`m_workqueue`) 是一个共享资源，保存着所有待处理的任务。
  - 每当有新任务到来时，会将任务添加到队列中。

  **线程池数组**

  - 线程池数组 (`m_threads`) 包含了多个线程，每个线程都是在初始化时创建的。
  - 这些线程不断地从任务队列中取出任务进行处理。



**==总结来说==**：就是线程数组池的线程从任务队列中取出任务来处理



同步和互斥：为了防止多个线程同时访问任务队列而产生竞态条件，需要使用互斥锁 (`m_queuelocker`) 和信号量 (`m_queuestat`) 来进行同步。



<u>==继续==</u>

3、任务添加，当有新的请求到来时，调用 `append` 或 `append_p` 方法，将请求添加到任务队列中，并通知线程池有新的任务需要处理

4、工作线程：`worker`和`run`方法来执行任务

***

###  **Reactor 和 Proactor 模型**

- **Reactor 模型**：线程会根据请求的状态（读或写）分别处理。如果是读操作（`m_state == 0`），先读取数据，然后处理请求；如果是写操作，直接处理写操作。
- **Proactor 模型**：线程直接处理请求。



### 总结

通过线程池管理多个工作线程，服务器能够高效地处理并发请求。线程池的工作机制包括任务添加、线程启动、任务处理和选择不同的处理模型（Reactor 或 Proactor）。





## **为什么要使用线程池？**

当你需要限制你应用程序中同时运行的线程数时，线程池非常有用。因为启动一个新线程会带来性能开销，每个线程也会为其堆栈分配一些内存等。为了任务的并发执行，我们可以将这些任务任务传递到线程池，而不是为每个任务动态开启一个新的线程。



## **线程池中的线程数量是依据什么确定的？**

> 在StackOverflow上面发现了一个还不错的[回答](https://stackoverflow.com/a/16128493/7121726)，意思是：
> 线程池中的线程数量最直接的限制因素是中央处理器(CPU)的处理器(processors/cores)的数量`N`：如果你的CPU是4-cores的，对于CPU密集型的任务(如视频剪辑等消耗CPU计算资源的任务)来说，那线程池中的线程数量最好也设置为4（或者+1防止其他因素造成的线程阻塞）；对于IO密集型的任务，一般要多于CPU的核数，因为线程间竞争的不是CPU的计算资源而是IO，IO的处理一般较慢，多于cores数的线程将为CPU争取更多的任务，不至在线程处理IO的过程造成CPU空闲导致资源浪费，公式：`最佳线程数 = CPU当前可使用的Cores数 * 当前CPU的利用率 * (1 + CPU等待时间 / CPU处理时间)`（还有回答里面提到的Amdahl准则可以了解一下







# http解析与响应连接

## process()

1、在线程池的`run()`方法中调用了`process`来处理http请求任务

2、方法`process_read()`来解析HTTP报文从而解析HTTP请求，解析成功后，`modfd` 函数用于修改 epoll 事件表，将当前连接的事件修改为读事件 (`EPOLLIN`) 或写事件 (`EPOLLOUT`)。

3、`process_read()`函数的作用就是将请求报文进行解析，因为用户的请求内容包含在这个请求报文里面，只有通过解析，知道用户请求的内容是什么，是请求图片，还是视频，或是其他请求，我们根据这些请求返回相应的HTML页面等

***



==**讲下HHTP请求类型**==

**GET 请求**

- **目的**：从服务器获取资源。
- **特点**：数据通过 URL 参数传递，长度有限。
- **示例**：访问网页、查询数据等。

**POST 请求**

- **目的**：向服务器提交数据。
- **特点**：数据通过请求体传递，适合传输大数据量和敏感信息。
- **示例**：提交表单、上传文件等。



***



4、经过上述解析，当得到一个完整的，正确的HTTP请求时，就到了`do_request`方法

5、首先对**GET**请求和不同**POST**请求（登录，注册，请求图片，视频等等）做不同的预处理

6、生成响应，`process_write`，根据解析结果生成相应的 HTTP 响应，包括状态行、头部和内容。



***

==**先来看看这些不同请求是怎么来的**==

假设你已经搭好了你的HTTP服务器，然后你在本地浏览器中键入`localhost:9000`，然后回车，这时候你就给你的服务器发送了一个GET请求，什么都没做，然后服务器端就会解析你的这个HTTP请求，然后发现是个GET请求，然后返回给你一个静态HTML页面，也就是项目中的`judge.html`页面



那POST请求怎么来的呢？这时你会发现，返回的这个`judge`页面中包含着一些`新用户`和`已有账号`这两个`button`元素，当你用鼠标点击这个`button`时，你的浏览器就会向你的服务器发送一个POST请求，服务器段通过检查`action`来判断你的POST请求类型是什么，进而做出不同的响应。



## ==处理流程总结==

1. **初始化连接**：设置套接字和地址，添加文件描述符到 epoll 中，增加用户计数。
2. **读取请求**：从客户端读取数据，判断是水平触发（LT）还是边缘触发（ET）。
3. **解析请求**：根据 HTTP 请求行、请求头和请求内容的状态逐步解析完整的 HTTP 请求。
4. **生成响应**：根据解析结果生成相应的 HTTP 响应，包括状态行、头部和内容。
5. **主处理流程**：调用 `process_read` 读取和解析请求，如果请求不完整则继续等待；调用 `process_write` 生成并发送响应，如果发送失败则关闭连接。



# 数据库连接池

## 数据库连接池

在这个项目中，我们需要处理用户的注册和登录请求，并通过数据库验证用户名和密码的正确性

每次用户请求我们都需要新建一个数据库连接，请求结束后我们释放该数据库连接，当用户请求连接过多时，这种做法过于低效，所以类似**线程池**的做法，我们构建一个==数据库连接池==，预先生成一些数据库连接放在那里供用户请求使用。



对于一个数据库连接池来讲，就是预先生成多个这样的数据库连接，然后放在一个链表中，==同时维护最大连接数`MAX_CONN`，当前可用连接数`FREE_CONN`和当前已用连接数`CUR_CONN`这三个变量==。同样注意在对连接池操作时（获取，释放），要用到锁机制，因为它==被所有线程共享==。



1、初始化数据库连接池

在服务器启动时，初始化数据库连接池，创建一定数量的数据库连接，并将它们放入连接池中以供请求使用。

```c++
void connection_pool::init(string url, string User, string PassWord, string DBName, int Port, int MaxConn, int close_log) {
    // 初始化数据库连接池，创建连接并加入连接池
}
```

在初始化过程中：

- 保存数据库连接参数，创建指定数量的数据库连接（根据 MaxConn ）
- 然后将成功连接的MYSQL对象添加到连接池

```c++
connList.push_back(con);//list<MYSQL *> connList 连接池
++m_FreeConn; // 增加空闲连接计数
```

2、从连接池中获取一个数据库连接`GetConnection()`

```c++
    con = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;
```

3、释放、销毁连接池

4、==RAII机制管理连接==

RAII（Resource Acquisition Is Initialization）机制用于自动管理数据库连接的获取和释放，确保资源的正确释放，防止资源泄漏：

```c++
connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool) {
    *SQL = connPool->GetConnection();
    conRAII = *SQL;
    poolRAII = connPool;
}
connectionRAII::~connectionRAII() {
    poolRAII->ReleaseConnection(conRAII);
}
```



***

## 与线程池的协同工作

在处理 HTTP 请求时，线程池中的工作线程可能需要访问数据库。此时，线程通过连接池获取一个数据库连接，执行数据库操作后，再将连接归还连接池：

在`threadpool.h`中

```c++
connection_pool *m_connPool; ///< 数据库连接池
```

构造函数时：

```c++
template <typename T>
threadpool<T>::threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_requests)
    : m_actor_model(actor_model), m_thread_number(thread_number), m_max_requests(max_requests), m_threads(nullptr), m_connPool(connPool){
        
    }
```



***



### 先讲下`request`:

`request` 是一个指向任务对象的指针。这个任务对象是模板参数 `T` 类型的实例，通常代表一个需要处理的客户端请求（如 HTTP 请求）



1. **任务对象的类型**

==`request` 的类型由模板参数 `T` 决定==。在 Web 服务器中，`T` 通常是一个处理客户端请求的类，如 `http_conn`。这个类包含了处理 HTTP 请求所需的所有数据和方法。



2. **获取任务**

任务对象被添加到任务队列中，线程池中的工作线程从队列中获取任务进行处理：

```c++
T *request = m_workqueue.front();  // 获取队列中的第一个任务
m_workqueue.pop_front();           // 从队列中移除该任务
```



**3、示例：`http_conn` 类**

假设 `T` 是 `http_conn` 类，`http_conn` 类的定义可能如下：

```c++
class http_conn {
public:
    void process();        // 处理请求的函数
    bool read_once();      // 读取请求数据的函数
    bool write();          // 发送响应数据的函数

    int m_state;           // 请求状态（读或写）
    int improv;            // 改进标志
    int timer_flag;        // 定时器标志
    MYSQL *mysql;          // 数据库连接
    // 其他成员变量和方法...
};
```



所以`threadpool::run()`中：

==线程池和请求对象的协同工作==

1. **线程池中的线程从任务队列中获取任务对象 `request`。**
2. 根据 `request` 的状态，执行读操作或写操作:
   - 如果是读操作，调用 `request->read_once()`。
   - 如果读操作成功，设置 `improv` 标志，并使用数据库连接池获取数据库连接，然后调用 `request->process()` 处理请求。
   - 如果读操作失败，设置 `timer_flag` 标志。
   - 如果是写操作，调用 `request->write()`，根据写操作结果设置相应的标志。
3. **处理完请求后，线程继续获取下一个任务。**



*******

在`threadpool.h::run()`中

### **协同工作流程**

- **任务添加到线程池**：当有新的 HTTP 请求到来时，任务被添加到线程池的任务队列。
- **线程获取任务**：线程池中的工作线程从任务队列中获取任务。
- **获取数据库连接**：任务需要访问数据库时，通过 `connectionRAII` 从连接池获取一个数据库连接。
- **处理请求**：使用数据库连接处理请求，完成后将连接归还连接池。
- **任务完成**：任务处理完毕，线程继续获取下一个任务

```c++
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
```





# 信号量和互斥锁

`locker.h` 文件定义了信号量类 (`sem`)、互斥锁类 (`locker`) 和条件变量类 (`cond`)，这些类提供了线程同步和互斥的机制。



### `sem` 类（信号量类）

- **成员变量**：
  - `sem_t m_sem`：POSIX 信号量。

- **成员函数**：
  - `bool wait()`：等待信号量，阻塞直到信号量大于 0。
  - `bool post()`：增加信号量。

### `locker` 类（互斥锁类）

- **成员变量**：
  - `pthread_mutex_t m_mutex`：POSIX 互斥锁。

- **成员函数**：
  - `bool lock()`：锁定互斥锁。
  - `bool unlock()`：解锁互斥锁。
  - `pthread_mutex_t* get()`：获取互斥锁指针。





### `cond` 类（条件变量类）

- **成员变量**：
  - `pthread_cond_t m_cond`：POSIX 条件变量。

- **成员函数**：
  - `bool wait(pthread_mutex_t *m_mutex)`：等待条件变量。
  - `bool timewait(pthread_mutex_t *m_mutex, struct timespec t)`：定时等待条件变量。
  - `bool signal()`：发送信号唤醒一个等待线程。
  - `bool broadcast()`：广播信号唤醒所有等待线程。



## 作用

在项目中，线程池和数据库连接池协同工作以提高处理并发请求的效率：

1. **线程池**：
   - 线程池中的每个线程从任务队列中获取任务（如 HTTP 请求）。
   - 使用互斥锁 (`locker`) 保护对任务队列的访问。
   - 使用信号量 (`sem`) 控制任务的添加和获取。
2. **数据库连接池**：
   - 连接池在初始化时创建多个数据库连接，并在需要时分配连接。
   - 使用互斥锁 (`locker`) 保护对连接池的访问。
   - 使用信号量 (`sem`) 控制连接的获取和释放。





# 定时器处理非活动链接
