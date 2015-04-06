//
//  WindowController.m
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import "WindowController.h"
#import "KeyCode.h"

static NSObject *instance_ = nil;

@implementation WindowController

@synthesize window,array,view,mode;
@synthesize scrollSpeed;
@synthesize observedApp,deactivatedObserver,moveObserver,hideObserver,resizeObserver;
@synthesize last_Esc;
@synthesize  last_Key;

- (id) init
{
    self = [super init];
    if(self)
        instance_ = self;
    
    [self reset_];
    
    return self;
}
+ (void) reset
{
    [[WindowController instance] reset_];
    
}
- (void) reset_
{
    scrollSpeed = 20;
    observedApp = deactivatedObserver = moveObserver = hideObserver = resizeObserver = nil;
    last_Esc = 0;
    last_Key = 0;
    array = nil;
    mode = Ready;
    
    
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

    NSArray * word = [[NSArray alloc]initWithObjects:
                      @" E ",@" K ",@" D ",@" U ",@" S ",@" J ",@" L ",@" G ",@" I ",@" QK ",@" QP ",@" WH ",@" WJ ",@" WK ",@" WL ",@" WU ",@" WI ",@" WO ",@" WP ",@" PA ",@" PS ",@" PD ",@" PJ ",@" MA ",@" MQ ",@" MW ",@" ME ",@" VI ",@" VJ ",@" VK ",@" VP ",@" VL ",@" YA ",@" YS ",@" HQ ",@" HW ",@" HE ",@" HA ",@" NA ",@" NS ",@" ND ",@" NQ ",@" NW ",@" NE ",@" NF ",@" NG ",nil];

    
    int n = [word count];
    int c = array.count;
    
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
    WindowController* instance = [WindowController instance];
    
    NSString* noti = (__bridge NSString*)notification;
    if([@"AXApplicationDeactivated" isEqualToString:noti])
    {
        NSLog(@"AXApplicationDeactivated");
        [instance userCancel];
        [instance unObserveApp];
        return;
    }
    
    if([@"AXWindowMoved" isEqualToString:noti])
    {
        NSLog(noti);
        
        if([instance mode] == Showing)
        {
            [instance userCancel];
            [instance unObserveApp];
            [WindowController handleKeyPress:0x03];
        }
        else
        {
            [instance userCancel];
            [instance unObserveApp];
        }
        return;
    }
    if([@"AXMainWindowChanged" isEqualToString:noti])
    {
        NSLog(@"AXMainWindowChanged");
        [instance userCancel];
        [instance unObserveApp];
        return;
    }
    NSLog(@"unknown notification");
    
    
    
    
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
    if(err != kAXErrorSuccess)
    {
        NSLog(@"deactivatedObserver error %d",err);
        deactivatedObserver = nil;
        return;
    }
    
    err = AXObserverCreate(pid,observeAppCallBack,&moveObserver);
    if(err != kAXErrorSuccess)
    {
         NSLog(@"moveObserver error %d",err);
         moveObserver = nil;
        return;
    }
    
//    err = AXObserverCreate(pid,observeAppCallBack,&resizeObserver);
//    if(err != kAXErrorSuccess)
//    {
//        NSLog(@"moveObserver error %d",err);
//        resizeObserver = nil;
//        return;
//    }
    err = AXObserverCreate(pid,observeAppCallBack,&hideObserver);
    if(err != kAXErrorSuccess)
    {
        NSLog(@"hideObserver error %d",err);
        hideObserver = nil;
        return;
    }
    
    err = AXObserverAddNotification(moveObserver,appRef,kAXWindowMovedNotification,NULL);
    
    if(err != kAXErrorSuccess)
    {
        NSLog(@"add kAXWindowMovedNotification failed");
        return;
    }
    
    err = AXObserverAddNotification(deactivatedObserver,appRef,kAXApplicationDeactivatedNotification,NULL);
    if(err != kAXErrorSuccess)
    {
        NSLog(@"add kAXApplicationDeactivatedNotification failed");
        return;
    }
    
//    err = AXObserverAddNotification(resizeObserver,appRef,kAXWindowResizedNotification,NULL);
//    if(err != kAXErrorSuccess)
//    {
//        NSLog(@"add kAXWindowResizedNotification failed");
//        return;
//    }
//    
    err = AXObserverAddNotification(hideObserver,appRef,kAXMainWindowChangedNotification,NULL);
    if(err != kAXErrorSuccess)
    {
        NSLog(@"add kAXMainWindowChangedNotification failed");
        return;
    }

    
    
    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(self.deactivatedObserver), kCFRunLoopDefaultMode);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(self.moveObserver), kCFRunLoopDefaultMode);
