
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>//gl.h
#import "vmath.h"


CVReturn MyDisplayLinkCallback( CVDisplayLinkRef,
                               const CVTimeStamp *,
                               const CVTimeStamp *,
                               CVOptionFlags,
                               CVOptionFlags *,
                               void *);

//                                                  NEW
#define CHECK_IMAGE_WIDTH 64
#define CHECK_IMAGE_HIGHT 64

FILE *gpFile_DM = NULL;

using namespace vmath;

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
    gpFile_DM = fopen(pszlogFileNameWithPath,"w");
    if(gpFile_DM== NULL)
    {
        [self release];
        [NSApp terminate:self];//mag window
    }
    fprintf(gpFile_DM,"\n03.Blue Screen Program.\nProgram started successfully..!\n\n");
    
    NSRect win_Rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc] initWithContentRect:win_Rect
    styleMask:NSWindowStyleMaskTitled |
                NSWindowStyleMaskClosable |
                NSWindowStyleMaskMiniaturizable |
                NSWindowStyleMaskResizable
    backing:NSBackingStoreBuffered
    defer:NO];
    [window setTitle: @"DVM: MacOS 2D Checker_Board Texture."];
    [window center];
    
    myOpenGLView = [[MyOpenGLView alloc] initWithFrame:win_Rect];
    
    [window setContentView:myOpenGLView];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
    
}
-(void)applicationWillTerminate:(NSNotification *)aNotification
{
    //Code
    if(gpFile_DM)
    {
        fprintf(gpFile_DM,"Program terminated successfully.\n");
        fclose(gpFile_DM);
        gpFile_DM = NULL;
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
    
    enum
    {
        DVM_ATTTRIBUTE_POSITION = 0,
        DVM_ATTTRIBUTE_COLOR,
        DVM_ATTTRIBUTE_NORMAL,
        DVM_ATTTRIBUTE_TEXCORD,
    };


    GLubyte Check_Image[CHECK_IMAGE_HIGHT][CHECK_IMAGE_WIDTH][4];
    
    GLuint gVertexShaderObject_DM;
    GLuint gFragmentShaderObject_DM;
    GLuint gshaderProgramObject_DM;
    
    GLuint g_Quade_Vao_DM;
    GLuint g_Quade_Vertex_Vbo_DM;
    GLuint g_Quade_Texture_Vbo_DM;

    GLuint texture_Sampler_Uniform;

    GLuint My_Texture;

    GLuint gMVPMatrixUniform;
    
    mat4 gPerspectiveProjectMatix_DM;
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
            NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:Attributes] autorelease];
        
            if (pixelFormat == nil) {
                fprintf(gpFile_DM,"Can't get pixel format, Exiting!\n");
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
    
    //Initialize
    -(void) prepareOpenGL
    {
            //code
    
        [super prepareOpenGL];
        [[self openGLContext] makeCurrentContext];
            
        //Swap interval
        GLint swapInt = 1;
        [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
            
        //Shader code block
        fprintf(gpFile_DM, "\n**************************OpenGL Log ******************************\n");
        fprintf(gpFile_DM, "OpenGL Vender : %s\n", glGetString(GL_VENDOR));
        fprintf(gpFile_DM, "OpenGL Version : %s\n", glGetString(GL_VERSION));
        fprintf(gpFile_DM, "OpenGL GLSL(Graphic language Shading language) : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

        fprintf(gpFile_DM, "\n**************************OpenGL Enable Extations ******************************\n");
        GLint num_Ext;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_Ext);
        for (GLint i = 0; i < num_Ext; i++)
        {
            fprintf(gpFile_DM, "\n%s\n", glGetStringi(GL_EXTENSIONS, i));
        }
        fprintf(gpFile_DM,"\n");
        /* Vertex Shader Object */
        gVertexShaderObject_DM = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexShaderSourceCode =
            "#version 410 core  \n" \
            "in vec4 vPosition; \n" \
            "in vec2 VTexCoord;    \n" \
            "out vec2 out_TexCoord;    \n" \
            "uniform mat4 u_mvp_matrix;" \
            "void main(void){\n" \
            "gl_Position = u_mvp_matrix * vPosition;\n" \
            "out_TexCoord = -VTexCoord;  \n" \
            "}";

        glShaderSource(gVertexShaderObject_DM, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


        glCompileShader(gVertexShaderObject_DM);
        /*Here there should be vertex shader complication error Cheking */

        GLint iInfoLogLength = 0;
        GLint iShaderCompileStatus = 0;
        char* szInfoLog = NULL;

        glGetShaderiv(gVertexShaderObject_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(gVertexShaderObject_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gVertexShaderObject_DM, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Vertex Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        /* Fragment Shader Object */
        gFragmentShaderObject_DM = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar* fragmentShaderSourceCode =
            "#version 410 core  \n" \
            "out vec4 FragColor;\n" \
            "uniform sampler2D u_Texture_Sampler;" \
            "in vec2 out_TexCoord;  \n" \
            "void main(void){\n" \
            "FragColor = texture(u_Texture_Sampler, out_TexCoord);\n" \
            "}";

        glShaderSource(gFragmentShaderObject_DM, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

        glCompileShader(gFragmentShaderObject_DM);
        /*Here there should be Fragment shader complication error Cheking */

        glGetShaderiv(gFragmentShaderObject_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(gFragmentShaderObject_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gFragmentShaderObject_DM, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Fragment Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }


        gshaderProgramObject_DM = glCreateProgram();

        glAttachShader(gshaderProgramObject_DM, gVertexShaderObject_DM);
        glAttachShader(gshaderProgramObject_DM, gFragmentShaderObject_DM);

        glBindAttribLocation(gshaderProgramObject_DM, DVM_ATTTRIBUTE_POSITION, "vPosition");
        glBindAttribLocation(gshaderProgramObject_DM, DVM_ATTTRIBUTE_TEXCORD, "VTexCoord");
        
        glLinkProgram(gshaderProgramObject_DM);

        GLint iShaderProgramLinkStatus = 0;
        glGetProgramiv(gshaderProgramObject_DM, GL_LINK_STATUS, &iShaderProgramLinkStatus);
        if (iShaderProgramLinkStatus == GL_FALSE)
        {
            glGetProgramiv(gshaderProgramObject_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gFragmentShaderObject_DM, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Shader program Link Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        gMVPMatrixUniform = glGetUniformLocation(gshaderProgramObject_DM, "u_mvp_matrix");
        texture_Sampler_Uniform = glGetUniformLocation(gshaderProgramObject_DM, "u_Texture_Sampler");


        /*  Quade Vao */
       const GLfloat Cude_Vertices[] =
        {
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,

            2.41421f, 1.0f, -1.41421f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            2.41421f, -1.0f, -1.41421f
        };

        const GLfloat CubeColor[] =
        {
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,

            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
        };

        /* Quade */
        glGenVertexArrays(1, &g_Quade_Vao_DM);
        glBindVertexArray(g_Quade_Vao_DM);

        glGenBuffers(1, &g_Quade_Vertex_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Quade_Vertex_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Cude_Vertices), Cude_Vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &g_Quade_Texture_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Quade_Texture_Vbo_DM);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(CubeColor), CubeColor, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_TEXCORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_TEXCORD);
            
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        glClearColor( 0.0, 0.0, 0.0, 1.0);
        
        gPerspectiveProjectMatix_DM = mat4::identity();
        
        [self LoadGLTexture:&My_Texture];
        
        //core video and core graphic
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
            
        //callback
        CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
            
        CGLContextObj cglContextObj = (CGLContextObj)[[self openGLContext] CGLContextObj];
            
        CGLPixelFormatObj cglPixelFormatObj = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
            
        CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContextObj, cglPixelFormatObj);
            
        CVDisplayLinkStart(displayLink);
    }

    -(bool)LoadGLTexture:(GLuint *)Text_Image
    {
        bool bResult = true;

        //From here We are Starting the Texturing Code

        [self MakeCheckImage];

        glGenTextures(1, Text_Image);

        glBindTexture(GL_TEXTURE_2D, *Text_Image);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //Setting Texture Param
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECK_IMAGE_WIDTH, CHECK_IMAGE_HIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, Check_Image);

        return (bResult);
    }

    -(void) MakeCheckImage
    {
        int i, j, c;
        for (i = 0; i < CHECK_IMAGE_HIGHT; i++)
        {
            for (j = 0; j < CHECK_IMAGE_WIDTH; j++)
            {
                c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;

                Check_Image[i][j][0] = (GLubyte)c;
                Check_Image[i][j][1] = (GLubyte)c;
                Check_Image[i][j][2] = (GLubyte)c;
                Check_Image[i][j][3] = (GLubyte)c;
            }
        }
    }

    //Reshape
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
        
        gPerspectiveProjectMatix_DM = vmath::perspective(44.0f, (GLfloat)rect.size.width / (GLfloat)rect.size.height, 0.1f, 100.0f);
        
        CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
            
    }
    -(void)drawRect:(NSRect)dirtyRect// this is similar to WM_Paint
    {
        //Code
        [self drawView];
        
    }

    //Display
    -(void) drawView
    {
        
            //code
        [[self openGLContext] makeCurrentContext];
        CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(gshaderProgramObject_DM);

        mat4 TranslateMatrix = vmath::translate( 0.0f, 0.0f, -5.0f);
        mat4 modelViewMatrix = mat4::identity();
        mat4 modelViewProjectionMatrix = mat4::identity();

        modelViewMatrix = TranslateMatrix;

        modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

        glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, My_Texture);
        glUniform1i(texture_Sampler_Uniform, 0);

        glBindVertexArray(g_Quade_Vao_DM);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 8);
        
        glBindVertexArray(0);

        glUseProgram(0);

        //Update fuction call before CGFlush
        [self Update];
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
        CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
    }
    -(void)Update
    {


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
        
        if (My_Texture)
        {
            glDeleteTextures(1, &My_Texture);
            My_Texture = 0;
        }

        if (g_Quade_Vao_DM)
        {
            glDeleteVertexArrays(1, &g_Quade_Vao_DM);
            g_Quade_Vao_DM = 0;
        }
        if (g_Quade_Vertex_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Quade_Vertex_Vbo_DM);
            g_Quade_Vertex_Vbo_DM = 0;
        }

        if (g_Quade_Texture_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Quade_Texture_Vbo_DM);
            g_Quade_Texture_Vbo_DM = 0;
        }

        if (gshaderProgramObject_DM)
        {
            glUseProgram(gshaderProgramObject_DM);
            GLsizei shaderCount;
            glGetProgramiv(gshaderProgramObject_DM, GL_ATTACHED_SHADERS, &shaderCount);
            GLuint* pShader = NULL;
            pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
            if (pShader == NULL)
            {
                fprintf(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
            }
            glGetAttachedShaders(gshaderProgramObject_DM, shaderCount, &shaderCount, pShader);
            for (GLsizei i = 0; i < shaderCount; i++)
            {
                glDetachShader(gshaderProgramObject_DM, pShader[i]);
            }
            free(pShader);
            glDeleteShader(gVertexShaderObject_DM);
            gVertexShaderObject_DM = 0;
            glDeleteShader(gFragmentShaderObject_DM);
            gFragmentShaderObject_DM = 0;
            glDeleteProgram(gshaderProgramObject_DM);
            gshaderProgramObject_DM = 0;
            glUseProgram(0);
        }
        if (gpFile_DM)
        {
            fprintf(gpFile_DM, "Log File is Successfully closed.\n");
            fclose(gpFile_DM);
            gpFile_DM = NULL;
        }
        
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
