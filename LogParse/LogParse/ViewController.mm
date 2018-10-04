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
    LogViewController *logController;
public:
    ViewObserver(){
        viewController = nil;
        logController = nil;
    }
    virtual ~ViewObserver(){}
public:
    
    virtual void on_will_pull_setting(logtool::ALogParse *logParser){
        [logController appendLog:@"on_will_pull_setting"];
        
    }
    virtual void on_did_pull_setting(logtool::ALogParse *logParser, int code, const std::string &info, const logtool::LogParseSettingList &alllist){
        [logController appendCLog:"on_will_pull_setting: code = %d, info = %s", code, info.c_str()];
    }
    
    virtual void on_will_import_setting(logtool::ALogParse *logParser){
        [logController appendCLog:"on_will_import_setting"];
    }
    virtual void on_did_import_setting(logtool::ALogParse *logParser, int code, const std::string &info){
        [logController appendCLog:"on_did_import_setting: code = %d, info = %s", code, info.c_str()];
    }
    
    virtual void on_will_parse_log(logtool::ALogParse *logParser){
        [logController appendCLog:"on_will_parse_log"];
    }
    virtual void on_did_parse_log(logtool::ALogParse *logParser, int index, const std::string &info){
        [logController appendCLog:"on_did_parse_log: index = %d, info = %s", index, info.c_str()];
    }
    
    virtual void on_will_summary_results(logtool::ALogParse *logParser){
        [logController appendCLog:"on_will_summary_results"];
    }
    virtual void on_did_summary_results(logtool::ALogParse *logParser, int code, const std::string &info){
        [logController appendCLog:"on_did_summary_results : code = %d, info = %s", code, info.c_str()];
    }
    
    virtual void on_did_stop_parse(logtool::ALogParse *logParser, int code, const std::string &info){
        [viewController onDidStooLoop];
        [logController appendCLog:"on_did_stop_parse : code = %d, info = %s", code, info.c_str()];
    }
    
    virtual void on_will_export_result(logtool::ALogParse *logParser, const std::string& saveDir, const std::string& savename){
        [logController appendCLog:"on_will_export_result : saveDir = %d, savename = %s", saveDir.c_str(), savename.c_str()];
    }
    virtual void on_did_export_result(logtool::ALogParse *logParser, int code, const std::string &info, const std::string &resultFilePath) {
        [logController appendCLog:"on_did_stop_parse : code = %d, info = %s, resultFilePath = %s", code, info.c_str(), resultFilePath.c_str()];
    }
};

static std::shared_ptr<ViewObserver> logObserver = nullptr;

@implementation ViewController

- (void)onDidStooLoop
{
    dispatch_async(dispatch_get_main_queue(), ^{
        self.startButton.enabled = true;
        self.pullSettingButton.enabled = false;
        self.importLogButton.enabled = false;
        self.startParse.enabled = false;
        self.exportResultButton.enabled = false;
        self.stopButton.enabled = false;
        
        if (self->m_logParser)
        {
            delete self->m_logParser;
            self->m_logParser = nullptr;
        }
    });
    
}

- (IBAction)onStart:(id)sender {
    
    m_logParser = new logtool::LogParser();
    m_logParser->set_observer(logObserver);
    m_logParser->start_loop([&](bool succ) {
        if (succ){
            [logObserver->logController appendLog:@"start_loop"];
            self->_startButton.enabled = NO;
            self->_pullSettingButton.enabled = YES;
            self->_stopButton.enabled = YES;
        }
    });
}

- (IBAction)onPullSetting:(id)sender {
    m_logParser->async_pull_setting();
}

- (IBAction)onStop:(id)sender {
    m_logParser->async_stop_parse();
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    if (logObserver == nullptr) {
        logObserver.reset(new ViewObserver);
        
    }
    
    logObserver->viewController = self;
    
   
    // Do any additional setup after loading the view.
    
    
    
    
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    
    // Update the view, if already loaded.
}


@end


@implementation LogViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    if (logObserver == nullptr) {
        logObserver.reset(new ViewObserver);
        
    }
    logObserver->logController = self;
}

- (void)appendLog:(NSString *)message
{
//    if(message.length == 0) {
//        return;
//    }
//    
//    if([NSThread isMainThread]){
//        NSMutableAttributedString *mStr = [[NSMutableAttributedString alloc] initWithString:message];
//        [mStr appendAttributedString:[[NSAttributedString alloc] initWithString:@"\n"]];
//        [mStr addAttribute:NSForegroundColorAttributeName value:[NSColor blackColor] range:NSMakeRange(0, mStr.length)];
//        [mStr addAttribute:NSFontAttributeName value:[NSFont systemFontOfSize:13] range:NSMakeRange(0, mStr.length)];
//        [_logView.textStorage appendAttributedString:mStr];
//        [_logView scrollRangeToVisible: NSMakeRange(_logView.string.length, 0)];
//    }
//    else{
//        dispatch_async(dispatch_get_main_queue(), ^{
//            [self appendLog:message];
//        });
//    }
}

- (void)appendCLog:(const char *)format, ...
{
    va_list args;
    va_start(args, format);
    NSString *str = [[NSString alloc] initWithFormat:[NSString stringWithUTF8String:format] arguments:args];
    [self appendLog:str];
    va_end(args);
}

@end
