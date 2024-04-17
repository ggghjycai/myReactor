#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include"memory"

//这里Acceptor中的Channel为什么可以在栈上创建.
//而Connection中的Channel只能在堆上创建呢?
//因为一个服务程序中Acceptor只有一个,在栈上创建Channel没什么问题  . 
//而Connection随着客户端连接不断增加,如果在栈上创建Channel容易爆栈
// 封装了服务端的Channel
class Acceptor
{
private:
    //const std::unique_ptr<EventLoop>& loop_;                               // Acceptor对应的事件循环，在构造函数中传入。
    EventLoop *loop_;                                                    // Acceptor对应的事件循环，在构造函数中传入。
    Socket servsock_;                              // 服务端用于监听的socket，在构造函数中创建
    Channel acceptchannel_;       // Acceptor对应的channel，在构造函数中创建。
    std::function<void(std::unique_ptr<Socket>)> newconnectioncb_; // 处理客户端连接请求的回调函数
public:
    //Acceptor(const std::unique_ptr<EventLoop>& loop, const std::string &ip, const uint16_t port);
    Acceptor(EventLoop * loop, const std::string &ip, const uint16_t port);
    ~Acceptor();
     // 处理新客户端连接请求。
    void newconnection();
     // 设置处理新客户端连接请求的回调函数，将在创建Acceptor对象的时候（TcpServer类的构造函数中）设置。
    void setnewconnectioncb(std::function<void(std::unique_ptr<Socket>)>);
};