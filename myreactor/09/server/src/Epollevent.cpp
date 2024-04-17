#include"Epollevent.h"

/* class Epollevent
{
private:
    Epoll *ep_;
public:
    Epollevent();

    ~Epollevent();

    void run();
}; */

Epollevent::Epollevent()
{
    ep_=new Epoll();
}

Epollevent::~Epollevent()
{
    delete ep_;
}

Epoll* Epollevent::ep()
{
    return ep_;
}

void Epollevent::run()
{
     while (true)
    {
        // std::vector<epoll_event> evs;
        std::vector<Channel *> channels = ep_->loop(-1);

        for (auto &ch : channels)
        {
            ch->handleevents();
        }
    }
}