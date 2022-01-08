
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>//gl.h

CVReturn MyDisplayLinkCallback( CVDisplayLinkRef,
                               const CVTimeStamp *,
                               const CVTimeStamp *,
                               CVOptionFlags,
                               CVOptionFlags *,
                               void *);


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


@interface MyOpenGLView : NSOpenGLView

@end
/** ************************ App Delegate Start */
@implementation AppDelegate
{
    @private
    NSWindow *window;
    MyOpenGLView *myOpenGLView;
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
    fprintf(gpFile,"\n03.Blue Screen Program.\nProgram started successfully..!\n\n");
    
    NSRect win_Rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc] initWithContentRect:win_Rect
    styleMask:NSWindowStyleMaskTitled |
                NSWindowStyleMaskClosable |
                NSWindowStyleMaskMiniaturizable |
                NSWindowStyleMaskResizable
    backing:NSBackingStoreBuffered
    defer:NO];
    [window setTitle: @"DVM: MacOS BlueScreen."];
    [window center];
    
    myOpenGLView = [[MyOpenGLView alloc] initWithFrame:win_Rect];
    
    [window setContentView:myOpenGLView];
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
    [MyOpenGLView release];
    [window release];
    [super dealloc];

}
    
@end
/* App Delegate End */

@implementation MyOpenGLView

{
 @private
    CVDisplayLinkRef displayLink;
    
}

    -(id)initWithFrame:(NSRect)frame // frame == WinRect
    {
        //code
        //1. pixel format attribute
        //2. pixel format
        //3. OpenGL Context
        //4. OpenGL Set Cotext
        
        self = [super initWithFrame:frame];
        if (self)
        {
            NSOpenGLPixelFormatAttribute Attributes[] =
            {
                NSOpenGLPFAOpenGLProfile,//Which version OpenGL : 4.1
                NSOpenGLProfileVersion4_1Core,//openGL version is locked here by Mac
                NSOpenGLPFAScreenMask,//Which Display
                CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),//Main Display
                NSOpenGLPFANoRecovery,//Dont switch to software rendering
                NSOpenGLPFAAccelerated,// we Are asking for the Hardware Rendering
                NSOpenGLPFAColorSize,
                24,
                NSOpenGLPFADepthSize,
                24,
                NSOpenGLPFAAlphaSize,
                8,
                NSOpenGLPFADoubleBuffer,
                0
                
            };
            NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:&Attributes] autorelease];
        
            if (pixelFormat == nil) {
                fprintf(gpFile,"Can't get pixel format, Exiting!\n");
                [self release];
                [NSApp terminate:self];//mag window        }
        }
        //now get the contex from the pixel format
        NSOpenGLContext *glContex = [[[NSOpenGLContext alloc] initWithFormat:pixelFormat  shareContext:nil] autorelease];
        //now we will set our pixel format to our openGLview
            [self setPixelFormat:pixelFormat];
            //Set the context to OpenGLView
            [self setOpenGLContext:glContex];
        }
        return (self);
    }
        
    -(CVReturn) getFrameFromType:(const CVTimeStamp *)outputTime
    {
        //code
        NSAutoreleasePool *pool = [[ NSAutoreleasePool alloc] init]; //Apple supports multithreading
        [self drawView];
        [pool release];
        return (kCVReturnSuccess);
            
    }
    
        -(void) prepareOpenGL
        {
            //code
            
            [super prepareOpenGL];
            [[self openGLContext] makeCurrentContext];
            
            //Swap interval
            GLint swapInt = 1;
            [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
            
            //Shader code block
            
            glClearColor( 0.0, 0.0, 1.0, 1.0);
            
            //core video and core graphic
            CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
            
            //callback
            CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
            
            CGLContextObj cglContextObj = (CGLContextObj)[[self openGLContext] CGLContextObj];
            
            CGLPixelFormatObj cglPixelFormatObj = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
            
            CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContextObj, cglPixelFormatObj);
            
            CVDisplayLinkStart(displayLink);
        }
        -(void) reshape
        {
            //code
            [super reshape];
            CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
            NSRect rect = [self bounds];
            if(rect.size.height < 0)
            {
                rect.size.height = 1;
            }
            
            glViewport(0,0,(GLsizei)rect.size.width, (GLsizei)rect.size.height);
            CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
            
            
        }
//3.21

    
    -(void)drawRect:(NSRect)dirtyRect// this is similar to WM_Paint
    {
        //Code
        [self drawView];
        
    }
        
    -(void) drawView
    {
        //code
        [[self openGLContext] makeCurrentContext];
        CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
        CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
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
    
            CVDisplayLinkStop(displayLink);
            CVDisplayLinkRelease(displayLink);
            [super dealloc];
        }
@end

//this fuction should be returned globally
CVReturn MyDisplayLinkCallback( CVDisplayLinkRef displayLink,
                               const CVTimeStamp *now,
                               const CVTimeStamp *outputTime,
                               CVOptionFlags flagsIn,
                               CVOptionFlags *flagsOut,
                               void *displayLinkContext)
{
    //code
    CVReturn result = [(MyOpenGLView *)displayLinkContext getFrameFromType:outputTime];
    
    
    return (result);
}
