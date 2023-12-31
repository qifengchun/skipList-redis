#include "threadpool.h"

//构造函数，把线程插入线程队列，插入时调用embrace_back(),用匿名函数lambda初始化Thread对象
ThreadPool::ThreadPool(size_t threads) : stop(false){

    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
                    [this]
        {
            for(;;)
            {
                //task是一个函数类型，从人物队列接收任务
                std::function<void()> task;
                {
                    //给互斥量加锁，锁对象生命周期结束后自动解锁
                    std::unique_lock<std::mutex> lock(this->queue_mutex);

                    //（1）当匿名函数返回false时才阻塞线程，阻塞时自动释放锁。
                    //（2）当匿名函数返回true且受到通知时解阻塞，然后加锁。
                    this->condition.wait(lock,[this]{return this->stop||!this->tasks.empty();});

                    if(this->stop&&this->tasks.empty())
                        return;
                    //从任务队列取出一个任务
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        }
        );
}

//添加新的任务到任务队列
template<class F,class...Args>
auto ThreadPool::enqueue(F&& f,Args&&... args)
->std::future<typename std::result_of<F(Args...)>::type>
{
    //获取函数返回值类型
    using return_type = typename std::result_of<F(Args...)>::type;

    //创建一个指向任务的智能指针
    auto task = std::make_shared<std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f),std::forward<Args>(args)...)
                );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);//加锁
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        task.emplace([task](){(*task)();});//把任务加入队列
    }
    condition.notify_one();                //自动解锁
    return res;                            //通知条件变量，唤醒一个线程
}

//析构函数，删除所有线程
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker:workers)
        worker.join();
}
