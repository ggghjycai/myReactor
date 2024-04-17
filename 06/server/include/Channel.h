#pragma once
#include<sys/epoll.h>
#include"Epoll.h"

class Epoll;

class Channel
{
private:
    int fd_=-1;                //要加入epoll监听的fd
    Epoll* ep_=nullptr;
    bool inepoll_=false;
    uint32_t events_=0;        //要监听的事件
    uint32_t revents_=0;       //已经发生了的事件
public:
    Channel(Epoll* ep,int fd);
    ~Channel();

    int fd();               
    void useet();
    void enablereading();
    void setinepoll();
    void setrevents(uint32_t ev);
    bool inepoll();
    uint32_t events();
    uint32_t revents();
};