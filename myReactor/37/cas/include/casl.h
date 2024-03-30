#pragma once
#include<atomic>
class CASL
{
private:
    std::atomic<bool> flag;
public:
    CASL();  
    ~CASL();
    void lock();
    void unlock();
 
};