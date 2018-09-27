//
//  ViewController.h
//  LogParse
//
//  Created by AlexiChen on 2018/9/6.
//  Copyright © 2018年 AlexiChen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "ALogParse.h"

@interface ViewController : NSSplitViewController
{
    logtool::ALogParse *m_logParser;
}


@end

