
#import <Foundation/Foundation.h> // i want foundatation.h from Foundation framework
/* it is standard lib for all mac devices */
#import <Cocoa/Cocoa.h> //Analogus to Coco.h

/* It is means */
@interface AppDelegate : NSObject < NSApplicationDelegate, NSWindowDelegate >
/* NS : stands for Next Steps
 NSApplication  : mhantoy majhya functionality tu AppDelegate kadun karun ghe.
 NSWindow       : mhantoy majhya functionality tu AppDelegate kadun karun ghe.
 < >            : Hyala Delegation conformant, he sangtay ki hyala aamhi degation sathi conformation dilay.
*/

@end

int main( int argc, char* argv[]){
    //Code
    NSAutoreleasePool *pool = [[ NSAutoreleasePool alloc] init];
    //auto release pool la allocate kel aani init ne initialise kel
    NSApp = [NSApplication sharedApplication];
    //NSApp he globle variable aahe aani aata aapan sharedApplication kadun Aaapala NSApplication magatoy mhanjech HInstance
    //NSApp ha real process aahe (HInstance) NSApplication cha object aahe
    [NSApp setDelegate : [[ AppDelegate alloc] init]];// it is similar to create window........case WMCreate: is analogus to -> Application_Dead_Finish_Launching.
    // Above call is similar to ( function_name  (parameter lisr ))
    [NSApp run];
    // this is called as run loop OR Message loop
    [pool release];
    //release all y objects and end off program
    //when application get terminated NSApps chi aajhun ek method call hote which is similar to WM_Destroy: -> Application_Will_Terminate:
    return (0);
}


@interface MyView : NSView

@end

@implementation AppDelegate
//Apalyala App Madhe View Lagnart aahe mhanun tyala declare karav lagel hyachya varti

{
    @private
    NSWindow *window;
    MyView *view;
}
/* Now we need to create 4 methods
1.Appdeligate AppDeadFinish Launching()
2.Appdeligate AppWillTerminate()

3.Windows chi pan MEthod lagnar aahe : Window_Will_Close()
4.NSObject chi method DeAlloc() */

-(void)AppDeadFinishLaunching:(NSNotification *)aNotification
{
    //Code
    //NSRect is internally Core Graphics CGRect == NSRect
    NSRect win_Rect = NSMakeRect(0, 0, 800, 600); // This calling is done with 'C' core library that is why it is called in this manner.
    //1.NSPoint Structure mhanjech first two parameter == NSPoint.X and NSPoint.Y ||
    //2.NSSize Structure mhanjech first two parameter == NSPoint.X and NSPoint.Y ||
    window = [[NSWindow alloc] initWithContentRect: win_Rect
                                        //WS_Caption == NSWindowStyleMaskTitle
                                         styleMask: NSWindowStyleMaskTitled |
                                        //WS_CloseBox
                                                    NSWindowStyleMaskClosable |
                                        //WS_MinimiseBox
                                                    NSWindowStyleMaskMiniaturizable |
                                        //WS_Resize
                                                    NSWindowStyleMaskResizable
                                        //how to keep window in graphics card 1. retained karu? 2.render modegraphics. 3.Buffer Mode (store into graphics card memory )
                                        backing: NSBackingStoreBuffered
                                        //how to and when should i display window whether thodya vel thambun dakhau ki lagech dakhau
                                        //defer : YES means show it in low priority mode
                                        //defer : NO means show it in high priority mode
                                        defer:NO
                                        //WS_Overlapped_Window == styleMask
              ];
    [window setTitle: @"DVM:MacOS Window."];
    [window center];
    
    //Aapan ithun MyView La jato
    view = [[MyView alloc] initWithFrame:win_Rect];
    
    [window setContentView: view];
    //self == this in windows
    //Seteligate
    [window setDelegate: self];
    //make key Means majhya var focus kar ( keyboard event majhya var ghe )
    //makeKey == setFocus and getFocus
    //OrderFront == SetForground || in message box we say : MB_TopMost
    [window makeKeyAndOrderFront: self];
}
//After this what will happen
//1. window tayar hoil aani front la yeil
//2. window la view lagel
//3. focus aalya var yeil (i.e. makeKeyAndOrderFront)
//4. NSApp cha runloop start hoil

-(void)ApplicationWillTerminate:(NSNotification *)aNotification
{
    //Code

}

-(void)WindowWillClose:(NSNotification *)aNotification
{
    //Code
    //This is Winodws Method Not NS
    //ethe AppDelegate sangtoy He NSApp to terminate ho
    [NSApp terminate:self];
}

-(void)dealloc
{
    //Code
    [view release];
    [window release];
    [super dealloc];
    
    
    
}
    
@end


@implementation MyView
{
    
 @private
    NSString *central_Text;
}

//ID: jya method object return kartat tya method la id return kartat aas mhantat
-(id)initWithFrame:(NSRect)frame // frame == WinRect
{
    //aaplyala Myview tayar karayacha aahe
    self = [super initWithFrame:frame];// aata aapan actualy Baap initWithFrame la call kelay
    
    if (self)
    {
        central_Text = @"Hello World!!!";
        //WM_Paint == DrawRect
        //InvalidateRect == dirtyRect
       // -(void)drawRect:(NSRect)dirtyRect
       // {
            
       // }
    }
    return (self);
}

@end
