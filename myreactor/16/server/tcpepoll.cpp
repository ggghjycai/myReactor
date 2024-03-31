#include"TcpServer.h"

/*
I/O多路复用,全部socket采用非阻塞,并且服务端监听设置:
  1)
   SO_REUSEADDR选项允许在服务器套接字关闭后立即重新绑定到相同的端口。这对于快速重启服务器并在同一端口上监听新连接非常有用。
   在多个套接字绑定到相同的端口时，此选项可以确保套接字被成功绑定而不会报告“地址已在使用中”的错误。
  2)
   TCP_NODELAY选项用于禁用Nagle算法，该算法用于延迟发送小数据包，以便将多个小数据包合并成一个大的数据包发送，以提高网络利用率。
   在某些情况下（例如实时通信），禁用Nagle算法可以减少延迟，但可能会增加网络流量。
  3)
    SO_REUSEPORT选项允许多个套接字绑定到相同的IP地址和端口，这对于实现负载平衡或多线程服务器很有用。
    通过使用SO_REUSEPORT，多个套接字可以同时接收传入的连接，而不会被内核限制在一个套接字上。
  4)
    SO_KEEPALIVE选项允许在连接空闲时发送保持活动（keep-alive）消息以检测对等端的连接状态。
    如果对等端意外关闭连接或网络中断，服务器可以及时发现并采取相应的措施。
    这对于长时间空闲的连接或与不可靠网络连接的情况下很有用，但要注意，它会在网络上增加轻微的负载。
  服务端采用水平触发
  客户端采用边缘触发
    
    (Channel自产自销)
  在Channel中设置socket的事件
  Epollevent获取返回结果, 返回结果再由Channel处理 

  全部都封装到TcpServer类中

  本质上是Channel先把自身托付给Epoll, 收到响应后Channel再处理自身
  Channel由什么组成? 底层:  socket(...)  epoll_create(...)  accept(...)  
  Epoll由什么组成?  底层:  epoll_create(...)
  Epoll 和 Channel之间的连接桥梁? epoll_wait(...)

//这个桥梁的不应该是单行道,应该是双向的,
并且应该交给TcpServer类的成员对象处理
服务于服务端的Channel--->Acceptor 类
服务于客户端的Channel--->Connection类
*/

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage:./tcpepoll ip port\n");
        printf("example:./tcpepoll 192.168.126.200 5055\n\n");
        return -1;
    }
/* 
    Socket servsock(createnonblocking());
    InetAddress servaddr(argv[1], atoi(argv[2]));
    servsock.setSO_REUSEPORT(true);
    servsock.setSO_REUSEADDR(true);
    servsock.setTCP_NODELAY(true);
    servsock.setSO_KEEPALIVE(true);
    servsock.bind(servaddr);
    servsock.listen(128);


//目前为止. 没有解决Sock指针  Channel指针 释放的问题

    EventLoop loop;
    Channel *servchannel = new Channel(&loop, servsock.fd());
    //为服务端的channel指定回调函数
    servchannel->setreadcallback(std::bind(&Channel::newconnection,servchannel,&servsock));
    servchannel->enablereading();
     */
    TcpServer tcpserver(argv[1],atoi(argv[2]));
    tcpserver.start();       //运行事件循环

}