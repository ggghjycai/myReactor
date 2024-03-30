#include "Acceptor.h"

Acceptor::Acceptor(const std::unique_ptr<EventLoop>& loop, const std::string &ip, const uint16_t port) 
              : loop_(loop), servsock_(createnonblocking()),acceptchannel_(loop_, servsock_.fd())//隐式构造(没有禁用explicit)
{
  //servsock_ = new Socket(createnonblocking());
  InetAddress servaddr(ip, port);
  servsock_.setSO_REUSEPORT(true);
  servsock_.setSO_REUSEADDR(true);
  servsock_.setTCP_NODELAY(true);
  servsock_.setSO_KEEPALIVE(true);
  servsock_.bind(servaddr);
  servsock_.listen(128);

  //acceptchannel_ = new Channel(loop_, servsock_.fd());
  // 为服务端的channel指定回调函数
  // 点睛之笔
  acceptchannel_.setreadcallback(std::bind(&Acceptor::newconnection, this));
  acceptchannel_.enablereading();
}

#include "Connection.h"
// 处理新客户端连接请求
void Acceptor::newconnection()
{
  InetAddress clientaddr;

  std::unique_ptr<Socket> clientsock(new Socket(servsock_.accept(clientaddr)));

  clientsock->setipport(clientaddr.ip(),clientaddr.port());

  newconnectioncb_(std::move(clientsock));  // 回调TcpServer::newconnection()。
}

Acceptor::~Acceptor()
{
  //delete servsock_;
  //delete acceptchannel_;
}

void Acceptor::setnewconnectioncb(std::function<void(std::unique_ptr<Socket>)> fun)
{
  newconnectioncb_ = fun;
}