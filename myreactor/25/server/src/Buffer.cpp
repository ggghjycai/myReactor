#include"Buffer.h"


Buffer::Buffer()
{}

Buffer::~Buffer()
{}

void Buffer::append(const char* data,size_t size)
{
    strbuffer_.append(data,size);
}

void Buffer::appendwithhead(const char* data,size_t size)
{
    strbuffer_.append((char*)&size,4);
    strbuffer_.append(data,size);
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