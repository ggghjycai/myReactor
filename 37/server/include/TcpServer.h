#pragma once
#include "Acceptor.h"
#include "Connection.h"
#include <map>
#include"ThreadPool.h"
#include"memory"
#include"casl.h"

class TcpServer
{
private:
    std::unique_ptr<EventLoop>  mainloop_;              //主事件循环
    std::vector<std::unique_ptr<EventLoop>> subloops_;  //存放从事件循环的容器
    int threadnum_;                   //线程池的大小, 即事件存放的个数
    ThreadPool threadpool_;            //线程池
    Acceptor acceptor_;                // 一个Acceptor对应一个EventLoop事件循环 ,一个Tcpserver只有一个Acceptor对象
    std::map<int, spConnection> conns_; // 一个TcpServer有多个Connection对象,存放在map容器中


    std::function<void(spConnection)> newconnectioncb_;          // 回调EchoServer::HandleNewConnection()。
    std::function<void(spConnection)> closeconnectioncb_;        // 回调EchoServer::HandleClose()。
    std::function<void(spConnection)> errorconnectioncb_;         // 回调EchoServer::HandleError()。
    std::function<void(spConnection,std::string &message)> onmessagecb_;        // 回调EchoServer::HandleMessage()。
    std::function<void(spConnection)> sendcompletecb_;            // 回调EchoServer::HandleSendComplete()。
    std::function<void(EventLoop*)>  timeoutcb_;                       // 回调EchoServer::HandleTimeOut()。

    std::mutex mmutex_;

    std::unique_ptr<CASL> tcpcas_;

    int accepttype_;                                             //接受处理的类型
public:
    TcpServer(const std::string &ip, const uint16_t port,int threadnum,int timeval,int timeout,int accepttype); 
    ~TcpServer();
    
    // 运行事件循环
    void start(); 
    //终止I/O线程和事件循环
    void stop();

    // 处理新客户端连接请求，在Acceptor类中回调此函数。
    void newconnection(std::unique_ptr<Socket>);

    // 关闭客户端的连接，在Connection类中回调此函数。
    void closeconnection(spConnection conn);
    // 客户端的连接错误，在Connection类中回调此函数。
    void errorconnection(spConnection conn);
    // 处理客户端的请求报文，在Connection类中回调此函数。
    void onmessage(spConnection conn,std::string& message);
    // 数据发送完成后，在Connection类中回调此函数。
    void sendcomplete(spConnection);
    // epoll_wait()超时，在EventLoop类中回调此函数。
    void epolltimeout(EventLoop*);

    void setnewconnectioncb(std::function<void(spConnection)> fn);
    void setcloseconnectioncb(std::function<void(spConnection)> fn);
    void seterrorconnectioncb(std::function<void(spConnection)> fn);
    void setonmessagecb(std::function<void(spConnection,std::string &message)> fn);
    void setsendcompletecb(std::function<void(spConnection)> fn);
    void settimeoutcb(std::function<void(EventLoop*)> fn);

    // 删除conns_中的Connection对象，在EventLoop::handletimer()中将回调此函数。
    void removeconn(int fd);                 
};
