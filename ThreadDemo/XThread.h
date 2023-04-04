//
// Created by wjc on 2023/4/4.
//

#ifndef THREADDEMO_XTHREAD_H
#define THREADDEMO_XTHREAD_H
#include <iostream>
#include <thread>

class XThread
{
public:
    // 启动线程
    virtual void Start();
    // 设置线程退出标志 并等待
    virtual void Stop();
    // 等待线程退出（阻塞）
    virtual void Wait();
    // 线程是否退出
    bool is_exit();
private:
    // 线程入口
    virtual void Main() = 0;
    std::thread th_;
    bool is_exit_ = false;
};


#endif //THREADDEMO_XTHREAD_H