//    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(self.resizeObserver), kCFRunLoopDefaultMode);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), AXObserverGetRunLoopSource(self.hideObserver), kCFRunLoopDefaultMode);
    
}
- (void) unObserveApp
{
    if(observedApp == nil)
        return;
    
    if(deactivatedObserver != nil)
        AXObserverRemoveNotification([self deactivatedObserver],[self observedApp],kAXApplicationDeactivatedNotification);
    if(moveObserver != nil)
        AXObserverRemoveNotification([self moveObserver],[self observedApp],kAXWindowMovedNotification);
//    if(resizeObserver != nil)
//        AXObserverRemoveNotification([self resizeObserver],[self observedApp],kAXWindowMovedNotification);
    if(hideObserver != nil)
        AXObserverRemoveNotification([self hideObserver],[self observedApp],kAXWindowMovedNotification);
    
    return;

}

- (void) userCancel
{
    
    [self hideAllLabels];
    mode = Ready;
}

- (void) windowMove
{
    [self userCancel];
    [self unObserveApp];
    [WindowController handleKeyPress:0x03];
}

- (void) toInSertring
{
    [self userCancel];
    mode = InSerting;
}
- (void) toShowing
{
    mode = Showing;
    
}
- (void) toSleeping
{
    mode = sleeping;
}
- (void) toReady
{
    [self userCancel];
    mode = Ready;
}
- (void) backSpace
{
    if(array == nil) return;
    
    // 如果是用户取消f
    bool selected = [self anyLabelSelected];
    if(!selected)
    {
        [self userCancel];
        return;
    }
    
    // 回退
    [self backAllLabels];
    
    // 隐藏非选中
    if([self anyLabelSelected])
    {
        [self hideNoneSelectedLabels];
    }
    else
    {
        [self showAllLabels];
    }
    return;
}

- (void) handleLetter:(CGKeyCode)k
{
    char c = [self CGKeyCode2Char:k];
    
    // 先检测是否为第一次输入
    bool selected = [self anyLabelSelected];
    
    // 第一次输入，更新所有
    if(!selected)
    {
        [self appendAllLabels:c];
    }
    // 非第一次选中，只更新选中
    else
    {
        [self appendSelectedLabels:c];
    }
    // 先隐藏所有标签。再次检查是否有标签被选中，如果有，隐藏其它。否则全部显示
    
    selected = [self anyLabelSelected];
    
    if(selected)
    {
        [self hideNoneSelectedLabels];
    }
    else
    {
        [self showAllLabels];
    }
    
    [self.view updateConstraints];
    
    
    // 是否触发
    //    NSLog(@"instance.array.count = %@",instance.array.count);
    for(MyLabel* i in self.array)
    {
        if([i isOn])
        {
            NSLog(@"%@",i.s);
            mode = Ready;
            
//            if(i.role == input)
//                mode = InSerting;
//            else
//            {
//                mode = Ready;
//            }
            [i performAction];
            [self hideAllLabels];
            //[[instance targetApp] unhide];
            // [[instance targetApp] activateWithOptions:NSApplicationActivateIgnoringOtherApps];
            
            break;
        }
    }

}
void sendMouseSingleClick(CGEventRef down,CGEventRef up);
void sendMouseClickLeft(NSPoint pt);
void sendScroll(int offset);

