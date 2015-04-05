//
//  WindowController.m
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import "WindowController.h"

static NSObject *instance_ = nil;

@implementation WindowController

@synthesize window,array,view,active,deactivatedObserver,moveObserver,observedApp;

- (id) init
{
    self = [super init];
    array = nil;
    if(self)
        instance_ = self;
    active = false;
    return self;
}
- (void) showLabels:(bool)first;
{
    for(int i = 0;i < array.count;++i)
    {
        MyLabel* obj = [array objectAtIndex:i];
        if(obj.s == nil)
            continue;
        NSTextView* label = [obj label];
        [view addSubview:label];
        [obj showIndicator];
    }
    
    //[view updateConstraints];
    
}

+ (id) instance
{
    return instance_;
}



- (void) generateWord
{
//    NSArray * word = [[NSArray alloc]initWithObjects:
//                        @"E",@"K",@"M",@"A",@"D",@"U",@"V",@"S",@"J",@"H",@"I",@"QJ",@"QK",@"QP",@"WJ",@"WK",@"WP",@"PA",@"PS",@"PD",@"MA",@"MQ",@"MW",@"ME",@"VI",@"VJ",@"VK",@"VP",@"YA",@"YS",nil];
    
    NSArray * word = [[NSArray alloc]initWithObjects:
                      @" E ",@" K ",@" D ",@" U ",@" S ",@" J ",@" H ",@" L ",@" G ",@" I ",@" QJ ",@" QK ",@" QP ",@" WJ ",@" WK ",@" WP ",@" PA ",@" PS ",@" PD ",@" MA ",@" MQ ",@" MW ",@" ME ",@" VI ",@" VJ ",@" VK ",@" VP ",@" YA ",@" YS ",nil];
    
    int n = [word count];
    for(int i = 0;i < array.count;++i)
    {
        MyLabel* label = (MyLabel*)[array objectAtIndex:i];
        if(i < n)
        {
            label.s = [word objectAtIndex:i];
            label.updateLabel;
        }
        else
        {
            label.s = nil;
        }
    }
}
//-(void) setActiveAppName
//{
//    //NSDictionary *activeApp = [[NSWorkspace sharedWorkspace] activeApplication];
//    //self.appName = [activeApp objectForKey:@"NSApplicationName"];
//}
//-(bool) isActiveApp
//{
////    NSDictionary *activeApp = [[NSWorkspace sharedWorkspace] activeApplication];
////    NSString* appName = [activeApp objectForKey:@"NSApplicationName"];
////    // the first time ,return true
////    if([self appName] == nil)
////        return true;
////    if(![[self appName] isEqualToString:appName])
////        return false;
//    return true;
//}
- (void) showAllLabels
{
    if(array == nil) return;
    for(MyLabel* i in array)
        [i show];
    [view updateConstraints];
}
- (void) hideAllLabels
{
    if(array == nil) return;
    for(MyLabel* i in array)
    {
        [i hide];
        [[i label] removeFromSuperviewWithoutNeedingDisplay];
    }
    [array removeAllObjects];
    [view updateConstraints];
}
- (bool) anyLabelSelected
{
    for(MyLabel* i in array)
    {
        if([i isSelected])  return true;
    }
    return false;

}
- (void) showSelectedLabels
{
    for(MyLabel* i in array)
    {
        if([i isSelected]) [i show];
    }
    [view updateConstraints];
}

- (void) hideNoneSelectedLabels
{
    for(MyLabel* i in array)
    {
        if(![i isSelected]) [i hide];
    }
    [view updateConstraints];
}
- (void) backAllLabels
{
    for(MyLabel* i in array)
    {
        [i back];
    }
}
- (void) appendAllLabels:(char) c
{
    for(MyLabel* i in array)
    {
        [i append:c];
    }
}
- (void) appendSelectedLabels:(char) c
{
    for(MyLabel* i in array)
    {
        if([i isSelected])  [i append:c];
    }
}

void observeAppCallBack( AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void *refcon)
{
    
    NSLog(@"observeAppCallBack");
    id instance = [WindowController instance];
    
    if([@"AXApplicationDeactivated" isEqualToString:(__bridge NSString*)notification])
    {
        NSLog(@"AXApplicationDeactivated");
        [instance unObserveApp];
        return;
    }
    
    if([@"AXWindowMoved" isEqualToString:(__bridge NSString*)notification])
    {
        NSLog(@"AXWindowMoved");
        [instance windowMove];
        return;
    }
    NSLog(@"unkonwn notification");
    
    
    
    
//    NSString* s = [instance appName];
//    
//    
//    CFTypeRef title;
//    // should be kAXChildrenAttribute
//    AXUIElementCopyAttributeValue(element, kAXTitleAttribute, (CFTypeRef *)&title);
//    NSString *a = (__bridge NSString *)title;
//    if(![s isEqualToString:a])
//        return;
//    NSLog(@"%@",notification);
    
    
}
pid_t activeAppRef();

