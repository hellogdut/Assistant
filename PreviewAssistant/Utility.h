//
//  Utility.m
//  PreviewAssistant
//
//  Created by milu on 15/3/31.
//  Copyright (c) 2015年 milu. All rights reserved.
//



#ifndef PreviewAssistant_Utility_h
#define PreviewAssistant_Utility_h
#import "KeyCode.h"
#import "MyLabel.h"
#import "WindowController.h"
#include <time.h>


int mouseSpeed = 30;
int scrollSpeed = 20;

bool isActive = true;
bool withModifier = false;
int  lastKey = -1;
int  lastModifier = -1;
int  k_count = 0;
clock_t lastTime = 0;
int  modifier[4] = {0,0,0,0}; // command opt  ctr shift
CFMachPortRef eventTap;



bool handleModifier(CGKeyCode k)
{
    
    switch (k)
    {
        case Key_Command:
            modifier[0] ^= 0x1;break;
        case Key_Option:
            modifier[1] ^= 0x1;break;
        case Key_Control:
            modifier[2] ^= 0x1;break;
        case Key_Shift:
            modifier[3] ^= 0x1;break;
    }
    lastModifier = k;
    withModifier = false;
    for(int i = 0;i < 4;++i)
    {
        withModifier |= modifier[i];
    }
    return true;
}

CGEventRef myCallback(CGEventTapProxy proxy, CGEventType type,CGEventRef event,void* refcon)
{
    
    CGKeyCode k = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    if(type  == kCGEventTapDisabledByTimeout || type == kCGEventTapDisabledByUserInput)
    {
        
        NSLog(@"disable");
        return nil;
    }
    NSLog(@"type = %d",type);
    NSLog(@"myCallback %d",k);
    if(!isActive)
    {
        NSLog(@"!isActive %d",k);
        return event;
    }
    if(type == kCGEventKeyUp)
    {
        NSLog(@"key up :%d",k);
        return event;
    }
    
    if(type == kCGEventFlagsChanged)
    {
        handleModifier(k);
        NSLog(@"kCGEventFlagsChanged :%d",k);
        return event;
    }
    if(withModifier)
    {
        
        NSLog(@"withModifier :%d",k);
        return event;
    }
    if(type != kCGEventKeyDown)
        return event;
    
    [WindowController handleKeyPress:k];
    CGEventTapEnable(eventTap, true);
    return nil;
    
    //    if(type == kCGEventFlagsChanged && isAskedToActive(k))
    //    {
    //        isActive = isActive ? false : true;
    //        /* when switch to negative,hide cursor */
    //        if(!isActive)
    //            CGDisplayHideCursor(kCGDirectMainDisplay);
    //        else
    //            CGDisplayShowCursor(kCGDirectMainDisplay);
    //
    //        for(int i = 0;i < 4;++i) modifier[i] = 0;
    //        withModifier = false;
    //        return 0;
    //    }
    //
    //
    //    /* do no thing */
    //    if(!isActive) return event;
    //
    //    /* ignore keyUp event*/
    //    if(type == kCGEventKeyUp)
    //        return event;
    ////    enumWindow();
    //
    //    //enumWindow();
    //
    //
    //    /* handle modifier */
    //    if(type == kCGEventFlagsChanged)
    //    {
    //        handleModifier(k);
    //        return event;       /* return */
    //    }
    //    /* ignore key with Modifier */
    //    if(withModifier)
    //    {
    //        printf("with modifier %d\n",k);
    //        return event;
    //    }
    //
    //    /* handle mouse click */
    //    /*
    //        Key_I Key_L
    //     */
    //    if (handleMouseClick(k)) return 0;
    //
    //    /* handle scroll */
    //    if (handleScroll(k)) return 0;
    //
    //    /* handle dictionary */
    //    if(handleDictionary(k)) return 0;
    //
    //    /* handle resize */
    //
    //    if (handleResize(k)) return 0;
    //
    //    /* handle mouse move */
    //
    //    if(handleMouseMove(k)) return 0;
    //    
    //    /* change speed of scroll and mouse move */
    //    if(handleAccelerate(k)) return 0;
    //    
    //    if(isIgnoreKey(k)) return 0;
    //    
    //    return event;
}




