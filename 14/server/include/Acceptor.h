#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

//封装了服务端的Channel
class Acceptor
{
private:
    EventLoop* loop_;            // Acceptor对应的事件循环，在构造函数中传入。
    Socket* servsock_;         // 服务端用于监听的socket，在构造函数中创建
    Channel* acceptchannel_;  // Acceptor对应的channel，在构造函数中创建。
public:
    Acceptor(EventLoop* loop,const std::string& ip,const uint16_t port);
    ~Acceptor();
    void newconnection();
};