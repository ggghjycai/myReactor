#include "Channel.h"

Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd)
{
}

Channel::~Channel()
{
}

int Channel::fd()
{
    return fd_;
}

void Channel::useet()
{
    events_ = events_ | EPOLLET;
}

void Channel::enablereading()
{
    events_ = events_ | EPOLLIN;
    loop_->updatechannel(this);
}

void Channel::setinepoll()
{
    inepoll_ = true;
}

void Channel::setrevents(uint32_t ev)
{
    revents_ = ev;
}

bool Channel::inepoll()
{
    return inepoll_;
}

uint32_t Channel::events()
{
    return events_;
}

uint32_t Channel::revents()
{
    return revents_;
}

void Channel::handleevents()
{
    if (this->revents_ & EPOLLRDHUP) // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
    {
        // printf("client(eventfd=%d) disconnected.\n",fd_);
        // close(fd_);            // 关闭客户端的fd。
        closecallback_(); // 回调Connection::closecallback()。
        return;
    }
    else if (this->revents_ & (EPOLLIN | EPOLLPRI)) // 接收缓冲区中有数据可以读。
    {
        readcallback_();
    }
    else if (this->revents_ & EPOLLOUT) // 有数据需要写，暂时没有代码，以后再说.
    {
        return;
    }
    else // 其它事件，都视为错误。
    {
        // printf("client(eventfd=%d) error.\n",fd_);
        // close(fd_);            // 关闭客户端的fd。
        errorcallback_(); // 回调Connection::errorcallback()。
        return;
    }
}

/* #include"Connection.h"
// 处理新客户端连接请求
void Channel::newconnection(Socket *servsock)
{
    InetAddress clientaddr;

    Socket *clientsock = new Socket(servsock->accept(clientaddr));

    printf("accept client(fd=%d,ip=%s,port=%d) ok.\n", clientsock->fd(), clientaddr.ip(), clientaddr.port());

    // ep.addfd(clientsock->fd(),EPOLLIN|EPOLLET);//边缘触发

     Channel *clientchannel = new Channel(this->loop_, clientsock->fd());
    //为客户端的channel指定回调函数
    clientchannel->setreadcallback(std::bind(&Channel::onmessage,clientchannel));
    // 如果要使用边缘触发,那么下面两行顺序一定不能乱
    clientchannel->useet();         // 开启边缘触发
    clientchannel->enablereading(); // 开启读事
    // 由于析构函数中会关闭socketfd,所以这里不能delete,并且也必须使用new
    //因为Channel是epoll_ctl函数返回数据的内容,是浅拷贝,必须使用new出来的指针

    Connection* conn=new Connection(loop_,clientsock);

}
*/

/* // 处理对端发送过来的消息
void Channel::onmessage()
{
    char buffer[1024];
    ssize_t readn = 0;
    while (true)
    {

        // memset 是标准 C 库函数，而 bzero 是 BSD 库（Unix 系统）特有的函数。
        // 虽然在功能上它们是等价的，但是由于 bzero 是特定于 BSD 的函数，在跨平台开发时可能不太适用。
        // 通常建议使用 memset 来代替 bzero。

        memset(buffer, 0, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
        readn = recv(this->fd(), buffer, sizeof(buffer), 0);
        if (readn > 0)
        {
            // 把接收到的报文内容原封不动的发回去。
            printf("recv(eventfd=%d):%s\n", this->fd(), buffer);
            send(this->fd(), buffer, strlen(buffer), 0);
        }
        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
            continue;
        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            break;
        }
        else if (readn == 0) // 客户端连接已断开。
        {
            printf("2 client(%d) close.\n", this->fd());
            close(this->fd());
            break;
        }
    }
} */

void Channel::setreadcallback(std::function<void()> func)
{
    readcallback_ = func;
}

void Channel::setclosecallback(std::function<void()> func)
{
    closecallback_ = func;
}

void Channel::seterrorcallback(std::function<void()> func)
{
    errorcallback_ = func;
}