void setHook(CGEventMask eventMask,CGEventRef (*CallBack)(CGEventTapProxy, CGEventType,CGEventRef,void*))
{
    //CGEventMask eventMask = CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(NX_SYSDEFINED);
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap,
                                              0, eventMask, CallBack, NULL);
    if (!eventTap) {
        printf("%s","failed to create event tap\n");
        exit(1);
    }
    
    // Create a run loop source.
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(
                                                                     kCFAllocatorDefault, eventTap, 0);
    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
                       kCFRunLoopDefaultMode);
    // Enable the event tap.
    CGEventTapEnable(eventTap, true);
    
    // Set it all running.
    CFRunLoopRun();
}



void enableTap()
{
}

NSArray* copyActionNames(AXUIElementRef elem)
{
    AXError error = nil;
    CFArrayRef arr = nil;
    error = AXUIElementCopyActionNames(elem,(CFTypeRef *)&arr);
    if(error != kAXErrorSuccess)
        return nil;
    
    NSArray *arr_ = [(__bridge NSArray *) arr copy];
    CFRelease(arr);
    
    
    return arr_;
}
bool isInput(AXUIElementRef elem)
{
    
    
    NSArray *actions_ = copyActionNames(elem);
    if(actions_ == nil)
        return false;
    
    CFStringRef role;
    AXUIElementCopyAttributeValue(elem,kAXRoleAttribute,(CFTypeRef *)&role);
    NSArray* array = [[NSArray alloc] initWithObjects:@"AXTextField", nil];
    
    if([array containsObject:(__bridge NSString*)role])
    {
        //NSLog(@"%@ is input",elem);
        return true;
    }

    
    return false;

    
    
    
    
    return false;
}
bool isButton(AXUIElementRef elem)
{
    //    AXError error = nil;
    //    CFArrayRef actions = nil;
    //    error = AXUIElementCopyActionNames(elem,(CFTypeRef *)&actions);
    //
    //    if(error)
    //        return false;
    //    NSArray *actions_ = [(__bridge NSArray *) actions copy];
    //
    NSArray *actions_ = copyActionNames(elem);
    if(actions_ == nil)
        return false;
    AXError error = nil;
    CFTypeRef role;
    
    // should be kAXChildrenAttribute
    error = AXUIElementCopyAttributeValue(elem, kAXRoleDescriptionAttribute, (CFTypeRef *)&role);
    NSLog(@"the role of elem  %@  is %@",elem,role);
    
    NSLog(@"the actions of elem %@ is %@",elem,actions_);
    if([actions_ containsObject:(id)@"AXPress"])
    {
        //NSLog(@"action contains : %@",actions_);
        return true;
    }
    return false;
}

bool isCloseBtn(AXUIElementRef elem)
{
    // is close | minimzie | maximize button
    
    AXError error = nil;
    CFTypeRef role;
    
    // should be kAXChildrenAttribute
    error = AXUIElementCopyAttributeValue(elem, kAXRoleDescriptionAttribute, (CFTypeRef *)&role);
    //NSLog(@"the role of elem  %@  is %@",elem,role);
    NSString *a = (__bridge NSString *)role;
    NSArray* array = [[NSArray alloc] initWithObjects:@"关闭按钮",@"缩放按钮",@"最小化按钮",nil];
    if([array containsObject:a])
        return true;
    return false;
    
}
bool isEnable(AXUIElementRef elem)
{
    CFTypeRef val;
    // should be kAXChildrenAttribute
    AXUIElementCopyAttributeValue(elem, kAXEnabledAttribute, (CFTypeRef *)&val);
    
    CFBooleanRef enable = (CFBooleanRef)val;
    return enable == kCFBooleanTrue;
    //return true;
//    

}
void enumChilds(AXUIElementRef elem,NSPointerArray* arr)
{
    
    //    stringCopyable(elem);
    AXError error = nil;
    CFArrayRef cfchilds = nil;
    
    // should be kAXChildrenAttribute
    error = AXUIElementCopyAttributeValue(elem, kAXChildrenAttribute, (CFTypeRef *)&cfchilds);
    
    if(!cfchilds)
        return;
    
    for(int i = 0;i < CFArrayGetCount(cfchilds);++i)
    {
        AXUIElementRef child = (AXUIElementRef) CFArrayGetValueAtIndex( cfchilds, i);
        [arr addPointer:child];
        enumChilds(child,arr);
    }
}

