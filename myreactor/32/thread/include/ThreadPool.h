#pragma once
#include<thread>
#include<unistd.h>
#include<functional>
#include<condition_variable>
#include<mutex>
#include <atomic>
#include<pthread.h>
#include<queue>
#include<deque>
#include <sys/syscall.h>
#include<iostream>
class ThreadPool
{
private:    
    std::vector<std::thread> threadpool_;           //存放线程
    std::queue<std::function<void()>,std::deque<std::function<void()>>> taskQueue_;               //任务队列
    std::atomic<bool> stop_;                        //原子变量,状态器
    std::mutex mtx_;                                //用于多线程的锁
    std::condition_variable cond_;                  //唤醒线程的条件变量
    const std::string threadtype_;                  // 线程种类："IO"、"WORKS"
    std::string function_name_;              // 线程执行的函数 (类名+函数全称) 
public:
    //在构造函数中创建线程并运行
    ThreadPool(size_t threadnum,const std::string& threadtype);        
    //添加任务到任务队列
    void addtask(std::function<void()>,const std::string& function_name="unknown");         
    //返回线程池中线程的个数
    const uint32_t size();
    //析构函数中等待线程退出           
    ~ThreadPool();
};