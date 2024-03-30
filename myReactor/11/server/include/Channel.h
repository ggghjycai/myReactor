#pragma once
#include <sys/epoll.h>
#include "sys/socket.h"
#include <Socket.h>
#include<functional>
#include"EventLoop.h"
class EventLoop;

class Channel
{
private:
    int fd_ = -1;               // Channel拥有的fd，Channel和fd是一对一的关系。
    EventLoop *loop_ = nullptr;       // Channel对应的红黑树，Channel与Epollevent是多对一的关系，一个Channel只对应一个Epollevent。
    bool inepoll_ = false;      // Channel是否已添加到epoll树上，如果未添加，调用epoll_ctl()的时候用EPOLL_CTL_ADD，否则用EPOLL_CTL_MOD。
    uint32_t events_ = 0;       // fd_需要监视的事件。listenfd和clientfd需要监视EPOLLIN，clientfd还可能需要监视EPOLLOUT。
    uint32_t revents_ = 0;      // fd_已发生的事件。
    std::function<void()> readcallback_;   // fd_读事件的回调函数。
public:
    // 构造函数。
    Channel(EventLoop *ev, int fd);
    // 析构函数
    // 在析构函数中，不要销毁ep_，也不能关闭fd_，因为这两个东西不属于Channel类，Channel类只是需要它们，使用它们而已。
    ~Channel();

    // 返回fd_成员。
    int fd();
    // 采用边缘触发。
    void useet();
    // 让epoll_wait()监视fd_的读事件。
    void enablereading();
    // 把inepoll_成员的值设置为true。
    void setinepoll();
    // 设置revents_成员的值为参数ev。
    void setrevents(uint32_t ev);
    // 返回inepoll_成员。
    bool inepoll();
    // 返回events_成员。
    uint32_t events();
    // 返回revents_成员。
    uint32_t revents();
    // 事件处理函数，epoll_wait()返回的时候，执行它。
    void handleevents();

    //处理新客户端连接请求
    void newconnection(Socket* servsock);
    //处理对端发送过来的消息
    void onmessage();
    // 设置fd_读事件的回调函数。
    void setreadcallback(std::function<void()> func);
};