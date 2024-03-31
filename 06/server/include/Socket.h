#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h> // TCP_NODELAY需要包含这个头文件。
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "InetAddress.h"

//创建非阻塞的socket
int  createnonblocking();

class Socket
{
private:
    const int fd_;
public:
    Socket(const int fd);
    void closefd();
    ~Socket();
    
   const  int fd() const;

    /*1)
   SO_REUSEADDR选项允许在服务器套接字关闭后立即重新绑定到相同的端口。这对于快速重启服务器并在同一端口上监听新连接非常有用。
   在多个套接字绑定到相同的端口时，此选项可以确保套接字被成功绑定而不会报告“地址已在使用中”的错误。
   */
    void setSO_REUSEADDR(bool flag); 
    /*2)
   TCP_NODELAY选项用于禁用Nagle算法，该算法用于延迟发送小数据包，以便将多个小数据包合并成一个大的数据包发送，以提高网络利用率。
   在某些情况下（例如实时通信），禁用Nagle算法可以减少延迟，但可能会增加网络流量。
   */
    void setTCP_NODELAY(bool flag); 
    /*3)
    SO_REUSEPORT选项允许多个套接字绑定到相同的IP地址和端口，这对于实现负载平衡或多线程服务器很有用。
    通过使用SO_REUSEPORT，多个套接字可以同时接收传入的连接，而不会被内核限制在一个套接字上。
    */
    void setSO_REUSEPORT(bool flag); 
    /*4)
    SO_KEEPALIVE选项允许在连接空闲时发送保持活动（keep-alive）消息以检测对等端的连接状态。
    如果对等端意外关闭连接或网络中断，服务器可以及时发现并采取相应的措施。
    这对于长时间空闲的连接或与不可靠网络连接的情况下很有用，但要注意，它会在网络上增加轻微的负载。
   */
    void setSO_KEEPALIVE(bool flag); 

   
    
    void bind(InetAddress& listenaddr);   
    void listen( int __n);
    int accept(InetAddress& clientaddr);

};