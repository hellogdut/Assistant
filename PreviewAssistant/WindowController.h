//
//  WindowController.h
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//


#ifndef WindowController_h
#define WindowController_h

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "MyLabel.h"


@interface WindowController : NSObject
{
    
}
enum Mode {
    
    Ready,
    /* accept :
                i -> double click
                o -> singgle click
                ` -> Sleeping
                i -> InSerting
                f -> Showing
                j,k -> Scroll
     */
    InSerting,
    /*
        accept : except: ESC
     */
    Showing,
    /*
        f,ESC -> Ready
        i -> InSertring
     */
    sleeping
    /*
        ` -> Ready
     */
    
};

@property IBOutlet NSWindow* window;
@property (weak,readwrite)IBOutlet NSView* view;
@property (readwrite) NSMutableArray* array;
@property (readwrite) enum Mode mode;

@property (readwrite) int scrollSpeed;


/* obsetver */
@property (readwrite) AXUIElementRef observedApp;
@property (readwrite) AXObserverRef deactivatedObserver;
@property (readwrite) AXObserverRef moveObserver;
@property (readwrite) AXObserverRef hideObserver;
@property (readwrite) AXObserverRef resizeObserver;

/* Special Key */

@property (readwrite) CGKeyCode last_Key;
@property (readwrite) clock_t last_Esc;


- (id) init;
+ (id) instance;

+ (void) reset;
- (void) reset_;
// mode


- (void) toInSertring;
- (void) toShowing;
- (void) toSleeping;
- (void) toReady;

//- (void) observeApp;
- (void) observeApp;
- (void) unObserveApp;
- (void) windowMove;

/* key and mouse */
+ (bool) handleKeyPress:(CGKeyCode) k;
- (bool) handleKeyPress_:(CGKeyCode) k;
- (void) backSpace;

- (bool) isLetter:(CGKeyCode)k;
- (char) CGKeyCode2Char:(CGKeyCode)k;

- (void) handleLetter:(CGKeyCode)k;

- (void) singleClick;
- (void) doubleClick;

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

#endif
