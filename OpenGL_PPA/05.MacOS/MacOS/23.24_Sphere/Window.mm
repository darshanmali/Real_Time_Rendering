
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

bool bAnimate = false;
bool bLights = false;

bool per_Vertex = true;
bool per_Fragment = false;

bool X_Rotation = true;
bool Y_Rotation = false;
bool Z_Rotation = false;

int x;
int y;
int w;
int h;
int iterator_Color;



GLfloat material_Ambient[24][3] = { 

{0.0215f, 0.1745f, 0.0215f},
{0.135f, 0.2225f, 0.1575f},

{0.05375f, 0.05f, 0.06625f},
{0.25f, 0.20725f, 0.20725f},

{0.1745f, 0.01175f, 0.01175f},
{1.0f, 0.18725f, 0.1745f},

{0.329412f, 0.223529f, 0.027451f},
{0.2125f, 0.1275f, 0.054f},

{0.25f, 0.25f, 0.25f},
{0.19125f, 0.0735f, 0.0225f},

{0.24725f, 0.1995f, 0.0745f},
{0.19225f, 0.19225f, 0.19225f},

//1st - 3rd
{0.0f, 0.0f, 0.0f},
{0.0f,0.1f,0.06f},

{0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f},

{0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f},

//1st - 4rd
{0.02f, 0.02f, 0.02f},
{0.0f, 0.05f, 0.05f},

{0.0f, 0.05f, 0.0f},
{0.05f, 0.0f, 0.0f},

{0.05f, 0.05f, 0.05f},
{0.05f, 0.05f, 0.0f},

};

GLfloat material_Diffuse[24][3] = { 
    
{0.07568f, 0.61424f, 0.07568f},
{0.54f, 0.89f, 0.63f}, 

{0.18275f, 0.17f, 0.22525f},
{1.0f, 0.829f, 0.829f},

{0.61424f, 0.04136f, 0.04136f},
{0.396f, 0.74151f, 0.69102f}, 

{0.780392f, 0.568627f, 0.113725f},
{0.714f, 0.4284f, 0.18144f},

{0.4f, 0.4f, 0.4f},
{0.7038f, 0.27048f, 0.0828f},

{0.75164f, 0.60648f, 0.22648f},
{0.50754f, 0.50754f,0.50754f},

//1st - 3rd
{0.01f, 0.01f, 0.01f},
{0.0f, 0.50980392f, 0.50980392f},

{0.1f, 0.35f, 0.1f},
{0.5f, 0.0f, 0.0f},

{0.55f, 0.55f, 0.55f},
{0.5f, 0.5f, 0.0f},

//1st - 4rd
{0.01f, 0.01f, 0.01f},
{0.4f, 0.5f, 0.5f},

{0.4f, 0.5f, 0.4f},
{0.5f, 0.4f, 0.4f},

{0.5f, 0.5f, 0.5f},
{0.5f, 0.5f, 0.4f},

};

GLfloat material_Specular[24][3] = {
    
 {0.633f, 0.727811f, 0.633f},
 {0.316228f, 0.316228f, 0.316228f},

{0.332741f, 0.328634f, 0.346435f},
{0.296648f, 0.296648f, 0.296648f},

{0.727811f, 0.626959f, 0.626959f},
{0.297254f, 0.30829f, 0.306678f},

{0.992157f, 0.941176f, 0.807843f},
{0.393548f, 0.271906f, 0.166721f},

{0.774597f, 0.774597f, 0.774597f},
{0.256777f, 0.137622f, 0.086014f},

{0.628281f, 0.555802f, 0.366065f},
{0.508273f, 0.508273f, 0.508273f},

//1st - 3rd
{0.50f, 0.50f, 0.50f},
{0.50196078f, 0.50196078f, 0.50196078f},

{0.45f, 0.55f, 0.45f},
{0.7f, 0.6f, 0.6f},

{0.70f, 0.70f, 0.70f},
{0.60f, 0.60f, 0.50f},

//1st - 4rd
{0.4f, 0.4f, 0.4f},
{0.04f, 0.7f, 0.7f},

{0.04f, 0.7f, 0.04f},
{0.7f, 0.04f, 0.04f},

{0.7f, 0.7f, 0.7f},
{0.7f, 0.7f, 0.04f},
};

