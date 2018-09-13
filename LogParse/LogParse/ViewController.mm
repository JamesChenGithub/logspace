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

@implementation ViewController

- (void)dealloc
{

}

- (void)viewDidLoad {
    [super viewDidLoad];

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
