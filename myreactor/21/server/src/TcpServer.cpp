#include "TcpServer.h"

// TCP网络服务类
TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{
    acceptor_ = new Acceptor(&loop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{
    delete acceptor_;
    // 释放全部的Connection对象
    for (auto &val : conns_)
    {
        delete val.second;
    }
}

void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newconnection(Socket *clientsock)
{
    Connection *conn = new Connection(&loop_, clientsock); 
    conn->setclosecallback(std::bind(&TcpServer::closeconnection, this, std::placeholders::_1));
    conn->seterrorcallback(std::bind(&TcpServer::errorconnection, this, std::placeholders::_1));
    conn->setonmessagecallback(std::bind(&TcpServer::onmessage, this, std::placeholders::_1,std::placeholders::_2));
    
    printf("new connection(fd=%d,ip=%s,port=%d) ok.\n", conn->fd(), conn->ip().c_str(), conn->port());
    conns_[conn->fd()] = conn;
}

void TcpServer::closeconnection(Connection *conn) // TCP连接关闭 (断开) 的回调函数, 供Connection回调
{
    printf("1 client(%d) close.\n", conn->fd());
    // close(conn->fd());
    conns_.erase(conn->fd());
    delete conn;
}
void TcpServer::errorconnection(Connection *conn) // TCP连接错误的回调函数, 供Connection回调
{
    printf("client(%d) error.\n", conn->fd());
    // close(conn->fd());
    conns_.erase(conn->fd());
    delete conn;
}

void TcpServer::onmessage(Connection* conn,std::string message)
{
    message="reply:"+message;
    int len=message.size();

    std::string tmpbuf((char*)&len,4);
    tmpbuf.append(message);

    conn->send(tmpbuf.data(),tmpbuf.size());
}