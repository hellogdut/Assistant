//
//  MyLabel.m
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import "MyLabel.h"

@implementation MyLabel
@synthesize label,s,role;


- (id) initLabel:(AXUIElementRef)_elem :(int)_x :(int)_y :(int)_w :(int)_h :(NSString*)_s : (enum Role) _role
{
    self = [super init];
    if(self)
    {
        x =  _x;
        y =  _y;
        w =  _w;
        h = _h;
        
        i = 0;
        s = _s;
        elem = _elem;
        role = _role;
    }
    //[self initLabel];
    return self;
}
- (void) back
{

    i--;
    if(i < 0)
        i = 0;
    [self updateColor];
}

- (BOOL) isOn
{
    if(s != nil && i == s.length - 2)
        return true;
    return false;
}
- (void) append:(char) c
{
    if(i < (int)s.length - 2)
    {
        if(toupper((int)[s characterAtIndex : i + 1]) == toupper((int)c))
        {
            NSLog(@"append");
            i += 1;
            [self updateColor];
        }
    }
}

- (void) updateLabel
{
//    NSRect screen =  NSScreen.mainScreen.frame;
     NSRect screen =  NSScreen.mainScreen.frame;
//    NSLog(@"height :%f,width : %f",screen.size.height,screen.size.width);
    NSRect frameRect = NSMakeRect(x - 3,screen.size.height - y - h - 3,13 * [s length],20);
    
//    NSRect frameRect = NSMakeRect(x + w/2,screen.size.height - y - h/2 - 10,10 * [s length],20);
    label = [[NSTextView alloc] initWithFrame:frameRect];
    [label setString:s];
    [label setDrawsBackground:NO];
    
    [self updateColor];
    [label sizeToFit];
    
    CGFloat wh = [label frame].size.width;
    CGFloat ht = [label frame].size.height;
    
//    int posX = 0;
//    int posY = screen.size.height - y - 10;
//    if(w  50)
//    {
//        posX = x - wh;
//    }
//    else
//    {
//        posX = x + w - wh;
//        
//    }
//    [label setFrame:NSMakeRect(posX, posY, wh, ht)];
    
    
        int posX = x;
        int posY = screen.size.height - y;
//    NSLog(@"screen.size.height :%f,y : %f",screen.size.height,y);
//    NSLog(@"posX :%f,posY : %f",posX,posY);
        if(w < 100)
        {
            posX = x - MIN(w,wh);
            posY = posY - MIN(h,ht);
            
        }
        else
        {
            posX = x + w*0.7 - wh;
            posY = posY - h/2 - ht/2;
        }
        [label setFrame:NSMakeRect(posX, posY, wh, ht)];
//        NSLog(@"posX :%f,posY : %f",posX,posY);
   
    
}

- (void) updateColor
{
    
    NSString *str = s;
    
    NSMutableAttributedString *attrstr = [[NSMutableAttributedString alloc] initWithString:str];
    
    NSColor* buttonBackGround = [NSColor yellowColor];
    NSColor* buttonMatch = [NSColor colorWithDeviceRed:0.76 green:0.54 blue:0.34 alpha:1];
    
    NSColor* InputBackGround = [NSColor greenColor];
    
    
    NSColor* background = buttonBackGround;
    
    NSColor* match = buttonMatch;
    
//    if(role == button)
//    {
//        background = buttonBackGround;
//        match = buttonMatch;
//    }
//    else if (role == input)
//    {
//        background = InputBackGround;
//        match = buttonMatch;
//    }
    
    
    
    
    NSDictionary *BackGround = @{
                                 NSBackgroundColorAttributeName : background,
                                 NSFontAttributeName : [NSFont fontWithName:@"Helvetica-Bold" size:12.0]
                                 };

    NSDictionary *Match = @{
                                 NSBackgroundColorAttributeName : background,
                                 NSForegroundColorAttributeName : match,
                                 NSFontAttributeName : [NSFont fontWithName:@"Helvetica-Bold" size:12.0]
                                 };
    
    

    
    
    
    
    
    [attrstr setAttributes:BackGround range:NSMakeRange(0,s.length)];
    if(i > 0)
    {
        [attrstr setAttributes:Match range:NSMakeRange(1,i)];
        NSLog(@"NSForegroundColorAttributeName ");
    }

    
    
    [[self.label textStorage] setAttributedString:attrstr];
    
}

void setCursorPos(NSPoint pt);


void sendMouseSingleClick(CGEventRef down,CGEventRef up);
void sendMouseClickLeft(NSPoint pt);

- (void) performAction;
{
    AXError err = nil;
    
    if(role == button)
    {
        err = AXUIElementPerformAction(elem,kAXPressAction);
        NSPoint pt = NSMakePoint(x + w/2 , y);
        setCursorPos(pt);
        CGDisplayHideCursor(kCGDirectMainDisplay);
        if(err != kAXErrorSuccess)
        {
            sendMouseClickLeft(pt);
        }

        
        return;
    }
    if(role == input)
    {
        CGDisplayHideCursor(kCGDirectMainDisplay);
        NSPoint pt = NSMakePoint(x + w/2 , y);
        sendMouseClickLeft(pt);
        CGDisplayHideCursor(kCGDirectMainDisplay);
        
    }
}
- (void) showIndicator
{
    if(s == nil) return;
    int n = [s length];
    [label showFindIndicatorForRange:NSMakeRange(0, n)];
}
- (void) hide
{
    [label setHidden:YES];
}
- (void) show
{
    [label setHidden:NO];
}
- (bool) isSelected
{
    return i != 0;
}

@end
