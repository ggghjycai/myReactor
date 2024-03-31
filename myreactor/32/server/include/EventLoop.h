#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <memory>
#include <sys/syscall.h>
#include <mutex>
#include <queue>
#include <functional>
#include <sys/eventfd.h>
class Channel;
class Epoll;

// 事件循环类。
// 内部的ep_为什么不能使用栈内存而必须使用指针呢?
// 因为要解决头文件重复包含必须使用前置声明
// 而前置声明的时候还没有定义,所以只能使用指针
class EventLoop
{
private:
    std::unique_ptr<Epoll> ep_;                             // 每个事件循环只有一个Epoll
    std::function<void(EventLoop *)> epolltimeoutcallback_; // epoll_wait()超时的回调函数。
    pid_t threadid_;                                        // 事件循环所在线程的id。
    std::queue<std::function<void()>> taskqueue_;           // 事件循环线程被eventfd唤醒后执行的任务队列。
    std::mutex mutex_;                                        // 任务队列同步的互斥锁。
    int wakeupfd_;                                          // 用于唤醒事件循环线程的eventfd。
    std::unique_ptr<Channel> wakechannel_;                // eventfd的Channel。
public:
    // 在构造函数中new 一个Epoll类对象
    EventLoop();
    // 在析构函数中delete 这个Epoll对象
    ~EventLoop();
    // 执行监听channel的事件并处理结果
    void run();
    // 更改channel的监听事件
    void updatechannel(Channel *ch);
    // 从黑树上删除channel。
    void removechannel(Channel *ch);
    // 设置epoll_wait()超时的回调函数。
    void setepolltimeoutcallback(std::function<void(EventLoop *)>);

    // 判断当前线程是否为事件循环线程
    bool isinloopthread();

   // 把任务添加到队列中。
    void queueinloop(std::function<void()>);
    // 唤醒I/O线程
    void wakeup();
    // 回调函数,用于I/O线程执行任务队列中的任务
    void handlewakeup();
};