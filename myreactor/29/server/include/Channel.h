#pragma once
#include <sys/epoll.h>
#include "sys/socket.h"
#include <Socket.h>
#include <functional>
#include "EventLoop.h"
class EventLoop;

class Channel
{
private:
    int fd_ = -1;                         // Channel拥有的fd，Channel和fd是一对一的关系。
    EventLoop *loop_ = nullptr;           // Channel对应的红黑树，Channel与Epollevent是多对一的关系，一个Channel只对应一个Epollevent。
    bool inepoll_ = false;                // Channel是否已添加到epoll树上，如果未添加，调用epoll_ctl()的时候用EPOLL_CTL_ADD，否则用EPOLL_CTL_MOD。
    uint32_t events_ = 0;                 // fd_需要监视的事件。listenfd和clientfd需要监视EPOLLIN，clientfd还可能需要监视EPOLLOUT。
    uint32_t revents_ = 0;                // fd_已发生的事件。
    std::function<void()> readcallback_;  // fd_读事件的回调函数。如果是acceptchannel，将回调Acceptor::newconnection()，如果是clientchannel，将回调Channel::onmessage()
    std::function<void()> closecallback_; // 关闭fd_的回调函数，将回调Connection::closecallback()。
    std::function<void()> errorcallback_; // fd_发生了错误的回调函数，将回调Connection::errorcallback()
    std::function<void()> writecallback_; // fd_写事件的回调函数，将回调Connection::writecallback()。
public:
    // 构造函数。
    // Channel是Acceptor和Connection的下层类
    Channel(EventLoop *loop, int fd);
    // 析构函数
    // 在析构函数中，不要销毁ep_，也不能关闭fd_，因为这两个东西不属于Channel类，Channel类只是需要它们，使用它们而已。
    ~Channel();

    // 返回fd_成员。
    int fd();
    // 采用边缘触发。
    void useet();
    // 让epoll_wait()监视fd_的读事件。 注册读事件
    void enablereading();
    // 取消读事件。
    void disablereading();
    // 注册写事件。
    void enablewriting();
    // 取消写事件。
    void disablewriting();
    //取消全部的事件
    void disableall();
    //把监听的sockfd从epoll红黑树中删除
    void remove();

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


    // 设置fd_读事件的回调函数。
    void setreadcallback(std::function<void()> func);
    // 设置关闭fd_的回调函数。
    void setclosecallback(std::function<void()> func);
    // 设置fd_发生了错误的回调函数。
    void seterrorcallback(std::function<void()> func);
    // 设置写事件的回调函数。
    void setwritecallback(std::function<void()> func);
};