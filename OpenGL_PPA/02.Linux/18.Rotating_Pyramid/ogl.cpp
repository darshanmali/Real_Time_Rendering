#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h> /* Memset */
#include<GL/glew.h>

#include"vmath.h"

#include<X11/Xlib.h>  /* similar windows.h */
#include<X11/Xutil.h> /* XVisual info */
#include<X11/XKBlib.h> /* KeyBoard */
#include<X11/keysym.h> /* key Symbols for taking Event */

#include<GL/gl.h>/* for openGL API */
#include<GL/glx.h>/* for GLX API */

using namespace std;
using namespace vmath;

enum
{
    DVM_ATTTRIBUTE_POSITION = 0,
    DVM_ATTTRIBUTE_COLOR,
    DVM_ATTTRIBUTE_NORMAL,
    DVM_ATTTRIBUTE_TEXCORD,
};


bool bFullscreen = true;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;

GLXContext gGLXContext;

int giWindowWidth = 800;
int giWindowHeight = 600;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gshaderProgramObject;


//Changes for PP
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

GLXFBConfig gGLXFBConfig;

GLfloat T_angle = 0.0f;

GLuint gVao_Pyramid;
GLuint gVbo_Pyramid_Position;
GLuint gVbo_Pyramid_Normal;


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

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 0.0f, 10.0f, 20.0f, 1.0f };

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 128.0f;

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


mat4 gPerspectiveProjectMatix;
    /* Local variables */
    void Initialize(void);
    void Resize(int , int);
    void Render(void);
    void uninitialize(void);


int main(void)
{
    /* Local Fuction Declaraton */
    void CreateWindow(void);
    void ToggleFullscreen(void);
    
    /* for game loop */
    bool bDone = false;

    int winWidth = giWindowWidth;
    int winHeight = giWindowHeight;

    //Graphics Context(Software HDC) 
    CreateWindow();

    Initialize();

    XEvent event;
    KeySym keysym;


    while(bDone == false)
    {
        while(XPending(gpDisplay))
        {
            XNextEvent(gpDisplay,&event);
            switch(event.type)
            {
                /* KeyBoard Messages */
                case KeyPress:
                keysym = XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
    
                switch(keysym)
                {
                    case XK_Escape:
                    bDone = true;
                    break;
                    case XK_A:
                    case XK_a:
                        if (bAnimate == true)
                        {
                            bAnimate = false;
                        }
                        else
                        {
                            bAnimate = true;
                        }
                    break;
                    case XK_L:
                    case XK_l:
                        if (bLights == true)
                        {
                            bLights = false;
                        }
                        else
                        {
                            bLights = true;
                        }
                    break;
                    case XK_F:
                    case XK_f:
                        if(bFullscreen == true)
                        {
                            bFullscreen = false;                        
                            ToggleFullscreen();
                            printf("inside bFullscreen == false ( if )\n");
                        }
                        else
                        {
                            bFullscreen = true;
                            ToggleFullscreen();
                            printf("inside bFullscreen == true (else)\n");
                        }
                    break;
                    default:
                    break;
                }
                break;
                /* Mouse Events */
                case ButtonPress:
                    switch(event.xbutton.button)
                    {
                        case 1:
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        default:
                            break;
                    }
                    break;
                /* WM_MOVE (when Window Moves)*/
                case MotionNotify:
                    break;
                /* WM_SIZE */
                case ConfigureNotify:
                    winWidth = event.xconfigure.width;
                    winHeight = event.xconfigure.height;
                    Resize(winWidth, winHeight);
                    break;
                /* WM_PAINT */
                case Expose:
    
                    break;
                /* WM_DISTROY */
                case DestroyNotify:
                    break;
                /* Close button */
                case 33:
                    bDone = true;
                    break;
                default:
                    break;
            }
        }
        /* render if window doesn't have any message (event) */
        Render();
    }
    
    uninitialize();
    return(0);
}

