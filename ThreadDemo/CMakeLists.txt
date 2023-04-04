cmake_minimum_required(VERSION 3.23)
project(ThreadDemo)

set(CMAKE_CXX_STANDARD 23)

add_executable(ThreadDemo main.cpp XThread.cpp XThread.h XMsgServer.cpp XMsgServer.h SafeDequeue.cpp SafeDequeue.h)