- (void) observeApp
{
    //NSLog(@"observeApp");
    pid_t pid = activeAppRef();
    
    AXUIElementRef appRef = AXUIElementCreateApplication(pid);
    observedApp = appRef;
    
    AXError err = nil;

    err = AXObserverCreate(pid,observeAppCallBack,&deactivatedObserver);
    err = AXObserverCreate(pid,observeAppCallBack,&moveObserver);
    if(err != kAXErrorSuccess)
        NSLog(@"AXObserverCreate error %d",err);
    
    err = AXObserverAddNotification(deactivatedObserver,appRef,kAXApplicationDeactivatedNotification,NULL);
    err = AXObserverAddNotification(moveObserver,appRef,kAXWindowMovedNotification,NULL);
    
    if(err == kAXErrorSuccess)
        NSLog(@"AXObserverAddNotification error %d",err);
    
    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(self.deactivatedObserver), kCFRunLoopDefaultMode);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(self.moveObserver), kCFRunLoopDefaultMode);
}
- (void) unObserveApp
{
    
    AXObserverRemoveNotification([self deactivatedObserver],[self observedApp],kAXApplicationDeactivatedNotification);
    AXObserverRemoveNotification([self moveObserver],[self observedApp],kAXWindowMovedNotification);
    return;

}

- (void) userCancel
{
    
    [self hideAllLabels];
    active = false;
}

- (void) windowMove
{
    [self userCancel];
    [self unObserveApp];
    [WindowController handleKeyPress:0x03];
}
char KeyCodeToChar(CGKeyCode k);

+ (void) handleKeyPress:(CGKeyCode)k
{
    
    NSLog(@"new key %d",k);
    WindowController* instance = [WindowController instance];
    char c = KeyCodeToChar(k);
    
    // 创建标签
    if(!instance.active && c == 'f')
    {
        NSLog(@"!instance.active && c == 'f'");
        instance.active = true;
        [instance enumButtons];
        [instance generateWord];
        [instance showLabels:YES];
        return;
    }
    
    if(!instance.active)
    {
        NSLog(@"!instance.active");
        // 只处理滚轮按键等
        return;
    }
    
    
    // Esc
    if(c == 1)
    {
        NSLog(@"ESC");
        [instance userCancel];
        return;

    }
    // BackSpace
    if(c == 2)
    {
        NSLog(@"BackSpace");
        if(instance.array == nil) return;
        
        // 如果是用户取消f
        bool selected = [instance anyLabelSelected];
        if(!selected)
        {
            [instance userCancel];
            return;
            
        }
        
        // 回退
        [instance backAllLabels];
        
        // 隐藏非选中
        if([instance anyLabelSelected])
        {
            [instance hideNoneSelectedLabels];
        }
        else
        {
            [instance showAllLabels];
        }
        return;
    }
    
    NSLog(@"active");
    
    // 处理字母
    // 先检测是否为第一次输入
    bool selected = [instance anyLabelSelected];
    
    // 第一次输入，更新所有
    if(!selected)
    {
        [instance appendAllLabels:c];
    }
    // 非第一次选中，只更新选中
    else
    {
        [instance appendSelectedLabels:c];
    }
    
    // 先隐藏所有标签。再次检查是否有标签被选中，如果有，隐藏其它。否则全部显示
    
    //[instance hideAllLabels];
    
    selected = [instance anyLabelSelected];
    
    if(selected)
    {
        [instance hideNoneSelectedLabels];
    }
    else
    {
        [instance showAllLabels];
    }
    
    [instance.view updateConstraints];
    
    
    // 是否触发
    NSLog(@"instance.array.count = %d",instance.array.count);
    for(MyLabel* i in instance.array)
    {
        if([i isOn])
        {
            NSLog(@"%@",i.s);
            instance.active = false;
            [i performAction];
            [instance hideAllLabels];
            //[[instance targetApp] unhide];
           // [[instance targetApp] activateWithOptions:NSApplicationActivateIgnoringOtherApps];
            
            
            break;
        }
    }

    
    return;

    
//    if(c == 0)
//        return;
//
//    // Esc
//    if(c == 1)
//    {
//        if(instance.active && instance.array != nil)
//        {
//            for(MyLabel* i in instance.array)
//            {
//                [i hide];
//            }
//        }
//        return;
//    }
//    
//    // BackSpace
//    if(c == 2)
//    {
//        if(instance.active && instance.array != nil)
//        {
//            for(MyLabel* i in instance.array)
//            {
//                [i back];
//                
//            }
//        }
//
//        //[instance view updateConstraints];
//        return;
//    }
//    
//    // letter
//    for(MyLabel* i in instance.array)
//    {
//        [i append:c];
//    }
}
NSMutableArray* enumAppButtons();
- (void) enumButtons
{
//    [self setActiveAppName];
    [self observeApp];
    array = enumAppButtons();
    if(array.count == 0)
        active = false;
    //targetApp = [[NSWorkspace sharedWorkspace] frontmostApplication];
    //[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
}
@end
