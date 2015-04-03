//
//  Assistant.h
//  PreviewAssistant
//
//  Created by milu on 15/3/31.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#ifndef PreviewAssistant_Assistant_h
#define PreviewAssistant_Assistant_h

#include <time.h>
#import "MyLabel.h"
#import "WindowController.h"
#import "MyLabel.h"
int mouseSpeed = 30;
int scrollSpeed = 20;

bool isActive = true;
bool withModifier = false;
int  lastKey = -1;
int  lastModifier = -1;
int  k_count = 0;
clock_t lastTime = 0;
int  modifier[4] = {0,0,0,0}; // command opt  ctr shift


bool handleMouseClick(CGKeyCode k)
{
    if(k == Key_I || k == Key_L)
    {
        if(k == Key_I)
            sendMouseClickLeft();
        else
            sendMouseClickRight();
        return true;
    }
    return false;
}
bool handleScroll(CGKeyCode k)
{
    if(k == Key_J || k == Key_K )
    {
        if(k == Key_J)
            sendScroll(scrollSpeed);
        else
            sendScroll(-scrollSpeed);
        return true;
    }
    return false;
}

bool handleDictionary(CGKeyCode k)
{
    if(k == Key_Q )
    {
        sendKeyWithCmdCtrl(Key_D);
        return true;
    }
    return false;
}
bool handleResize(CGKeyCode k)
{
    if(k == Key_F)
    {
        sendKeyWithCmdCtrl(Key_F);
        return true;
    }
    return false;
}
bool handleMouseMove(CGKeyCode k)
{
    if (k == Key_W || k == Key_A || k == Key_S || k == Key_D)
    {
        NSPoint pt = getCursorPos();
        switch (k)
        {
            case Key_W:
                pt.y -= mouseSpeed;break;
            case Key_S:
                pt.y += mouseSpeed;break;
            case Key_A:
                pt.x -= mouseSpeed;break;
            case Key_D:
                pt.x += mouseSpeed;break;
        }
        setCursorPos(pt);
        return true;
    }
    return false;
}
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
bool isAskedToActive(CGKeyCode k)
{
    
    if (k != Key_Command)
        return false;
    if(lastModifier != k)
    {
        k_count = 1;
        lastModifier = k;
        lastTime = clock();
        printf("lastModifier != k\n");
        return false;
    }

    k_count += 1;
    if(k_count != 4)
    {
        printf("k_count = %d",k_count);
        return false;
    }
    
   
    clock_t cur = clock();
    double duration = (double)(cur - lastTime);
    lastTime = cur;
    lastModifier = -1;
    k_count = 0;
    if(duration < 10000)
    {
        
        printf("duration is %f\n",duration);
        return true;
    }
    else
    {
        printf("not duration is %f\n",duration);
    }
    return false;
}
bool handleAccelerate(CGKeyCode k)
{
    if(k != Key_E && k != Key_U)
        return false;
    switch (k) {
        case Key_E:
            if(mouseSpeed > 15) mouseSpeed /= 1.5;
            if(scrollSpeed > 5) scrollSpeed /= 1.5;
            break;
        case Key_U:
            if(mouseSpeed < 20) mouseSpeed *= 1.5;
            if(scrollSpeed < 15) scrollSpeed *= 1.5;
            break;
            break;
            
        default:
            break;
    }
    
    return true;
}
bool isIgnoreKey(CGKeyCode k)
{
    /* letter */
    if(0x00 <= k && k <= 0x11) return true;
    if(0x1F <= k && k <= 0x20) return true;
    if(0x22 <= k && k <= 0x26) return true;
    if(0x28 <= k && k <= 0x28) return true;
    if(0x2D <= k && k <= 0x2E) return true;
    
    return false;
    
}
void enumWindow()
{
    NSArray* arr = [NSWindow windowNumbersWithOptions:NSWindowNumberListAllApplications|NSWindowNumberListAllSpaces];
    for(NSNumber *i in arr)
    {
        NSWindow *w = [NSApp windowWithWindowNumber:[i integerValue]];
        if(w)
        {
            NSLog(@"%@",[w title]);
        }
    }
}


