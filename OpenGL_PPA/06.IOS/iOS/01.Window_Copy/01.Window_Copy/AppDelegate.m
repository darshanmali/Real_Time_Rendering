//
//  AppDelegate.m
//  01.Window
//
//  Created by Darshan Mali on 02/10/21.
//

#import "AppDelegate.h"

#import "ViewController.h"

#import "MyView.h"


@interface AppDelegate ()

    

@end

@implementation AppDelegate
{
    @private
    UIWindow *window;
    ViewController *viewController;
    MyView *view;
    
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(nullable NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions{
    
    //code
    
    CGRect win_rect = [[UIScreen mainScreen]bounds];
    
    window = [[UIWindow alloc]initWithFrame:win_rect];
    
    viewController = [[ViewController alloc]init];
    
    [window setRootViewController:viewController];
    
    view = [[MyView alloc]initWithFrame:win_rect];
    
    [viewController setView:view];
    
    [view release];
    
    [window makeKeyAndVisible];
    
    return(YES);
}

//closing application
-(void)applicationWillTerminate:(UIApplication *)application
{
    //code
}

//inactive application
- (void)applicationWillResignActive:(UIApplication *)application
{
    //code
}

//going to background
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    //code
    
}

//forground la aalya var
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    //code
    
}


-(void)applicationDidBecomeActive:(UIApplication *)application
{
    //code
    
}

-(void)dealloc
{
    //code
    [view release];
    
    [viewController release];
    
    [window release];
    
    [super dealloc];
    
}

@end


