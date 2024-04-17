#pragma once
#include"Acceptor.h"
#include"Connection.h"



class TcpServer
{
private:
    EventLoop loop_; // 一个TcpServer可以有多个事件循环, 现在是单线程, 暂时只用一个事件循环   个人认为一个loop_对应一个Connection对象,形成pair 一个TcpServer可以有一个map
    Acceptor* acceptor_;     //一个Acceptor对应一个EventLoop事件循环 ,一个Tcpserver只有一个Acceptor对象
  
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();

    void start(); // 运行事件循环
};