void CreateWindow(void)
{
    /* Local Fuction Declaraton */
    void uninitialize(void);
    
    /* Local variables */
    XSetWindowAttributes winAttribs;

    //OpenGL PP
    GLXFBConfig *pGLXFBConfig;
    GLXFBConfig best_GLXFBConfig;
    XVisualInfo *pTempXVisualInfo;
    int numFBConfig = 0;


    int defaultScreen;
    
    int stylemask;

    /* similar TO Pixel Format Attribute (windows)*/
                        /* WHICH TYPE OF COLOR BUFFER WE WANT */
    /*Replaced 1 by 8 */
    static int frameBufferAttributes[] = { 
                        //OpenGL PP
                        GLX_X_RENDERABLE,   True,
                        GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT, 
                        GLX_RENDER_TYPE,    GLX_RGBA_BIT,
                        GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
                        GLX_RED_SIZE,       8,
                        GLX_GREEN_SIZE,     8,
                        GLX_BLUE_SIZE,      8,
                        GLX_ALPHA_SIZE,     8,
                        /* V4L(Video for Linux, it is controled under Device Driver(VA(Video Acceleration ) Api / VDPAU API(Video Driver Presentation Unix API ) Componant)) Recomonds you 24 size*/
                        GLX_DEPTH_SIZE,     24,
                        //openGL pp
                        GLX_STENCIL_SIZE,   8,
                        GLX_DOUBLEBUFFER, True,
                        /* LAST PARAMETER COULD BE None or 0 (Here we are indicating that we don't want more space and componants you can                           take default)*/
                        None};


    /* Opening connection for XServer */ 
    gpDisplay = XOpenDisplay(NULL);
    if(gpDisplay == NULL)
    {
        printf("ERROR : Unable to Open X Display.\n Exitting Now...\n");
        uninitialize();
        exit(1);
    }


    defaultScreen = XDefaultScreen(gpDisplay);

    gpXVisualInfo = (XVisualInfo *)malloc(sizeof(XVisualInfo));
    if(gpXVisualInfo == NULL)
    {
        printf("ERROR : Unable To Allocate Memory for Visual Info .\n Exitting Now..\n");
        uninitialize();
        exit(1);
    }

    /* ******************************************OpenGL PP ******************************************************* */
    /* get the Frame buffer from Server 
        error who gave the size to the pGLXConfig  = gglxChoose FBConfig */

    pGLXFBConfig = glXChooseFBConfig(gpDisplay,//konta display
                                DefaultScreen(gpDisplay),//konti screen
                                frameBufferAttributes,//Buffer which we have created
                                &numFBConfig);//number of buffers 
    

    int bestFrameBufferConfig = -1, WorstFramebufferConfig = -1, bestnNumberOfSamples = -1, WorstNubmerOfSamples = 999;
    
     if(!pGLXFBConfig)
    {
        printf("ERROR : Unable TO Get GLXFBConfig. \n Exitting now...\n");
        uninitialize();
        exit(1);
    }
    else{
        for(int i = 0; i < numFBConfig; i++)
        {
            pTempXVisualInfo = glXGetVisualFromFBConfig(gpDisplay,pGLXFBConfig[i]);
            if(pTempXVisualInfo)
            {
                int samplesbuffers = 1, sample = 0;
                                                                 
                glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], 1, &samplesbuffers);
                glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], 4, &sample);
    
                if(bestFrameBufferConfig < 0 || samplesbuffers && sample > bestnNumberOfSamples)
                {
                    bestFrameBufferConfig = i;
                    bestnNumberOfSamples = sample;
                }
                if(WorstFramebufferConfig < 0 || !samplesbuffers || sample < WorstNubmerOfSamples )
                {
                    WorstFramebufferConfig = i;
                    WorstNubmerOfSamples = sample;
                }
                
            }
            XFree(pTempXVisualInfo);
        }   
    }
       
    printf("\nFound Number of FBConfig : %d\n", numFBConfig);

    best_GLXFBConfig = pGLXFBConfig[bestFrameBufferConfig];
    gGLXFBConfig = best_GLXFBConfig;

    if(!gGLXFBConfig)
    {
        printf("ERROR : Unable TO Get gGLXFBConfig. \n Exitting now...\n");
        uninitialize();
        exit(1);
    }
    XFree(pGLXFBConfig);

    gpXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, gGLXFBConfig);

    if(gpXVisualInfo == NULL)
    {
        printf("ERROR : Unable TO Get Visual. \n Exitting now...\n");
        uninitialize();
        exit(1);
    }

    //normal code
    
    winAttribs.border_pixel = 0;
    winAttribs.background_pixmap = 0;
    winAttribs.colormap = XCreateColormap(gpDisplay,
                    RootWindow(gpDisplay, gpXVisualInfo->screen),
                    gpXVisualInfo->visual,
                    AllocNone);
    gColormap = winAttribs.colormap;

    winAttribs.background_pixel = BlackPixel(gpDisplay,defaultScreen);

    winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
    
    stylemask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;

    gWindow = XCreateWindow(gpDisplay,
            RootWindow(gpDisplay,gpXVisualInfo->screen),
            0,
            0,
            giWindowWidth,
            giWindowHeight,
            0,
            gpXVisualInfo->depth,
            InputOutput,
            gpXVisualInfo->visual,
            stylemask,
            &winAttribs);
    if(!gWindow)
    {
        printf("ERROR : Failed To Create Main window.\n Exitting Now..\n");
        uninitialize();
        exit(1);
    }
    
    XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP : 3D Pyramid With Lights ");
        
    Atom windowManagerDelete = XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
    XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);
    
    XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
    Atom wm_state;
    Atom fullscreen;
    XEvent xev = {0};

    
    wm_state = XInternAtom(gpDisplay,"_NET_WM_STATE",false);
    memset(&xev, 0, sizeof(xev));

    xev.type = ClientMessage;
    xev.xclient.window = gWindow;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = bFullscreen ? 0:1;
    
    fullscreen = XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",false);
    xev.xclient.data.l[1] = fullscreen;

    XSendEvent(gpDisplay,
            RootWindow(gpDisplay,gpXVisualInfo->screen),
            false,
            StructureNotifyMask,
            &xev);
}

