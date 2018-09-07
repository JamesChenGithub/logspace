//
//  LogParam.h
//  LogParse
//
//  Created by AlexiChen on 2018/9/6.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#ifndef LogParam_h
#define LogParam_h

#include <string>
#include <vector>

namespace logtool
{
    class LogVar
    {
    public:
        
        typedef enum LogState
        {
            LogState_Stopped,
            LogState_Parsing,
            LogState_Stoping,
        }LogState;
        // 日志类型
        typedef enum LogType
        {
            AVSDK_LOG,
            IMSDK_LOG,
            ILIVE_LOG,
            XLIVE_LOG,
            XCAST_LOG,
        }LogType;
        
        // 日志类型
        typedef enum LogAPIType
        {
            Log_NONE,
            Log_SYNC,   // 直接输出
            Log_CALL,   // 用户调用
            Log_TIP,    // SDK内部统计
        }LogAPIType;
        
        // 日志类型
        typedef enum LogResultLevel
        {
            Result_OK,
            Result_Warn,
            Result_Err,
        }LogResultLevel;
        
        typedef std::pair<std::string, std::string> LogKeyTip;
        typedef std::tuple<LogResultLevel, std::string, std::string> LogResultTip;
        
        typedef struct LogSettingItem
        {
            std::string task    = "";
            LogAPIType  type    = Log_NONE;
            std::string name    = "行为";
            std::vector<LogKeyTip> apiKey; // type 不为Log_NONE时，必填项；Log_SYNC：表示信息，Log_CALL：调用API起始，Log_TIP：tips关键字
            std::vector<LogKeyTip> apiAux;  // 执行过程中的辅助项
            std::vector<LogKeyTip> apiEnd; // type = Log_CALL时必填, 表现结束项
        private:
            ~LogSettingItem()
            {
                apiKey.clear();
                apiAux.clear();
                apiEnd.clear();
            }
        }LogSettingItem;
        
        typedef struct LogLineItem
        {
            long linenum = 0;
            std::string line;
            std::vector<LogResultTip> result;
        }LogLineItem;
        
    
    };
}


#endif /* LogParam_h */
