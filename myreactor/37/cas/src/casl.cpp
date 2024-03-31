#include"casl.h"

CASL::CASL():flag(false)
{}

CASL::~CASL(){}

void CASL::lock()
{
    bool expect=false;

    //这段代码使用了C++中的原子操作，具体是使用了std::atomic提供的compare_exchange_strong成员函数。
    //这是多线程同步中常用的一个操作，用于实现锁的功能。
    //std::atomic<bool> flag; 假设这里flag是一个原子布尔类型的变量。
    //compare_exchange_strong是一种比较并交换的原子操作，它的行为如下：
    //1) 首先，它将flag当前的值与expect的值进行比较。
    //2) 如果这两个值相等（表示flag目前是未锁定状态），
    //它就将flag的值设置为compare_exchange_strong第二个参数（这里是true），表示现在锁定状态，并返回true。
    //3) 如果不相等（表示flag已经被其他线程锁定），则它将flag当前的值重新存储到expect中，以便你可以知道flag的当前值，并返回false
    
    while(!flag.compare_exchange_strong(expect,true))
    {
        expect=false;
    }
}

void CASL::unlock()
{
 flag.store(false);
}