NSString* getStringFromPoint(AXUIElementRef elem,NSPoint pt)
{
    AXError err = nil;
    CFTypeRef rangeValue;
    CFTypeRef parameter = AXValueCreate( kAXValueCGPointType, &pt);
    err = AXUIElementCopyParameterizedAttributeValue(elem, kAXRangeForPositionParameterizedAttribute,parameter,(CFTypeRef*)&rangeValue );
    CFRelease(parameter);
    if(err != kAXErrorSuccess )
        return nil;
    CFStringRef stringValue;
    
    CFRange decodedRange = CFRangeMake( 0, 0 );
    bool b = AXValueGetValue((AXValueRef)rangeValue, kAXValueCFRangeType, &decodedRange );
    CFRelease(rangeValue);
    if(b)
    {
        int fromPos = decodedRange.location - 127;
        if( fromPos < 0 )
            fromPos = 0;
        int wordPos = decodedRange.location - fromPos;  // Cursor position in result string
        
        CFRange range = CFRangeMake( fromPos, wordPos + 1 );
        parameter = AXValueCreate( kAXValueCFRangeType, &decodedRange );
        err = AXUIElementCopyParameterizedAttributeValue( elem, kAXStringForRangeParameterizedAttribute,parameter, (CFTypeRef *)&stringValue );
        CFRelease(parameter);
        if( err != kAXErrorSuccess )
            return nil;
        NSLog(@"getStringFromPoint:%@",stringValue);
        
    }
    return nil;
}

NSArray* copyParameterizedAttNames(AXUIElementRef elem)
{
    //    AXLineForIndex,
    //    AXRangeForLine,
    //    AXBoundsForRange,
    //    AXStringForRange,
    //    AXAttributedStringForRange,
    //    AXRangeForPosition,
    //    AXRangeForIndex,
    //    AXStyleRangeForIndex,
    //    AXRTFForRange
    
    AXError error = nil;
    CFArrayRef arr = nil;
    error = AXUIElementCopyParameterizedAttributeNames(elem,(CFTypeRef *)&arr);
    
    if(error != kAXErrorSuccess)
        return nil;
    
    NSArray *arr_ = [(__bridge NSArray *) arr copy];
    CFRelease(arr);
   // NSLog(@"%@",arr_);
    return arr_;
}


