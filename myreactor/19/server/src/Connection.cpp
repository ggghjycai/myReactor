#include "Connection.h"

Connection::Connection(EventLoop *loop, Socket *clientsock) : loop_(loop), clientsock_(clientsock)
{
    clientchannel_ = new Channel(this->loop_, clientsock_->fd());
    // 为客户端的channel指定回调函数
    clientchannel_->setreadcallback(std::bind(&Connection::onmessage, this));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback, this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback, this));

    // 如果要使用边缘触发,那么下面两行顺序一定不能乱
    clientchannel_->useet();         // 开启边缘触发
    clientchannel_->enablereading(); // 开启读事
}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}

const int Connection::fd() const
{
    return clientsock_->fd();
}
const std::string Connection::ip() const
{
    return clientsock_->ip();
}
const uint16_t Connection::port() const
{
    return clientsock_->port();
}

void Connection::closecallback()
{
    // printf("1 client(%d) close.\n", this->fd());
    // close(this->fd());
    closecallback_(this); // 回调TcpServer::closeconnection()。
}

void Connection::errorcallback()
{
    // printf("client(%d) error.\n", this->fd());
    // close(this->fd());
    errorcallback_(this); // 回调TcpServer::errorconnection()。
}

void Connection::setclosecallback(std::function<void(Connection *)> func)
{
    closecallback_ = func; // 回调TcpServer::closeconnection()。
}
void Connection::seterrorcallback(std::function<void(Connection *)> func)
{
    errorcallback_ = func; // 回调TcpServer::errorconnection()。
}

// 处理对端发送过来的消息
void Connection::onmessage()
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
            /* printf("recv(eventfd=%d):%s\n", this->fd(), buffer);
            send(this->fd(), buffer, strlen(buffer), 0); */
            inputbuffer_.append(buffer,readn);
        }
        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
            continue;
        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            printf("recv(eventfd=%d):%s\n",fd(),inputbuffer_.data());
            //假设在这里处理了数据
            outputbuffer_=inputbuffer_;
            inputbuffer_.clear();
            send(this->fd(),outputbuffer_.data(),outputbuffer_.size(),0);
            break;
        }
        else if (readn == 0) // 客户端连接已断开。
        {
            closecallback();
            break;
        }
    }
}