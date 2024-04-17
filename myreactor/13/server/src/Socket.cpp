#include "Socket.h"
int createnonblocking()
{
    const int listenfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0)
    {
        // perror("socket() failed"); exit(-1);
        // 文件名    //函数名       //行号   //错误代码
        printf("%s:%s:%d listen socket create error:%d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
    return listenfd;
}

Socket::Socket(const int fd) : fd_(fd)
{
}

void Socket::closefd()
{
    ::close(fd_);
}

Socket::~Socket()
{
    closefd();
}

const int Socket::fd() const
{
    return fd_;
}

void Socket::setSO_REUSEADDR(bool flag)
{
    if (flag == true)
    {
        int opt = 1;
        ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
    }
}

void Socket::setTCP_NODELAY(bool flag)
{
    if (flag == true)
    {
        int opt = 1;
        ::setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof(opt)));
    }
}

void Socket::setSO_REUSEPORT(bool flag)
{
    if (flag == true)
    {
        int opt = 1;
        ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
    }
}

void Socket::setSO_KEEPALIVE(bool flag)
{
    if (flag == true)
    {
        int opt = 1;
        ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof(opt)));
    }
}

void Socket::bind(InetAddress &listenaddr)
{
    if (::bind(fd_, listenaddr.addr(), sizeof(sockaddr)) < 0)
    {
        perror("bind() failed");
        closefd();
        exit(-1);
    }
}

void Socket::listen(int __n)
{
    if (::listen(fd_, __n) < 0)
    {
        perror("listen() failed");
        closefd();
        exit(-1);
    }
}

int Socket::accept(InetAddress &clientaddr)
{
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    // 将accept改为accept4,加上SOCK_NONBLOCK 直接设置为非阻塞
    int clientfd = ::accept4(fd_, (sockaddr *)&peeraddr, &len, SOCK_NONBLOCK);

    clientaddr.setaddr(peeraddr);

    return clientfd;
}
