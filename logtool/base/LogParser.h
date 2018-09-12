//
//  LogParser.hpp
//  LogParse
//
//  Created by AlexiChen on 2018/9/7.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#ifndef LogParser_H
#define LogParser_H

#include "ALogParse.h"
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

namespace logtool
{
    class LogParser : public ALogParse, public ALogParseObserver
    {
    protected:
        logtool::LogVar::LogState               m_logParseState;
        std::string                             m_logPath;
        LogParseSettingList                     m_logParsingSettingList;
        LogParseSettingList                     m_allLogSettingList;
        std::weak_ptr<ALogParseObserver>        m_logObserver;
        LogParseResultMap                       m_logResultMap;
        std::shared_ptr<ALogResultExporter>     m_logResultExporter;
        
    protected:
        std::thread                             m_workThread;
        std::queue<std::function<void(void)>>   m_work_queue;
        std::condition_variable                 m_work_notify;
        std::mutex                              m_setting_mutex;
        std::mutex                              m_result_mutex;
    public:
        LogParser();
        virtual ~LogParser();
        
        
        
    private:
        static void work_runloop(ALogParse *parse);
        
    private:
        LogParser& operator=(const LogParser&) = delete;
        LogParser(const LogParser&) = delete;
        LogParser(const LogParser&&) = delete;
        
    protected:
        void add_task(std::function<void(void)> action);
        
        // 日志行为
    public:
        // 设置监听
        virtual void set_observer(std::shared_ptr<ALogParseObserver> observer);
        
        virtual void async_pull_setting();
        
        // 导入分析配置文件
        virtual void async_import_setting(const LogParseSettingList &list);
        
        
        // 开始根据配置文件进行分析
        virtual void async_parse_log(std::string logpath);
        
        // 将分析结果进行合并
        virtual void async_summary_results();
        
        // 将合并后的结果进行统一汇总整理
        virtual void async_export_result(const std::string& saveDir = "", const std::string& savename = "");
        
        // 停止分析
        virtual void async_stop_parse();
        virtual void gen_result_exporter();
    protected:
        virtual void on_will_pull_setting(ALogParse *logParser);
        virtual void on_did_pull_setting(ALogParse *logParser, int code, const std::string &info, const LogParseSettingList &alllist);
        
        virtual void on_will_import_setting(ALogParse *logParser);
        virtual void on_did_import_setting(ALogParse *logParser, int code, const std::string &info);
        
        virtual void on_will_parse_log(ALogParse *logParser);
        virtual void on_did_parse_log(ALogParse *logParser, int index, const std::string &info);
        
        virtual void on_will_summary_results(ALogParse *logParser);
        virtual void on_did_summary_results(ALogParse *logParser, int code, const std::string &info);
        
        virtual void on_will_export_result(ALogParse *logParser, const std::string& saveDir, const std::string& savename);
        virtual void on_did_export_result(ALogParse *logParser, int code, const std::string &info, const std::string &resultFilePath);
        
        virtual void on_did_stop_parse(ALogParse *logParser, int code, const std::string &info);
    };
}

#endif /* LogParser_hpp */
