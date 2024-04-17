#include"EchoServer.h"
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

reactor模型的核心是回调函数.
在Reactor模型中，不建议使用信号，因为信号处理起来很麻烦，没有必要。------ 陈硕

底层类:
Socket：无论是客户端还是服务器端，Socket都是建立网络通信的基础。
InetAddress：整个网络通信中，需要确定的就是网络地址，所以这个类的重要性很高。
Channel：所有的I/O操作都由Channel提供，所以它的重要性也是毋容置疑的。
Buffer：Buffer中存放了实际的数据，Channel会将数据读到Buffer内，或者将Buffer里的数据写到通道内。没有这个类，我们就无法操作数据。
Connection：建立和处理Connection是网络通信的重要步骤。
Epoll：操作系统级别的I/O复用技术，用于提升服务器性能，管理多个Socket连接。
EventLoop：负责将不同的事件分发到对应的处理程序。
Acceptor：在服务端接受客户端连接的组件，比起其他核心组件其重要性相对要小一些。
TcpServer：具体的应用往往是围绕某个具体的Server进行的，如HttpServer，FtpServer，所以TcpServer重要，但在这些核心类中，可能排在最后。

业务类:
EchoServer:
"回显服务器"（Echo Server）的主要用途是测试和诊断网络问题。它通过对传入消息进行简单的回显（即原样返回收到的内容）来进行操作。
以下是回显服务器的主要用途：
网络测试：回显服务器可以被用来测试网络连接的质量，例如确认数据是否被正确地发送和接收，或者检查网络延迟、丢包等问题。
系统测试：它也可以作为一个简单的工具来测试网络应用程序的功能，例如确认应用程序是否能正确地向服务器发送数据以及接收服务器返回的数据。由于回显服务器只是简单地返回发送的数据,因此可以方便地检查发送数据和接收数据是否一致。
教学目的：回显服务器通常用于教学目的，因为它们的实现比较简单，可以帮助学者理解基本的网络编程概念和技术。
请注意：回显服务器在实际的生产环境中不常使用，主要原因是它没有实际的业务逻辑，无法满足复杂的业务需求。


*/

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("usage:./echoServer ip port\n");
    printf("example:./echoServer 192.168.126.200 5055\n\n");
    return -1;
  }
  
  EchoServer echoserver(argv[1],atoi(argv[2]),3,5);

  echoserver.Start();
}