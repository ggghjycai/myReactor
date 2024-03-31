#include"Connection.h"

Connection::Connection(EventLoop* loop,Socket* clientsock):loop_(loop),clientsock_(clientsock)
{
    clientchannel_ = new Channel(this->loop_, clientsock_->fd());
    //为客户端的channel指定回调函数
    clientchannel_->setreadcallback(std::bind(&Channel::onmessage,clientchannel_));
  
    // 如果要使用边缘触发,那么下面两行顺序一定不能乱
    clientchannel_->useet();         // 开启边缘触发
    clientchannel_->enablereading(); // 开启读事
}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}
 // clientchannel_->setreadcallback(std::bind(&Connection::onmessage,this));
/* // 处理对端发送过来的消息
void Connection::onmessage()
{
    char buffer[1024];
    ssize_t readn = 0;
    while (true)
    {
        
        //memset 是标准 C 库函数，而 bzero 是 BSD 库（Unix 系统）特有的函数。
        //虽然在功能上它们是等价的，但是由于 bzero 是特定于 BSD 的函数，在跨平台开发时可能不太适用。
        //通常建议使用 memset 来代替 bzero。
      
        // memset(buffer, 0, sizeof(buffer));
        bzero(buffer, sizeof(buffer));
        readn = recv(clientchannel_->fd(), buffer, sizeof(buffer), 0);
        if (readn > 0)
        {
            // 把接收到的报文内容原封不动的发回去。
            printf("recv(eventfd=%d):%s\n", clientchannel_->fd(), buffer);
            send(clientchannel_->fd(), buffer, strlen(buffer), 0);
        }
        else if ((readn == -1) && (errno == EINTR)) // 读取数据的时候被信号中断，继续读取。
            continue;
        else if (readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            break;
        }
        else if (readn == 0) // 客户端连接已断开。
        {
            printf("2 client(%d) close.\n", clientchannel_->fd());
            close(clientchannel_->fd());
            break;
        }
    }
} */