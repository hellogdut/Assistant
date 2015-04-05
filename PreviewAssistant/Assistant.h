//
//  Assistant.h
//  PreviewAssistant
//
//  Created by milu on 15/3/31.
//  Copyright (c) 2015年 milu. All rights reserved.
//


#ifndef PreviewAssistant_Assistant_h
#define PreviewAssistant_Assistant_h



//
//bool handleMouseClick(CGKeyCode k)
//{
//    if(k == Key_I || k == Key_L)
//    {
//        if(k == Key_I)
//            sendMouseClickLeft();
//        else
//            sendMouseClickRight();
//        return true;
//    }
//    return false;
//}
//bool handleScroll(CGKeyCode k)
//{
//    if(k == Key_J || k == Key_K )
//    {
//        if(k == Key_J)
//            sendScroll(scrollSpeed);
//        else
//            sendScroll(-scrollSpeed);
//        return true;
//    }
//    return false;
//}
//
//bool handleDictionary(CGKeyCode k)
//{
//    if(k == Key_Q )
//    {
//        sendKeyWithCmdCtrl(Key_D);
//        return true;
//    }
//    return false;
//}
//bool handleResize(CGKeyCode k)
//{
//    if(k == Key_F)
//    {
//        sendKeyWithCmdCtrl(Key_F);
//        return true;
//    }
//    return false;
//}
//bool handleMouseMove(CGKeyCode k)
//{
//    if (k == Key_W || k == Key_A || k == Key_S || k == Key_D)
//    {
//        NSPoint pt = getCursorPos();
//        switch (k)
//        {
//            case Key_W:
//                pt.y -= mouseSpeed;break;
//            case Key_S:
//                pt.y += mouseSpeed;break;
//            case Key_A:
//                pt.x -= mouseSpeed;break;
//            case Key_D:
//                pt.x += mouseSpeed;break;
//        }
//        setCursorPos(pt);
//        return true;
//    }
//    return false;
//}
//bool isAskedToActive(CGKeyCode k)
//{
//    
//    if (k != Key_Command)
//        return false;
//    if(lastModifier != k)
//    {
//        k_count = 1;
//        lastModifier = k;
//        lastTime = clock();
//        printf("lastModifier != k\n");
//        return false;
//    }
//
//    k_count += 1;
//    if(k_count != 4)
//    {
//        printf("k_count = %d",k_count);
//        return false;
//    }
//    
//   
//    clock_t cur = clock();
//    double duration = (double)(cur - lastTime);
//    lastTime = cur;
//    lastModifier = -1;
//    k_count = 0;
//    if(duration < 10000)
//    {
//        
//        printf("duration is %f\n",duration);
//        return true;
//    }
//    else
//    {
//        printf("not duration is %f\n",duration);
//    }
//    return false;
//}
//bool handleAccelerate(CGKeyCode k)
//{
//    if(k != Key_E && k != Key_U)
//        return false;
//    switch (k) {
//        case Key_E:
//            if(mouseSpeed > 15) mouseSpeed /= 1.5;
//            if(scrollSpeed > 5) scrollSpeed /= 1.5;
//            break;
//        case Key_U:
//            if(mouseSpeed < 20) mouseSpeed *= 1.5;
//            if(scrollSpeed < 15) scrollSpeed *= 1.5;
//            break;
//            break;
//            
//        default:
//            break;
//    }
//    
//    return true;
//}
//bool isIgnoreKey(CGKeyCode k)
//{
//    /* letter */
//    if(0x00 <= k && k <= 0x11) return true;
//    if(0x1F <= k && k <= 0x20) return true;
//    if(0x22 <= k && k <= 0x26) return true;
//    if(0x28 <= k && k <= 0x28) return true;
//    if(0x2D <= k && k <= 0x2E) return true;
//    
//    return false;
//    
//}
//void enumWindow()
//{
//    NSArray* arr = [NSWindow windowNumbersWithOptions:NSWindowNumberListAllApplications|NSWindowNumberListAllSpaces];
//    for(NSNumber *i in arr)
//    {
//        NSWindow *w = [NSApp windowWithWindowNumber:[i integerValue]];
//        if(w)
//        {
//            NSLog(@"%@",[w title]);
//        }
//    }
//}

#endif







