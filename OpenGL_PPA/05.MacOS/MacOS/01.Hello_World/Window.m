
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

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
    NSRect win_Rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc] initWithContentRect:win_Rect
    styleMask:NSWindowStyleMaskTitled |
                NSWindowStyleMaskClosable |
                NSWindowStyleMaskMiniaturizable |
                NSWindowStyleMaskResizable
    backing:NSBackingStoreBuffered
    defer:NO];
    [window setTitle: @"DVM: MacOS Window."];
    [window center];
    
    view = [[MyView alloc] initWithFrame:win_Rect];
    
    [window setContentView:view];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
    
}
-(void)applicationWillTerminate:(NSNotification *)aNotification
{
    //Code

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
    self = [super initWithFrame:frame];
    if (self)
    {
        central_Text = @" Hello World!!!";
    }
  
    return (self);
}
-(void)drawRect:(NSRect)dirtyRect// this is similar to WM_Paint
  {
      //Code
      NSColor *backgraundColor = [NSColor blackColor];
      [backgraundColor set];
      NSRectFill(dirtyRect);
      
      NSDictionary *dictionaryForText = [NSDictionary dictionaryWithObjectsAndKeys:
                                         [NSFont fontWithName:@"Helvetica" size:50.0], NSFontAttributeName,
                                         [NSColor greenColor], NSForegroundColorAttributeName,
                                         nil];
      
      NSSize TextSize = [central_Text sizeWithAttributes:dictionaryForText];
      NSPoint point;
      point.x = (dirtyRect.size.width / 2) - (TextSize.width / 2);
      point.y = (dirtyRect.size.height / 2) - (TextSize.height / 2);
      
      [central_Text drawAtPoint:point withAttributes:dictionaryForText];
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
    central_Text = @"Left Mouse Button is Click.";
    [self setNeedsDisplay:YES];//Invalidate Rect
    
}


-(void)rightMouseDown:(NSEvent *)theEvent//WM_RDOWN
{
    //code
    central_Text = @"Right Mouse Button is Click.";
    [self setNeedsDisplay:YES];//Invalidate Rect
    
}

-(void)otherMouseDown:(NSEvent *)theEvent//WM_MDOWN
{
    //code
    central_Text = @"Hello World..!!!";
    [self setNeedsDisplay:YES];//Invalidate Rect
    
}

-(void)dealloc
{
    
    [super dealloc];
}

@end

