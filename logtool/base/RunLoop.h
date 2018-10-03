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
    bool                                    m_loop_started;
    bool                                    m_loop_stopped;
    
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
};

#endif /* Runloop_hpp */
