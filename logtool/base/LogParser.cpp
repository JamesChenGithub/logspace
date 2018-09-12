//
//  LogParser.cpp
//  LogParse
//
//  Created by AlexiChen on 2018/9/7.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#include "LogParser.h"
#include "Logger.h"

namespace logtool
{
    void LogParser::work_runloop(LogParser *self)
    {
        
        if (self)
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(self->m_logparser_mutext);
                self->m_work_notify.wait(lock);
                
                while (!(self->m_work_queue.empty()))
                {
                    std::unique_lock<std::mutex> qlock(self->m_queue_mutex);
                    std::function<void(void)> action = self->m_work_queue.front();
                    self->m_work_queue.pop();
                    if (action)
                    {
                        action();
                    }
                }
                
            }
        }
        
    }
    
    LogParser::LogParser():logtool::ALogParse(), logtool::ALogParseObserver()
    {
        m_workThread = std::thread(&LogParser::work_runloop, this);
        m_workThread.detach();
    }
    
    LogParser::~LogParser()
    {
        m_logPath = "";
        
        std::for_each(m_allLogSettingList.begin(), m_allLogSettingList.end(), [](LogParseSettingItem &item){
            item.reset();
        });
        m_allLogSettingList.clear();
        
        std::for_each(m_logParsingSettingList.begin(), m_logParsingSettingList.end(), [](LogParseSettingItem &item){
            item.reset();
        });
        m_logParsingSettingList.clear();
        
        auto mapIt = m_logResultMap.begin();
        while(mapIt != m_logResultMap.end())
        {
            std::shared_ptr<logtool::LogVar::LogLineItem> result =  mapIt->second;
            result.reset();
            m_logResultMap.erase(mapIt++);
        }
        
    }
    
    // 设置监听
    void LogParser::set_observer(std::shared_ptr<ALogParseObserver> observer)
    {
        m_logObserver.reset();
        m_logObserver = observer;
    }
    
    void LogParser::add_task(std::function<void(void)> action)
    {
        if (action)
        {
            std::unique_lock<std::mutex> qlock(this->m_queue_mutex);
            this->m_work_queue.push(action);
        }
        
        this->m_work_notify.notify_one();
    }
    
    // 日志行为
    void LogParser::async_pull_setting()
    {
        this->add_task([=] {
            std::thread([=]{
                this->add_task([=] {
                    this->on_will_pull_setting(this);
                });
                this->async_load_setting_from_server();
            }).detach();
        });
    }
    
    // 导入分析配置文件
    void LogParser::async_import_setting(const LogParseSettingList &list)
    {
        // CMD 同步导入设置
        
        
        // UI 异步导入设置
    }
    
    // 开始根据配置文件进行分析
    void LogParser::async_parse_log(std::string logpath)
    {
        
    }
    
    // 将分析结果进行合并
    void LogParser::async_summary_results(){}
    
    // 将合并后的结果进行统一汇总整理
    void LogParser::async_export_result(const std::string& saveDir, const std::string& savename){}
    
    // 停止分析
    void LogParser::async_stop_parse(){}
    void LogParser::gen_result_exporter(){}
    
    void LogParser::on_will_pull_setting(ALogParse *logParser)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_will_pull_setting(this);
        }
    }
    
    void LogParser::on_did_pull_setting(ALogParse *logParser, int code, const std::string &info, const LogParseSettingList &alllist)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_did_pull_setting(this, code, info, alllist);
        }
    }
    
    void LogParser::on_will_import_setting(ALogParse *logParser)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_will_import_setting(this);
        }
    }
    
    void LogParser::on_did_import_setting(ALogParse *logParser, int code, const std::string &info)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_did_import_setting(this, code, info);
        }
    }
    
    void LogParser::on_will_parse_log(ALogParse *logParser)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_will_parse_log(this);
        }
    }
    
    void LogParser::on_did_parse_log(ALogParse *logParser, int index, const std::string &info)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_did_parse_log(this, index, info);
        }
    }
    
    void LogParser::on_will_summary_results(ALogParse *logParser)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_will_summary_results(this);
        }
    }
    
    void LogParser::on_did_summary_results(ALogParse *logParser, int code, const std::string &info)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_did_summary_results(this, code, info);
        }
    }
    
    void LogParser::on_will_export_result(ALogParse *logParser, const std::string& saveDir, const std::string& savename)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_will_export_result(this, saveDir, savename);
        }
    }
    
    void LogParser::on_did_export_result(ALogParse *logParser, int code, const std::string &info, const std::string &resultFilePath)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_did_export_result(this, code, info, resultFilePath);
        }
    }
    
    void LogParser::on_did_stop_parse(ALogParse *logParser, int code, const std::string &info)
    {
        std::unique_lock<std::mutex> qlock(this->m_observer_mutex);
        if (!m_logObserver.expired()) {
            auto observer = m_logObserver.lock();
            observer->on_did_stop_parse(this, code, info);
        }
    }
    
    //===============================
    void LogParser::async_load_setting_from_server()
    {
        // 同步下载
        // 同步解析
        
        this->add_task([=] {
        
            this->on_did_pull_setting(this, 0, "pull setting succ", this->m_allLogSettingList);
        });
        
    }
}
