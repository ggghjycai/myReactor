#include"Buffer.h"



Buffer::Buffer(int seq):seq_(seq)
{}

Buffer::~Buffer()
{}

void Buffer::append(const char* data,size_t size)
{
    strbuffer_.append(data,size);
}

void Buffer::appendwithseq(const char* data,size_t size)
{
    if(seq_==0)         //不处理粘包和分包
    {
        strbuffer_.append(data,size);
    }
    else if(seq_==1)    //4字节的报文头部,处理粘包和分包
    {
        strbuffer_.append((char*)&size,4);
        strbuffer_.append(data,size);
    }
    else if(seq_==2)    //"\r\n\r\n"分隔符, 处理http协议
    {
        std::string message=sformat("HTTP/1.1 200 OK\r\n"\
        "Content-Length: %lu\r\n"\
        "Server: echoServer\r\n\r\n",size);
        message.append(data,size);
        strbuffer_.append(message);
    }
}
//专门用于提取http信息
/*
HTTP请求并不总是包含Content-Length头部。Content-Length头部字段用于指定请求或响应体的大小（以字节为单位），但它在以下情况中是不必要或不会出现的：
1)GET请求：通常，一个GET请求不会包含请求体（body），因此也就不会有Content-Length头部。
GET请求通常用于从服务器检索数据，相关的数据是通过URL的路径或查询字符串传递的。
2)HEAD请求：HEAD请求和GET请求类似，但服务器在响应中不返回消息体。由于没有消息体，通常不会出现Content-Length头部。
3)没有消息体的其他HTTP方法：任何其他不包含消息体的HTTP请求，例如，OPTIONS请求，通常也不会包含Content-Length头部。
4)Transfer-Encoding：如果请求使用了Transfer-Encoding: chunked头部，那么Content
*/
bool Buffer::pickhttp(std::string& message)
{
    // 查找头部与消息体的分隔符，获取分隔符后字节的位置
    int header_end = strbuffer_.find_first_of("\r\n\r\n");
    if(header_end != std::string::npos)
    {
        //注意：find返回的是分隔符序列的第一个字符位置
        header_end += 4; //跳过分隔符，指向可能的消息体的开始
    
        // 提取整个头部字符串
        std::string header = strbuffer_.substr(0, header_end);
    
        // 查找Content-Length字段
        size_t content_length_pos = header.find("Content-Length: ");
        if(content_length_pos != std::string::npos)
        {
            // 提取Content-Length的值
            content_length_pos += strlen("Content-Length: "); // 跳过字段名
            size_t end_of_field = header.find_first_of("\r\n", content_length_pos);

            std::string content_length_str = header.substr(content_length_pos, end_of_field - content_length_pos);
            int content_length = std::stoi(content_length_str);

            //将header_end后面的content_length长度的报文体放入message;
            message.append(header_end,content_length);

            // 计算整个HTTP报文的长度
            int total_length = header_end + content_length;
            // 现在 total_length 中保存了整个 HTTP 报文的长度
            //从strbuffer_中删除该条报文信息
            strbuffer_.erase(0,total_length);
        }
        else 
        {
            strbuffer_.erase(0,header_end);
        }
    }
    else {return false;}

    return true;
}

bool Buffer::pickmessage(std::string& message)
{
    if(strbuffer_.size()==0)return false;

    message.clear();

    if(seq_==0)         //不处理粘包和分包
    {
        message.append(strbuffer_);
        strbuffer_.clear();
    }
    else if(seq_==1)    //4字节的报文头部,处理粘包和分包
    {
        int len;
        memcpy(&len, strbuffer_.data(), 4); // 从strtbuffer_中获取报文头部。
        // 如果strtbuffer_中的数据量小于报文头部，说明inputbuffer中的报文内容不完整。
        if (strbuffer_.size() < len + 4)return false;
        message.append(strbuffer_.data() + 4, len); 
        strbuffer_.erase(0, len + 4);   
    }
    else if(seq_==2)    //"\r\n\r\n"分隔符, 处理http协议
    {
      if(pickhttp(message)==false)return false;
    }
    return  true;
}

const char* Buffer::data()
{
    return strbuffer_.data();
}

size_t Buffer::size()
{
    return strbuffer_.size();
}

void Buffer::clear()
{
    strbuffer_.clear();
}

void Buffer::erase(size_t pos,size_t n)
{
    strbuffer_.erase(pos,n);
}


//返回容量
int Buffer::capacity()
{
    return strbuffer_.capacity();
}

void Buffer::reserve(int capacity)
{
    strbuffer_.reserve(capacity);
}
/*
int main()
{
    std::string s1="aaaaaaaaaaaaab";
    Buffer buf(1);
    buf.appendwithsep(s1.data(),s1.size());
    std::string s2;
    buf.pickmessage(s2);
    printf("s2=%s\n",s2.c_str());
}
// g++ -g -o test Buffer.cpp
*/