pid_t activeAppRef()
{
    NSDictionary *activeApp = [[NSWorkspace sharedWorkspace] activeApplication];
    NSString* appName = [activeApp objectForKey:@"NSApplicationName"];
    NSLog(@"ActiveApp is :%@",appName);
    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    NSArray* arr = CFBridgingRelease(windowList);
    pid_t pid = nil;
    for (NSMutableDictionary* entry in arr)
    {
        
        NSString* owner = [entry objectForKey:(id)kCGWindowOwnerName];
        NSInteger layer = [entry objectForKey:(id)kCGWindowLayer];
        
        if(![appName isEqualToString:owner])
        {
            continue;
        }
        if(layer != 39) // front-most-window  (0 equal 39 ? )
        {
            continue;
        }
        
        
        // Get window PID
        pid = [[entry objectForKey:(id)kCGWindowOwnerPID] intValue];
        NSLog(@"pid is :%d",pid);
        
    }
    return pid;
}
NSPointerArray * enumAppWindows()
{
    
    // Get window PID
    pid_t pid = activeAppRef();
    
    // Get AXUIElement using PID
    AXUIElementRef appRef = AXUIElementCreateApplication(pid);
    
    if (!appRef) {
        NSLog(@"Error: failed to obtain UIElement for app:");
        return nil;
    }
    NSLog(@"Ref = %@",appRef);
    
    
    CFArrayRef windows = nil;
    AXError error = 0;
    error = AXUIElementCopyAttributeValue(appRef, kAXWindowsAttribute, (CFTypeRef *)&windows);
    CFRelease(appRef);
    if (error) {
        NSLog(@"Error: failed to obtain children attribute value for app:%@",windows);
    }
    
    if ((!windows) || CFArrayGetCount(windows)<1)
        return nil;
    NSLog(@"Windows = %@", windows);
    
    
    NSPointerArray *arr = [NSPointerArray weakObjectsPointerArray];
    
    // enum all windows
    for(int i = 0;i < CFArrayGetCount(windows);++i)
    {
        AXUIElementRef window = (AXUIElementRef) CFArrayGetValueAtIndex(windows, i);
        CFBooleanRef main;
        AXUIElementCopyAttributeValue(window,kAXMainAttribute,(CFTypeRef *)&main);
        if(main != kCFBooleanTrue)
        {
            NSLog(@"not main window,continue");
            continue;
        }
        enumChilds(window,arr);
    }
    return arr;
}
//}
NSString* getElemType(AXUIElementRef elem)
{
    /*  1. Check  enable */
    
    CFTypeRef val;
    CFTypeRef role;
    AXError error = nil;
    NSString* type = nil;
    CFStringRef input;
    
    AXUIElementCopyAttributeValue(elem, kAXEnabledAttribute, (CFTypeRef *)&val);
    CFBooleanRef enable = (CFBooleanRef)val;
    
    if(enable != kCFBooleanTrue)
        return type;
    
    /*  2. Check  actions */
    
    
    NSArray *actions_ = copyActionNames(elem);
    
    if(actions_ == nil || actions_.count == 0)
        return type;
    
    
    /* 3. check input */

    AXUIElementCopyAttributeValue(elem,kAXRoleAttribute,(CFTypeRef *)&role);
    NSArray* array = [[NSArray alloc] initWithObjects:@"AXTextField", nil];
    
    if([array containsObject:(__bridge NSString*)role])
    {
        type = @"input";
        return type;
    }

    /* 4. check button */
    /* 4.1 ignore close btn */
    error = AXUIElementCopyAttributeValue(elem, kAXRoleDescriptionAttribute, (CFTypeRef *)&role);
    NSString *a = (__bridge NSString *)role;
    array = [[NSArray alloc] initWithObjects:@"关闭按钮",@"缩放按钮",@"最小化按钮",nil];
    if([array containsObject:a])
        return type;
    
    if([actions_ containsObject:(id)@"AXPress"])
    {
        type = @"button";
        return type;
    }
    
    return type;
    
}
NSMutableArray* enumAppButtons()
{
    NSMutableArray* buttons = [[NSMutableArray alloc] init];
    NSPointerArray *windows = enumAppWindows();
    
    for(int i = 0;i < windows.count;++i)
    {
        AXUIElementRef elem = [windows pointerAtIndex:i];
        NSString* type = getElemType(elem);
        if(type != nil)
        {
            
            CFTypeRef position,Size;
            CGPoint point;
            CGSize  size_;
            
            AXUIElementCopyAttributeValue(elem, kAXPositionAttribute, (CFTypeRef *)&position);
            AXUIElementCopyAttributeValue(elem,kAXSizeAttribute,(CFTypeRef *)&Size);
            AXValueGetValue(position, kAXValueCGPointType, &point);
            AXValueGetValue(Size,kAXValueCGSizeType,&size_);
            
            NSString *s = @"SS";
            
            
            // 生成label
            enum Role role = button;
            if([type isEqualToString:@"input"]) role = input;
            MyLabel* label = [[MyLabel alloc] initLabel:elem :point.x :point.y :size_.width :size_.height :s :role];
            [buttons addObject:label];
            
            // Debugging (always zeros?)
            //NSLog(@"point=%f,%f size=%f,%f", point.x,point.y,size_.width,size_.height);
        }
    }
    return buttons;
}





