cmake_minimum_required(VERSION 3.10)
project(TinyWebServer)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(TinyWebServer
    src/main.cpp
    src/server.cpp
    src/server.h
    src/http.cpp
    src/http.h
    src/threadpool.cpp
    src/threadpool.h
    src/logger.cpp
    src/logger.h
    src/timer.cpp
    src/timer.h
)
