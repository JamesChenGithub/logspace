//
//  LogParser.cpp
//  LogParse
//
//  Created by AlexiChen on 2018/9/7.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#include "LogParser.h"
#include "Logger.h"
#include "curl.h"
namespace logtool
{
    void LogParser::work_runloop(LogParser *self)
    {
        self->m_loopstarted.set_value(true);
        Log("LogParser Thread started");
        if (self)
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(self->m_logparser_mutext);
                self->m_work_notify.wait(lock);
                
                while (!(self->m_work_queue.empty()))
                {
                    std::function<void(void)> action = nullptr;
                    {
                        std::unique_lock<std::mutex> qlock(self->m_queue_mutex);
                        action = self->m_work_queue.front();
                        self->m_work_queue.pop();
                    }
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
        
    }
    
    void LogParser::start_loop(std::function<void(bool)> callback)
    {
    
        m_workThread = std::thread(&LogParser::work_runloop, this);
        m_workThread.detach();
        std::future<bool> f = m_loopstarted.get_future();
        auto r = f.get();
        if (r)
        {
            callback(r);
        }
        
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
        Log("post async_pull_setting task");
        this->add_task([=] {
            
            std::promise<bool> willpull;
            
            std::thread([&](std::promise<bool>& pull){
                pull.set_value(true);
                this->async_load_setting_from_server();
            },std::ref(willpull)).detach();
            
            std::future<bool> result = willpull.get_future();
            auto succ = result.get();
            if (succ) {
                this->add_task([=] {
                    Log("post on_will_pull_setting callback");
                    this->on_will_pull_setting(this);
                });
            }
        });
    }
    
    // 导入分析配置文件
    void LogParser::async_import_setting(const LogParseSettingList &list)
    {
    
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
    std::string  LogParser::setting_url()
    {
        return "http://127.0.0.1:8080/logparse/sdk_parse_config.json";
    }
    
    static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
    {
        std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
        if( NULL == str || NULL == buffer )
        {
            return -1;
        }
        
        char* pData = (char*)buffer;
        str->append(pData, size * nmemb);
        return nmemb;
    }
    
    void LogParser::async_load_setting_from_server()
    {
        std::string strResponse;
        // 同步下载
        {
            std::string strUrl = this->setting_url();
            
            CURL* curl = curl_easy_init();
            if(NULL == curl)
            {
                Log("post create curl failed");
                this->add_task([=] {
                    this->on_did_pull_setting(this, -1, "create curl failed", this->m_allLogSettingList);
                });
                return ;
            }
            
            CURLcode res;
            
            curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);//设定为不验证证书和HOST
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
            
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        
        if (strResponse.empty()) {
            Log("post download setting failed");
            this->add_task([=] {
                this->on_did_pull_setting(this, -2, "download setting failed", this->m_allLogSettingList);
            });
            return;
        }
        
        // 同步解析
        
        this->add_task([=] {
            Log("post download setting succ");
            this->on_did_pull_setting(this, 0, "pull setting succ", this->m_allLogSettingList);
        });
        
    }
}
