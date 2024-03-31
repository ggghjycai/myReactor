#include"string"

class Buffer
{
private:
    std::string strbuffer_;
public:
    Buffer();
    ~Buffer();
    //追加
   void  append(const char* ch,size_t size);
    // 把数据追加到buf_中，附加报文头部。
   void appendwithhead(const char* ch,size_t size);
   //返回首地址
   const char* data();
   //返回数据长度(单位字节)
   size_t size();
   //清空容器
    void clear();
    //从指定位置开始删除n个字节
    void erase(size_t pos,size_t n);
};