#include"string"

class Buffer
{
private:
    std::string strbuffer_;
public:
    Buffer();
    ~Buffer();
    //追加
   void  append(const char* ch,ssize_t size);
   //返回首地址
   const char* data();
   //返回数据长度(单位字节)
   size_t size();
   //清空容器
    void clear();
    //从指定位置开始删除n个字节
    void erase(int pos,int n);
};