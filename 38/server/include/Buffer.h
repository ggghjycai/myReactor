#pragma once
#include <string.h>
#include <string>

// 模板函数,用于拼接字符串
template <typename... Args>
std::string sformat(const std::string &fn, Args... args)
{
    int len = snprintf(nullptr, 0, fn.c_str(), args...);

    std::string str;
    str.resize(len);

    snprintf(&str[0], len + 1, fn.c_str(), args...);

    return str;
}
class Buffer
{
private:
    std::string strbuffer_; // 用于存放数据。
    uint16_t seq_;          // 报文的分隔符：0-无分隔符(固定长度、视频会议)；1-四字节的报头；2-"\r\n\r\n"分隔符（http协议）。
    // 专门用于提取http信息
    /*
    HTTP请求并不总是包含Content-Length头部。Content-Length头部字段用于指定请求或响应体的大小（以字节为单位），但它在以下情况中是不必要或不会出现的：
    1)GET请求：通常，一个GET请求不会包含请求体（body），因此也就不会有Content-Length头部。
    GET请求通常用于从服务器检索数据，相关的数据是通过URL的路径或查询字符串传递的。
    2)HEAD请求：HEAD请求和GET请求类似，但服务器在响应中不返回消息体。由于没有消息体，通常不会出现Content-Length头部。
    3)没有消息体的其他HTTP方法：任何其他不包含消息体的HTTP请求，例如，OPTIONS请求，通常也不会包含Content-Length头部。
    4)Transfer-Encoding：如果请求使用了Transfer-Encoding: chunked头部，那么Content
    */
    bool pickhttp(std::string &message);

public:
    Buffer(int seq);
    ~Buffer();

    // 追加数据到strbuffer_中(inputbuffer_)
    void append(const char *ch, size_t size);
    // 把数据追加到strbufferbuf_中(outputbuffer_)
    void appendwithseq(const char *ch, size_t size);
    // 从strbuffer_中取出一条数据放入到传入的string中(inputbuffer_)
    bool pickmessage(std::string &message);

    // 返回首地址
    const char *data();
    // 返回数据长度(单位字节)
    size_t size();
    // 清空容器
    void clear();
    // 从指定位置开始删除n个字节
    void erase(size_t pos, size_t n);
    //返回容量
    int capacity();
};
