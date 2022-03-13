//
//  AppDelegate.m
//  01.Window
//
//  Created by Darshan Mali on 02/10/21.
//

#import "AppDelegate.h"

#import "ViewController.h"

#import "GLESView.h"


@interface AppDelegate ()

    

@end

@implementation AppDelegate
{
    @private
    UIWindow *window;
    ViewController *viewController;
    GLESView *view;
    
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(nullable NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions{
    
    //code
    
    CGRect win_rect = [[UIScreen mainScreen]bounds];
    
    window = [[UIWindow alloc]initWithFrame:win_rect];
    
    viewController = [[ViewController alloc]init];
    
    [window setRootViewController:viewController];
    
    view = [[GLESView alloc]initWithFrame:win_rect];
    
    [viewController setView:view];
    
    [view release];
    
    [window makeKeyAndVisible];
    
    [view startAnimation];

    
    
    return(YES);
}


//inactive application
- (void)applicationWillResignActive:(UIApplication *)application
{
    //code
    [view stopAnimation];
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
    [view startAnimation]; 
    
}

//closing application
-(void)applicationWillTerminate:(UIApplication *)application
{
    //code
    [view stopAnimation];
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