GLfloat Shininess[24] = { 
    0.6f * 128.0f,
    0.1f * 128.0f,
 
    0.3f * 128.0f,
    0.088f * 128.0f,
    
    0.6f * 128.0f,
    0.1f * 128.0f,
    
    0.21794872f *128.0f,
    0.2f * 128.0f,
    
    0.6f * 128.0f,
    0.1f * 128.0f,

    0.4f * 128.0f,
    0.4f * 128.0f,

    //1st - 3rd
    0.25f * 128.0f,
    0.25f * 128.0f,

    0.25f * 128.0f,
    0.25f * 128.0f,
    
    0.25f * 128.0f,
    0.25f * 128.0f,

    //1st - 4rd
    0.078125f * 128.0f,
    0.078125f * 128.0f,

    0.078125f * 128.0f,
    0.078125f * 128.0f,
    
    0.078125f * 128.0f,
    0.078125f * 128.0f,

};

struct Light
    {
        vmath::vec3 lAmbient;
        vmath::vec3 lDefues;
        vmath::vec3 lSpecular;
        vmath::vec4 lPosition;
    };
    
    float C_angle = 0.0f;
    
    
struct Light Lights[3];


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
    [window setTitle: @"DVM: MacOS Lights Shader Toggling."];
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
 @public
    CVDisplayLinkRef displayLink;
    
    enum
    {
        DVM_ATTTRIBUTE_POSITION = 0,
        DVM_ATTTRIBUTE_COLOR,
        DVM_ATTTRIBUTE_NORMAL,
        DVM_ATTTRIBUTE_TEXCORD,
    };

    
    /* Fragment shader */
    GLuint gshaderProgramObject_PF_DM;
    GLuint gVertexShaderObject_PF_DM;
    GLuint gFragmentShaderObject_PF_DM;
    
    /* Vertex shader */
    GLuint gshaderProgramObject_PV_DM;
    GLuint gVertexShaderObject_PV_DM;
    GLuint gFragmentShaderObject_PV_DM;

    /* Object variables  */
    GLuint g_Sphere_Vao_DM;
    GLuint g_Sphere_Vertex_Vbo_DM;
    GLuint g_Sphere_Normal_Vbo_DM;
    GLuint g_Sphere_Element_Vbo_DM;

    /*New For Sphere*/
    float sphere_vertices[1146];
    float sphere_normals[1146];
    float sphere_textures[764];
    unsigned short sphere_elements[2280];

    GLuint gNumVertices;
    GLuint gNumElements;

    /*splite 1.model view, 2.projection matrics */
    GLuint gMMatrixUniform__PV_DM;
    GLuint gVMatrixUniform__PV_DM;
    GLuint gPMatrixUniform__PV_DM;

    GLuint LKeyPressedUniform__PV_DM;/* key passing */

    GLuint LA_Uniform_PV_DM[3];/*Ambient*/
    GLuint LD_Uniform_PV_DM[3];/*Deffuse*/
    GLuint LS_Uniform_PV_DM[3];/*Specular*/
    GLuint LPosition_Uniform__PV_DM[3];/*Positon*/

    GLuint KA_Uniform_PV_DM; /*Material Ambient*/
    GLuint KD_Uniform_PV_DM; /*Material Deffuse*/
    GLuint KS_Uniform_PV_DM; /*Material Specular*/
    GLuint shininess_PV_DM;

    /*splite 1.model view, 2.projection matrics */
    GLuint gMMatrixUniform_DM;
    GLuint gVMatrixUniform_DM;
    GLuint gPMatrixUniform_DM;

    GLuint LKeyPressedUniform_DM;/* key passing */

    GLuint LA_Uniform_DM[3];/*Ambient*/
    GLuint LD_Uniform_DM[3];/*Deffuse*/
    GLuint LS_Uniform_DM[3];/*Specular*/
    GLuint LPosition_Uniform_DM[3];/*Positon*/

    GLuint KA_Uniform_DM; /*Material Ambient*/
    GLuint KD_Uniform_DM; /*Material Deffuse*/
    GLuint KS_Uniform_DM; /*Material Specular*/
    GLuint shininess_DM;

    vmath::mat4 gPerspectiveProjectMatix_DM;
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
        

        /*Lights properties initialization */

    
