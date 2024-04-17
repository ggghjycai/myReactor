#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <memory>
#include <sys/syscall.h>
#include <mutex>
#include <queue>
#include <functional>
#include <sys/eventfd.h>
#include <sys/timerfd.h>      // 定时器需要包含这个头文件。
#include<map>
#include"Connection.h"
#include"casl.h"
#include<atomic>

class CASL;
class Channel;
class Epoll;
class Connection;
using spConnection = std::shared_ptr<Connection>;

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
    std::mutex mutex_;                                      // 任务队列同步的互斥锁。
    int wakeupfd_;                                          // 用于唤醒事件循环线程的eventfd。
    std::unique_ptr<Channel> wakechannel_;                  // eventfd的Channel。


    int timeval_;                                           //时间间隔,多久扫描一次Connection
    int timeout_;                                           //Connection的超时时间
    int timerfd_;                                           // 定时器的fd。
    std::unique_ptr<Channel> timechannel_;                  // 定时器的Channel。
    bool mainloop_;                                         // true-是主事件循环，false-是从事件循环。

    std::unique_ptr<CASL> loopcas_;                                               //保护conns_的自旋锁
    std::map<int,spConnection> conns_;                      //存放运行在该事件循环上全部的Connection对象
    std::function<void(int)>  timercallback_;               // 删除TcpServer中超时的Connection对象，将被设置为TcpServer::removeconn()

    std::mutex mmutex_;                                     //保护conns_的互斥锁
    std::atomic_bool stop_;                                 //初始值为false,如果设置为true,表示终止事件循环

public:
    // 构造函数
    EventLoop(bool mainloop,int timeval=5,int timeout_=10);                                
    // 在析构函数中关闭wakeupfd_
    ~EventLoop();
    // 执行监听channel的事件并处理结果
    void run();
    //停止事件循环
    void stop();

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
    // 闹钟响时执行的函数。
    void handletimer();

    // 把Connection对象保存在conns_中。
    void newconnection(spConnection conn);     

    //不是由客户端主动断开的,而是经过扫描发现超时
    // 将被设置为TcpServer::removeconn(),用于删除主线程的conns_中的Connection对象
    void settimercallback(std::function<void(int)> fn);       

    //是由客户端主动断开的, 没有经过扫描发现超时
    //用于删除I/O线程中指定的Connection对象
     void removeconn(int connfd);

};