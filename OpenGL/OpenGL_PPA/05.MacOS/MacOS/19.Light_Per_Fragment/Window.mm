
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

bool bAnimate = false;
bool bLights = false;

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };
GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 50.0f;

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
    [window setTitle: @"DVM: MacOS Light Per Fragment."];
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

    /* Object variables  */
    GLuint g_Cube_Vao_DM;
    GLuint g_Cube_Vertex_Vbo_DM;
    GLuint g_Cube_Normal_Vbo_DM;
/*
    GLuint gVao_Sphere;
    GLuint gVbo_sphere_position;
    GLuint gVbo_sphere_normal;
    GLuint gVbo_sphere_element;

*/ 
    /*splite 1.model view, 2.projection matrics */
    GLuint gMMatrixUniform_DM;
    GLuint gVMatrixUniform_DM;
    GLuint gPMatrixUniform_DM;

    GLuint LKeyPressedUniform_DM;/* key passing */

    GLuint LA_Uniform_DM;/*Ambient*/
    GLuint LD_Uniform_DM;/*Deffuse*/
    GLuint LS_Uniform_DM;/*Specular*/
    GLuint LPosition_Uniform_DM;/*Positon*/

    GLuint KA_Uniform_DM;/*Material Ambient*/
    GLuint KD_Uniform_DM;/*Material Deffuse*/
    GLuint KS_Uniform_DM;/*Material Specular*/
    GLuint shininess;


    GLfloat C_angle;
     
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
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_p_matrix;" \
            "uniform int u_lKey;" \
            "uniform vec4 u_light_position;" \
            "out vec3 transformed_normal;"  \
            "out vec3 light_direction;\n" \
            "out vec3 view_vector;\n" \
            "void main(void){\n" \
            "if(u_lKey == 1){\n" \
            "vec4 eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" \
            "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
            "light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" \
            "view_vector = normalize(-eye_cordinates.xyz );\n" \
            "}\n" \
            "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
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
            "uniform int u_lKey;" \
            "uniform vec3 u_la;" \
            "uniform vec3 u_ld;" \
            "uniform vec3 u_ls;" \
            "uniform vec3 u_ka;" \
            "uniform vec3 u_kd;" \
            "uniform vec3 u_ks;" \
            "uniform float u_shininess;\n" \
            "in vec3 transformed_normal;\n"  \
            "in vec3 light_direction;\n" \
            "in vec3 view_vector;\n" \
            "out vec4 FragColor;\n" \
            "void main(void){\n" \
            "if(u_lKey == 1){\n" \
            "vec3 normalized_transformed_normal = transformed_normal;\n" \
            "vec3 normalized_light_direction = light_direction;\n" \
            "vec3 normalized_view_vector = view_vector;\n" \
            "vec3 reflection_vector = reflect(-normalized_light_direction , normalized_transformed_normal);\n" \
            "vec3 ambient = u_la * u_ka;\n" \
            "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normal), 0.0f);\n" \
            "vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, normalized_view_vector), 0.0f), u_shininess);\n" \
            "vec3 fong_ads_light = ambient + diffuse + specular;\n" \
            "FragColor = vec4(fong_ads_light, 1.0f);\n"\
            "}\n" \
            "else{\n" \
            "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
            "}\n"
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
        glBindAttribLocation(gshaderProgramObject_DM, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    
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

        gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_model_matrix");
        gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_view_matrix");
        gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_p_matrix");

        LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_lKey");

        LA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_la");
        LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ld");
        LS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ls");
        LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position");

        KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ka");
        KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_kd");
        KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ks");
        shininess = glGetUniformLocation(gshaderProgramObject_DM, "u_shininess");
        
        /*New for traingle */
        const GLfloat CubeVertices[] =
        {
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, -0.5f,
            
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
             
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f
        };

        const GLfloat CubeNormals[] =
        {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };


        /* Triangle */
        glGenVertexArrays(1, &g_Cube_Vao_DM);
        glBindVertexArray(g_Cube_Vao_DM);

        glGenBuffers(1, &g_Cube_Vertex_Vbo_DM);

        glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Vertex_Vbo_DM);

        glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &g_Cube_Normal_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Cube_Normal_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CubeNormals), CubeNormals, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
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

        if (bLights)
        {
            glUniform1i(LKeyPressedUniform_DM, 1);
            glUniform1f(shininess, Shininess);
            glUniform3fv(LA_Uniform_DM, 1, (GLfloat*)lAmbient);
            glUniform3fv(LD_Uniform_DM, 1, (GLfloat*)lDefues);
            glUniform3fv(LS_Uniform_DM, 1, (GLfloat*)lSpecular);
            glUniform3fv(KA_Uniform_DM, 1, (GLfloat*)material_Ambient);
            glUniform3fv(KD_Uniform_DM, 1, (GLfloat*)material_Diffuse);
            glUniform3fv(KS_Uniform_DM, 1, (GLfloat*)material_Specular);
            glUniform4fv(LPosition_Uniform_DM, 1, (GLfloat*)lPosition);
        }
        else
        {
            glUniform1i(LKeyPressedUniform_DM, 0);
        }
        
        mat4 ModelMatrix;
        mat4 ViewMatrix;
        mat4 TranslateMatrix;
        mat4 RotationMatrtix;
        mat4 PMatrix;

        ModelMatrix = mat4::identity();
        ViewMatrix = mat4::identity();
        RotationMatrtix = mat4::identity();
        TranslateMatrix = mat4::identity();
        PMatrix = mat4::identity();

        TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
        RotationMatrtix = vmath::rotate(C_angle, 0.0f, 1.0f, 0.0f);
        ModelMatrix = TranslateMatrix * RotationMatrtix;

        PMatrix =    gPerspectiveProjectMatix_DM;

        glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

        glBindVertexArray(g_Cube_Vao_DM);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
            glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
            glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
            glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
            glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

        glBindVertexArray(0);

        glUseProgram(0);

        //Update fuction call before CGFlush
        if(bAnimate)
            [self Update];
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
        CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
    }
    -(void)Update
    {
        if (C_angle >= 360.0f)
        {
            C_angle = 0.0f;
        }
        C_angle = C_angle + 0.75f;
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
            case 'A':
            case 'a':
             if (bAnimate == true)
            {
                bAnimate = false;
            }
            else
            {
                bAnimate = true;
            }
            break;
            case 'L':
            case 'l':
            if (bLights == true)
            {
                bLights = false;
            }
            else
            {
                bLights = true;
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
        
        if (g_Cube_Vao_DM)
        {
            glDeleteVertexArrays(1, &g_Cube_Vao_DM);
            g_Cube_Vao_DM = 0;
        }
        if (g_Cube_Vertex_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Cube_Vertex_Vbo_DM);
            g_Cube_Vertex_Vbo_DM = 0;
        }

        if (g_Cube_Normal_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Cube_Normal_Vbo_DM);
            g_Cube_Normal_Vbo_DM = 0;
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