/*************************** Fragment  shader ****************************************************/

        /* Vertex Shader Object*/ 
           gVertexShaderObject_PF_DM = glCreateShader(GL_VERTEX_SHADER);

        const GLchar* vertexShaderSourceCode_PF =
            "#version 410 core  \n" \
            "in vec4 vPosition;" \
            "in vec3 vNormal;" \
            "uniform mat4 u_model_matrix;" \
            "uniform mat4 u_view_matrix;" \
            "uniform mat4 u_p_matrix;" \
            "out vec4 eye_cordinates;"  \
            "out vec3 transformed_normal;"  \
            "void main(void){\n" \
            "eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" \
            "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
            "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
            "}";

        glShaderSource(   gVertexShaderObject_PF_DM, 1, (const GLchar**)&vertexShaderSourceCode_PF, NULL);


        glCompileShader(   gVertexShaderObject_PF_DM);
        /*Here there should be vertex shader complication error Cheking */

        GLint iInfoLogLength = 0;
        GLint iShaderCompileStatus = 0;
        char* szInfoLog = NULL;
    

        glGetShaderiv(   gVertexShaderObject_PF_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(   gVertexShaderObject_PF_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(   gVertexShaderObject_PF_DM, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Vertex Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        /* Fragment Shader Object */
        gFragmentShaderObject_PF_DM = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar* fragmentShaderSourceCode_PF =
            "#version 410 core  \n" \
            "uniform int u_lKey;" \
            "uniform vec3 u_la[1];" \
            "uniform vec3 u_ld[1];" \
            "uniform vec3 u_ls[1];" \
            "uniform vec4 u_light_position[1];" \
            "uniform vec3 u_ka;" \
            "uniform vec3 u_kd;" \
            "uniform vec3 u_ks;" \
            "uniform float u_shininess;\n" \
            "in vec4 eye_cordinates;\n" \
            "in vec3 transformed_normal;\n"  \
            
            "out vec4 FragColor;\n" \
            "vec3 color_1;\n" \

            "vec3 fong_ads_light;\n" \

            "void main(void){\n" \
            "if(u_lKey == 1){\n" \
            "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \

            "vec3 ambient[1];\n" \
            "vec3 diffuse[1];\n" \
            "vec3 specular[1];\n" \

            "vec3 light_direction[1];\n" \
            "vec3 reflection_vector[1];\n" \
        
            "light_direction[0] = normalize(vec3(u_light_position[0] - eye_cordinates));\n"\  
            "reflection_vector[0] = reflect(-light_direction[0] , transformed_normal);\n" \
            "ambient[0] = u_la[0] * u_ka;\n" \
            "diffuse[0] = u_ld[0] * u_kd * max(dot(light_direction[0], transformed_normal), 0.0);\n" \
            "specular[0] = u_ls[0] * u_ks * pow( max(dot(reflection_vector[0], view_vector), 0.0), u_shininess);\n" \
            "color_1 = ambient[0] + diffuse[0] + specular[0];\n" \

            "fong_ads_light = color_1;\n"\

            "FragColor = vec4(fong_ads_light, 1.0f);\n"\
            "}\n" \
            "else{\n" \
            "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
            "}\n"
            "}";

        glShaderSource(gFragmentShaderObject_PF_DM, 1, (const GLchar**)&fragmentShaderSourceCode_PF, NULL);

        glCompileShader(gFragmentShaderObject_PF_DM);
        /*Here there should be Fragment shader complication error Cheking */

        glGetShaderiv(gFragmentShaderObject_PF_DM, GL_COMPILE_STATUS, &iShaderCompileStatus);
        if (iShaderCompileStatus == GL_FALSE)
        {
            glGetShaderiv(gFragmentShaderObject_PF_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);

            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gFragmentShaderObject_PF_DM, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Fragment Shader Compilation Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        gshaderProgramObject_PF_DM = glCreateProgram();

        glAttachShader(gshaderProgramObject_PF_DM,    gVertexShaderObject_PF_DM);
        glAttachShader(gshaderProgramObject_PF_DM, gFragmentShaderObject_PF_DM);

        glBindAttribLocation(gshaderProgramObject_PF_DM, DVM_ATTTRIBUTE_POSITION, "vPosition");
        glBindAttribLocation(gshaderProgramObject_PF_DM, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    
        glLinkProgram(gshaderProgramObject_PF_DM);

        GLint iShaderProgramLinkStatus = 0;
        glGetProgramiv(gshaderProgramObject_PF_DM, GL_LINK_STATUS, &iShaderProgramLinkStatus);
        if (iShaderProgramLinkStatus == GL_FALSE)
        {
            glGetProgramiv(gshaderProgramObject_PF_DM, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(gshaderProgramObject_PF_DM, iInfoLogLength, &written, szInfoLog);
                    fprintf(gpFile_DM, "Shader program Link Log : %s \n", szInfoLog);
                    free(szInfoLog);
                    [self release];//Myview release hoil
                    [NSApp terminate:self];//mag window
                }
            }
        }

        gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_model_matrix");
        gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_view_matrix");
        gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_p_matrix");

        LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_lKey");

        LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_la[0]");
        LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ld[0]");
        LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ls[0]");
        LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_light_position[0]");

        KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ka");
        KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_kd");
        KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_ks");
        shininess_DM = glGetUniformLocation(gshaderProgramObject_PF_DM, "u_shininess");

        /* New for Sphere */
        Sphere sphere;
        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        gNumVertices = sphere.getNumberOfSphereVertices();
        gNumElements = sphere.getNumberOfSphereElements();

            /* Sphere */
        glGenVertexArrays(1, &g_Sphere_Vao_DM);
        glBindVertexArray(g_Sphere_Vao_DM);
 
        /* For Vertices */
        glGenBuffers(1, &g_Sphere_Vertex_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Vertex_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /* For Normals */
        glGenBuffers(1, &g_Sphere_Normal_Vbo_DM);
        glBindBuffer(GL_ARRAY_BUFFER, g_Sphere_Normal_Vbo_DM);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
        glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        /* For elements */
        glGenBuffers(1, &g_Sphere_Element_Vbo_DM);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
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
        
        glUseProgram(gshaderProgramObject_PF_DM);

        NSRect rect = [self bounds];

        if (bLights)
        {
            glUniform1i(LKeyPressedUniform_DM, 1);
            
            if(X_Rotation)
            {
                
                Lights[0].lAmbient[0] = 0.0f;
                Lights[0].lAmbient[1] = 0.0f;
                Lights[0].lAmbient[2] = 0.0f;

                Lights[0].lDefues[0] = 1.0f;
                Lights[0].lDefues[1] = 1.0f;
                Lights[0].lDefues[2] = 1.0f;

                Lights[0].lSpecular[0] = 1.0f;
                Lights[0].lSpecular[1] = 1.0f;
                Lights[0].lSpecular[2] = 1.0f;

                Lights[0].lPosition[0] = 0.0f;
                Lights[0].lPosition[1] = 1.5f * sinf(C_angle);
                Lights[0].lPosition[2] = 2.5f * cosf(C_angle);
                Lights[0].lPosition[3] = 1.0f;
 
                glUniform3fv(LA_Uniform_DM[0], 1,  Lights[0].lAmbient);
                glUniform3fv(LD_Uniform_DM[0], 1,  Lights[0].lDefues);
                glUniform3fv(LS_Uniform_DM[0], 1,  Lights[0].lSpecular);
                glUniform4fv(LPosition_Uniform_DM[0], 1,  Lights[0].lPosition);
            }

            if(Y_Rotation)
            {
                
                Lights[0].lAmbient[0] = 0.0f;
                Lights[0].lAmbient[1] = 0.0f;
                Lights[0].lAmbient[2] = 0.0f;

                Lights[0].lDefues[0] = 1.0f;
                Lights[0].lDefues[1] = 1.0f;
                Lights[0].lDefues[2] = 1.0f;

                Lights[0].lSpecular[0] = 1.0f;
                Lights[0].lSpecular[1] = 1.0f;
                Lights[0].lSpecular[2] = 1.0f;

                Lights[0].lPosition[0] = 1.5f * sinf(C_angle);
                Lights[0].lPosition[1] = 0.0f;
                Lights[0].lPosition[2] = 2.5f * cosf(C_angle);
                Lights[0].lPosition[3] = 1.0f;

                glUniform3fv(LA_Uniform_DM[0], 1,  Lights[0].lAmbient);
                glUniform3fv(LD_Uniform_DM[0], 1,  Lights[0].lDefues);
                glUniform3fv(LS_Uniform_DM[0], 1,  Lights[0].lSpecular);
                glUniform4fv(LPosition_Uniform_DM[0], 1,  Lights[0].lPosition);
            }

            if(Z_Rotation)
            {
                Lights[0].lAmbient[0] = 0.0f;
                Lights[0].lAmbient[1] = 0.0f;
                Lights[0].lAmbient[2] = 0.0f;

                Lights[0].lDefues[0] = 1.0f;
                Lights[0].lDefues[1] = 1.0f;
                Lights[0].lDefues[2] = 1.0f;

                Lights[0].lSpecular[0] = 1.0f;
                Lights[0].lSpecular[1] = 1.0f;
                Lights[0].lSpecular[2] = 1.0f;

                Lights[0].lPosition[0] = 1.5f * sinf(C_angle);
                Lights[0].lPosition[1] = 2.5f * cosf(C_angle);
                Lights[0].lPosition[2] = 0.0f;
                Lights[0].lPosition[3] = 1.0f;
        
                glUniform3fv(LA_Uniform_DM[0], 1,  Lights[0].lAmbient);
                glUniform3fv(LD_Uniform_DM[0], 1,  Lights[0].lDefues);
                glUniform3fv(LS_Uniform_DM[0], 1,  Lights[0].lSpecular);
                glUniform4fv(LPosition_Uniform_DM[0], 1,  Lights[0].lPosition);
            }

        }
        else
        {
            glUniform1i(LKeyPressedUniform_DM, 0);

        }

            mat4 ModelMatrix;
            mat4 ViewMatrix;
            mat4 PMatrix;
            mat4 TranslateMatrix;
            
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {

            x = j * (rect.size.width / 6);
            y = i * (rect.size.width / 6);
            w = rect.size.width / 6;
            h = rect.size.height / 6;
            
            glViewport( x, y, w, h);
        
            ModelMatrix = mat4::identity();
            ViewMatrix = mat4::identity();
            TranslateMatrix = mat4::identity();
            PMatrix = mat4::identity();

            glUniform3fv(KA_Uniform_DM, 1,  material_Ambient[i * j]);
            glUniform3fv(KD_Uniform_DM, 1,  material_Diffuse[i * j]);
            glUniform3fv(KS_Uniform_DM, 1,  material_Specular[i * j]);
            glUniform1f(shininess_DM,Shininess[i * j]);

    
            TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
            
            ModelMatrix = TranslateMatrix;

            PMatrix = gPerspectiveProjectMatix_DM;

            glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
            glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
            glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

            glBindVertexArray(g_Sphere_Vao_DM);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Sphere_Element_Vbo_DM);
            glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

            glBindVertexArray(0);
            iterator_Color++;
            if(iterator_Color > 24)
            {
                iterator_Color = 0;
            }
            }
        }
            glUseProgram(0);


        //Update fuction call before CGFlush
        [self Update];
        
        CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);
        CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
        
    }
    -(void)Update
    {
        C_angle += 0.05f;
        if (C_angle >= 360.0f)
        {
            C_angle = 0.0f;
        }
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
                [[self window] toggleFullScreen:self];
                break;
            case 'Q':
            case 'q':
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
            case 'V':
            case 'v':
                if (per_Vertex == false)
                {
                    per_Vertex = true;
                    per_Fragment = false;
                }
                else
                {
                    per_Vertex = true;
                }
                break;
            case 'X':
            case 'x':
                X_Rotation = true;
                Y_Rotation = false;
                Z_Rotation = false;
            break;
            case 'Y':
            case 'y':
                X_Rotation = false;
                Y_Rotation = true;
                Z_Rotation = false;
            break;
            case 'Z':
            case 'z':
                X_Rotation = false;
                Y_Rotation = false;
                Z_Rotation = true;
            break;

            case 0x46:
            case 0x66:
                if (per_Fragment == false)
                {
                    per_Fragment = true;
                    per_Vertex = false;
                }
                else
                {
                    per_Fragment = true;
                }
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

        if (g_Sphere_Normal_Vbo_DM)
        {
            glDeleteBuffers(1, &g_Sphere_Normal_Vbo_DM);
            g_Sphere_Normal_Vbo_DM = 0;
        }

        if (gshaderProgramObject_PF_DM)
        {
            glUseProgram(gshaderProgramObject_PF_DM);
            GLsizei shaderCount;
            glGetProgramiv(gshaderProgramObject_PF_DM, GL_ATTACHED_SHADERS, &shaderCount);
            GLuint* pShader = NULL;
            pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
            if (pShader == NULL)
            {
                fprintf(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
            }
            glGetAttachedShaders(gshaderProgramObject_PF_DM, shaderCount, &shaderCount, pShader);
            for (GLsizei i = 0; i < shaderCount; i++)
            {
                glDetachShader(gshaderProgramObject_PF_DM, pShader[i]);
            }
            free(pShader);
            glDeleteShader(   gVertexShaderObject_PF_DM);
               gVertexShaderObject_PF_DM = 0;
            glDeleteShader(gFragmentShaderObject_PF_DM);
            gFragmentShaderObject_PF_DM = 0;
            glDeleteProgram(gshaderProgramObject_PF_DM);
            gshaderProgramObject_PF_DM = 0;
            glUseProgram(0);
        }

        if (gshaderProgramObject_PV_DM)
        {
            glUseProgram(gshaderProgramObject_PV_DM);
            GLsizei shaderCount;
            glGetProgramiv(gshaderProgramObject_PV_DM, GL_ATTACHED_SHADERS, &shaderCount);
            GLuint* pShader = NULL;
            pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
            if (pShader == NULL)
            {
                fprintf(gpFile_DM, "\nUnable to allocate memory to pShader.\n");
            }
            glGetAttachedShaders(gshaderProgramObject_PV_DM, shaderCount, &shaderCount, pShader);
            for (GLsizei i = 0; i < shaderCount; i++)
            {
                glDetachShader(gshaderProgramObject_PV_DM, pShader[i]);
            }
            free(pShader);
            glDeleteShader(   gVertexShaderObject_PV_DM);
               gVertexShaderObject_PV_DM = 0;
            glDeleteShader(gFragmentShaderObject_PV_DM);
            gFragmentShaderObject_PV_DM = 0;
            glDeleteProgram(gshaderProgramObject_PV_DM);
            gshaderProgramObject_PV_DM = 0;
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











