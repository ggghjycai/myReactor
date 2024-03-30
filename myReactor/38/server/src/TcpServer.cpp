/*
    TcpServer中的conns 和  EventLoop 中的conns 都需要加锁
    因为会在主线程和从线程中同时执行:       Connection超时  或者 Connection主动断开连接
*/

#include "TcpServer.h"

// TCP网络服务类
TcpServer::TcpServer(const std::string &ip, const uint16_t port,int threadnum,int timeval,int timeout,int accepttype)  
                    :threadnum_(threadnum),mainloop_(new EventLoop(true,timeval,timeout)),acceptor_(mainloop_.get(),ip,port),
                    threadpool_(threadnum_,"IO"),tcpcas_(new CASL()),accepttype_(accepttype)
{
     // mainloop_=new EventLoop;       // 创建主事件循环。
    mainloop_->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1)); // 设置timeout超时的回调函数。

    // acceptor_=new Acceptor(mainloop_,ip,port);
      acceptor_.setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));
   
    // threadpool_=new ThreadPool(threadnum_,"IO");       // 创建线程池。
    for(int ii=0;ii<threadnum_;ii++)
    {
        subloops_.emplace_back(new EventLoop(false,timeval,timeout));   //创建从事件循环, 存人subloops_容器中
        subloops_[ii]->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1)); // 设置timeout超时的回调函数。
        subloops_[ii]->settimercallback(bind(&TcpServer::removeconn,this,std::placeholders::_1));        // 设置清理空闲TCP连接的回调函数。
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

void TcpServer::stop()
{
    //停止主事件循环
    mainloop_->stop();
    printf("主事件循环已终止.\n");
    //停止从事件循环
    for(int ii=0;ii<threadnum_;ii++)
    {
        subloops_[ii]->stop();
    }
    printf("从事件循环已终止.\n");
    //停止I/O线程
    threadpool_.stop();
    printf("I/O线程已终止.\n");
}

//主线程线程
void TcpServer::newconnection(std::unique_ptr<Socket> clientsock)
{
    //Connection *conn = new Connection(mainloop_, clientsock); 
    //Connection *conn = new Connection(subloops_[clientsock->fd()%threadnum_], clientsock); 
    

    int random=clientsock->fd()%threadnum_;
                                                                                    //.get()超级细节
    spConnection conn=std::make_shared<Connection>(subloops_[random].get(), std::move(clientsock),accepttype_);   // C++11标准，效率更高。
    //全部使用到shared_ptr的回调函数在绑定的时候都采用参数占位符的形式.
    //这样子可以保证调用函数时计数器加1, 函数执行完毕时计数器就减1, 最终才能保证对象的正确销毁
    conn->setclosecallback(std::bind(&TcpServer::closeconnection, this, std::placeholders::_1)); 
    conn->seterrorcallback(std::bind(&TcpServer::errorconnection, this, std::placeholders::_1));
    conn->setonmessagecallback(std::bind(&TcpServer::onmessage, this, std::placeholders::_1,std::placeholders::_2));
    conn->setsendcompletecallback(std::bind(&TcpServer::sendcomplete,this,std::placeholders::_1));
    
   
    {
      std::lock_guard<std::mutex> lg(mmutex_);
     //tcpcas_->lock();
     conns_[conn->fd()] = conn;                   //将conn存放到TcpServer的map容器中
     //tcpcas_->unlock();
    }

    subloops_[random]->newconnection(conn);      //将conn存放到EventLoop的map容器中

    if(newconnectioncb_)newconnectioncb_(conn);
}

//I/O线程
// 删除主线程conns_中的Connection对象，在EventLoop::handletimer()中将回调此函数。
//并且,在I/O线程中的conns_也会删除对应的Connection对象
void TcpServer::removeconn(int fd)
{  
    std::lock_guard<std::mutex> lg(mmutex_);
    //tcpcas_->lock();
    //下面两行顺序千万不能换,否则段错误
    printf("connection closed(fd=%d,ip=%s,port=%d).\n", fd, conns_[fd]->ip().c_str(), conns_[fd]->port());
    conns_.erase(fd);
    //tcpcas_->unlock();
}

//I/O线程
//客户端主动断开的
void TcpServer::closeconnection(spConnection conn) // TCP连接关闭 (断开) 的回调函数, 供Connection回调
{
    if(closeconnectioncb_)closeconnectioncb_(conn);
    
    //printf("client(%d) close.\n", conn->fd());
    // close(conn->fd());

    //这里删除了主线程的conns中的Connection对象.
    //由于不是经过I/O线程的超时检查删除的,所以在I/O线程中的conns_并没有立即清除对应的Connection对象
    //所以这里要加入删除I/O线程中conns_的Connection对象的操作
    {
        std::lock_guard<std::mutex> lg(mmutex_);
        //tcpcas_->lock();
        conns_.erase(conn->fd());
        //tcpcas_->unlock();
    }

    //只有原有的fd关闭了之后才能被申请到,所以这里不必关心析构过程中Acceptor是否会接收到一样的fd.
    conn->useloopremoveconn();
    
    //delete conn;
}

//I/O线程
//客户端主动断开的
void TcpServer::errorconnection(spConnection conn) // TCP连接错误的回调函数, 供Connection回调
{
    if(errorconnectioncb_)errorconnectioncb_(conn);

    //printf("client(%d) error.\n", conn->fd());
    //这里删除了主线程的conns中的Connection对象.
    //由于不是经过I/O线程的超时检查删除的,所以在I/O线程中的conns_并没有立即清除对应的Connection对象
    //所以这里要加入删除I/O线程中conns_的Connection对象的操作
    {
        std::lock_guard<std::mutex> lg(mmutex_);
        //tcpcas_->lock();
        conns_.erase(conn->fd());
        //tcpcas_->unlock();
    }

    conn->useloopremoveconn();

    //delete conn;
}

//I/O线程
void TcpServer::onmessage(spConnection conn,std::string& message)
{
    if(onmessagecb_)onmessagecb_(conn,message);
}

//I/O线程
void TcpServer::sendcomplete(spConnection conn)
{
   
    if(sendcompletecb_)sendcompletecb_(conn); 
    // printf("send complete.\n");
}

//I/O线程
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

