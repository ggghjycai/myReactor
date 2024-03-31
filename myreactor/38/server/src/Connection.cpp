#include "Connection.h"

Connection::Connection(EventLoop *loop, std::unique_ptr<Socket> clientsock,int accepttype)
    : loop_(loop), clientsock_(std::move(clientsock)), disconnect_(false), out_(false),
      clientchannel_(new Channel(loop_, clientsock_->fd())), lastattime_(time(0)),inputbuffer_(accepttype),outputbuffer_(accepttype)
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
    //printf("调用了析构函数!\n");
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
    if(out_==false)
    {
    out_.store(true);
    disconnect_ = true;
    clientchannel_->remove();           // 从事件循环中删除channel
    closecallback_(shared_from_this()); // 回调TcpServer::closeconnection()。
    }
}

void Connection::errorcallback()
{
    if(out_==false)
    {
    out_.store(true);
    disconnect_ = true;
    clientchannel_->remove();           // 从事件循环中删除channel
    errorcallback_(shared_from_this()); // 回调TcpServer::errorconnection()。
    }
}

void Connection::setclosecallback(std::function<void(spConnection)> func)
{
    closecallback_ = func; // 回调TcpServer::closeconnection()。
}
void Connection::seterrorcallback(std::function<void(spConnection)> func)
{
    errorcallback_ = func; // 回调TcpServer::errorconnection()。
}

void Connection::setsendcompletecallback(std::function<void(spConnection)> func)
{
    sendcompletecallback_ = func;
}

void Connection::setonmessagecallback(std::function<void(spConnection, std::string &message)> func)
{
    onmessagecallback_ = func;
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
         memset(buffer, 0, sizeof(buffer));

        //bzero(buffer, sizeof(buffer));
        ssize_t readn = recv(this->fd(), buffer, ONCEREAD, 0);
        if (readn > 0)
        {
            /* printf("recv(eventfd=%d):%s\n", this->fd(), buffer);
            send(this->fd(), buffer, strlen(buffer), 0); */
            inputbuffer_.append(buffer, readn);
            //continue;
        }
        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
        {continue;}
        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
           std::string message;
            while (true)
            {
                //////////////////////////////////////////////////////////////
                // 可以把以下代码封装在Buffer类中，还可以支持固定长度、指定报文长度和分隔符等多种格式。
               if(inputbuffer_.pickmessage(message)==false){break;}
                //////////////////////////////////////////////////////////////

                    //printf("message (eventfd=%d):%s\n", fd(), message.c_str());
                    
                    if(out_==false)
                    {   
                        lastattime_= Timestamp::now();
                        onmessagecallback_(shared_from_this(), message);
                    }
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
//void Connection::send(const char *data, size_t size)
void Connection::send(const char *data, size_t size)
{
    if (disconnect_ == true){ printf("客户端连接已断开了，send()直接返回。\n");return; }

    if (loop_->isinloopthread()) // 判断当前线程是否为事件循环线程（IO线程）。
    {
        // 如果当前线程是IO线程，直接调用sendinloop()发送数据。
        //printf("send() 在事件循环的线程中。\n");
 
        sendinloop(data, size);
    }
    else
    {
        // 如果当前线程不是IO线程，调用EventLoop::queueinloop()，把sendinloop()交给事件循环线程去执行。
        //printf("send() 不在事件循环的线程中。\n");

        // 准备任务
        loop_->queueinloop(std::bind(&Connection::sendinwloop,this,std::string(data),size));
    }
}


void Connection::sendinloop(const char *data, size_t size)
{
    outputbuffer_.appendwithseq(data, size);
    clientchannel_->enablewriting();
}

void Connection::sendinwloop(const std::string message, size_t size)
{
    outputbuffer_.appendwithseq(message.c_str(), size);

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



 // 判断TCP连接是否超时（空闲太久）。
 bool Connection::timeout(time_t now,int val)           
 {
    return now-lastattime_.toint()>val;    
 }

// 删除EventLoop中存放的Connection对象
void Connection::useloopremoveconn()
{
    loop_->removeconn(fd());
}