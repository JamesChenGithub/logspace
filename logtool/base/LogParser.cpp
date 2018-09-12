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
    void LogParser::work_runloop(ALogParse *parse)
    {
        
    }
    
    LogParser::LogParser():logtool::ALogParse(), logtool::ALogParseObserver()
    {

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
    
    void LogParser::add_task(std::function<void(void)> action){}
    
    // 日志行为
    void LogParser::async_pull_setting()
    {
        
    }
    
    // 导入分析配置文件
    void LogParser::async_import_setting(const LogParseSettingList &list)
    {
        // CMD 同步导入设置
        
        
        // UI 异步导入设置
    }
    
    // 开始根据配置文件进行分析
    void LogParser::async_parse_log(std::string logpath){}
    
    // 将分析结果进行合并
    void LogParser::async_summary_results(){}
    
    // 将合并后的结果进行统一汇总整理
    void LogParser::async_export_result(const std::string& saveDir, const std::string& savename){}
    
    // 停止分析
    void LogParser::async_stop_parse(){}
    void LogParser::gen_result_exporter(){}
    
    void LogParser::on_will_pull_setting(ALogParse *logParser) {}
    void LogParser::on_did_pull_setting(ALogParse *logParser, int code, const std::string &info, const LogParseSettingList &alllist) {}
    
    void LogParser::on_will_import_setting(ALogParse *logParser){}
    void LogParser::on_did_import_setting(ALogParse *logParser, int code, const std::string &info){}
    
    void LogParser::on_will_parse_log(ALogParse *logParser){}
    void LogParser::on_did_parse_log(ALogParse *logParser, int index, const std::string &info){}
    
    void LogParser::on_will_summary_results(ALogParse *logParser){}
    void LogParser::on_did_summary_results(ALogParse *logParser, int code, const std::string &info){}
    
    void LogParser::on_will_export_result(ALogParse *logParser, const std::string& saveDir, const std::string& savename){}
    void LogParser::on_did_export_result(ALogParse *logParser, int code, const std::string &info, const std::string &resultFilePath){}
    
    void LogParser::on_did_stop_parse(ALogParse *logParser, int code, const std::string &info){}
}
