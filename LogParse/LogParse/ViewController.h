//
//  ViewController.h
//  LogParse
//
//  Created by AlexiChen on 2018/9/6.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "ALogParse.h"

@interface ViewController : NSViewController
{
    logtool::ALogParse *m_logParser;
}
@property (weak) IBOutlet NSButton *startButton;
@property (weak) IBOutlet NSButton *pullSettingButton;
@property (weak) IBOutlet NSButton *importLogButton;
@property (weak) IBOutlet NSButton *startParse;
@property (weak) IBOutlet NSButton *exportResultButton;
@property (weak) IBOutlet NSButtonCell *stopButton;

- (void)onDidStooLoop;
@end


@interface LogViewController : NSViewController
@property (weak) IBOutlet NSTextView *logView;
- (void)appendLog:(NSString *)message;
- (void)appendCLog:(const char *)format, ...;
@end
