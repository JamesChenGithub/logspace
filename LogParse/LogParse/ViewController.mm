//
//  ViewController.m
//  LogParse
//
//  Created by AlexiChen on 2018/9/6.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#import "ViewController.h"

#include "LogParam.h"
#include "LogParser.h"


class ViewObserver : public logtool::ALogParseObserver
{
public:
    ViewController *viewController;
public:
    ViewObserver(){
        viewController = nil;
    }
    virtual ~ViewObserver(){}
public:
    
    virtual void on_will_pull_setting(logtool::ALogParse *logParser){
        
    }
    virtual void on_did_pull_setting(logtool::ALogParse *logParser, int code, const std::string &info, const logtool::LogParseSettingList &alllist){
        
    }
    
    virtual void on_will_import_setting(logtool::ALogParse *logParser){
        
    }
    virtual void on_did_import_setting(logtool::ALogParse *logParser, int code, const std::string &info){
        
    }
    
    virtual void on_will_parse_log(logtool::ALogParse *logParser){
        
    }
    virtual void on_did_parse_log(logtool::ALogParse *logParser, int index, const std::string &info){
        
    }
    
    virtual void on_will_summary_results(logtool::ALogParse *logParser){
        
    }
    virtual void on_did_summary_results(logtool::ALogParse *logParser, int code, const std::string &info){
        
    }
    
    virtual void on_did_stop_parse(logtool::ALogParse *logParser, int code, const std::string &info){
        
    }
    
    virtual void on_will_export_result(logtool::ALogParse *logParser, const std::string& saveDir, const std::string& savename){
        
    }
    virtual void on_did_export_result(logtool::ALogParse *logParser, int code, const std::string &info, const std::string &resultFilePath) {
    }
};

static ViewObserver logObserver;

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];

    logObserver.viewController = self;
    // Do any additional setup after loading the view.
    m_logParser = new logtool::LogParser();
    m_logParser->start_loop([&](bool succ){
        if (succ){
            m_logParser->async_pull_setting();
        }
    });
    
    
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
