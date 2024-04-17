#pragma once
#include"Acceptor.h"

// 目前为止. 没有解决Sock指针  Channel指针 释放的问题
class TcpServer
{
private:
    EventLoop loop_; // 一个TcpServer可以有多个事件循环, 现在是单线程, 暂时只用一个事件循环
    Acceptor* acceptor_;     //一个Acceptor对应一个EventLoop事件循环 ,一个Tcpserver只有一个Acceptor对象
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();

    void start(); // 运行事件循环
};
