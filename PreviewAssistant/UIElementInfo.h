//
//  UIElementInfo.h
//  PreviewAssistant
//
//  Created by milu on 15/4/6.
//  Copyright (c) 2015年 milu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UIElementInfo : NSObject

enum Type { buttonType,inputType };
    
@property (readwrite) AXUIElementRef ref;
@property (readwrite) NSString* parentRole;
@property (readwrite) NSString* parentSubRole;
@property (readwrite) NSString* role;
@property (readwrite) NSString* subRole;
@property (readwrite) NSArray* action;
@property (readwrite) CGPoint point;
@property (readwrite) CGSize size;
@property (readwrite) enum Type type;

@end
