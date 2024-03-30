#include "EchoServer.h"


EchoServer::EchoServer(const std::string &ip,const uint16_t port,int subthreadnum,int workthreadnum,int wroktimevaL,int worktimeout)
:tcpserver_(ip,port,subthreadnum,wroktimevaL,worktimeout),threadpool_(workthreadnum,"WORKS")    //初始化tcpserver_和 工作线程threadpool_
{
    // 以下代码不是必须的，业务关心什么事件，就指定相应的回调函数。
    tcpserver_.setnewconnectioncb(std::bind(&EchoServer::HandleNewConnection, this, std::placeholders::_1));
    tcpserver_.setcloseconnectioncb(std::bind(&EchoServer::HandleClose, this, std::placeholders::_1));
    tcpserver_.seterrorconnectioncb(std::bind(&EchoServer::HandleError, this, std::placeholders::_1));
    tcpserver_.setonmessagecb(std::bind(&EchoServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.setsendcompletecb(std::bind(&EchoServer::HandleSendComplete, this, std::placeholders::_1));
    // tcpserver_.settimeoutcb(std::bind(&EchoServer::HandleTimeOut, this, std::placeholders::_1));
}

EchoServer::~EchoServer()
{

}

// 启动服务。
void EchoServer::Start()                
{
    tcpserver_.start();
}

// 处理新客户端连接请求，在TcpServer类中回调此函数。
void EchoServer::HandleNewConnection(spConnection conn)    
{
    std::cout<<"Main thread:("<<syscall(SYS_gettid)<< ")--->New Connection Come in." << std::endl;
    //printf("EchoServer::HandleNewConnection() thread is %ld.\n",syscall(SYS_gettid));

    // 根据业务的需求，在这里可以增加其它的代码。
}

// 关闭客户端的连接，在TcpServer类中回调此函数。 
void EchoServer::HandleClose(spConnection conn)  
{
    std::cout <<"Main thread:("<<syscall(SYS_gettid)<< ")--->EchoServer conn closed." << std::endl;
    

    // 根据业务的需求，在这里可以增加其它的代码。
}

// 客户端的连接错误，在TcpServer类中回调此函数。
void EchoServer::HandleError(spConnection conn)  
{
    std::cout <<"Main thread:("<<syscall(SYS_gettid)<< ")--->EchoServer conn error." << std::endl;

    // 根据业务的需求，在这里可以增加其它的代码。
}

// 处理客户端的请求报文，在TcpServer类中回调此函数。
void EchoServer::HandleMessage(spConnection conn,std::string& message)     
{
    //printf("EchoServer::HandleMessage() thread is %ld.\n",syscall(SYS_gettid));
    
    //问题: 如果在处理业务的时候客户端断开了连接,那么会在IO线程中调用Connection的析构函数---->关闭sockfd
    //      这样Connection就成了野指针.   使用野指针的后果不可预知
    //解决办法:  使用智能指针!  

    //判断工作线程数,当业务很简单时可以没有工作线程
    if(threadpool_.size()==0)
    {
        OnMessage(conn,message);
    }
    else
    {
    //工作线程处理业务
    threadpool_.addtask(std::bind(&EchoServer::OnMessage,this,conn,message),
                        "EchoServer::OnMessage(spConnectionconn,std::string& message)");
    }
}

// 数据发送完成后，在TcpServer类中回调此函数。
void EchoServer::HandleSendComplete(spConnection conn)     
{
    std::cout<<"Main thread:("<<syscall(SYS_gettid)<< ")--->Message send complete." << std::endl;

    // 根据业务的需求，在这里可以增加其它的代码。
}

/*
// epoll_wait()超时，在TcpServer类中回调此函数。
void EchoServer::HandleTimeOut(EventLoop *loop)         
{
    std::cout << "EchoServer timeout." << std::endl;

    // 根据业务的需求，在这里可以增加其它的代码。
}
*/



//下面处理业务的过程已经使用智能指针.避免了野指针的问题.
//但是,还有个问题,使用智能指针之后, 当客户端连接在业务处理过程中断开时,到调用Connection的析构函数 之间存在时间差.
//也就是说,在这个时间差之内,会因为一只没有处理客户端连接断开的event导致重复提示(水平触发下会, 边缘触发不会,但是也会影响性能) 
//解决办法: 1)当客户端连接断开时立即取消全部的事件,并且将监听的sockfd从epoll中删除
//         2)并且这个时候发送数据也没有意义了,所以还要终止发送数据
void EchoServer::OnMessage(spConnection conn,std::string& message)
{
    ////报文: (报文长度+报文体)
    //为了防止分包和粘包 ,采用4字节报文长度+报文体格式

    // 在这里，将经过若干步骤的运算。
    message="reply:"+message;          // 回显业务。    

    conn->send(message.data(),message.size());   // 把临时缓冲区中的数据发送出去。
}