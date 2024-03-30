#include"Connection.h"

Connection::Connection(EventLoop* loop,Socket* clientsock):loop_(loop),clientsock_(clientsock)
{
    clientchannel_ = new Channel(this->loop_, clientsock_->fd());
    //为客户端的channel指定回调函数
    clientchannel_->setreadcallback(std::bind(&Channel::onmessage,clientchannel_));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback,this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback,this));
  
    // 如果要使用边缘触发,那么下面两行顺序一定不能乱
    clientchannel_->useet();         // 开启边缘触发
    clientchannel_->enablereading(); // 开启读事
}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}

const int Connection::fd() const
{
    return clientsock_->fd();
}
const std::string Connection::ip() const
{
    return clientsock_->ip();
}
const uint16_t Connection::port() const
{
    return clientsock_->port();
}

void Connection::closecallback()
{
    printf("1 client(%d) close.\n", this->fd());
    close(this->fd());
}

void Connection::errorcallback()
{
    printf("client(%d) error.\n", this->fd());
    close(this->fd());
}