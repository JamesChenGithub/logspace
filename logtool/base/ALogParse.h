//
//  ALogParse.h
//  LogParse
//
//  Created by AlexiChen on 2018/9/6.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#ifndef ALogParse_h
#define ALogParse_h

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <list>
#include <algorithm>

#include "LogParam.h"

namespace logtool {
    
    class ALogParse;
    class ALogParseObserver;
    class ALogResultExporter;
    
    typedef std::shared_ptr<logtool::LogVar::LogSettingItem> LogParseSettingItem;
    typedef std::list<LogParseSettingItem> LogParseSettingList;
    typedef std::map<long, std::shared_ptr<logtool::LogVar::LogLineItem>> LogParseResultMap;
    
    class ALogParse
    {
    public:
        virtual ~ALogParse(){}
        // 设置监听
        virtual void set_observer(std::shared_ptr<ALogParseObserver> observer) = 0;
        
        // 导入分析配置文件
        virtual void async_pull_setting() = 0;
        
        // 导入分析配置文件
        virtual void async_import_setting(const LogParseSettingList &list) = 0;
        
        // 开始根据配置文件进行分析
        virtual void async_parse_log(std::string logpath) = 0;
        
        // 将分析结果进行合并
        virtual void async_summary_results() = 0;
        
        // 将合并后的结果进行统一汇总整理
        virtual void async_export_result(const std::string& saveDir = "", const std::string& savename = "") = 0;
        
        // 停止分析
        virtual void async_stop_parse() = 0;
        
    protected:
        virtual void gen_result_exporter() = 0;
        
        
    };
    
    
    class ALogResultExporterObserver
    {
    protected:
        virtual ~ALogResultExporterObserver(){};
        
    protected:
        virtual void on_will_export_result(ALogParse *logParser, const std::string& saveDir, const std::string& savename) = 0;
        virtual void on_did_export_result(ALogParse *logParser, int code, const std::string &info, const std::string &resultFilePath) = 0;
    };
    
    class ALogResultExporter
    {
    protected:
        virtual ~ALogResultExporter(){};
        
    public:
        virtual void set_observer(std::shared_ptr<ALogResultExporterObserver> observer) = 0;
        
    public:
        virtual void set_log_result(const LogParseResultMap &map) = 0;
        virtual void export_result() = 0;
        virtual void start_export() = 0;
        virtual void stop_export() = 0;
        
    };
    
    
    class ALogParseObserver : public ALogResultExporterObserver
    {
    protected:
        virtual ~ALogParseObserver(){};
    protected:
        
        virtual void on_will_pull_setting(ALogParse *logParser) = 0;
        virtual void on_did_pull_setting(ALogParse *logParser, int code, const std::string &info, const LogParseSettingList &alllist) = 0;
        
        virtual void on_will_import_setting(ALogParse *logParser) = 0;
        virtual void on_did_import_setting(ALogParse *logParser, int code, const std::string &info) = 0;
        
        virtual void on_will_parse_log(ALogParse *logParser) = 0;
        virtual void on_did_parse_log(ALogParse *logParser, int index, const std::string &info) = 0;
        
        virtual void on_will_summary_results(ALogParse *logParser){};
        virtual void on_did_summary_results(ALogParse *logParser, int code, const std::string &info){};
        
        virtual void on_did_stop_parse(ALogParse *logParser, int code, const std::string &info) = 0;
    };
}


#endif /* ALogParse_h */