char KeyCodeToChar(CGKeyCode k)
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
            // what is 10?
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
            
        case Key_Escape: return 1;
        case Key_Delete: return 2;  // backSpace
    }
    return 0;
}

//NSString* getStringFromPoint(AXUIElementRef elem,NSPoint pt)
//{
//    AXError err = nil;
//    CFTypeRef rangeValue;
//    CFTypeRef parameter = AXValueCreate( kAXValueCGPointType, &pt);
//    err = AXUIElementCopyParameterizedAttributeValue(elem, kAXRangeForPositionParameterizedAttribute,parameter,(CFTypeRef*)&rangeValue );
//    CFRelease(parameter);
//    if(err != kAXErrorSuccess )
//        return nil;
//    CFStringRef stringValue;
//    
//    CFRange decodedRange = CFRangeMake( 0, 0 );
//    bool b = AXValueGetValue((AXValueRef)rangeValue, kAXValueCFRangeType, &decodedRange );
//    CFRelease(rangeValue);
//    if(b)
//    {
//        int fromPos = decodedRange.location - 127;
//        if( fromPos < 0 )
//            fromPos = 0;
//        int wordPos = decodedRange.location - fromPos;  // Cursor position in result string
//        
//        CFRange range = CFRangeMake( fromPos, wordPos + 1 );
//        parameter = AXValueCreate( kAXValueCFRangeType, &decodedRange );
//        err = AXUIElementCopyParameterizedAttributeValue( elem, kAXStringForRangeParameterizedAttribute,parameter, (CFTypeRef *)&stringValue );
//        CFRelease(parameter);
//        if( err != kAXErrorSuccess )
//            return nil;
//        NSLog(@"getStringFromPoint:%@",stringValue);
//        
//    }
//    return nil;
//}

