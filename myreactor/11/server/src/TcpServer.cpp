#include "TcpServer.h"

// 作废的方案
/* TcpServer::TcpServer(const char* ip,uint16_t port):
    servsock_(Socket(createnonblocking())),
    servaddr_(InetAddress(ip,port)),            //这里要非常小心,我在内部才把主机字节序转换为网络字节序,也就是这里不要使用htons(port)
    loop_(EventLoop())
{
    this->servsock_.setSO_REUSEPORT(true);
    this->servsock_.setSO_REUSEADDR(true);
    this->servsock_.setTCP_NODELAY(true);
    this->servsock_.setSO_KEEPALIVE(true);
    this->servsock_.bind(servaddr_);
    this->servsock_.listen(128);

    this->servchannel_=new Channel(&loop_,servsock_.fd());
    this->servchannel_->setreadcallback(std::bind(&Channel::newconnection,servchannel_,&servsock_));
    this->servchannel_->enablereading();
}

TcpServer::~TcpServer()
{}

void TcpServer::run()
{
    loop_.run();
} */

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{
    // 要new出来,因为析构函数中会关闭sockfd
    Socket *servsock = new Socket(createnonblocking());
    InetAddress servaddr(ip, port);
    servsock->setSO_REUSEPORT(true);
    servsock->setSO_REUSEADDR(true);
    servsock->setTCP_NODELAY(true);
    servsock->setSO_KEEPALIVE(true);
    servsock->bind(servaddr);
    servsock->listen(128);

    // 目前为止. 没有解决Sock指针  Channel指针 释放的问题

    Channel *servchannel = new Channel(&loop_, servsock->fd());
    // 为服务端的channel指定回调函数
    servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel, servsock));
    servchannel->enablereading();
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    loop_.run();
}