#include "Channel.h"

Channel::Channel(const std::unique_ptr<EventLoop>& loop, int fd) : loop_(loop), fd_(fd)
{
}

Channel::~Channel()
{
}

int Channel::fd()
{
    return fd_;
}

void Channel::useet()
{
    events_ = events_ | EPOLLET;
}

void Channel::enablereading()
{
    events_ |= EPOLLIN;
    loop_->updatechannel(this);
}


void Channel::disablereading()                    // 取消读事件。
{
    events_ &= ~EPOLLIN;
    loop_->updatechannel(this);
}

void Channel::enablewriting()                      // 注册写事件。
{
    events_ |= EPOLLOUT;
    loop_->updatechannel(this);
}

void Channel::disablewriting()                     // 取消写事件。
{
    events_ &= ~EPOLLOUT;
    loop_->updatechannel(this);
}

void Channel::disableall()
{
    events_=0;
    loop_->updatechannel(this);
}

void Channel::remove()
{
    disableall();
    loop_->removechannel(this);
}

void Channel::setinepoll()
{
    inepoll_ = true;
}

void Channel::setrevents(uint32_t ev)
{
    revents_ = ev;
}

bool Channel::inepoll()
{
    return inepoll_;
}

uint32_t Channel::events()
{
    return events_;
}

uint32_t Channel::revents()
{
    return revents_;
}

void Channel::handleevents()
{
    if (this->revents_ & EPOLLRDHUP) // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
    {
        //remove();          //从事件循环中删除channel  已经在Connection的closecallback中调用了
        closecallback_();
    }
    else if (this->revents_ & (EPOLLIN | EPOLLPRI)) // 接收缓冲区中有数据可以读。
    {
        readcallback_();
    }
    else if (this->revents_ & EPOLLOUT) // 有数据需要写
    {
        writecallback_();
    }
    else // 其它事件，都视为错误。
    {
        //remove();          //从事件循环中删除channel  已经在Connection的errorcallback中调用了
        errorcallback_(); // 回调Connection::errorcallback()。
    }
}

void Channel::setreadcallback(std::function<void()> func)
{
    readcallback_ = func;
}

void Channel::setclosecallback(std::function<void()> func)
{
    closecallback_ = func;
}

void Channel::seterrorcallback(std::function<void()> func)
{
    errorcallback_ = func;
}

void Channel::setwritecallback(std::function<void()> func)
{
    writecallback_ = func;
}