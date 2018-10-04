//
//  Runloop.hpp
//  LogParse
//
//  Created by 陈耀武 on 2018/10/3.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#ifndef Runloop_hpp
#define Runloop_hpp

#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <future>
#include "Logger.h"

class RunLoop
{
private:
    std::string                             m_loopName;
    std::mutex                              m_loop_mutex;
private:
    std::thread                             m_loopThread;
    std::queue<std::function<void(void)>>   m_loop_queue;
    std::mutex                              m_loop_queue_mutex;
    std::condition_variable                 m_loop_notify;
    
private:
    std::atomic<bool>                       m_loop_started;
    std::atomic<bool>                       m_loop_stopped;
    
public:
    RunLoop(const std::string loopname);
protected:
    virtual ~RunLoop(){};
    
private:
    static void mainloop(RunLoop *loop);
    
private:
    RunLoop() = delete;
    RunLoop& operator=(const RunLoop&) = delete;
    RunLoop(const RunLoop&) = delete;
    RunLoop(const RunLoop&&) = delete;
    
private:
    void postTask(std::function<void(void)> action);
public:
    void async(std::function<void(void)> action);
    void sync(std::function<void(void)> action);
    
public:
    void cancel();
    
public:
public:
    // 提交一个任务
    // 调用.get()获取返回值会等待任务执行完,获取返回值
    // 有两种方法可以实现调用类成员，
    // 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
    // 一种是用 mem_fn： .commit(std::mem_fn(&Dog::sayHello), &dog)
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
    {
        if (this->m_loop_stopped.load())
        {
            Log("[%s] post task", this->m_loopName.c_str());
            return;
        }
        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()> >(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> future = task->get_future();
        
        {
            std::unique_lock<std::mutex> qlock(this->m_loop_queue_mutex);
            this->m_loop_queue.push([task]{
                (*task)();
            });
        }
        
        if (this->m_loop_started.load())
        {
            Log("[%s] commit task", this->m_loopName.c_str());
            this->m_loop_notify.notify_one();
        }
        
        return future;
    }

};

#endif /* Runloop_hpp */
