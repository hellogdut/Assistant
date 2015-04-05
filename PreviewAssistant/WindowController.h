//
//  WindowController.h
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "MyLabel.h"



@interface WindowController : NSObject
{
    
}

@property IBOutlet NSWindow* window;
@property (weak,readwrite)IBOutlet NSView* view;
@property (readwrite) NSMutableArray* array;
@property (readwrite) bool active;
@property (readwrite) AXObserverRef deactivatedObserver;
@property (readwrite) AXObserverRef moveObserver;
@property (readwrite) AXUIElementRef observedApp;
- (id) init;
+ (id) instance;



//- (void) observeApp;
- (void) observeApp;
- (void) unObserveApp;
- (void) windowMove;

/* key and mouse */
+ (void) handleKeyPress:(CGKeyCode) k;
//- (bool) isActiveApp;

- (void) userCancel;

/* labels init */
- (void) enumButtons;
- (void) generateWord;
- (void) showLabels:(bool)first;

/* labels update */
- (void) backAllLabels;
- (void) appendAllLabels:(char) c;
- (void) appendSelectedLabels:(char) c;

- (bool) anyLabelSelected;
- (void) showSelectedLabels;
- (void) hideNoneSelectedLabels;
- (void) showAllLabels;
- (void) hideAllLabels;
@end
