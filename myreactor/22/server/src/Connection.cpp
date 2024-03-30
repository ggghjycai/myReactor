#include "Connection.h"

Connection::Connection(EventLoop *loop, Socket *clientsock) : loop_(loop), clientsock_(clientsock)
{
    clientchannel_ = new Channel(this->loop_, clientsock_->fd());
    // 为客户端的channel指定回调函数
    clientchannel_->setreadcallback(std::bind(&Connection::onmessage, this));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback, this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback, this));
    clientchannel_->setwritecallback(std::bind(&Connection::writecallback,this));
    
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
    while (true)
    {

        // memset 是标准 C 库函数，而 bzero 是 BSD 库（Unix 系统）特有的函数。
        // 虽然在功能上它们是等价的，但是由于 bzero 是特定于 BSD 的函数，在跨平台开发时可能不太适用。
        // 通常建议使用 memset 来代替 bzero。
        //memset(buffer, 0, sizeof(buffer));

        bzero(buffer, sizeof(buffer));
        ssize_t readn = recv(this->fd(), buffer, sizeof(buffer), 0);
        if (readn > 0)
        {
            /* printf("recv(eventfd=%d):%s\n", this->fd(), buffer);
            send(this->fd(), buffer, strlen(buffer), 0); */
            inputbuffer_.append(buffer, readn);
        }
        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
            continue;
        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            while (true)
            {
                //////////////////////////////////////////////////////////////
                // 可以把以下代码封装在Buffer类中，还可以支持固定长度、指定报文长度和分隔符等多种格式。
                int len;
                memcpy(&len, inputbuffer_.data(), 4); // 从inputbuffer中获取报文头部。
                // 如果inputbuffer中的数据量小于报文头部，说明inputbuffer中的报文内容不完整。
                if (inputbuffer_.size() < len + 4)
                    break;

                std::string message(inputbuffer_.data() + 4, len); // 从inputbuffer中获取一个报文。
                inputbuffer_.erase(0, len + 4);                    // 从inputbuffer中删除刚才已获取的报文。
                //////////////////////////////////////////////////////////////

                printf("message (eventfd=%d):%s\n", fd(), message.c_str());

                // message="reply:"+message;
                // len=message.size();
                // std::string tmpbuf((char*)&len,4);
                // tmpbuf.append(message);
                // send(this->fd(),tmpbuf.data(),tmpbuf.size(),0);

                onmessagecallback_(this, message);
            }
            //这个break至关重要
            break;
        }
        else if (readn == 0) // 客户端连接已断开。
        {
            closecallback();
            break;
        }
    }
}

void Connection::setonmessagecallback(std::function<void(Connection *, std::string message)> func)
{
    onmessagecallback_ = func;
}

void Connection::send(const char *data, size_t size)
{
    outputbuffer_.append(data, size);
    clientchannel_->enablewriting();
}

void Connection::writecallback()
{
    int writen = ::send(fd(), outputbuffer_.data(), outputbuffer_.size(), 0);

    if (writen > 0)
        outputbuffer_.erase(0, writen);

    //如果已经将发送缓冲区数据发送完毕,回调TcpServer::sendcomplete
    if (outputbuffer_.size() == 0)
    {
        clientchannel_->disablewriting();
        sendcompletecallback_(this);
    } 
}

void Connection::setsendcompletecallback(std::function<void(Connection *)> func)
{   
    sendcompletecallback_=func;
}