#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "InetAddress.h"
#include"Socket.h"
#include"Epoll.h"

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

    Epoll ep;
    ep.addfd(servsock.fd(),EPOLLIN);    //水平触发
    std::vector<epoll_event> evs;
    while (true)
    {
        evs=ep.loop(-1);

        for (auto &ev:evs)
        {
            if (ev.events & EPOLLRDHUP) // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
            {
                printf("1 client(%d) close.\n", ev.data.fd);
                close(ev.data.fd);
                continue;
            }
            else if (ev.events & (EPOLLIN | EPOLLPRI)) // 接收缓冲区中有数据可以读。
            {
                if (ev.data.fd == servsock.fd()) // 如果是listenfd有事件，表示有新的客户端连上来。
                {
                        InetAddress clientaddr;
                       
                        Socket *clientsock=new Socket(servsock.accept(clientaddr));

                        printf("accept client(fd=%d,ip=%s,port=%d) ok.\n", clientsock->fd(), clientaddr.ip(), clientaddr.port());

                        ep.addfd(clientsock->fd(),EPOLLIN|EPOLLET);//边缘触发

                        //由于析构函数中会关闭socketfd,所以这里不能delete,并且也必须使用new
                }
                else
                {
                    char buffer[1024];
                    ssize_t readn = 0;
                    while (true)
                    {
                        /*
                        memset 是标准 C 库函数，而 bzero 是 BSD 库（Unix 系统）特有的函数。
                        虽然在功能上它们是等价的，但是由于 bzero 是特定于 BSD 的函数，在跨平台开发时可能不太适用。
                        通常建议使用 memset 来代替 bzero。
                        */
                       // memset(buffer, 0, sizeof(buffer));
                       bzero(buffer,sizeof(buffer));
                        readn = recv(ev.data.fd, buffer, sizeof(buffer), 0);
                        if (readn > 0)
                        {
                            // 把接收到的报文内容原封不动的发回去。
                            printf("recv(eventfd=%d):%s\n", ev.data.fd, buffer);
                            send(ev.data.fd, buffer, strlen(buffer), 0);
                        }
                        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
                            continue;
                        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
                        {
                            break;
                        }
                        else if (readn == 0) // 客户端连接已断开。
                        {
                            printf("2 client(%d) close.\n", ev.data.fd);
                            close(ev.data.fd);
                            break;
                        }
                    }
                }
            }
            else if (ev.events & EPOLLOUT)
            {
                break;
            }
            else
            {
                printf("client(%d) error.\n", ev.data.fd);
                close(ev.data.fd);
                break;
            }
        }
    }
}