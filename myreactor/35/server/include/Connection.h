#pragma once
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"
#include <memory>
#include <atomic>
#include <sys/syscall.h>
#include "Timestamp.h"

// 这里需要先前置声明. 当然也可以把起别名代码写入类中
class EventLoop;
class Channel;
class Connection;
using spConnection = std::shared_ptr<Connection>;

class Connection : public std::enable_shared_from_this<Connection>
{
private:
    // const std::unique_ptr<EventLoop>& loop_;                                                  // Connection对应的事件循环，在构造函数中传入。
    EventLoop *loop_;                                                    // Connection对应的事件循环，在构造函数中传入。
    std::unique_ptr<Socket> clientsock_;                                 // 与客户端通讯的Socket。在构造函数中传入
    std::unique_ptr<Channel> clientchannel_;                             // Connection对应的channel，在构造函数中创建。
    std::function<void(spConnection)> closecallback_;                    // 关闭fd_的回调函数，将回调TcpServer::closeconnection()。
    std::function<void(spConnection)> errorcallback_;                    // fd_发生了错误的回调函数，将回调TcpServer::errorconnection()。
    Buffer inputbuffer_;                                                 // 输入缓冲区
    Buffer outputbuffer_;                                                // 输出缓冲区
    std::function<void(spConnection, std::string &)> onmessagecallback_; // 处理报文的回调函数，将回调TcpServer::onmessage()。
    std::function<void(spConnection)> sendcompletecallback_;             // 发送数据完成后的回调函数，将回调TcpServer::sendcomplete()。

    // 客户端连接是否断开,如果已断开,则设置为true
    // 这里采用原子类型 至关重要
    // 因为 在I/O 线程中会修改这个变量的值  在工作线程中会判断这个变量的值
    std::atomic_bool disconnect_;           
    Timestamp lastattime_;                                               // 时间戳，创建Connection对象时为当前时间，每接收到一个报文，把时间戳更新为当前时间。
    
    //为什么采用这个原子变量呢?
    //因为在epoll中返回事件的处理顺序是不同的,有可能对端发送的消息和对端关闭的消息被一次epoll_wait处理.
    //如果消息处理在前没什么问题.
    //但是如果退出处理在前,那么可能会把spConnection对象的计数器减少到0, 析构Connection对象(如果工作线程执行任务繁重就不会), 
    //这个时候要使用原子变量控制后面事件的处理
    std::atomic_bool out_;                              
public:
    // Connection(const std::unique_ptr<EventLoop>& loop, std::unique_ptr<Socket> clientsock);
    Connection(EventLoop *loop, std::unique_ptr<Socket> clientsock);
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
    // 处理写事件的回调函数，供Channel回调。
    void writecallback();

    // 设置关闭fd_的回调函数。
    void setclosecallback(std::function<void(spConnection)>);
    // 设置fd_发生了错误的回调函数。
    void seterrorcallback(std::function<void(spConnection)>);
    // 设置处理报文的回调函数。
    void setonmessagecallback(std::function<void(spConnection, std::string &message)>);
    // 发送数据完成后的回调函数。
    void setsendcompletecallback(std::function<void(spConnection)>);

    // 处理对端发送过来的消息
    void onmessage();
    // 发送数据。
    void send(const char *data, size_t size);
    // 发送数据，如果当前线程是IO线程，直接调用此函数，如果是工作线程，将把此函数传给IO线程去执行。
    void sendinloop(const char *data, size_t size);
    
    void sendinwloop(std::string message, size_t size);
    
    // 判断TCP连接是否超时（空闲太久）。
    bool timeout(time_t now,int val);     

    // 删除EventLoop中存放的Connection对象
    void useloopremoveconn();
     
};