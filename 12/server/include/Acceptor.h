#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

class Acceptor
{
private:
    EventLoop* loop_;           //不属于Acceptor,从外面传入
    Socket* servsock_;         
    Channel* acceptchannel_;
public:
    Acceptor(EventLoop* loop,const std::string& ip,const uint16_t port);
    ~Acceptor();
};