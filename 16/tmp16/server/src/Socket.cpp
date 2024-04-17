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

Socket::Socket(const int fd,const std::string& clientip, uint16_t clientport):
fd_(fd),clientip_(clientip),clientport_(clientport)
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

void Socket::bind(InetAddress &servaddr)
{
    if (::bind(fd_, servaddr.addr(), sizeof(sockaddr)) < 0)
    {
        perror("bind() failed");
        closefd();
        exit(-1);
    }
    servip_=servaddr.ip();
    servport_=servaddr.port();
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

    clientip_=clientaddr.ip();
    clientport_=clientaddr.port();
    return clientfd;
}

const std::string Socket::servip ()const
{
    return servip_;
}

const uint16_t Socket::servport ()const
{
    return servport_;
}

const std::string Socket::clientip() const
{
    return clientip_;
}

const uint16_t Socket::clientport() const
{
    return clientport_;
}