void Initialize(void)
{
    void Resize(int, int);

    //OpenGL PP

    //Get Address of fuction glXCreateContextAttribtss
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");

    //Changes
    const int attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 5,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None 
    };

    gGLXContext = glXCreateContextAttribsARB( gpDisplay, gGLXFBConfig, 0, GL_TRUE, attribs);

    if(gGLXContext)
    {
        printf("\nGot the Context of 4.5.\n");
    }

    if(!gGLXContext)
    {
        const int attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        None 
    };
        
    gGLXContext = glXCreateContextAttribsARB( gpDisplay, gGLXFBConfig, 0, GL_TRUE, attribs);

    printf("\nGot the Context of 1.0\n");

    }

    Bool bisdirectContext = glXIsDirect(gpDisplay, gGLXContext);
    if(bisdirectContext == true)
    {
        printf("context is Hardware rendering context.\n");
    }


    glXMakeCurrent(gpDisplay, gWindow, gGLXContext);

    GLenum glew_error = glewInit();

    /*Lights properties initialization */
    Lights[0].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[0].lDefues = { 1.0f, 0.0f, 0.0f };
    Lights[0].lSpecular = { 1.0f, 0.0f, 0.0f };
    Lights[0].lPosition = { 2.0f, 0.0f, 0.0f, 1.0f};
    
    Lights[1].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[1].lDefues = { 0.0f, 0.0f, 1.0f };
    Lights[1].lSpecular = { 0.0f, 0.0f, 1.0f };
    Lights[1].lPosition = { -2.0f, 0.0f, 0.0f, 1.0f};



    /* Vertex Shader Object */
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
         "#version 450 core\n" \
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
                printf("Vertex Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    /* Fragment Shader Object */
    gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSourceCode =
        "#version 450 core\n" \
        "in vec3 fong_ads_light;\n" \
        "out vec4 FragColor;\n" \
        "void main(void){\n" \
        "FragColor = vec4(fong_ads_light, 1.0f);\n"\
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
                printf("Fragment Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                XDestroyWindow(gpDisplay,gWindow);
            }
        }
    }

    gshaderProgramObject = glCreateProgram();

    glAttachShader(gshaderProgramObject, gVertexShaderObject);
    glAttachShader(gshaderProgramObject, gFragmentShaderObject);

    glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_NORMAL, "vNormal");

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
                printf( "Shader program Link Log : %s \n", szInfoLog);
                free(szInfoLog);
                XDestroyWindow(gpDisplay,gWindow);
            }
        }
    }

    gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_model_matrix");
    gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_p_matrix");

    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject, "u_lKey");

    LA_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_la[0]");
    LD_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_ld[0]");
    LS_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_ls[0]");
    LPosition_Uniform_DM[0] = glGetUniformLocation(gshaderProgramObject, "u_light_position[0]");
    
    LA_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_la[1]");
    LD_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_ld[1]");
    LS_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_ls[1]");
    LPosition_Uniform_DM[1] = glGetUniformLocation(gshaderProgramObject, "u_light_position[1]");

    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject, "u_shininess");

    /*New for Pyramid*/
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
       
   
    /*Pyramid Begen*/
    glGenVertexArrays(1, &gVao_Pyramid);
    glBindVertexArray(gVao_Pyramid);

    /*Position */
    glGenBuffers(1, &gVbo_Pyramid_Position);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Pyramid_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVertices), PyramidVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &gVbo_Pyramid_Normal);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Pyramid_Normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidNormals), PyramidNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gPerspectiveProjectMatix = mat4::identity();

    Resize( giWindowWidth, giWindowHeight);

}

