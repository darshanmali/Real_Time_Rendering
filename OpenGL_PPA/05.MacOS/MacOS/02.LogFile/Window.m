
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

FILE *gpFile = NULL;

@interface AppDelegate : NSObject < NSApplicationDelegate, NSWindowDelegate >

@end

int main( int argc, char* argv[]){
    //Code
    NSAutoreleasePool *pool = [[ NSAutoreleasePool alloc] init];
   
    NSApp = [NSApplication sharedApplication];
                           
    [NSApp setDelegate : [[ AppDelegate alloc] init]];
    
    [NSApp run];
    
    [pool release];
    return (0);
}


@interface MyView : NSView

@end

@implementation AppDelegate
{
    @private
    NSWindow *window;
    MyView *view;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    //Code
    NSBundle *appBundle = [NSBundle mainBundle];//packege is internally bundle
    NSString *appDirPath = [appBundle bundlePath];//taking bundle's path
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];//Bundle chya dir madhun
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/DM_log.txt",parentDirPath];
    
    const char *pszlogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
    gpFile = fopen(pszlogFileNameWithPath,"w");
    if(gpFile== NULL)
    {
        [self release];
        [NSApp terminate:self];//mag window
    }
    fprintf(gpFile,"Program started successfully..!\n\n");
    
    NSRect win_Rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc] initWithContentRect:win_Rect
    styleMask:NSWindowStyleMaskTitled |
                NSWindowStyleMaskClosable |
                NSWindowStyleMaskMiniaturizable |
                NSWindowStyleMaskResizable
    backing:NSBackingStoreBuffered
    defer:NO];
    [window setTitle: @"DVM: MacOS Log File."];
    [window center];
    
    view = [[MyView alloc] initWithFrame:win_Rect];
    
    [window setContentView:view];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
    
}
-(void)applicationWillTerminate:(NSNotification *)aNotification
{
    //Code
    if(gpFile)
    {
        fprintf(gpFile,"Program terminated successfully.\n");
        fclose(gpFile);
        gpFile = NULL;
    }

}

-(void)windowWillClose:(NSNotification *)aNotification
{
    //Code
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

-(id)initWithFrame:(NSRect)frame // frame == WinRect
{
    //code
    self = [super initWithFrame:frame];
    if (self)
    {
        
    }
  
    return (self);
}
-(void)drawRect:(NSRect)dirtyRect// this is similar to WM_Paint
  {
      //Code
      
  }

-(BOOL)acceptsFirstResponder
{
    //Code
    [[self window] makeFirstResponder:nil];
    return (YES);
}

-(void)keyDown:(NSEvent *)theEvent//WM_KEYDOWN
{
    //code
    int key = [[theEvent characters] characterAtIndex:0];
    switch (key) {
        case 27:
            [self release];//Myview release hoil
            [NSApp terminate:self];//mag window
            break;
        case 'f':
        case 'F':
            [[self window] toggleFullScreen:self];
            break;
        default:
            break;
    }
    
}


-(void)mouseDown:(NSEvent *)theEvent//WM_LDOWN
{
    //code
    
}


-(void)rightMouseDown:(NSEvent *)theEvent//WM_RDOWN
{
    //code
    
}

-(void)otherMouseDown:(NSEvent *)theEvent//WM_MDOWN
{
    //code
    
}

-(void)dealloc
{
    
    [super dealloc];
}

@end

