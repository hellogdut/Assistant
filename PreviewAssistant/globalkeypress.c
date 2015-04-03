//
//  main.m
//  PreviewAssistant
//
//  Created by milu on 15/3/30.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>


// This callback will be invoked every time the mouse moves.
//
CGEventRef
myCGEventCallback(CGEventTapProxy proxy, CGEventType type,
                  CGEventRef event, void *refcon)
{
    
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged && type != kCGEventKeyUp)
        return event;
    
    CGEventFlags eventFlags = CGEventGetFlags(event);
    if (type == kCGEventFlagsChanged) {
        printf("%lld\n", eventFlags);
    }
    // Paranoid sanity check.
    if (type != kCGEventKeyDown && type != kCGEventKeyUp)
        return event;
    
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
                                                               event, kCGKeyboardEventKeycode);

    printf("%d\n", keycode);
    //Keypress code goes here.
    // We must return the event for it to be useful.
    return event;
}


int main(int argc, const char * argv[]) {
    
    
    // The screen size of the primary display.
//    screenBounds = CGDisplayBounds(CGMainDisplayID());
//    printf("The main screen is %dx%d\n", (int)screenBounds.size.width,
//           (int)screenBounds.size.height);
    
    // Create an event tap. We are interested in mouse movements.
    //
    //CGEventMask eventMask = CGEventMaskBit(kCGEventKeyUp) | CGEventMaskBit(kCGEventFlagsChanged);
    //CGEventMask eventMask = kCGEventMaskForAllEvents;
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) |CGEventMaskBit(kCGEventFlagsChanged);
    
    CFMachPortRef eventTap = CGEventTapCreate(
                                kCGHIDEventTap, kCGHeadInsertEventTap,
                                0, eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
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
    
    // In a real program, one would have arranged for cleaning up.

    
}
