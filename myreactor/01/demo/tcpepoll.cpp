/*
 * 程序名：tcpepoll.cpp，此程序用于演示采用epoll模型实现网络通讯的服务端。
 * 作者：蔡钊
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>          
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>      // TCP_NODELAY需要包含这个头文件。

class Channel
{
private:
    int fd_;
    bool islisten_=false;          // true-监听的fd，false-客户端连上来的fd。
    // ......更多的成员变量。
public:
    Channel(int fd,bool islisten=false):fd_(fd),islisten_(islisten) {}
    int fd() { return fd_; }
    bool islisten() { return islisten_; }
    // ......更多的成员函数。
};

// 设置非阻塞的IO。
void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main(int argc,char *argv[])
{
    if (argc != 3) 
    { 
        printf("usage: ./tcpepoll ip port\n"); 
        printf("example: ./tcpepoll 192.168.126.200 5055\n\n"); 
        return -1; 
    }

    // 创建服务端用于监听的listenfd。
    int listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("socket() failed"); return -1;
    }

    // 设置listenfd的属性，如果对这些属性不熟悉，百度之。
    int opt = 1; 
    /*1)
    SO_REUSEADDR选项允许在服务器套接字关闭后立即重新绑定到相同的端口。这对于快速重启服务器并在同一端口上监听新连接非常有用。
    在多个套接字绑定到相同的端口时，此选项可以确保套接字被成功绑定而不会报告“地址已在使用中”的错误。
    */
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。
    /*2)
    TCP_NODELAY选项用于禁用Nagle算法，该算法用于延迟发送小数据包，以便将多个小数据包合并成一个大的数据包发送，以提高网络利用率。
    在某些情况下（例如实时通信），禁用Nagle算法可以减少延迟，但可能会增加网络流量。
    */
    setsockopt(listenfd,SOL_SOCKET,TCP_NODELAY   ,&opt,static_cast<socklen_t>(sizeof opt));    // 必须的。
    /*3)
    SO_REUSEPORT选项允许多个套接字绑定到相同的IP地址和端口，这对于实现负载平衡或多线程服务器很有用。
    通过使用SO_REUSEPORT，多个套接字可以同时接收传入的连接，而不会被内核限制在一个套接字上。
    */
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT ,&opt,static_cast<socklen_t>(sizeof opt));    // 有用，但是，在Reactor中意义不大。
    /*4)
    SO_KEEPALIVE选项允许在连接空闲时发送保持活动（keep-alive）消息以检测对等端的连接状态。
    如果对等端意外关闭连接或网络中断，服务器可以及时发现并采取相应的措施。
    这对于长时间空闲的连接或与不可靠网络连接的情况下很有用，但要注意，它会在网络上增加轻微的负载。
    */
    setsockopt(listenfd,SOL_SOCKET,SO_KEEPALIVE   ,&opt,static_cast<socklen_t>(sizeof opt));    // 可能有用，但是，建议自己做心跳。

    setnonblocking(listenfd);    // 把服务端的listenfd设置为非阻塞的。

    struct sockaddr_in servaddr;                                  // 服务端地址的结构体。
    servaddr.sin_family = AF_INET;                              // IPv4网络协议的套接字类型。
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);      // 服务端用于监听的ip地址。
    servaddr.sin_port = htons(atoi(argv[2]));               // 服务端用于监听的端口。

    if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    {
        perror("bind() failed"); close(listenfd); return -1;
    }

    if (listen(listenfd,128) != 0 )        // 在高并发的网络服务器中，第二个参数要大一些。
    {
        perror("listen() failed"); close(listenfd); return -1;
    }

    int epollfd=epoll_create(1);        // 创建epoll句柄（红黑树）。

    Channel *servchannel=new Channel(listenfd,true);

    // 为服务端的listenfd准备读事件。
    struct epoll_event ev;              // 声明事件的数据结构。
    // ev.data.fd=listenfd;                 // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
    ev.data.ptr=servchannel;                 // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
    ev.events=EPOLLIN;                // 让epoll监视listenfd的读事件，采用水平触发。

    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);     // 把需要监视的listenfd和它的事件加入epollfd中。

    struct epoll_event evs[10];      // 存放epoll_wait()返回事件的数组。

    while (true)        // 事件循环。
    {
        int infds=epoll_wait(epollfd,evs,10,-1);       // 等待监视的fd有事件发生。

        // 返回失败。
        if (infds < 0)
        {
            perror("epoll_wait() failed"); break;
        }

        // 超时。
        if (infds == 0)
        {
            printf("epoll_wait() timeout.\n"); continue;
        }

        // 如果infds>0，表示有事件发生的fd的数量。
        for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组evs。
        {
            Channel *ch=(Channel *)evs[ii].data.ptr;
            // if (evs[ii].data.fd==listenfd)   // 如果是listenfd有事件，表示有新的客户端连上来。
            if ( ch->islisten()==true)   // 如果是listenfd有事件，表示有新的客户端连上来。
            {
                if(&evs[ii]==&ev)printf("evs[ii]的地址和ev的地址是相同的!\n");
                if(evs[ii].data.ptr==ev.data.ptr)printf("evs[ii].data.ptr 和 ev.data.ptr的内容是相同的!\n");
                ////////////////////////////////////////////////////////////////////////
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,&len);
                setnonblocking(clientfd);         // 客户端连接的fd必须设置为非阻塞的。

                printf ("accept client(fd=%d,ip=%s,port=%d) ok.\n",clientfd,inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

                // 为新客户端连接准备读事件，并添加到epoll中。
                Channel *clientchannel=new Channel(clientfd);
                ev.data.ptr=clientchannel;
                ev.events=EPOLLIN|EPOLLET;           // 边缘触发。
                epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&ev);
                ////////////////////////////////////////////////////////////////////////
            }
            else                                        // 如果是客户端连接的fd有事件。
            {
                ////////////////////////////////////////////////////////////////////////
                if (evs[ii].events & EPOLLRDHUP)                     // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
                {
                    printf("1client(eventfd=%d) disconnected.\n",ch->fd());
                    close(ch->fd());            // 关闭客户端的fd。
                }                                //  普通数据  带外数据
                else if (evs[ii].events & (EPOLLIN|EPOLLPRI))   // 接收缓冲区中有数据可以读。
                {
                    char buffer[1024];
                    while (true)             // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
                    {    
                        bzero(&buffer, sizeof(buffer));
                        ssize_t nread = read(ch->fd(), buffer, sizeof(buffer));
                        if (nread > 0)      // 成功的读取到了数据。
                        {
                            // 把接收到的报文内容原封不动的发回去。
                            printf("recv(eventfd=%d):%s\n",ch->fd(),buffer);
                            send(ch->fd(),buffer,strlen(buffer),0);
                        } 
                        else if (nread == -1 && errno == EINTR) // 读取数据的时候被信号中断，继续读取。
                        {  
                            continue;
                        } 
                        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
                        {
                            break;
                        } 
                        else if (nread == 0)  // 客户端连接已断开。
                        {  
                            printf("2client(eventfd=%d) disconnected.\n",ch->fd());
                            close(ch->fd());            // 关闭客户端的fd。
                            break;
                        }
                    }
                }
                else if (evs[ii].events & EPOLLOUT)                  // 有数据需要写，暂时没有代码，以后再说。
                {
                }
                else                                                                    // 其它事件，都视为错误。
                {
                    printf("3client(eventfd=%d) error.\n",ch->fd());
                    close(ch->fd());            // 关闭客户端的fd。
                }
                ////////////////////////////////////////////////////////////////////////
            }
        }
    }

  return 0;
}