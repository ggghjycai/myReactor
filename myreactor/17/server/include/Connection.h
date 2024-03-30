#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
class Connection
{
private:
    EventLoop *loop_;                // Connection对应的事件循环，在构造函数中传入。 
    Socket* clientsock_;            // 与客户端通讯的Socket。在构造函数中传入
    Channel* clientchannel_;        // Connection对应的channel，在构造函数中创建。
public:
    Connection(EventLoop* loop,Socket* clientsock);
    ~Connection();
    void onmessage();
    const int fd() const;           //返回客户端的fd
    const std::string ip() const;   //返回客户端的ip
    const uint16_t port() const;    //返回客户端的port

    void closecallback();           //TCP连接关闭 (断开) 的回调函数, 供Channel回调
    void errorcallback();           //TCP连接错误的回调函数, 供Channel回调

};