NSString* getStringFromPos(AXUIElementRef elem)
{
    NSPoint p = getCursorPos();
    NSArray *arr = copyParameterizedAttNames(elem);
    
    if(arr == nil)
        return nil;
    if(![arr containsObject:(id)@"AXRangeForPosition"] || ![arr containsObject:(id)@"AXStringForRange"])
        return nil;
    
    getStringFromPoint(elem,p);
    return nil;
    
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
void stringCopyable(AXUIElementRef elem)
{
    getStringFromPos(elem);
}


void enumChilds(AXUIElementRef elem,NSPointerArray* arr)
{
    
    stringCopyable(elem);
    AXError error = nil;
    CFArrayRef cfchilds = nil;
    CFArrayRef cfnames = nil;
    
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


bool elemPressable(AXUIElementRef elem)
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
    
    if([actions_ containsObject:(id)@"AXPress"])
    {
        //NSLog(@"action contains : %@",actions_);
        return true;
    }


    return false;
}

void enumWindow1()
{
    
    
    NSDictionary *activeApp = [[NSWorkspace sharedWorkspace] activeApplication];
    NSString* appName = [activeApp objectForKey:@"NSApplicationName"];
    NSLog(@"ActiveApp is :%@",appName);
    
    
    CFArrayRef windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    NSArray* arr = CFBridgingRelease(windowList);
    // Loop through the windows
    for (NSMutableDictionary* entry in arr)
    {
        
        NSString* owner = [entry objectForKey:(id)kCGWindowOwnerName];
        NSInteger layer = [entry objectForKey:(id)kCGWindowLayer];
        
        
        if(![appName isEqualToString:owner])
        {
//            NSLog(@"continue1: %@",owner);
            continue;
        }
        
        if(layer != 39) // front-most-window  (0 equal 39 ? )
        {
//            NSLog(@"continue2 owner: %@",owner);
//            NSLog(@"continue2: %d",layer);
            continue;
        }
        
        
        
        // Get window PID
        pid_t pid = [[entry objectForKey:(id)kCGWindowOwnerPID] intValue];
        // Get AXUIElement using PID
        AXUIElementRef appRef = AXUIElementCreateApplication(pid);
        if (!appRef) {
            NSLog(@"Error: failed to obtain UIElement for app:");
            return;
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
            continue;
        NSLog(@"Windows = %@", windows);
        
        
        NSPointerArray *arr = [NSPointerArray weakObjectsPointerArray];
        NSPointerArray *btns = [NSPointerArray weakObjectsPointerArray];
        
        // enum all windows
        for(int i = 0;i < CFArrayGetCount(windows);++i)
        {
            AXUIElementRef window = (AXUIElementRef) CFArrayGetValueAtIndex(windows, i);
            
            enumChilds(window,arr);
        }
        
        // get all item in all windows
        
         NSLog(@"show all btn");
        
        NSMutableArray* array = [[NSMutableArray alloc] init];
        
        for(int i = 0;i < arr.count;++i)
        {
            AXUIElementRef window = [arr pointerAtIndex:i];
            if(elemPressable(window))
            {
                CFTypeRef position,Size;
                CGPoint point;
                CGSize  size_;
                
                
                AXUIElementCopyAttributeValue(window, kAXPositionAttribute, (CFTypeRef *)&position);
                AXUIElementCopyAttributeValue(window,kAXSizeAttribute,(CFTypeRef *)&Size);
                
                AXValueGetValue(position, kAXValueCGPointType, &point);
                AXValueGetValue(Size,kAXValueCGSizeType,&size_);
                
                NSString *s = @" SS ";
                
                // 生成label
                MyLabel* label = [[MyLabel alloc] initLabel:window :point.x :point.y :size_.width :size_.height :s :press ];
                [array addObject:label];
                
                // Debugging (always zeros?)
                NSLog(@"point=%f,%f size=%f,%f", point.x,point.y,size_.width,size_.height);
            }
        }
        [[WindowController instance] setLabels:array];
        
    }
    
}

CGEventRef myCallback(CGEventTapProxy proxy, CGEventType type,CGEventRef event,void* refcon)
{
    
    CGKeyCode k = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    [WindowController keyPress:k];
    //NSLog(@"key is :%d",k);
    
    if(type == kCGEventFlagsChanged && isAskedToActive(k))
    {
        isActive = isActive ? false : true;
        /* when switch to negative,hide cursor */
        if(!isActive)
            CGDisplayHideCursor(kCGDirectMainDisplay);
        else
            CGDisplayShowCursor(kCGDirectMainDisplay);
        
        for(int i = 0;i < 4;++i) modifier[i] = 0;
        withModifier = false;
        return 0;
    }
    
    
    /* do no thing */
    if(!isActive) return event;
    
    /* ignore keyUp event*/
    if(type == kCGEventKeyUp)
        return event;
//    enumWindow();
    
    enumWindow1();
    
    
    /* handle modifier */
    if(type == kCGEventFlagsChanged)
    {
        handleModifier(k);
        return event;       /* return */
    }
    /* ignore key with Modifier */
    if(withModifier)
    {
        printf("with modifier %d\n",k);
        return event;
    }
    
    /* handle mouse click */
    /*
        Key_I Key_L
     */
    if (handleMouseClick(k)) return 0;
    
    /* handle scroll */
    if (handleScroll(k)) return 0;
    
    /* handle dictionary */
    if(handleDictionary(k)) return 0;
    
    /* handle resize */
    
    if (handleResize(k)) return 0;
    
    /* handle mouse move */

    if(handleMouseMove(k)) return 0;
    
    /* change speed of scroll and mouse move */
    if(handleAccelerate(k)) return 0;
    
    if(isIgnoreKey(k)) return 0;
    
    return event;
}


#endif
