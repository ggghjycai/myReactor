#pragma once
#include"Acceptor.h"
#include"Connection.h"
#include<map>


class TcpServer
{
private:
    EventLoop loop_; // 一个TcpServer可以有多个事件循环, 现在是单线程, 暂时只用一个事件循环   
    Acceptor* acceptor_;     //一个Acceptor对应一个EventLoop事件循环 ,一个Tcpserver只有一个Acceptor对象
    std::map<int,Connection*> conns_;   //一个TcpServer有多个Connection对象,存放在map容器中
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();

    void start(); // 运行事件循环

    // 处理新客户端连接请求
    void newconnection(Socket* clientsock);
};