void Resize(int Width, int Height)
{

    if(Height == 0)
    {
        Height = 1;
    }
    glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);
    gPerspectiveProjectMatix = vmath::perspective(44.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
        
}

void Render(void)
{
    void Update();

    /* Code Began */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject);

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        glUniform1f(shininess, Shininess);
        glUniform3fv(LA_Uniform_DM[0], 1, Lights[0].lAmbient);
        glUniform3fv(LD_Uniform_DM[0], 1, Lights[0].lDefues);
        glUniform3fv(LS_Uniform_DM[0], 1, Lights[0].lSpecular);
        glUniform4fv(LPosition_Uniform_DM[0], 1, Lights[0].lPosition);
        
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

    mat4 ModelMatrix = mat4::identity();
    mat4 ViewMatrix = mat4::identity();
    mat4 RotationMatrtix = mat4::identity();
    mat4 TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
    mat4 PMatrix = mat4::identity();
    
    TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
    RotationMatrtix = vmath::rotate(T_angle, 0.0f, 1.0f, 0.0f);
    ModelMatrix = TranslateMatrix * RotationMatrtix;

    PMatrix = gPerspectiveProjectMatix;

    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

    glBindVertexArray(gVao_Pyramid);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    glDrawArrays(GL_TRIANGLES, 6, 3);
    glDrawArrays(GL_TRIANGLES, 9, 3);

    glBindVertexArray(0);

    glUseProgram(0);

    Update();

    glXSwapBuffers( gpDisplay, gWindow);
}

void Update()
{
    if (T_angle >= 360.0f)
    {
        T_angle = 0.0f;
    }
    T_angle = T_angle + 0.25f;
}

void uninitialize(void)
{
    GLXContext currentGLXContext;
    
    currentGLXContext = glXGetCurrentContext();

    if( currentGLXContext == gGLXContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
        if(gGLXContext)
        {
            glXDestroyContext(gpDisplay, gGLXContext);
        }
    }

    if (gVao_Pyramid)
    {
        glDeleteVertexArrays(1, &gVao_Pyramid);
        gVao_Pyramid = 0;
    }

    if (gVbo_Pyramid_Position)
    {
        glDeleteBuffers(1, &gVbo_Pyramid_Position);
        gVbo_Pyramid_Position = 0;
    }

    if (gVbo_Pyramid_Normal)
    {
        glDeleteBuffers(1, &gVbo_Pyramid_Normal);
        gVbo_Pyramid_Normal = 0;
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
            printf("\nUnable to allocate memory to pShader.\n");
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

    if(gWindow)
    {
        XDestroyWindow(gpDisplay,gWindow);
    }
    if(gColormap)
    {
        XFreeColormap(gpDisplay,gColormap);
    }
    if(gpXVisualInfo)
    {
        free(gpXVisualInfo);
        gpXVisualInfo = NULL;
    }
    if(gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }
}
