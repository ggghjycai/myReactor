#include "ThreadPool.h"



ThreadPool::ThreadPool(size_t threadnum) : stop_(false)
{

    for (size_t ii = 0; ii < threadnum; ii++)
    {
        threadpool_.emplace_back
        ([this]{          

            printf("create thread(%ld).\n",syscall(SYS_gettid));     // 显示线程ID。
            std::cout << "子线程：" << std::this_thread::get_id() << std::endl;

            while(stop_==false)
            {
                std::function<void()> task;     //存放任务队列中的任务
                //下面的代码用于获取资源
                {   // 锁作用域的开始。 ///////////////////////////////////
                    std::unique_lock<std::mutex> ulock(this->mtx_);

                    //带条件的被阻塞：wait函数设置了谓词(Predicate)，
                    //只有当pred条件为false时调用该wait函数才会阻塞当前线程，并且在收到其它线程的通知后只有当pred为true时才会被解除阻塞。因此，等效于：
                    //while(!pred()) wait(lck)
                    this->cond_.wait(ulock,[this]{
                        
                        return  ((this->stop_==true)||(this->taskqueue_.empty()==false)); //如果stop为true或者队列不为空,那么就不阻塞
                    });
                    
                    //如果stop_为true 并且 任务队列中没有任务了,才退出函数
                    //否则, 即使stop_设置为true, 也要等到任务队列为空
                    if((this->stop_==true)&&(this->taskqueue_.empty()==true)) return;


                    //获取一个任务
                    task=std::move(this->taskqueue_.front());
                    this->taskqueue_.pop();

                }   // 锁作用域的结束。 ///////////////////////////////////

                printf("thread is %ld.\n",syscall(SYS_gettid));
                //接下来执行任务:
                task();
            }   
        });
    }
}

void ThreadPool::addtask(std::function<void()> task)
{
    std::lock_guard<std::mutex> lokcguard(mtx_);

    taskqueue_.push(task);

    cond_.notify_one();
}


ThreadPool::~ThreadPool()
{
    stop_.store(true);

    cond_.notify_all();

    for(std::thread &val:threadpoll_)
    {
        val.join();
    }
}



