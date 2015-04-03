//
//  main.m
//  PreviewAssistant
//
//  Created by milu on 15/3/30.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import <Cocoa/Cocoa.h>


// This callback will be invoked every time the mouse moves.
//
//CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type,
//                  CGEventRef event, void *refcon)
//{
//    
//    if (type == NSKeyDown || type == kCGEventKeyUp)
//    {
//        
//        CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
//        printf("%d\n", keycode);
//        
//        // 在“辅助模式”下
//            // a w s d 模拟鼠标移动
//        
//            // j k 模拟触摸板滚动
//        
//            // h 模拟左键按下
//        
//            // l 模拟右键按下
//        
//            // i 模拟触摸板3指按下
//        
//        
//   }
//    return event;
//}

//    // Paranoid sanity check.
////    if (type != kCGEventKeyDown && type != kCGEventKeyUp)
////        return event;
//
//    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
//                                                               event, kCGKeyboardEventKeycode);
//
//    printf("%d\n", keycode);
//    //Keypress code goes here.
//    // We must return the event for it to be useful.
//    return event;


int main(int argc, const char * argv[]) {
    
    return NSApplicationMain(argc, argv);
    
}



