//
//  HIMLogger.cpp
//  HelloIM
//
//  Created by AlexiChen on 2017/10/10.
//  Copyright © 2017年 AlexiChen. All rights reserved.
//

#include "Logger.h"


namespace logtool {
    
    Logger::Logger():_logStream(NULL), _consoleStream(std::cout)
    {
        
    }
    
    Logger::~Logger()
    {
        this->force_flush();
    }
    
    Logger& Logger::getInstance()
    {
        static logtool::Logger logger;
        static bool hastinitlogger = false;
        if (!hastinitlogger) {
            logger.init();
        }
        hastinitlogger = true;
        return logger;
    }
    
    void Logger::init(const std::string logDir, std::string logFileName, bool console, LogLevel lgl)
    {
        _level = lgl;
        _console = console;
        
        if (logFileName.length() == 0)
        {
            logFileName = Logger::defaultLogFileName();
            logFileName += ".log";
        }
        
        
        std::string fullName = logDir+logFileName;
        
        std::ofstream *stream = new std::ofstream();
        
        stream->open(fullName);
        
        if (stream->good())
        {
            _logStream = std::move(stream);
            
            _logStream->seekp(0, std::ios::end);
            
            std::ifstream ifile(fullName);
            ifile.seekg(0, std::ios::end);
            _totalSize = (uint32_t)ifile.tellg();
            ifile.close();
            
        }
        else
        {
            stream->close();
            delete stream;
            stream = NULL;
        }
        
        _logBuf += "**************************\n";
        if (_console)
        {
            _consoleStream << _logBuf;
        }
        
    }
    
    
    void Logger::catlog(LogLevel level, const std::string module, const std::string funcname, const std::string logcontent)
    {
        if (level == ELogLevel_Off)
        {
            // do nothing;
            return;
        }
        
        std::string time = Logger::_timeMSString();
        std::string lstr;
        switch (level)
        {
            case ELogLevel_Info:
                lstr = "I";
                break;
            case ELogLevel_Warn:
                lstr = "W";
                break;
            case ELogLevel_Error:
                lstr = "E";
                break;
            case ELogLevel_Debug:
                lstr = "D";
                break;
                
            default:
                break;
        }
        
        logCore(time, lstr, module, funcname, logcontent);
    }
    
    void Logger::logCore(const std::string time, const std::string levelstr, const std::string module, const std::string funcname, const std::string logcontent)
    {
        std::lock_guard<std::recursive_mutex> lock(_logMutex);
        std::stringstream ss;
        ss << time << " | " << levelstr << " | " << module << " | " << funcname << " | " << logcontent << std::endl;
        
        std::string log = ss.str();
        
        _logBuf += log;
        
        
        if (_console)
        {
            _consoleStream << ss.str();
        }
        
        if (_logBuf.length() > kBase_Log_BufferSize)
        {
            
            if (_logStream)
            {
                _totalSize += _logBuf.length();
                *_logStream << _logBuf;
                _logStream->flush();
                
                _logBuf = "";
                
            }
        }
        
        
    }
    
    void Logger::force_flush()
    {
        if (_console)
        {
            _consoleStream << "\n**************************\n";
        }
        if (_logBuf.length())
        {
            _logBuf += "\n**************************\n";
            
            _totalSize += _logBuf.length();
            *_logStream << _logBuf;
            _logStream->flush();
            
            ((std::ofstream *)_logStream)->close();
            _logStream = NULL;
        }
        
    }
    
    std::string Logger::defaultLogFileName()
    {
        return Logger::_timeDateString();
    }
    
    
    std::string Logger::_timeDateString()
    {
        return _timeMSString(1);
        
    }
    
    std::string Logger::_timeHMString()
    {
        return _timeMSString(2);
    }
    
    std::string Logger::_timeMSString(int level)
    {
        
        time_t timeS = 0;
        time_t timeMs = 0;
        
        struct timeval tv;
        
        if (0 == gettimeofday(&tv, NULL))
        {
            timeS = tv.tv_sec;
            timeMs = tv.tv_usec / 1000;
        }
        else
        {
            timeS = time(NULL);
        }
        
        struct tm curTime;
        if (NULL == ::localtime_r(&timeS, &curTime))
        {
            return std::string();
        }
        
        
        char dateBuf[255];
        
        if (level == 0)
        {
            strftime(&dateBuf[0], sizeof(dateBuf), "%Y-%m-%d %H:%M:%S", &curTime);
            
            
            std::string buf = dateBuf;
            std::stringstream ss;
            
            char ms[10];
            sprintf(ms, "%03d", (int)timeMs);
            ss << buf << "." << ms;
            
            
            return ss.str();
        }
        else if (level == 1)
        {
            strftime(&dateBuf[0], sizeof(dateBuf), "%Y-%m-%d", &curTime);
            std::string buf = dateBuf;
            return buf;
        }
        else if (level == 2)
        {
            strftime(&dateBuf[0], sizeof(dateBuf), "%Y-%m-%d %H:%M", &curTime);
            std::string buf = dateBuf;
            return buf;
        }
        return "";
    }
    
}
