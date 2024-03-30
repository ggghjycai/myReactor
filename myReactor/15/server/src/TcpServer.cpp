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
}

void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newconnection(Socket* clientsock)
{
    Connection* conn=new Connection(&loop_,clientsock);     //这里new出来的Connection还没有释放,以后解决
}