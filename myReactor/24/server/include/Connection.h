#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"
class Connection
{
private:
    EventLoop *loop_;                                                  // Connection对应的事件循环，在构造函数中传入。
    Socket *clientsock_;                                               // 与客户端通讯的Socket。在构造函数中传入
    Channel *clientchannel_;                                           // Connection对应的channel，在构造函数中创建。
    std::function<void(Connection *)> closecallback_;                  // 关闭fd_的回调函数，将回调TcpServer::closeconnection()。
    std::function<void(Connection *)> errorcallback_;                  // fd_发生了错误的回调函数，将回调TcpServer::errorconnection()。
    Buffer inputbuffer_;                                               // 输入缓冲区
    Buffer outputbuffer_;                                              // 输出缓冲区
    std::function<void(Connection *, std::string&)> onmessagecallback_; // 处理报文的回调函数，将回调TcpServer::onmessage()。
    std::function<void(Connection *)> sendcompletecallback_;           // 发送数据完成后的回调函数，将回调TcpServer::sendcomplete()。

public:
    Connection(EventLoop *loop, Socket *clientsock);
    ~Connection();
    // 返回客户端的fd
    const int fd() const;
    // 返回客户端的ip
    const std::string ip() const;
    // 返回客户端的port
    const uint16_t port() const;
    // TCP连接关闭 (断开) 的回调函数, 供Channel回调
    void closecallback();
    // TCP连接错误的回调函数, 供Channel回调
    void errorcallback();
    // 设置关闭fd_的回调函数。
    void setclosecallback(std::function<void(Connection *)>);
    // 设置fd_发生了错误的回调函数。
    void seterrorcallback(std::function<void(Connection *)>);
    // 处理对端发送过来的消息
    void onmessage();
    // 设置处理报文的回调函数。
    void setonmessagecallback(std::function<void(Connection *, std::string& message)>);
    // 发送数据。
    void send(const char *data, size_t size);
    // 处理写事件的回调函数，供Channel回调。
    void writecallback();
    // 发送数据完成后的回调函数。
    void setsendcompletecallback(std::function<void(Connection *)>);
};