#include "EventLoop.h"

EventLoop::EventLoop():ep_(new Epoll)
{

}

EventLoop::~EventLoop()
{
    delete ep_;
}

void EventLoop::run()
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

void EventLoop::updatechannel(Channel *ch)
{
    ep_->updatechannel(ch);
}
