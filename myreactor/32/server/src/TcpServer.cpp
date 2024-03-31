#include "TcpServer.h"

// TCP网络服务类
TcpServer::TcpServer(const std::string &ip, const uint16_t port,int threadnum)  
                    :threadnum_(threadnum),mainloop_(new EventLoop),acceptor_(mainloop_.get(),ip,port),threadpool_(threadnum_,"IO")
{
     // mainloop_=new EventLoop;       // 创建主事件循环。
    mainloop_->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1)); // 设置timeout超时的回调函数。

    // acceptor_=new Acceptor(mainloop_,ip,port);
      acceptor_.setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));
   
    // threadpool_=new ThreadPool(threadnum_,"IO");       // 创建线程池。
    for(int ii=0;ii<threadnum_;ii++)
    {
        subloops_.emplace_back(new EventLoop);   //创建从事件循环, 存人subloops_容器中
        subloops_[ii]->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1)); // 设置timeout超时的回调函数。
        threadpool_.addtask(std::bind(&EventLoop::run,subloops_[ii].get()),"EventLoop::run()");    // 在线程池中运行从事件循环。
    }
}   


TcpServer::~TcpServer()
{
    //delete acceptor_;

    //delete mainloop_;
    // 释放全部的Connection对象
   /*  
    for (auto &val : conns_)
    {
        delete val.second;
    } 
    */
    // 释放从事件循环。
   /*  for (auto &aa:subloops_)
    {
           delete aa;
    } */
   // delete threadpool_;   // 释放线程池。
}

void TcpServer::start()
{
    mainloop_->run();
}

void TcpServer::newconnection(std::unique_ptr<Socket> clientsock)
{
    //Connection *conn = new Connection(mainloop_, clientsock); 
    //Connection *conn = new Connection(subloops_[clientsock->fd()%threadnum_], clientsock); 
    
                                                                                    //.get()超级细节
    spConnection conn=std::make_shared<Connection>(subloops_[clientsock->fd()%threadnum_].get(), std::move(clientsock));   // C++11标准，效率更高。
    //全部使用到shared_ptr的回调函数在绑定的时候都采用参数占位符的形式.
    //这样子可以保证调用函数时计数器加1, 函数执行完毕时计数器就减1, 最终才能保证对象的正确销毁
    conn->setclosecallback(std::bind(&TcpServer::closeconnection, this, std::placeholders::_1));
    conn->seterrorcallback(std::bind(&TcpServer::errorconnection, this, std::placeholders::_1));
    conn->setonmessagecallback(std::bind(&TcpServer::onmessage, this, std::placeholders::_1,std::placeholders::_2));
    conn->setsendcompletecallback(std::bind(&TcpServer::sendcomplete,this,std::placeholders::_1));
    
    //printf("new connection(fd=%d,ip=%s,port=%d) ok.\n", conn->fd(), conn->ip().c_str(), conn->port());
 
    conns_[conn->fd()] = conn;                   //这个容器的存储保证了shared_ptr计数器值的持久性

    if(newconnectioncb_)newconnectioncb_(conn);
}

void TcpServer::closeconnection(spConnection conn) // TCP连接关闭 (断开) 的回调函数, 供Connection回调
{
    if(closeconnectioncb_)closeconnectioncb_(conn);
    
    //printf("client(%d) close.\n", conn->fd());
    // close(conn->fd());
    conns_.erase(conn->fd());
    //delete conn;
}
void TcpServer::errorconnection(spConnection conn) // TCP连接错误的回调函数, 供Connection回调
{
    if(errorconnectioncb_)errorconnectioncb_(conn);

    //printf("client(%d) error.\n", conn->fd());
    conns_.erase(conn->fd());
    //delete conn;
}

void TcpServer::onmessage(spConnection conn,std::string& message)
{
    if(onmessagecb_)onmessagecb_(conn,message);
}

void TcpServer::sendcomplete(spConnection conn)
{
   
    if(sendcompletecb_)sendcompletecb_(conn); 
    // printf("send complete.\n");
}

void TcpServer::epolltimeout(EventLoop *loop)
{ 
    if(timeoutcb_)timeoutcb_(loop);
    
   // printf("epoll_wait() timeout.\n");
   
}




void TcpServer::setnewconnectioncb(std::function<void(spConnection)> fn)
{
    newconnectioncb_=fn;
}

void TcpServer::setcloseconnectioncb(std::function<void(spConnection)> fn)
{
    closeconnectioncb_=fn;
}

void TcpServer::seterrorconnectioncb(std::function<void(spConnection)> fn)
{
    errorconnectioncb_=fn;
}

void TcpServer::setonmessagecb(std::function<void(spConnection,std::string &message)> fn)
{
    onmessagecb_=fn;
}

void TcpServer::setsendcompletecb(std::function<void(spConnection)> fn)
{
    sendcompletecb_=fn;
}

void TcpServer::settimeoutcb(std::function<void(EventLoop*)> fn)
{
    timeoutcb_=fn;
}