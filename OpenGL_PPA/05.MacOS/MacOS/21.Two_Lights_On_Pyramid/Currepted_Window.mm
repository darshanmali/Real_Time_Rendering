
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>//gl.h
#import "vmath.h"

using namespace vmath;

CVReturn MyDisplayLinkCallback( CVDisplayLinkRef,
                               const CVTimeStamp *,
                               const CVTimeStamp *,
                               CVOptionFlags,
                               CVOptionFlags *,
                               void *);

struct Light
{
    vec3 lAmbient;
    vec3 lDefues;
    vec3 lSpecular;
    vec4 lPosition;
};

struct Light Lights[2];
bool bAnimate = false;
bool bLights = false;


FILE *gpFile_DM = NULL;

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 3.0f, 0.0f, 2.0f, 1.0f };

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess_Varible = 128.0f;



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
    [window setTitle: @"DVM: MacOS 3D Pyramid Animation."];
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

    GLuint g_Pyramid_Vao_DM;
    GLuint g_Pyramid_Vertex_Vbo_DM;
    GLuint g_Pyramid_Normal_Vbo_DM;

    /*splite 1.model view, 2.projection matrics */
    GLuint gMMatrixUniform_DM;
    GLuint gVMatrixUniform_DM;
    GLuint gPMatrixUniform_DM;

    GLuint LKeyPressedUniform_DM;/* key passing */

    GLuint LA_Uniform_DM[2];/*Ambient*/
    GLuint LD_Uniform_DM[2];/*Deffuse*/
    GLuint LS_Uniform_DM[2];/*Specular*/
    GLuint LPosition_Uniform_DM[2];/*Positon*/

    GLuint KA_Uniform_DM;/*Material Ambient*/
    GLuint KD_Uniform_DM;/*Material Deffuse*/
    GLuint KS_Uniform_DM;/*Material Specular*/
    GLuint shininess;

    GLfloat P_angle;
    
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
           /* Vertex Shader Object */
        gVertexShaderObject_DM = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexShaderSourceCode =
            "#version 410 core\n" \
            "in vec4 vPosition\n;" \
            "in vec3 vNormal\n;" \
            "uniform mat4 u_model_matrix;\n" \
            "uniform mat4 u_view_matrix;\n" \
            "uniform mat4 u_p_matrix;\n" \
            "uniform int u_lKey;\n" \
            "uniform vec3 u_la[2];\n" \
            "uniform vec3 u_ld[2];\n" \
            "uniform vec3 u_ls[2];\n" \
            "uniform vec4 u_light_position[2];\n" \
            "uniform vec3 u_ka;\n" \
            "uniform vec3 u_kd;\n" \
            "uniform vec3 u_ks;\n" \
            "uniform float u_shininess;\n" \
            "out vec3 fong_ads_light;\n" \
            "void main(void){\n" \
            "if(u_lKey == 1){\n" \
            "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
            "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
            "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \
            "for( int i = 0; i < 2; i++){\n"
            "vec3 ambient[2];\n" \
            "vec3 diffuse[2];\n" \
            "vec3 specular[2];\n" \
            "vec3 light_direction[2];\n" \
            "vec3 reflection_vector[2];\n" \
            "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
            "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);\n" \
            "ambient[i] = u_la[i] * u_ka;\n" \
            "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
            "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), u_shininess);\n" \
            "fong_ads_light = fong_ads_light + ambient[i] + diffuse[i] + specular[i];\n" \
            "}\n" \
            "}\n" \
            "else{\n" \
            "fong_ads_light = vec3(1.0f, 1.0f, 1.0f);" \
            "}\n" \
            "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
            "}";

        glShaderSource(gVertexShaderObject_DM, 1, (const GLchar**)&vertexShaderSourceCode, NULL);


        glCompileShader(gVertexShaderObject_DM);
        /* Here there should be vertex shader complication error Cheking */

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
            "#version 410 core\n" \
            "in vec3 fong_ads_light;\n" \
            "out vec4 FragColor;\n" \
            "void main(void){\n" \
            "FragColor = vec4(fong_ads_light, 1.0f);\n"\
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

        LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_la[0]");
        LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_ld[0]");
        LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_ls[0]");
        LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position[0]");

        LA_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_la[1]");
        LD_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_ld[1]");
        LS_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_ls[1]");
        LPosition_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject_DM, "u_light_position[1]");

        KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ka");
        KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_kd");
        KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_DM, "u_ks");
        shininess = glGetUniformLocation(gshaderProgramObject_DM, "u_shininess");


        /*New for traingle */
        const GLfloat PyramidVertices[] =
        {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,

            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, -0.5f,

            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f
        };

        const GLfloat PyramidNormals[] =
        {
            0.0f,0.447214f,0.894427f,
            0.0f,0.447214f,0.894427f,
            0.0f,0.447214f,0.894427f,

            0.894427f, 0.447214f, 0.0f,
            0.894427f, 0.447214f, 0.0f,
            0.894427f, 0.447214f, 0.0f,

            0.0f, 0.447214f, -0.894427f,
            0.0f, 0.447214f, -0.894427f,
            0.0f, 0.447214f, -0.894427f,

            -0.894427f, 0.447214f, 0.0f,
            -0.894427f, 0.447214f, 0.0f,
            -0.894427f, 0.447214f, 0.0f
        };

        /*Lights properties initialization */
            Lights[0].lAmbient[0] =  0.0f;
            Lights[0].lAmbient[1] =  0.0f;
            Lights[0].lAmbient[2] =  0.0f;
            
            Lights[0].lDefues[0] = 1.0f;
            Lights[0].lDefues[1] = 0.0f;
            Lights[0].lDefues[2] = 0.0f;

            Lights[0].lSpecular[0] = 1.0f;
            Lights[0].lSpecular[1] = 0.0f;
            Lights[0].lSpecular[2] = 0.0f;

            Lights[0].lPosition[0] = 2.0f;
            Lights[0].lPosition[1] = 0.0f;
            Lights[0].lPosition[2] = 0.0f;
            Lights[0].lPosition[3] = 1.0f;

            Lights[1].lAmbient[0] = 0.0f;
            Lights[1].lAmbient[1] = 0.0f;
            Lights[1].lAmbient[2] = 0.0f;
            
            Lights[1].lDefues[0] = 0.0f;
            Lights[1].lDefues[1] = 0.0f;
            Lights[1].lDefues[2] = 1.0f;

            Lights[1].lSpecular[0] = 0.0f;
            Lights[1].lSpecular[1] = 0.0f;
            Lights[1].lSpecular[2] = 1.0f;
            
            Lights[1].lPosition[0] = -2.0f;
            Lights[1].lPosition[1] = 0.0f;
            Lights[1].lPosition[2] = 0.0f;
            Lights[1].lPosition[3] = 1.0f;

        /* Triangle */
        glGenVertexArrays(1, &g_Pyramid_Vao_DM);
        glBindVertexArray(g_Pyramid_Vao_DM);

        glGenBuffers(1, &g_Pyramid_Vertex_Vbo_DM);

        glBindBuffer(GL_ARRAY_BUFFER, g_Pyramid_Vertex_Vbo_DM);

        glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVertices), PyramidVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &g_Pyramid_Normal_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Pyramid_Normal_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidNormals), PyramidNormals, GL_STATIC_DRAW);
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
            glUniform1f(shininess, Shininess_Varible);
            glUniform3fv(LA_Uniform_DM[0], 1, (GLfloat*)lAmbient);
            glUniform3fv(LD_Uniform_DM[0], 1, (GLfloat*)lDefues);
            glUniform3fv(LS_Uniform_DM[0], 1, (GLfloat*)lSpecular);
            glUniform4fv(LPosition_Uniform_DM[0], 1,(GLfloat*)lPosition);
            
            glUniform3fv(LA_Uniform_DM[1], 1, Lights[1].lAmbient);
            glUniform3fv(LD_Uniform_DM[1], 1, Lights[1].lDefues);
            glUniform3fv(LS_Uniform_DM[1], 1, Lights[1].lSpecular);
            glUniform4fv(LPosition_Uniform_DM[1], 1, Lights[1].lPosition);

            glUniform3fv(KA_Uniform_DM, 1, (GLfloat*)material_Ambient);
            glUniform3fv(KD_Uniform_DM, 1, (GLfloat*)material_Diffuse);
            glUniform3fv(KS_Uniform_DM, 1, (GLfloat*)material_Specular);
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

        TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
        RotationMatrtix = vmath::rotate(P_angle, 0.0f, 1.0f, 0.0f);
        ModelMatrix = TranslateMatrix * RotationMatrtix;

        PMatrix = gPerspectiveProjectMatix_DM;

        glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

        glBindVertexArray(g_Pyramid_Vao_DM);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawArrays(GL_TRIANGLES, 6, 3);
        glDrawArrays(GL_TRIANGLES, 9, 3);

        glBindVertexArray(0);

        glUseProgram(0);

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
        
        if (g_Pyramid_Vao_DM)
        {
            glDeleteVertexArrays(1, &g_Pyramid_Vao_DM);
            g_Pyramid_Vao_DM = 0;
        }
        if (g_Pyramid_Vertex_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Pyramid_Vertex_Vbo_DM);
            g_Pyramid_Vertex_Vbo_DM = 0;
        }

        if (g_Pyramid_Normal_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Pyramid_Normal_Vbo_DM);
            g_Pyramid_Normal_Vbo_DM = 0;
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

