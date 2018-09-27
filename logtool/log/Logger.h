//
//  HIMLogger.hpp
//  HelloIM
//
//  Created by AlexiChen on 2017/10/10.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#ifndef HIMLogger_hpp
#define HIMLogger_hpp

#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <thread>
namespace logtool
{
    
    
    enum LogLevel
    {
        ELogLevel_Off,      // 关掉日志
        ELogLevel_Error,    // 错误日志
        ELogLevel_Warn,     // 警告日志
        ELogLevel_Info,     // 信息日志
        ELogLevel_Debug,    // 调试日志
        ELogLevel_Verbose,  // 所有信息
        
    };
    
    const static uint32_t kBase_Log_MaxSize = 50 * 1024 * 1024;
    const static size_t kBase_Log_BufferSize = 32 * 1024;
    const static int kBase_Log_LogMaxNum = 10;
    
    
    class Logger
    {
    private:
        std::string             _fileName;      // 日志文件名
        
        std::ostream            *_logStream;
        std::ostream            &_consoleStream;
        
        std::string             _logBuf;
        
        LogLevel                _level;
        bool                    _console;
        
        uint32_t                _totalSize;
        
        std::recursive_mutex    _logMutex;
        
    private:
        Logger();
        
        ~Logger();
    private:
        Logger& operator=(const Logger&) = delete;
        Logger(const Logger&) = delete;
        Logger(const Logger&&) = delete;
        
        // 对外接口
    private:
        void init(const std::string logDir = "", std::string logFileName = "", bool console = true, LogLevel lgl = ELogLevel_Debug);
        
    public:
        static Logger& getInstance();
        
    public:
        void catlog(LogLevel level, const std::string module, const std::string funcname, int line, const std::string &format, ...);
        void catlog(LogLevel level, const std::string module, const std::string funcname, int line, const char *format, ...);
        void force_flush();
    private:
        void appendLog(LogLevel level, const std::string module, const std::string funcname, int line, const std::string logcontent);
        void logCore(const std::string time, const std::string levelstr, const std::string module, const std::string funcname, int line, const std::string logcontent);
    private:
        static std::string _timeDateString();
        static std::string _timeHMString();
        static std::string _timeMSString(int level = 0);
        static std::string defaultLogFileName();
    };

#define __FILENAME__ (strrchr(__FILE__, '/') + 1)

#define Log(fmt, ...)  logtool::Logger::getInstance().catlog(ELogLevel_Debug, __FILENAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
    
}

#endif /* HIMLogger_hpp */
