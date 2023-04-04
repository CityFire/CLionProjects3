//
// Created by wjc on 2023/4/4.
//

#ifndef THREADDEMO_XMSGSERVER_H
#define THREADDEMO_XMSGSERVER_H

#pragma once
#include "XThread.h"
#include <string>
#include <list>
#include <mutex>

class XMsgServer : public XThread {
public:
    // 给当前线程发消息
    void SendMsg(std::string msg);
private:
    // 处理消息的线程入口函数
    void Main() override;
    // 消息队列缓冲
    std::list<std::string> msgs_;
    // 互斥访问消息队列
    std::mutex mux_;
};


#endif //THREADDEMO_XMSGSERVER_H
