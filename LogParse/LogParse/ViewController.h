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


@end


@interface LogViewController : NSViewController

@end
