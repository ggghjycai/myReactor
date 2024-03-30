#include"Buffer.h"


Buffer::Buffer()
{}

Buffer::~Buffer()
{}

void Buffer::append(const char* ch,ssize_t size)
{
    strbuffer_.append(ch,size);
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
