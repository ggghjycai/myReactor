#include"Acceptor.h"

Acceptor::Acceptor(EventLoop* loop,const std::string& ip,const uint16_t port):loop_(loop)
{
    servsock_ = new Socket(createnonblocking());
    InetAddress servaddr(ip, port);
    servsock_->setSO_REUSEPORT(true);
    servsock_->setSO_REUSEADDR(true);
    servsock_->setTCP_NODELAY(true);
    servsock_->setSO_KEEPALIVE(true);
    servsock_->bind(servaddr);
    servsock_->listen(128);


    acceptchannel_ = new Channel(loop_, servsock_->fd());
    // 为服务端的channel指定回调函数
    acceptchannel_->setreadcallback(std::bind(&Channel::newconnection, acceptchannel_, servsock_));
    acceptchannel_->enablereading();
}

Acceptor::~Acceptor()
{
    delete servsock_;
    delete acceptchannel_;
}