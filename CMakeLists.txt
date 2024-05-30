cmake_minimum_required(VERSION 3.10)
project(WebServer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/CGImysql)
include_directories(${PROJECT_SOURCE_DIR}/src/config)
include_directories(${PROJECT_SOURCE_DIR}/src/http)
include_directories(${PROJECT_SOURCE_DIR}/src/lock)
include_directories(${PROJECT_SOURCE_DIR}/src/log)
include_directories(${PROJECT_SOURCE_DIR}/src/server)
include_directories(${PROJECT_SOURCE_DIR}/src/threadpool)
include_directories(${PROJECT_SOURCE_DIR}/src/timer)

# 查找源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 生成可执行文件
add_executable(WebServer ${SOURCES} main.cpp)

# 链接库
find_package(Threads REQUIRED)

# 设置MySQL库路径
# 手动指定MySQL库和头文件路径
set(MYSQL_INCLUDE_DIR /usr/include/mysql)
set(MYSQL_LIBRARIES /usr/lib/x86_64-linux-gnu/libmysqlclient.so)

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(WebServer Threads::Threads ${MYSQL_LIBRARIES})


# 设置根目录，使用相对于 src 目录的路径
set(ROOT_DIR "${PROJECT_SOURCE_DIR}/root")
target_compile_definitions(WebServer PRIVATE ROOT_DIR="${ROOT_DIR}")

