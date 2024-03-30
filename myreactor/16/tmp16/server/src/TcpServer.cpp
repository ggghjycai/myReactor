#include "TcpServer.h"



//TCP网络服务类
TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{
    acceptor_=new Acceptor(&loop_,ip,port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));
}

TcpServer::~TcpServer()
{
    delete acceptor_;
    //释放全部的Connection对象
    for(auto& val:conns_)
    {
        delete val.second;
    }
}

void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newconnection(Socket* clientsock)
{
    Connection* conn=new Connection(&loop_,clientsock);     //这里new出来的Connection还没有释放,以后解决
     printf("new connection(fd=%d,ip=%s,port=%d) ok.\n", conn->fd(),conn->ip().c_str(),conn->port());
     conns_[conn->fd()]=conn;
}