NSPoint getCursorPos();
- (void) singleClick
{
    NSPoint pt = getCursorPos();
    sendMouseClickLeft(pt);
    CGDisplayHideCursor(kCGDirectMainDisplay);
}

void sendMouseDoubleClick(NSPoint pt);
- (void) doubleClick
{
    NSPoint pt = getCursorPos();
    sendMouseDoubleClick(pt);
    CGDisplayHideCursor(kCGDirectMainDisplay);
}

char KeyCodeToChar(CGKeyCode k);
- (bool) handleKeyPress_:(CGKeyCode) k
{
    
    char c = KeyCodeToChar(k);
    // 创建标签
    if(mode  == Ready)
    {
        last_Key = k;
        
        if(c == 'f')
        {
            mode = Showing;
            [self enumButtons];
            [self generateWord];
            [self showLabels:YES];
            return true;
        }
        if(c == 'i')
        {
            [self toInSertring];
            return true;
        }
        if(c == 'h')
        {
            [self doubleClick];
            return true;
        }
        if(c == 'j')
        {
            sendScroll(-scrollSpeed);
            return true;
        }
        if(c == 'k')
        {
            sendScroll(scrollSpeed);
            return true;
        }
        if(k == Key_Grave)
        {
            [self toSleeping];
            return true;
        }
        return false;
    }
    
    if(mode == InSerting)
    {
        if(k == Key_Escape)
        {
            if(last_Key != Key_Escape)
            {
                last_Key = k;
                last_Esc = clock();
                return false;
            }
            else
            {
                clock_t cur = clock();
                double duration = (double)(cur - last_Esc);
                last_Esc = cur;
                if(duration < 10000)
                {
                    [self toReady];
                    return true;
                }
            }
        }
        
        return false;
    }
    
    if(mode == Showing)
    {
        last_Key = k;
        if(k == Key_Escape || c == 'f')
        {
            [self toReady];
            return true;
        }
        if(k == Key_Delete)
        {
            [self backSpace];
            return true;
        }
        if([self isLetter:k])
        {
            [self handleLetter:k];
            return true;
        }
        return false;
    }
    if(mode == sleeping)
    {
        last_Key = k;
        if(k == Key_Grave)
        {
            [self toReady];
            return true;
        }
    }
    last_Key = k;
    return false;
}

+ (bool) handleKeyPress:(CGKeyCode)k
{
    
    WindowController* instance = [WindowController instance];
    return [instance handleKeyPress_:k];

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
NSMutableArray* getWindowLabels();
- (void) enumButtons
{
//    [self setActiveAppName];
    [self observeApp];
    array = getWindowLabels();
    if(array.count == 0)
        mode = Ready;
}
- (char) CGKeyCode2Char:(CGKeyCode)k;
{
    switch (k)
    {
        case 0: return 'a';
        case 1: return 's';
        case 2: return 'd';
        case 3: return 'f';
        case 4: return 'h';
        case 5: return 'g';
        case 6: return 'z';
        case 7: return 'x';
        case 8: return 'c';
        case 9: return 'v';
        case 11: return 'b';
        case 12: return 'q';
        case 13: return 'w';
        case 14: return 'e';
        case 15: return 'r';
        case 16: return 'y';
        case 17: return 't';
        case 31: return 'o';
        case 32: return 'u';
        case 34: return 'i';
        case 35: return 'p';
        case 37: return 'l';
        case 38: return 'j';
        case 40: return 'k';
        case 45: return 'n';
        case 46: return 'm';
    }
    return 0;
}
- (bool)isLetter:(CGKeyCode) k
{
    return [self CGKeyCode2Char:k] != 0;
}

@end
