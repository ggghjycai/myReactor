#include "EchoServer.h"


EchoServer::EchoServer(const std::string &ip,const uint16_t port,int subthreadnum,int workthreadnum)
:tcpserver_(ip,port,subthreadnum),threadpool_(workthreadnum,"WORKS")    //初始化tcpserver_和 工作线程threadpool_
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
void EchoServer::HandleNewConnection(Connection *conn)    
{
    std::cout<<"Main thread:("<<syscall(SYS_gettid)<< ")--->New Connection Come in." << std::endl;
    //printf("EchoServer::HandleNewConnection() thread is %ld.\n",syscall(SYS_gettid));

    // 根据业务的需求，在这里可以增加其它的代码。
}

// 关闭客户端的连接，在TcpServer类中回调此函数。 
void EchoServer::HandleClose(Connection *conn)  
{
    std::cout <<"Main thread:("<<syscall(SYS_gettid)<< ")--->EchoServer conn closed." << std::endl;
    

    // 根据业务的需求，在这里可以增加其它的代码。
}

// 客户端的连接错误，在TcpServer类中回调此函数。
void EchoServer::HandleError(Connection *conn)  
{
    std::cout <<"Main thread:("<<syscall(SYS_gettid)<< ")--->EchoServer conn error." << std::endl;

    // 根据业务的需求，在这里可以增加其它的代码。
}

// 处理客户端的请求报文，在TcpServer类中回调此函数。
void EchoServer::HandleMessage(Connection *conn,std::string& message)     
{
    //printf("EchoServer::HandleMessage() thread is %ld.\n",syscall(SYS_gettid));
    
    //问题: 如果在处理业务的时候客户端断开了连接,那么会在IO线程中调用Connection的析构函数---->关闭sockfd
    //      这样Connection就成了野指针.   使用野指针的后果不可预知
    //解决办法:  使用智能指针!  
    //理由如下:
       /* 
       示例二
        class AA
            {
            public:
                void show()
                {
                    printf("我是一只傻傻鸟。\n");
                }

                ~AA() 
                {
                    printf("调用了析构函数。\n");
                }
            };

            #include <memory>

            void fun(std::shared_ptr<AA> aa)
            {
                sleep(5);
                aa->show();
            }

            int main()
            {
                ThreadPool tp(2,"TEST");

                {
                    std::shared_ptr<AA> aa(new AA);
                    tp.addtask(std::bind(fun,aa));
                }

                sleep(10);   // 让线程有时间可以运行。

                return 0;
            }
     */
    //当你在std::bind函数中使用std::shared_ptr对象作为参数时，std::bind会创建一个新的std::shared_ptr对象并增加引用计数。
    //std::bind函数返回一个将参数绑定到给定函数的函数对象。当你传递std::shared_ptr对象到std::bind时，它会复制你的std::shared_ptr（而不是裸指针），
    //结果函数对象内部持有std::shared_ptr对象的一个拷贝。这个拷贝也就是新的std::shared_ptr对象，并指向你的AA对象。这样就增加了引用计数。
    //这也是为什么在你的第二个示例中，虽然std::shared_ptr<AA> aa在离开作用域后被销毁，
    //但AA对象并没有立即被销毁，因为还有一个std::bind内部创建的std::shared_ptr对象在持有它。
    //只有当该绑定函数对象被销毁时（即线程执行完任务时），才会减少引用计数，此时如果引用计数为0，则会销毁AA对象。

    //工作线程处理业务
    threadpool_.addtask(std::bind(&EchoServer::OnMessage,this,conn,message),
                        "EchoServer::OnMessage(Connection *conn,std::string& message)");

}

// 数据发送完成后，在TcpServer类中回调此函数。
void EchoServer::HandleSendComplete(Connection *conn)     
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

void EchoServer::OnMessage(Connection *conn,std::string& message)
{
    ////报文: (报文长度+报文体)
    //为了防止分包和粘包 ,采用4字节报文长度+报文体格式

    // 在这里，将经过若干步骤的运算。
    message="reply:"+message;          // 回显业务。    
    conn->send(message.data(),message.size());   // 把临时缓冲区中的数据发送出去。
}