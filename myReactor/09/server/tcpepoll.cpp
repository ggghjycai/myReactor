#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "InetAddress.h"
#include "Socket.h"
#include"Epollevent.h"
#include "Channel.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage:./tcpepoll ip port\n");
        printf("example:./tcpepoll 192.168.126.200 5055\n\n");
        return -1;
    }

    Socket servsock(createnonblocking());
    InetAddress servaddr(argv[1], atoi(argv[2]));
    servsock.setSO_REUSEPORT(true);
    servsock.setSO_REUSEADDR(true);
    servsock.setTCP_NODELAY(true);
    servsock.setSO_KEEPALIVE(true);
    servsock.bind(servaddr);
    servsock.listen(128);

    Epollevent ev;
    Channel *servchannel = new Channel(ev.ep(), servsock.fd());
    //为服务端的channel指定回调函数
    servchannel->setreadcallback(std::bind(&Channel::newconnection,servchannel,&servsock));
    servchannel->enablereading();
    ev.run();
}