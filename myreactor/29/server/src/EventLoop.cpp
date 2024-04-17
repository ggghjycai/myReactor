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
        std::vector<Channel *> channels = ep_->loop(10*1000);
        if(channels.size()==0)
        {
            epolltimeoutcallback_(this);
        }
        else
        {
           for (auto &ch : channels)
            {
                ch->handleevents();
            } 
        }
        
    }
}

void EventLoop::updatechannel(Channel *ch)
{
    ep_->updatechannel(ch);
}

void EventLoop::setepolltimeoutcallback(std::function<void(EventLoop*)> func)
{
    epolltimeoutcallback_=func;
}

void EventLoop::removechannel(Channel *ch)
{
    ep_->removechannel(ch);
}