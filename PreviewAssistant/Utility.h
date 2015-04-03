//
//  Utility.m
//  PreviewAssistant
//
//  Created by milu on 15/3/31.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import "KeyCode.h"

/* keyboard */
void sendKeyWithCmdCtrl(CGKeyCode key)
{
    // true Key down
    // fasle key Up
    CGEventRef event1 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Control, true);
    CGEventRef event2 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Command, true);
    CGEventRef event3 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)key, true);
    
    CGEventSetFlags(event3, (kCGEventFlagMaskControl | kCGEventFlagMaskCommand)); /* very important */
    
    CGEventRef event10 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Control, false);
    CGEventRef event20 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_Command, false);
    CGEventRef event30 = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)Key_D, false);
    
    CGEventPost(kCGSessionEventTap, event1);CGEventPost(kCGSessionEventTap, event2);CGEventPost(kCGSessionEventTap, event3);
    CGEventPost(kCGSessionEventTap, event10);CGEventPost(kCGSessionEventTap, event20);CGEventPost(kCGSessionEventTap, event30);
    
    CFRelease(event1);CFRelease(event2);CFRelease(event3);
    CFRelease(event10);CFRelease(event20);CFRelease(event30);
}

/* trackpad */
void sendScroll(int offset)
{
    CGEventRef wheelEvent = CGEventCreateScrollWheelEvent(NULL,kCGScrollEventUnitLine,1,offset);
    CGEventPost(kCGSessionEventTap, wheelEvent);
    CFRelease(wheelEvent);
}

/*  mouse */

NSPoint getCursorPos()
{
    CGEventRef ourEvent = CGEventCreate(NULL);
    return CGEventGetLocation(ourEvent);
}

void setCursorPos(NSPoint pt)
{
    CGWarpMouseCursorPosition(pt);
}



void sendMouseSingleClick(CGEventRef down,CGEventRef up)
{
    CGEventPost(kCGSessionEventTap, down);
    CGEventPost(kCGSessionEventTap, up);
    CFRelease(down);
    CFRelease(up);
}
void sendMouseClickLeft()
{
    CGEventRef ourEvent = CGEventCreate(NULL);
    NSPoint pt = CGEventGetLocation(ourEvent);
    CGEventRef down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, pt, 1);
    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, pt, 1);
    sendMouseSingleClick(down,up);
    
}
void sendMouseClickRight()
{
    CGEventRef ourEvent = CGEventCreate(NULL);
    NSPoint pt = CGEventGetLocation(ourEvent);
    CGEventRef down = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, pt, 1);
    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, pt, 1);
    sendMouseSingleClick(down,up);
}

void setHook(CGEventMask eventMask,CGEventRef (*CallBack)(CGEventTapProxy, CGEventType,CGEventRef,void*))
{
    //CGEventMask eventMask = CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(NX_SYSDEFINED);
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap,
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

