#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

// 封装了服务端的Channel
class Acceptor
{
private:
    EventLoop *loop_;                               // Acceptor对应的事件循环，在构造函数中传入。
    Socket *servsock_;                              // 服务端用于监听的socket，在构造函数中创建
    Channel *acceptchannel_;                        // Acceptor对应的channel，在构造函数中创建。
    std::function<void(Socket *)> newconnectioncb_; // 处理客户端连接请求的回调函数
public:
    Acceptor(EventLoop *loop, const std::string &ip, const uint16_t port);
    ~Acceptor();
     // 处理新客户端连接请求。
    void newconnection();
     // 设置处理新客户端连接请求的回调函数，将在创建Acceptor对象的时候（TcpServer类的构造函数中）设置。
    void setnewconnectioncb(std::function<void(Socket *)>);
};

