#include"Connection.h"
/* #pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
class Connection()
{
private:
    EventLoop *loop_;
    Socket* clientsock_;
    Channel* clientchannel_;
public:
    Connection(EventLoop* loop,Socket* clientsock);
    ~Connection();
};
 */

Connection::Connection(EventLoop* loop,Socket* clientsock):loop_(loop),clientsock_(clientsock)
{
    clientchannel_ = new Channel(this->loop_, clientsock_->fd());
    //为客户端的channel指定回调函数
    clientchannel_->setreadcallback(std::bind(&Channel::onmessage,clientchannel_));
    // 如果要使用边缘触发,那么下面两行顺序一定不能乱
    clientchannel_->useet();         // 开启边缘触发
    clientchannel_->enablereading(); // 开启读事
}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}
