
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
    [window setTitle: @"DVM: MacOS Tessallation Shader."];
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
    
    GLuint gVertexShaderObject;
    GLuint gFragmentShaderObject;
    GLuint gshaderProgramObject;
    
    GLuint gVao;
    GLuint gVbo;
    GLuint gMVPMatrixUniform;

    //tesallation shader variable
    GLuint tesellationControlShader;
    GLuint tesellationEvaluationShader;

    GLuint numberOfSegamentsUniform;
    GLuint numberOfStripsUniform;
    GLuint lineColorUniform;
    unsigned int uiNumberOfLineShagament;

    
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
        gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexShaderSourceCode =
            "#version 410 core" \
            "\n" \
            "in vec2 vPosition;" \
            "uniform mat4 u_mvp_matrix;" \
            "void main(void)" \
            "\n" \
            "{" \
            "\n" \
            "gl_Position = vec4(vPosition, 0.0, 1.0);\n" \
            "\n" \
            "}";

        glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


        glCompileShader(gVertexShaderObject);
        /*Here there should be vertex shader complication error Cheking */

        GLint iInfoLogLength = 0;
        GLint iShaderCompileStatus = 0;
        char* szInfoLog = NULL;

        glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Vertex Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        /* tessalation Control Shader */
    tesellationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);

    const GLchar* TesselationControlSourceCode =
        "#version 410 core\n" \
        "layout(vertices = 4)out;\n" \
        "uniform int numberOfSegaments;\n"\
        "uniform int numberOfStrips;\n"\
        "void main(void){\n" \
        "gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"\
        "gl_TessLevelOuter[0] = float(numberOfStrips);\n"\
        "gl_TessLevelOuter[1] = float(numberOfSegaments);\n"\
        "}";

    /*line number 3 : changes is in declaration should not give 3 at declaration 
    * it was this we changed it  "uniform int numberOfSegaments = 3;\n"\
    * 
    */
    glShaderSource(tesellationControlShader, 1, (const GLchar**)&TesselationControlSourceCode, NULL);


    glCompileShader(tesellationControlShader);
    /*Here there should be vertex shader complication error Cheking */


    iInfoLogLength = 0;
    iShaderCompileStatus = 0;
    szInfoLog = NULL;

    glGetShaderiv(tesellationControlShader, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(tesellationControlShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tesellationControlShader, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile_DM, "Tesselation Control Shader Compilation Log Darshan : %s \n", szInfoLog);
                free(szInfoLog);
                [self release];//Myview release hoil
                [NSApp terminate:self];//mag window
            }
        }
    }

    /* tessalation Evaluation Shader */
    tesellationEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);

    const GLchar* TesselationEvaluationSourceCode =
        "#version 410 core\n" \
        "layout(isolines)in;\n" \
        "uniform mat4 u_mvp_matrix;\n" \
        "void main(void){\n" \
        "float tess_Coord = gl_TessCoord.x;\n"\
        "vec3 p0 = gl_in[0].gl_Position.xyz;\n"\
        "vec3 p1 = gl_in[1].gl_Position.xyz;\n"\
        "vec3 p2 = gl_in[2].gl_Position.xyz;\n"\
        "vec3 p3 = gl_in[3].gl_Position.xyz;\n"\
        "vec3 p = p0 * (1.0 - tess_Coord) * (1.0 - tess_Coord) * (1.0 - tess_Coord) + " \
                 "p1 * (3.0 * tess_Coord) * (1.0 - tess_Coord) * (1.0 - tess_Coord) + " \
                 "p2 * (3.0 * tess_Coord * tess_Coord) * (1.0 - tess_Coord) + " \
                 "p3 * (tess_Coord * tess_Coord * tess_Coord);"
        "gl_Position = u_mvp_matrix * vec4( p, 1.0);\n"\
        "}";


    glShaderSource(tesellationEvaluationShader, 1, (const GLchar**)&TesselationEvaluationSourceCode, NULL);


    glCompileShader(tesellationEvaluationShader);
    /*Here there should be vertex shader complication error Cheking */

    iInfoLogLength = 0;
    iShaderCompileStatus = 0;
    szInfoLog = NULL;

    glGetShaderiv(tesellationEvaluationShader, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(tesellationEvaluationShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tesellationEvaluationShader, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile_DM, "Tesselation Evaluation Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                [self release];//Myview release hoil
                [NSApp terminate:self];//mag window
            }
        }
    }



        /* Fragment Shader Object */
        gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar* fragmentShaderSourceCode =
            "#version 410 core\n" \
            "out vec4 FragColor;\n" \
            "uniform vec4 lineColor;\n"\
            "void main(void){\n" \
            "FragColor = lineColor;\n" \
            "}";

        glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

        glCompileShader(gFragmentShaderObject);
        /*Here there should be Fragment shader complication error Cheking */

        glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Fragment Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }



        gshaderProgramObject = glCreateProgram();

        glAttachShader(gshaderProgramObject, gVertexShaderObject);
        glAttachShader(gshaderProgramObject, tesellationControlShader);
        glAttachShader(gshaderProgramObject, tesellationEvaluationShader);
        glAttachShader(gshaderProgramObject, gFragmentShaderObject);

        glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_POSITION, "vPosition");

        glLinkProgram(gshaderProgramObject);

        GLint iShaderProgramLinkStatus = 0;
        glGetProgramiv(gshaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
        if (iShaderProgramLinkStatus == GL_FALSE)
        {
            glGetProgramiv(gshaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Shader program Link Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        
    gMVPMatrixUniform = glGetUniformLocation(gshaderProgramObject, "u_mvp_matrix");
    numberOfSegamentsUniform = glGetUniformLocation(gshaderProgramObject, "numberOfSegaments");
    numberOfStripsUniform = glGetUniformLocation(gshaderProgramObject, "numberOfStrips");
    lineColorUniform = glGetUniformLocation(gshaderProgramObject, "lineColor");

        /*New for traingle */
        const GLfloat triangleVertices[] =
        {
        -1.0f, -1.0f,
        -0.5f, 1.5f,
        0.5f, -1.5f,
        1.0f, 1.0f
        };

        glGenVertexArrays(1, &gVao);
        glBindVertexArray(gVao);

        glGenBuffers(1, &gVbo);
        glBindBuffer(GL_ARRAY_BUFFER, gVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        uiNumberOfLineShagament = 1;

        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        
        glClearColor( 0.0, 0.0, 1.0, 1.0);
        
        gPerspectiveProjectMatix_DM = mat4::identity();
            
        //core video and core graphic
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
            
        //callback
        CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
            
        CGLContextObj cglContextObj = (CGLContextObj)[[self openGLContext] CGLContextObj];
            
        CGLPixelFormatObj cglPixelFormatObj = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
            
        CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContextObj, cglPixelFormatObj);
            
        CVDisplayLinkStart(displayLink);
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
        
        glUseProgram(gshaderProgramObject);
        mat4 TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
        mat4 modelViewMatrix = mat4::identity();
        /*New Matrix for Translation the trangle */
        mat4 modelViewProjectionMatrix = mat4::identity();
        /*Assigning Translation matrix to model view matrix*/
        modelViewMatrix = TranslateMatrix;

        modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

        glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

        glUniform1i(numberOfSegamentsUniform, uiNumberOfLineShagament);
        glUniform1i(numberOfStripsUniform, 1);
        glUniform4fv(lineColorUniform, 1, vec4(1.0f, 1.0f, 1.0f, 1.0f));

        glBindVertexArray(gVao);
        
        glPatchParameteri(GL_PATCH_VERTICES, 4);

        glDrawArrays(GL_PATCHES, 0, 4);

        glBindVertexArray(0);

        glUseProgram(0);

        //Update fuction call before CGFlush
        
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
            
            case 'w':
            case 'W':
            uiNumberOfLineShagament++;
            if (uiNumberOfLineShagament >= 30)
            {
                uiNumberOfLineShagament = 30;
            }
            break;
            case 's':
            case 'S':
            uiNumberOfLineShagament--;
            if (uiNumberOfLineShagament <= 0)
            {
                uiNumberOfLineShagament = 1;
            }
            
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
            
            if (gVao)
            {
                glDeleteVertexArrays(1, &gVao);
                gVao = 0;
            }

            if (gVbo)
            {
                glDeleteBuffers(1, &gVbo);
                gVbo = 0;
            }

            if (gshaderProgramObject)
            {
                glUseProgram(gshaderProgramObject);
                GLsizei shaderCount;
                glGetProgramiv(gshaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);
                GLuint* pShader = NULL;
                pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
                if (pShader == NULL)
                {
                    fprintf(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
                }
                glGetAttachedShaders(gshaderProgramObject, shaderCount, &shaderCount, pShader);
                for (GLsizei i = 0; i < shaderCount; i++)
                {
                    glDetachShader(gshaderProgramObject, pShader[i]);
                }
                free(pShader);

                glDeleteShader(gVertexShaderObject);
                gVertexShaderObject = 0;

                glDeleteShader(gFragmentShaderObject);
                gFragmentShaderObject = 0;

                glDeleteProgram(gshaderProgramObject);
                gshaderProgramObject = 0;

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
