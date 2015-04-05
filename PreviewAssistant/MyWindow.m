//
//  MyWindow.m
//
//  Created by markhunte on 12/12/2010.
//  Copyright 2010 __markosx.com__. All rights reserved.
//

#import "MyWindow.h"
#import "WindowController.h"
@implementation MyWindow


- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{
    NSRect frame = NSScreen.mainScreen.frame;
    self = [super initWithContentRect:frame styleMask:(NSBorderlessWindowMask) backing:NSBackingStoreBuffered defer:NO];
        if (self)
        {
            self.backgroundColor = NSColor.clearColor;
            self.level = CGShieldingWindowLevel();
            [self setOpaque:NO];
            [self setHasShadow:NO];
            self.hidesOnDeactivate = NO;
            //[self setFrameTopLeftPoint:NSMakePoint(0,frame.size.height)];
        }
        return self;
    
}



@end
