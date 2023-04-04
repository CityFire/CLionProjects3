//
// Created by wjc on 2023/4/4.
//

#ifndef DEMO_XMSGSERVER_H
#define DEMO_XMSGSERVER_H

#pragma once
#include "XMThread.h"
#include <string>
#include <list>
#include <mutex>

class XMsgServer : public XMThread {
public:
    // 给当前线程发消息
    void SendMsg(std::string msg);

    void Stop() override;
private:
    // 处理消息的线程入口函数
    void Main() override;
    // 消息队列缓冲
    std::list<std::string> msgs_;
    // 互斥访问消息队列
    std::mutex mux_;

    std::condition_variable cv_;
};


#endif //DEMO_XMSGSERVER_H