//NSArray* copyParameterizedAttNames(AXUIElementRef elem)
//{
//    //    AXLineForIndex,
//    //    AXRangeForLine,
//    //    AXBoundsForRange,
//    //    AXStringForRange,
//    //    AXAttributedStringForRange,
//    //    AXRangeForPosition,
//    //    AXRangeForIndex,
//    //    AXStyleRangeForIndex,
//    //    AXRTFForRange
//    
//    AXError error = nil;
//    CFArrayRef arr = nil;
//    error = AXUIElementCopyParameterizedAttributeNames(elem,(CFTypeRef *)&arr);
//    
//    if(error != kAXErrorSuccess)
//        return nil;
//    
//    NSArray *arr_ = [(__bridge NSArray *) arr copy];
//    CFRelease(arr);
//    // NSLog(@"%@",arr_);
//    return arr_;
//}


//NSString* getStringFromPos(AXUIElementRef elem)
//{
//    NSPoint p = getCursorPos();
//    NSArray *arr = copyParameterizedAttNames(elem);
//    
//    if(arr == nil)
//        return nil;
//    if(![arr containsObject:(id)@"AXRangeForPosition"] || ![arr containsObject:(id)@"AXStringForRange"])
//        return nil;
//    
//    getStringFromPoint(elem,p);
//    return nil;
//    
//}

//void stringCopyable(AXUIElementRef elem)
//{
//    getStringFromPos(elem);
//}


/* keyboard */
//void sendKeyWithCmdCtrl(CGKeyCode key)
//{
//    // true Key down
//    // fasle key Up
//    CGEventRef event1 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Control, true);
//    CGEventRef event2 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Command, true);
//    CGEventRef event3 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)key, true);
//
//    CGEventSetFlags(event3, (kCGEventFlagMaskControl | kCGEventFlagMaskCommand)); /* very important */
//
//    CGEventRef event10 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Control, false);
//    CGEventRef event20 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Command, false);
//    CGEventRef event30 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_D, false);
//
//    CGEventPost(kCGSessionEventTap, event1);CGEventPost(kCGSessionEventTap, event2);CGEventPost(kCGSessionEventTap, event3);
//    CGEventPost(kCGSessionEventTap, event10);CGEventPost(kCGSessionEventTap, event20);CGEventPost(kCGSessionEventTap, event30);
//
//    CFRelease(event1);CFRelease(event2);CFRelease(event3);
//    CFRelease(event10);CFRelease(event20);CFRelease(event30);
//}

///* trackpad */
//void sendScroll(int offset)
//{
//    CGEventRef wheelEvent = CGEventCreateScrollWheelEvent(NULL,kCGScrollEventUnitLine,1,offset);
//    CGEventPost(kCGSessionEventTap, wheelEvent);
//    CFRelease(wheelEvent);
//}

/*  mouse */

//NSPoint getCursorPos()
//{
//    CGEventRef ourEvent = CGEventCreate(NULL);
//    return CGEventGetLocation(ourEvent);
//}

//void setCursorPos(NSPoint pt)
//{
//    CGWarpMouseCursorPosition(pt);
//}
//


//void sendMouseSingleClick(CGEventRef down,CGEventRef up)
//{
//    CGEventPost(kCGSessionEventTap, down);
//    CGEventPost(kCGSessionEventTap, up);
//    CFRelease(down);
//    CFRelease(up);
//}
//void sendMouseClickLeft()
//{
//    CGEventRef ourEvent = CGEventCreate(NULL);
//    NSPoint pt = CGEventGetLocation(ourEvent);
//    CGEventRef down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, pt, 1);
//    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, pt, 1);
//    sendMouseSingleClick(down,up);
//
//}
//void sendMouseClickRight()
//{
//    CGEventRef ourEvent = CGEventCreate(NULL);
//    NSPoint pt = CGEventGetLocation(ourEvent);
//    CGEventRef down = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, pt, 1);
//    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, pt, 1);
//    sendMouseSingleClick(down,up);
//}

#endif
