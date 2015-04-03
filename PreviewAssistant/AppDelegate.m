//
//  AppDelegate.m
//  PreviewAssistant
//
//  Created by milu on 15/3/30.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import "AppDelegate.h"
#import "Utility.h"
#import "KeyCode.h"
#import "Assistant.h"
@interface AppDelegate ()


@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate



/* 记录变量 */


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    CGEventMask eventMask = NSKeyDownMask |  NSFlagsChangedMask | NSKeyUpMask;
    setHook(eventMask,myCallback);
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    
}




@end
