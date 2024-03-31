#include "InetAddress.h"

InetAddress::InetAddress(const std::string &ip, const uint16_t port)
{
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const sockaddr_in &addr) : addr_(addr)
{
}

InetAddress::~InetAddress()
{
}

void InetAddress::setaddr(sockaddr_in clientaddr)
{
  addr_ = clientaddr;
}

const char *InetAddress::ip()
{
  return inet_ntoa(addr_.sin_addr);
}

uint16_t InetAddress::port()
{
  return ntohs(addr_.sin_port);
}

const sockaddr *InetAddress::addr()
{
  return (const sockaddr *)&addr_;
}
