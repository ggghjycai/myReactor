#include "Connection.h"

Connection::Connection(EventLoop *loop, std::unique_ptr<Socket> clientsock)
    : loop_(loop), clientsock_(std::move(clientsock)), disconnect_(false), 
      clientchannel_(new Channel(loop_, clientsock_->fd())), lastattime_(time(0))
{
    // 为客户端的channel指定回调函数

    clientchannel_->setreadcallback(std::bind(&Connection::onmessage, this));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback, this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback, this));
    clientchannel_->setwritecallback(std::bind(&Connection::writecallback, this));

    // 如果要使用边缘触发,那么下面两行顺序一定不能乱
    clientchannel_->useet();         // 开启边缘触发
    clientchannel_->enablereading(); // 开启读事
}

Connection::~Connection()
{
    // delete clientsock_;
    // delete clientchannel_;
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
    disconnect_ = true;
    clientchannel_->remove();           // 从事件循环中删除channel
    closecallback_(shared_from_this()); // 回调TcpServer::closeconnection()。
}

void Connection::errorcallback()
{
    disconnect_ = true;
    clientchannel_->remove();           // 从事件循环中删除channel
    errorcallback_(shared_from_this()); // 回调TcpServer::errorconnection()。
}

void Connection::setclosecallback(std::function<void(spConnection)> func)
{
    closecallback_ = func; // 回调TcpServer::closeconnection()。
}
void Connection::seterrorcallback(std::function<void(spConnection)> func)
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
        // memset(buffer, 0, sizeof(buffer));

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

                lastattime_= Timestamp::now();
                printf("%s\n",lastattime_.tostring().c_str());
            //如果时间超过了定时器的时间,可以.......
                onmessagecallback_(shared_from_this(), message);
            }
            // 这个break至关重要
            break;
        }
        else if (readn == 0) // 客户端连接已断开。
        {
            // clientchannel_->remove();       //从事件循环中删除channel 已经在closecallback()中调用了
            closecallback();
            break;
        }
    }
}

void Connection::setonmessagecallback(std::function<void(spConnection, std::string &message)> func)
{
    onmessagecallback_ = func;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*
这里有个明显的问题:
当工作线程个数不为0时,可以看到,
            send函数是在工作线程中执行的,在send函数中操作了Connection的outputbuffer_
            write函数是在I/O线程中执行的,在write函数中也操作了Connection的outputbuffer_
    这样子就存在线程竞争的问题.

解决办法:   使用线程间通信的技术:  eventfd 或者 pipe无名管道 均可   ,首选eventfd,更强大
    I/O线程 专门负责数据接收和发送  (也就是专门处理inputbuffer_ 和 outputbuffer_)
    工作线程  专门负责数据处理  (不应该操作outputbuffer_)
*/

// 如果工作线程个数不为0 工作线程中执行
// 否则 在I/O线程中执行
void Connection::send(const char *data, size_t size)
{
    if (disconnect_ == true){ printf("客户端连接已断开了，send()直接返回。\n");return; }

    if (loop_->isinloopthread()) // 判断当前线程是否为事件循环线程（IO线程）。
    {
        // 如果当前线程是IO线程，直接调用sendinloop()发送数据。
        printf("send() 在事件循环的线程中。\n");
        outputbuffer_.appendwithhead(data, size);
        sendinloop(data, size);
    }
    else
    {
        // 如果当前线程不是IO线程，调用EventLoop::queueinloop()，把sendinloop()交给事件循环线程去执行。
        printf("send() 不在事件循环的线程中。\n");

        // 准备任务
        outputbuffer_.appendwithhead(data, size);
        loop_->queueinloop(std::bind(&Connection::sendinloop,this,data,size));
    }
}

//为什么这里不可以把 : outputbuffer_.appendwithhead(data, size); 放到下面的函数体里面呢?
//因为在Connection::onmessage中  message是临时创建的对象,经过层层回调之后最终会执行到Connection::send就返回, 返回之后就退出作用域了.
//1)在I/O线程中,其实是可以把 : outputbuffer_.appendwithhead(data, size); 放到下面的函数体里面的.
//2)但是,在工作线程中,不可以把 : outputbuffer_.appendwithhead(data, size); 放到下面的函数体里面.
//  因为在工作线程中只是绑定了sendinloop,并不是执行.  这样子会存在message退出作用域之后工作线程才执行的情况,也就是操作了野指针. 
//为了方便管理,
void Connection::sendinloop(const char *data, size_t size)
{
    clientchannel_->enablewriting();
}

// I/O线程中执行
void Connection::writecallback()
{
    int writen = ::send(fd(), outputbuffer_.data(), outputbuffer_.size(), 0);

    if (writen > 0)outputbuffer_.erase(0, writen);

    // 如果已经将发送缓冲区数据发送完毕,回调TcpServer::sendcomplete
    if (outputbuffer_.size() == 0)
    {
        clientchannel_->disablewriting();
        sendcompletecallback_(shared_from_this());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////

void Connection::setsendcompletecallback(std::function<void(spConnection)> func)
{
    sendcompletecallback_ = func;
}