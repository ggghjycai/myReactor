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
#include <netinet/tcp.h> // TCP_NODELAY需要包含这个头文件。
#include "InetAddress.h"

// 将socket设置为非阻塞
void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage:./tcpepoll ip port\n");
        printf("example:./tcpepoll 192.168.126.200 5055\n\n");
        return -1;
    }
    // 加入SOCK_NOBLOCK 可以直接设置为非阻塞
    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("socket() failed");
        return -1;
    }

    int opt = 1;
    /*1)
   SO_REUSEADDR选项允许在服务器套接字关闭后立即重新绑定到相同的端口。这对于快速重启服务器并在同一端口上监听新连接非常有用。
   在多个套接字绑定到相同的端口时，此选项可以确保套接字被成功绑定而不会报告“地址已在使用中”的错误。
   */
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
    /*2)
   TCP_NODELAY选项用于禁用Nagle算法，该算法用于延迟发送小数据包，以便将多个小数据包合并成一个大的数据包发送，以提高网络利用率。
   在某些情况下（例如实时通信），禁用Nagle算法可以减少延迟，但可能会增加网络流量。
   */
    setsockopt(listenfd, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof(opt)));
    /*3)
    SO_REUSEPORT选项允许多个套接字绑定到相同的IP地址和端口，这对于实现负载平衡或多线程服务器很有用。
    通过使用SO_REUSEPORT，多个套接字可以同时接收传入的连接，而不会被内核限制在一个套接字上。
    */
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
    /*4)
   SO_KEEPALIVE选项允许在连接空闲时发送保持活动（keep-alive）消息以检测对等端的连接状态。
   如果对等端意外关闭连接或网络中断，服务器可以及时发现并采取相应的措施。
   这对于长时间空闲的连接或与不可靠网络连接的情况下很有用，但要注意，它会在网络上增加轻微的负载。
   */
    setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof(opt)));

    InetAddress servaddr(argv[1], atoi(argv[2]));
    if (bind(listenfd, servaddr.addr(), sizeof(sockaddr)) < 0)
    {
        perror("bind() failed");
        close(listenfd);
        return -1;
    }

    if (listen(listenfd, 128) < 0)
    {
        perror("listen() failed");
        close(listenfd);
        return -1;
    }

    int epollfd = epoll_create(12);

    epoll_event ev;
    ev.data.fd = listenfd;
    // ev.events = EPOLLIN; // 水平触发即可 但是我使用边缘触发
    ev.events = EPOLLIN | EPOLLET; // 水平触发即可 但是我使用边缘触发

    epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.data.fd, &ev);

    epoll_event evs[10];

    while (true)
    {
        int infds = epoll_wait(epollfd, evs, 10, -1);

        if (infds == 0)
        {
            printf("epoll_wait() timeout\n");
            continue;
        }

        if (infds < 0)
        {
            perror("epoll_wait() failed");
            break;
        }

        for (int ii = 0; ii < infds; ii++)
        {
            if (evs[ii].events & EPOLLRDHUP) // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
            {
                printf("1 client(%d) close.\n", evs[ii].data.fd);
                close(evs[ii].data.fd);
                continue;
            }
            else if (evs[ii].events & (EPOLLIN | EPOLLPRI)) // 接收缓冲区中有数据可以读。
            {
                if (evs[ii].data.fd == listenfd) // 如果是listenfd有事件，表示有新的客户端连上来。
                {
                    while (true)
                    {
                        sockaddr_in peeraddr;
                        socklen_t len = sizeof(peeraddr);
                        // 将accept改为accept4,加上SOCK_NONBLOCK 直接设置为非阻塞
                        int clientfd = accept4(listenfd, (sockaddr *)&peeraddr, &len, SOCK_NONBLOCK);

                        if (clientfd < 0)
                        {
                            if (errno == EAGAIN)
                                break;
                            else
                                return -1;
                        }
                        InetAddress clientaddr(peeraddr);
                        printf("accept client(fd=%d,ip=%s,port=%d) ok.\n", clientfd, clientaddr.ip(), clientaddr.port());

                        epoll_event ev;
                        ev.data.fd = clientfd;
                        ev.events = EPOLLIN | EPOLLET; // 边缘触发

                        epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
                    }
                }
                else
                {
                    char buffer[1024];
                    ssize_t readn = 0;
                    while (true)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        readn = recv(evs[ii].data.fd, buffer, sizeof(buffer), 0);
                        if (readn > 0)
                        {
                            // 把接收到的报文内容原封不动的发回去。
                            printf("recv(eventfd=%d):%s\n", evs[ii].data.fd, buffer);
                            send(evs[ii].data.fd, buffer, strlen(buffer), 0);
                        }
                        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
                            continue;
                        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
                        {
                            break;
                        }
                        else if (readn == 0) // 客户端连接已断开。
                        {
                            printf("2 client(%d) close.\n", evs[ii].data.fd);
                            close(evs[ii].data.fd);
                            break;
                        }
                    }
                }
            }
            else if (evs[ii].events & EPOLLOUT)
            {
                break;
            }
            else
            {
                printf("client(%d) error.\n", evs[ii].data.fd);
                close(evs[ii].data.fd);
                break;
            }
        }
    }
}