//
//  MyLabel.m
//  myWindow
//
//  Created by milu on 15/4/3.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import "MyLabel.h"

@implementation MyLabel
@synthesize label,s;


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
    NSRect screen = NSScreen.mainScreen.frame;
    
//    NSRect frameRect = NSMakeRect(x - 3,screen.size.height - y - h - 3,13 * [s length],20);
    NSRect frameRect = NSMakeRect(x + w/2,screen.size.height - y - h/2 - 10,13 * [s length],20);
//    NSLog(@"w = %f",w);
    
    label = [[NSTextView alloc] initWithFrame:frameRect];
    
    [label setString:s];
    [label setDrawsBackground:NO];
    
    [self updateColor];
    
}

- (void) updateColor
{
    
    NSString *str = s;
    
    NSMutableAttributedString *attrstr = [[NSMutableAttributedString alloc] initWithString:str];
    
    NSColor* background = [NSColor yellowColor];
    NSColor* match = [NSColor colorWithDeviceRed:0.76 green:0.54 blue:0.34 alpha:1];
    
    NSDictionary *backGround = @{
                                 NSBackgroundColorAttributeName : background,
                                 NSFontAttributeName : [NSFont fontWithName:@"Helvetica-Bold" size:12.0]
                                 };

    NSDictionary *matchColor = @{
                                 NSBackgroundColorAttributeName : background,
                                 NSForegroundColorAttributeName : match,
                                 NSFontAttributeName : [NSFont fontWithName:@"Helvetica-Bold" size:14.0]
                                 };
    
    
    
    [attrstr setAttributes:backGround range:NSMakeRange(0,s.length)];
    if(i > 0)
    {
        [attrstr setAttributes:matchColor range:NSMakeRange(1,i)];
        NSLog(@"NSForegroundColorAttributeName ");
    }

    
    
    [[self.label textStorage] setAttributedString:attrstr];
    
}
- (void) performAction;
{
    if(role == button)
    {
        NSLog(@"kAXPressAction");
        AXUIElementPerformAction(elem,kAXPressAction);
        return;
    }
    if(role == input)
    {
        NSLog(@"kAXConfirmAction");
        AXUIElementSetAttributeValue(elem, kAXFocusedAttribute,kCFBooleanTrue);
        return;

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
