#pragma once
#include "Epoll.h"
#include "Channel.h"
class Channel;
class Epoll;

class EventLoop
{
private:
    Epoll *ep_;

public:
    // 在构造函数中new 一个Epoll类对象
    EventLoop();
    // 在析构函数中delete 这个Epoll对象
    ~EventLoop();
    // 执行监听channel的事件并处理结果
    void run();
    // 更改channel的监听事件
    void updatechannel(Channel *ch);
};