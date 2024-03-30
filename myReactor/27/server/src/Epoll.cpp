#include "Epoll.h"

Epoll::Epoll()
{
    epollfd_ = ::epoll_create(1);
    if (epollfd_ == -1)
    {
        printf("epoll_create() failed(%d).\n", errno);
        exit(-1);
    }
}

Epoll::~Epoll()
{
    ::close(epollfd_);
}


void Epoll::updatechannel(Channel *ch)
{
    epoll_event ev;
    ev.data.ptr = ch;
    ev.events = ch->events();
    if (ch->inepoll())
    {
        if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, ch->fd(), &ev) == -1)
        {
            printf("epoll_ctl() failed(%d).\n", errno);
            exit(-1);
        }
    }
    else
    {   
        if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, ch->fd(), &ev) == -1)
        {
            printf("epoll_ctl() failed(%d).\n", errno);
            exit(-1);
        }
        ch->setinepoll();
    }
}


std::vector<Channel *> Epoll::loop(int timeout)
{
    std::vector<Channel *> channels;

    /*
        这里内部有指针为什么可以清理?
        因为这里的指针是外部传入的(events_[ii].data.ptr的内容与外部一致),只要外部不变,就可以通过外部执行析构函数,
        不会造成内存泄漏
    */
    memset(events_, 0, sizeof(events_));
                                                  //如果timeout是-1 ,返回值既不会是0 也不会是-1      
    int infds = epoll_wait(epollfd_, events_, MAXEVENTS, timeout);

    // 出错,退出
    if (infds == -1)
    {
        //出错的情况不考虑了,直接退出就行了
        // EBADF ：epfd不是一个有效的描述符。
        // EFAULT ：参数events指向的内存区域不可写。
        // EINVAL ：epfd不是一个epoll文件描述符，或者参数maxevents小于等于0。
        // EINTR ：阻塞过程中被信号中断，epoll_pwait()可以避免，或者错误处理中，解析error后重新调用epoll_wait()。
        // 在Reactor模型中，不建议使用信号，因为信号处理起来很麻烦，没有必要。------ 陈硕
        perror("epoll_wait() failed");
        close(epollfd_);
        exit(-1);
    }    
    
    // 超时 返回空容器  
    if (infds == 0)
    {
        //超时应该通知TcpServer. 在上级EventLoop中调用TcpServer::epolltimeout
        //printf("epoll_wait() timeout.\n"); 
        return channels;
    }
  

    for (int ii = 0; ii < infds; ii++)
    {
        Channel *ch = (Channel *)events_[ii].data.ptr;
        ch->setrevents(events_[ii].events);
        channels.push_back(ch);
    }

    // 不必担心会造成拷贝浪费大量资源.
    // 编译器做了特殊处理,不会进行拷贝，会直接使用
    return channels;
}