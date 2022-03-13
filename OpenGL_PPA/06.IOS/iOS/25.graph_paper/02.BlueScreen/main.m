//
//  main.m
//  01.Window
//
//  Created by Darshan Mali on 02/10/21.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[]) {
    
    //Code
    NSAutoreleasePool *pool = [[ NSAutoreleasePool alloc] init];
    
    NSString * appDelegateClassName;
    
    appDelegateClassName = NSStringFromClass([AppDelegate class]);
    
    int ret = UIApplicationMain(argc, argv, nil, appDelegateClassName);
    
    [pool release];
    
    return (ret);
}
