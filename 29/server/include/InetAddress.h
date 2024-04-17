#pragma once
#include <arpa/inet.h>
#include <netinet/tcp.h> // TCP_NODELAY需要包含这个头文件。
#include <string>

// socket的地址协议类
class InetAddress
{
private:
    sockaddr_in addr_; // 表示地址协议的结构体。

public:
    InetAddress() = default;
    // 如果是监听的fd，用这个构造函数。
    InetAddress(const std::string &ip, const uint16_t port);
    // 如果是客户端连上来的fd，用这个构造函数。
    InetAddress(const sockaddr_in &addr);
    ~InetAddress();

    // 设置addr_成员的值。
    void setaddr(sockaddr_in clientaddr);
    // 返回字符串表示的地址，例如：192.168.150.128F
    const char *ip();
    // 返回整数表示的端口，例如：80、8080
    uint16_t port();
    // 返回addr_成员的地址，转换成了sockaddr。
    const sockaddr *addr() const;
};