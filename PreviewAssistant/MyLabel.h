//
//  MyLabel.h
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#ifndef PreviewAssistant_MyLabel_h
#define PreviewAssistant_MyLabel_h
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>


enum Role {input,button};

@interface MyLabel : NSObject
{
    AXUIElementRef elem;
    float x,y,w,h;
    int i;
    
    enum Role role;
}
@property (readwrite) NSString* s;
@property (readwrite) NSTextView* label;

- (id) initLabel:(AXUIElementRef)_elem :(int)_x :(int)_y :(int)_w :(int)_h :(NSString*)_s : (enum Role) _type;
- (void) back;
- (void) append:(char) c;
- (BOOL) isOn;

- (void) updateLabel;


- (void) hide;
- (void) show;
- (bool) isSelected;
- (void) updateColor;
- (void) showIndicator;

- (void) performAction;

@end

#endif