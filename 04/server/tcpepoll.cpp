#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "InetAddress.h"
#include"Socket.h"


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

    int epollfd = epoll_create(12);

    epoll_event ev;
    ev.data.fd = servsock.fd();
    // ev.events = EPOLLIN; // 水平触发即可 但是我使用边缘触发
    ev.events = EPOLLIN | EPOLLET; // 水平触发即可 但是我使用边缘触发

    epoll_ctl(epollfd, EPOLL_CTL_ADD, servsock.fd(), &ev);

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
                if (evs[ii].data.fd == servsock.fd()) // 如果是listenfd有事件，表示有新的客户端连上来。
                {
                        InetAddress clientaddr;
                       
                        Socket *clientsock=new Socket(servsock.accept(clientaddr));

                        printf("accept client(fd=%d,ip=%s,port=%d) ok.\n", clientsock->fd(), clientaddr.ip(), clientaddr.port());

                        epoll_event ev;
                        ev.data.fd = clientsock->fd();
                        ev.events = EPOLLIN | EPOLLET; // 边缘触发

                        epoll_ctl(epollfd, EPOLL_CTL_ADD, clientsock->fd(), &ev);

                        //由于析构函数中会关闭socketfd,所以这里不能delete,并且也必须使用new
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