#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>//数组
#include <queue>//队列
#include <memory>//内存
#include <thread>//线程
#include <mutex>//互斥锁
#include <condition_variable>//条件变量
#include <future>//处理异步编程、并发和未来任务的功能
#include <functional>//函数模板
#include <stdexcept>//异常类，用来处理异常

class ThreadPool
{
public:
    ThreadPool(size_t);                          //构造函数
    template<class F, class... Args>             //类模板
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;//任务入队

    template<class F>
    void AddTask(F&& task){
        {   //使用queue_mutex来保护临界区
            //确保在多线程环境中对共享资源的安全访问
            std::lock_guard<std::mutex> locker(queue_mutex);
            //emplace这一行将任务添加到任务队列
            //forward用于完美转发任务，以保留其原始类型
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();//这一行用于通知等待的线程，有一个新任务可用
    }
    ~ThreadPool();

private:
    std::vector<std::thread> worker;
    std::queue<std::function<void> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

}


#endif // THREADPOOL_H
