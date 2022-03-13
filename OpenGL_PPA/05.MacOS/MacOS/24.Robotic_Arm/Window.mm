
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>//gl.h
#import "vmath.h"
#import "Sphere.h"


CVReturn MyDisplayLinkCallback( CVDisplayLinkRef,
                               const CVTimeStamp *,
                               const CVTimeStamp *,
                               CVOptionFlags,
                               CVOptionFlags *,
                               void *);



FILE *gpFile_DM = NULL;

using namespace vmath;

mat4 PushMatrix_DM(mat4);
void PopMatrix_DM(void);
struct Matrix_Stack_DM* CreateMatrix_DM(void);

struct Matrix_Stack_DM
{
    mat4 Matrix;
    struct Matrix_Stack_DM* Next;
}*Start = NULL, *Top = NULL, *Prev = NULL;

int PushToStack_DM = 0;
GLfloat angle_Sholder_DM = 0.0;
GLfloat angle_Arm_DM = 0.0;

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
    [window setTitle: @"DVM: MacOS 3D Cube Animation."];
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
    
    GLuint gVertexShaderObject_DM;
    GLuint gFragmentShaderObject_DM;
    GLuint gshaderProgramObject_DM;

    GLuint g_Sphere_Vao_DM;
    GLuint g_Sphere_Vertex_Vbo_DM;
    GLuint g_Sphere_Color_Vbo_DM;
    GLuint gVbo_sphere_element;

    /*New For Sphere*/
    float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    unsigned short sphere_elements[2280];

    GLuint gNumVertices;
    GLuint gNumElements;

    GLfloat P_angle;
    
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
            "out vec3 OutColor;    \n" \
            "uniform mat4 u_mvp_matrix;" \
            "void main(void){\n" \
            "gl_Position = u_mvp_matrix * vPosition;\n" \
            "OutColor = vec3(1.0f, 1.0f, 1.0f);  \n" \
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
            "in vec3 OutColor;  \n" \
            "void main(void){\n" \
            "FragColor = vec4(OutColor, 1.0);\n" \
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

        /* New for Sphere */
        Sphere sphere;
        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        gNumVertices = sphere.getNumberOfSphereVertices();
        gNumElements = sphere.getNumberOfSphereElements();

        /* Sphere */
        glGenVertexArrays(1, &g_Sphere_Vao_DM);
        
        /*For Vertex */
        glBindVertexArray(g_Sphere_Vao_DM);
        
        glGenBuffers(1, &g_Sphere_Vertex_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Vertex_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        /*For elements */
        glGenBuffers(1, &gVbo_sphere_element);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        glClearColor( 0.0, 0.0, 0.0, 1.0);
        
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
        
        glUseProgram(gshaderProgramObject_DM);
        
        mat4 TranslateMatrix = mat4::identity();
        mat4 RotationMatrix = mat4::identity();
        mat4 ScaleMatrix =  mat4::identity();
        mat4 modelViewMatrix = mat4::identity();
        mat4 modelViewProjectionMatrix = mat4::identity();

        TranslateMatrix = vmath::translate( 0.0f, 0.0f, -8.0f);
        ScaleMatrix = vmath::scale(2.0f, 0.25f, 0.25f);
        RotationMatrix = vmath::rotate(angle_Sholder_DM, 0.0f, 0.0f, 1.0f);
        
        modelViewMatrix = RotationMatrix * TranslateMatrix * ScaleMatrix;
        
        
        //we are calling first push
        modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    
        TranslateMatrix = vmath::translate(0.5f, 0.0f, 0.0f);
        modelViewMatrix = modelViewMatrix * TranslateMatrix;
        modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;
        
        glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(g_Sphere_Vao_DM);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
        glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);

        modelViewMatrix = mat4::identity();
        TranslateMatrix = mat4::identity();
        RotationMatrix = mat4::identity();
        modelViewProjectionMatrix = mat4::identity();

        TranslateMatrix = vmath::translate(0.25f, 0.0f, 0.0f);
        RotationMatrix = vmath::rotate(angle_Arm_DM, 0.0f, 0.0f, 1.0f);

        modelViewMatrix = RotationMatrix * TranslateMatrix;


        //we are calling first push
        modelViewMatrix = PushMatrix_DM(modelViewMatrix);
        
        TranslateMatrix = vmath::translate(0.75f, 0.0f, 0.0f);
        
        PopMatrix_DM();
        PopMatrix_DM();
        
        modelViewMatrix = modelViewMatrix * TranslateMatrix;

        modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

        glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glBindVertexArray(g_Sphere_Vao_DM);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
        glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);
        
        //Update fuction call before CGFlush
        [self Update];
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
        CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
    }
    -(void)Update
    {
        if (P_angle >= 360.0f)
        {
            P_angle = 0.0f;
        }
        P_angle = P_angle + 0.75f;
    }

    mat4 PushMatrix_DM(mat4 matrix)
    {
        struct Matrix_Stack_DM* Temp = NULL;
    
        Temp = CreateMatrix_DM();
        if (Temp == NULL)
        {
            fprintf(gpFile_DM, "Node is empty");
        }
    
        Temp->Matrix = matrix;
        Temp->Next = NULL;

        if (PushToStack_DM < 1)
        {
            fprintf(gpFile_DM, " Top Matrix is null..!\n");
            Top = Temp;
            Start = Temp;
            Prev = Temp;
            PushToStack_DM++;
        }
        else if(Top != NULL && PushToStack_DM > 0)
        {
            matrix = Prev->Matrix * matrix;
            Temp->Matrix = matrix;
            Prev = Temp;
            Top->Next = Temp;
            Top = Temp;
            PushToStack_DM++;
        }

        return matrix;
    }

    struct Matrix_Stack_DM* CreateMatrix_DM()
    {
        struct Matrix_Stack_DM* Temp = NULL;
        Temp = (struct Matrix_Stack_DM*)malloc(sizeof(struct Matrix_Stack_DM));
        return(Temp);
    }

    void PopMatrix_DM()
    {
        struct Matrix_Stack_DM* Temp = NULL;
        struct Matrix_Stack_DM* Prev_Matrix = NULL;
        Temp = Start;

        while (Temp->Next != NULL)
        {
            Prev_Matrix = Temp;
            Temp = Temp->Next;
        }
        if(Prev_Matrix != NULL)
            Prev_Matrix->Next = NULL;

        Top = Prev_Matrix;
        free(Temp);
        Temp = NULL;
        PushToStack_DM--;
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
                case 'w'://UP
                case 'W':
                angle_Sholder_DM += 2.0f;
                if (angle_Sholder_DM > 360.0f)
                {
                    angle_Sholder_DM = 0.0f;
                }
                break;
                case 's'://UP
                case 'S':
                angle_Sholder_DM -= 2.0f;
                if (angle_Sholder_DM > 360.0f)
                {
                    angle_Sholder_DM = 0.0f;
                }
                break;
                
                case 'A'://UP
                case 'a':
                angle_Arm_DM += 2.0f;
                if (angle_Arm_DM > 360.0f)
                {
                    angle_Arm_DM = 0.0f;
                }
                break;
                
                case 'D'://UP
                case 'd':
                angle_Arm_DM -= 2.0f;
                if (angle_Arm_DM > 360.0f)
                {
                    angle_Arm_DM = 0.0f;
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
        
        if (g_Sphere_Vao_DM)
        {
            glDeleteVertexArrays(1, &g_Sphere_Vao_DM);
            g_Sphere_Vao_DM = 0;
        }
        if (g_Sphere_Vertex_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Sphere_Vertex_Vbo_DM);
            g_Sphere_Vertex_Vbo_DM = 0;
        }

        if (g_Sphere_Color_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Sphere_Color_Vbo_DM);
            g_Sphere_Color_Vbo_DM = 0;
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



/*
    //Second 
    modelViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    RotationMatrix = mat4::identity();

    TranslateMatrix = vmath::translate(1.5f, 0.0f, 0.0f);
    RotationMatrix = vmath::rotate(P_angle, 0.0f, 0.0f, 1.0f);

    modelViewProjectionMatrix = mat4::identity();
    modelViewMatrix = TranslateMatrix * RotationMatrix;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    PopMatrix_DM();
    PopMatrix_DM();

    TranslateMatrix = vmath::translate(0.5f, 0.0f, 0.0f);
    modelViewMatrix = modelViewMatrix * TranslateMatrix;

    modelViewProjectionMatrix = gPerspectiveProjectMatix_DM * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(g_Sphere_Vao_DM);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

*/
