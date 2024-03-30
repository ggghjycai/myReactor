#include"Epoll.h"


Epoll::Epoll()
{
    epollfd_=::epoll_create(2);
    if(epollfd_==-1)
    {
        printf("epoll_create() failed(%d).\n",errno); exit(-1);
    }
}

Epoll::~Epoll()
{
    ::close(epollfd_);
}

void Epoll::addfd(int fd,uint32_t opt)
{   
    epoll_event ev;
    ev.data.fd=fd;
    ev.events=opt;
    if(epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&ev)==-1)
    {
        printf("epoll_ctl() failed(%d).\n",errno); exit(-1);
    }
}


std::vector<epoll_event> Epoll::loop(int timeout)
{
    std::vector<epoll_event> evs;

    int infds=epoll_wait(epollfd_,events_,MAXEVENTS,timeout);

    //超时 返回空容器
    if(infds==0)
    {
        printf("epoll_wait() timeout.\n");
        return evs;
    }
    //出错,退出
    if(infds==-1)
    {
        perror("epoll_wait() failed"); 
        close(epollfd_);
        exit(-1);
    }

    for(int ii=0;ii<infds;ii++)
    {
        evs.push_back(events_[ii]);
    }
    //不必担心会造成拷贝浪费大量资源.
    //编译器做了特殊处理,不会进行拷贝
    return evs;
}