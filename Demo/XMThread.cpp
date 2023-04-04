//
// Created by wjc on 2023/4/4.
//

#include "XMThread.h"
using namespace std;

// 启动线程
void XMThread::Start()
{
    is_exit_ = false;
    th_ = std::thread(&XMThread::Main, this);
}
// 设置线程退出标志 并等待
void XMThread::Stop()
{
    is_exit_ = true;
    Wait();
}
// 等待线程退出（阻塞）
void XMThread::Wait()
{
    if (th_.joinable())
        th_.join();
}
// 线程是否退出
bool XMThread::is_exit()
{
    return is_exit_;
}