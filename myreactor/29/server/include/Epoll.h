#pragma once
#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include "Channel.h"
#include <string.h>

class Channel;
class EventLoop;

class Epoll
{
private:
    static const int MAXEVENTS = 100; // epoll_wait()返回事件数组的大小。
    int epollfd_ = -1;                // epoll句柄，在构造函数中创建。
    epoll_event events_[MAXEVENTS];   // 存放poll_wait()返回事件的数组，在构造函数中分配内存。
public:
    Epoll();  // 在构造函数中创建了epollfd_。
    ~Epoll(); // 在析构函数中关闭epollfd_。

    // 把fd和它需要监视的事件添加到红黑树上.
    void updatechannel(Channel *ch);
    // 从黑树上删除channel。
    void removechannel(Channel* ch);
    // 运行epoll_wait()，等待事件的发生，已发生的事件用vector容器返回。
    std::vector<Channel *> loop(int timeout = -1);
};