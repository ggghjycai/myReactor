#pragma once
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include<iostream>
#include"ThreadPool.h"
/*
EchoServer类：回显服务器
基于底层的9个类:  

Socket：无论是客户端还是服务器端，Socket都是建立网络通信的基础。
InetAddress：整个网络通信中，需要确定的就是网络地址，所以这个类的重要性很高。
Channel：所有的I/O操作都由Channel提供，所以它的重要性也是毋容置疑的。
Buffer：Buffer中存放了实际的数据，Channel会将数据读到Buffer内，或者将Buffer里的数据写到通道内。没有这个类，我们就无法操作数据。
Connection：建立和处理Connection是网络通信的重要步骤。
Epoll：操作系统级别的I/O复用技术，用于提升服务器性能，管理多个Socket连接。
EventLoop：负责将不同的事件分发到对应的处理程序。
Acceptor：在服务端接受客户端连接的组件，比起其他核心组件其重要性相对要小一些。
TcpServer：具体的应用往往是围绕某个具体的Server进行的，如HttpServer，FtpServer，所以TcpServer重要，但在这些核心类中，可能排在最后。

EchoServer中的每一个方法都对应着最高级的TcpServer中提供的接口.
核心: 这意味着EchoServer可以选择是否实现对应的功能.
注意: 但是,这些是基于回调函数实现的,如果没有实现对应的功能,
      那么TcpServer中对应的包装器函数为空,执行时会报错:
        terminate called after throwing an instance 'std::bad_function_call' what(): bac_function_call 已放弃
解决办法:
       TcpServer在调用回调函数的时候先if判断函数是否为空. 
                                                            
             

*/
class EchoServer
{
private:
    TcpServer tcpserver_;
    ThreadPool threadpool_;         //工作线程

public:
    EchoServer(const std::string &ip,const uint16_t port,int subthreadnum=3,int workthreadnum=5,int wroktimevaL=5,int worktimeout=10);
    ~EchoServer();
    // 启动服务。
    void Start();    
    //终止服务
    void Stop();

    // 处理新客户端连接请求，在TcpServer类中回调此函数。
    void HandleNewConnection(spConnection conn);     
     // 关闭客户端的连接，在TcpServer类中回调此函数。
    void HandleClose(spConnection conn);                      
    // 客户端的连接错误，在TcpServer类中回调此函数。
    void HandleError(spConnection conn);             
    // 处理客户端的请求报文，在TcpServer类中回调此函数。          
    void HandleMessage(spConnection conn,std::string& message);     
    // 数据发送完成后，在TcpServer类中回调此函数。
    void HandleSendComplete(spConnection conn);        
    // epoll_wait()超时，在TcpServer类中回调此函数。
    // void HandleTimeOut(EventLoop *loop);        

    //业务处理函数     
    //处理客户端的请求报文，用于添加给线程池。       
    void OnMessage(spConnection conn,std::string& message);
};