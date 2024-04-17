#include "TcpServer.h"

// TCP网络服务类
TcpServer::TcpServer(const std::string &ip, const uint16_t port,int threadnum):threadnum_(threadnum)
{
    mainloop_=new EventLoop();
    mainloop_->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1)); // 设置timeout超时的回调函数。

    acceptor_ = new Acceptor(mainloop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection, this, std::placeholders::_1));

    threadpool_=new ThreadPool(threadnum_);     //创建线程池

    for(int ii=0;ii<threadnum_;ii++)
    {
        subloops_.push_back(new EventLoop());   //创建从事件循环, 存人subloops_容器中
        subloops_[ii]->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1)); // 设置timeout超时的回调函数。
        threadpool_->addtask(std::bind(&EventLoop::run,subloops_[ii]));  // 在线程池中运行从事件循环。
    }


}   

TcpServer::~TcpServer()
{
    delete acceptor_;

    delete mainloop_;
    // 释放全部的Connection对象
    for (auto &val : conns_)
    {
        delete val.second;
    }
    // 释放从事件循环。
    for (auto &aa:subloops_)
    {
           delete aa;
    }
    delete threadpool_;   // 释放线程池。
}

void TcpServer::start()
{
    mainloop_->run();
}

void TcpServer::newconnection(Socket *clientsock)
{
    //Connection *conn = new Connection(mainloop_, clientsock); 
    Connection *conn = new Connection(subloops_[clientsock->fd()%threadnum_], clientsock); 
    conn->setclosecallback(std::bind(&TcpServer::closeconnection, this, std::placeholders::_1));
    conn->seterrorcallback(std::bind(&TcpServer::errorconnection, this, std::placeholders::_1));
    conn->setonmessagecallback(std::bind(&TcpServer::onmessage, this, std::placeholders::_1,std::placeholders::_2));
    conn->setsendcompletecallback(std::bind(&TcpServer::sendcomplete,this,std::placeholders::_1));
    
    //printf("new connection(fd=%d,ip=%s,port=%d) ok.\n", conn->fd(), conn->ip().c_str(), conn->port());
    conns_[conn->fd()] = conn;

    if(newconnectioncb_)newconnectioncb_(conn);
}

void TcpServer::closeconnection(Connection *conn) // TCP连接关闭 (断开) 的回调函数, 供Connection回调
{
    if(closeconnectioncb_)closeconnectioncb_(conn);
    
    //printf("client(%d) close.\n", conn->fd());
    // close(conn->fd());
    conns_.erase(conn->fd());
    delete conn;
}
void TcpServer::errorconnection(Connection *conn) // TCP连接错误的回调函数, 供Connection回调
{
    if(errorconnectioncb_)errorconnectioncb_(conn);

    //printf("client(%d) error.\n", conn->fd());
    conns_.erase(conn->fd());
    delete conn;
}

void TcpServer::onmessage(Connection* conn,std::string& message)
{
    if(onmessagecb_)onmessagecb_(conn,message);
}

void TcpServer::sendcomplete(Connection * conn)
{
   
    if(sendcompletecb_)sendcompletecb_(conn); 
    // printf("send complete.\n");
}

void TcpServer::epolltimeout(EventLoop *loop)
{ 
    if(timeoutcb_)timeoutcb_(loop);
    
   // printf("epoll_wait() timeout.\n");
   
}




void TcpServer::setnewconnectioncb(std::function<void(Connection*)> fn)
{
    newconnectioncb_=fn;
}

void TcpServer::setcloseconnectioncb(std::function<void(Connection*)> fn)
{
    closeconnectioncb_=fn;
}

void TcpServer::seterrorconnectioncb(std::function<void(Connection*)> fn)
{
    errorconnectioncb_=fn;
}

void TcpServer::setonmessagecb(std::function<void(Connection*,std::string &message)> fn)
{
    onmessagecb_=fn;
}

void TcpServer::setsendcompletecb(std::function<void(Connection*)> fn)
{
    sendcompletecb_=fn;
}

void TcpServer::settimeoutcb(std::function<void(EventLoop*)> fn)
{
    timeoutcb_=fn;
}