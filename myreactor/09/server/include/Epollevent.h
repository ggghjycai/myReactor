#pragma once
#include"Epoll.h"
#include"Channel.h"

class Epollevent
{
private:
    Epoll *ep_;
public:
    Epollevent();

    ~Epollevent();

    Epoll* ep();

    void run();
};