#pragma once
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

// 作废的方案
/* class TcpServer
{
private:
    Socket servsock_;
    InetAddress servaddr_;
    EventLoop loop_;            //一个TcpServer可以有多个事件循环, 现在是单线程, 暂时只用一个事件循环
    Channel* servchannel_;
public:
    TcpServer(const char* ip,uint16_t port);
    ~TcpServer();

    void run();
}; */

// 目前为止. 没有解决Sock指针  Channel指针 释放的问题
class TcpServer
{
private:
    EventLoop loop_; // 一个TcpServer可以有多个事件循环, 现在是单线程, 暂时只用一个事件循环
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();

    void start(); // 运行事件循环
};
