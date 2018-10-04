//
//  Runloop.cpp
//  LogParse
//
//  Created by 陈耀武 on 2018/10/3.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#include "RunLoop.h"



RunLoop::RunLoop(const std::string loopname)
{
    m_loopName = loopname;
    m_loopThread = std::thread(&RunLoop::mainloop, this);
    m_loopThread.detach();
}
void RunLoop::mainloop(RunLoop *self)
{
    if (self)
    {
        Log("[%s] Thread started", self->m_loopName.c_str());
        while (true && !self->m_loop_stopped)
        {
            if (!self->m_loop_started)
            {
                self->m_loop_started = true;
            }
            Log("[%s] wait task", self->m_loopName.c_str());
            
            if (!((!(self->m_loop_queue.empty()) && !self->m_loop_stopped))) {
                // if had posted task but thread suppend before wait, then the posted not execute immediately
                std::unique_lock<std::mutex> lock(self->m_loop_mutex);
                self->m_loop_notify.wait(lock);
            }
            
            
            while (!(self->m_loop_queue.empty()) && !self->m_loop_stopped)
            {
                std::function<void(void)> action = nullptr;
                {
                    std::unique_lock<std::mutex> qlock(self->m_loop_queue_mutex);
                    action = self->m_loop_queue.front();
                    self->m_loop_queue.pop();
                }
                if (action)
                {
                    action();
                }
            }
            
        }
        
        if (self->m_loop_stopped) {
            Log("[%s] loop stopped", self->m_loopName.c_str());
        }
    }
    Log("[%s] loop exit", self->m_loopName.c_str());
    
}

void RunLoop::cancel()
{
    this->m_loop_stopped = true;
    if (this->m_loop_started)
    {
        Log("[%s] cancel", this->m_loopName.c_str());
        this->m_loop_notify.notify_one();
    }
}

void RunLoop::postTask(std::function<void ()> action)
{
    if (this->m_loop_stopped.load())
    {
        Log("[%s] post task, but loop has stopped", this->m_loopName.c_str());
        return;
    }
    
    if (action)
    {
        std::unique_lock<std::mutex> qlock(this->m_loop_queue_mutex);
        this->m_loop_queue.push(action);
    }
    if (this->m_loop_started)
    {
        Log("[%s] post task", this->m_loopName.c_str());
        this->m_loop_notify.notify_one();
    }
}


void RunLoop::async(std::function<void(void)> action)
{
    // 如果是在当前线程？
    this->postTask(action);
}

void RunLoop::sync(std::function<void(void)> action)
{
    // 如果是在当前线程
    std::promise<bool> sync;
    this->postTask([=, &sync](){
        if (action) {
            action();
        }
        sync.set_value(true);
    });
    std::future<bool> f = sync.get_future();
    f.get();
}


