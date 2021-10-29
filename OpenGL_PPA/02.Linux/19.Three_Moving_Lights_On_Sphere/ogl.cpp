#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h> /* Memset */
#include<GL/glew.h>

#include"vmath.h"
#include"Sphere.h"

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

GLfloat L_angle_1 = 0.0f;
GLfloat L_angle_2 = 0.0f;
GLfloat L_angle_3 = 0.0f;

GLXContext gGLXContext;

int giWindowWidth = 800;
int giWindowHeight = 600;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gshaderProgramObject_PV;

GLuint gVertexShaderObject_PF;
GLuint gFragmentShaderObject_PF;
GLuint gshaderProgramObject_PF;


//Changes for PP
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

GLXFBConfig gGLXFBConfig;

GLfloat S_angle = 0.0f;



/*New For Sphere*/
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint gNumVertices;
GLuint gNumElements;

GLuint gVao_Sphere;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

/*splite 1.model view, 2.projection matrics */
GLuint gMMatrixUniform_DM;
GLuint gVMatrixUniform_DM;
GLuint gPMatrixUniform_DM;

GLuint LKeyPressedUniform_DM;/* key passing */

GLuint LA_Uniform_PV_DM[3];/*Ambient*/
GLuint LD_Uniform_PV_DM[3];/*Deffuse*/
GLuint LS_Uniform_PV_DM[3];/*Specular*/
GLuint LP_Uniform_PV_DM[3];/*Positon*/

GLuint KA_Uniform_PV_DM;/*Material Ambient*/
GLuint KD_Uniform_PV_DM;/*Material Deffuse*/
GLuint KS_Uniform_PV_DM;/*Material Specular*/
GLuint shininess_PV_DM;

GLuint LA_Uniform_PF_DM[3];/*Ambient*/
GLuint LD_Uniform_PF_DM[3];/*Deffuse*/
GLuint LS_Uniform_PF_DM[3];/*Specular*/
GLuint LP_Uniform_PF_DM[3];/*Positon*/

GLuint KA_Uniform_PF_DM;/*Material Ambient*/
GLuint KD_Uniform_PF_DM;/*Material Deffuse*/
GLuint KS_Uniform_PF_DM;/*Material Specular*/
GLuint shininess_PF_DM;

struct Light
{
    vec3 lAmbient;
    vec3 lDefues;
    vec3 lSpecular;
    vec4 lPosition;
};

struct Light Lights[3];

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 180.0f;

bool bAnimate = false;
bool bLights = false;

bool per_Vertex = true;
bool per_Fragment = false;

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
						
						if (per_Vertex == false)
                        {
                            per_Vertex = true;
                            per_Fragment = false;
                        }
                        else
                        {
                            per_Vertex = false;
                        }
					break;

                    case XK_V:
					case XK_v:
                    if (per_Fragment == false)
                        {
                            per_Fragment = true;
                            per_Vertex = false;
                        }
                        else
                        {
                            per_Fragment = false;
                        }
					break;
                    
                    case XK_Q:
                    case XK_q:
                        bDone = true;
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
	/*Replaced#pragma comment(lib,"Sphere.lib")
 1 by 8 */
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
						/* LAST PARAMETER COULD BE None or 0 (Here we are indicating that we don't want more space and componants you can 							take default)*/
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
	
	XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP : Three Lights On Sphere.");
		
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
    Lights[0].lPosition = { 0.0f, 0.0f, 0.0f, 1.0f};
    
    Lights[1].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[1].lDefues = { 0.0f, 1.0f, 0.0f };
    Lights[1].lSpecular = { 0.0f, 1.0f, 0.0f };
    Lights[1].lPosition = { -0.0f, 0.0f, 0.0f, 1.0f};
    
    Lights[2].lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights[2].lDefues = { 0.0f, 0.0f, 1.0f };
    Lights[2].lSpecular = { 0.0f, 0.0f, 1.0f };
    Lights[2].lPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
    

    /* Vertex Shader  */


    /* Vertex Shader Object */
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
        "#version 450 core\n" \
        "in vec4 vPosition;\n" \
        "in vec3 vNormal;\n" \
        "uniform mat4 u_model_matrix;\n" \
        "uniform mat4 u_view_matrix;\n" \
        "uniform mat4 u_p_matrix;\n" \
        "uniform int u_lKey;\n" \
        "uniform vec3 u_la[3];\n" \
        "uniform vec3 u_ld[3];\n" \
        "uniform vec3 u_ls[3];\n" \
        "uniform vec4 u_light_position[3];\n" \
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
        "for( int i = 0; i < 3; i++){\n"
        "vec3 ambient[3];\n" \
        "vec3 diffuse[3];\n" \
        "vec3 specular[3];\n" \
        "vec3 light_direction[3];\n" \
        "vec3 reflection_vector[3];\n" \
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

    gshaderProgramObject_PV = glCreateProgram();

    glAttachShader(gshaderProgramObject_PV, gVertexShaderObject);
    glAttachShader(gshaderProgramObject_PV, gFragmentShaderObject);

    glBindAttribLocation(gshaderProgramObject_PV, DVM_ATTTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(gshaderProgramObject_PV, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    
	glLinkProgram(gshaderProgramObject_PV);

    GLint iShaderProgramLinkStatus = 0;
    glGetProgramiv(gshaderProgramObject_PV, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gshaderProgramObject_PV, GL_INFO_LOG_LENGTH, &iInfoLogLength);
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

    gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_model_matrix");
    gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_p_matrix");

    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_lKey");

    LA_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_la[0]");
    LD_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_ld[0]");
    LS_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_ls[0]");
    LP_Uniform_PV_DM[0] = glGetUniformLocation(gshaderProgramObject_PV, "u_light_position[0]");
    
    LA_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_la[1]");
    LD_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_ld[1]");
    LS_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_ls[1]");
    LP_Uniform_PV_DM[1] = glGetUniformLocation(gshaderProgramObject_PV, "u_light_position[1]");

    
    LA_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_la[2]");
    LD_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_ld[2]");
    LS_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_ls[2]");
    LP_Uniform_PV_DM[2] = glGetUniformLocation(gshaderProgramObject_PV, "u_light_position[2]");

    KA_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_ka");
    KD_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_kd");
    KS_Uniform_PV_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_ks");
    shininess_PV_DM = glGetUniformLocation(gshaderProgramObject_PV, "u_shininess");


    


/*************************** Fragment  shader ****************************************************/

	/* Vertex Shader Object */
    gVertexShaderObject_PF = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCod_F =
        "#version 450 core\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_p_matrix;" \
        "out vec4 eye_cordinates;\n" \
        "out vec3 transformed_normal;\n"  \
        "void main(void){\n" \
        "eye_cordinates = normalize(vec4(u_view_matrix * u_model_matrix * vPosition));\n" \
        "transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        "gl_Position = u_p_matrix * u_view_matrix * u_model_matrix * vPosition;\n" \
        "}";

    glShaderSource(gVertexShaderObject_PF, 1, (const GLchar**)&vertexShaderSourceCod_F, NULL);


    glCompileShader(gVertexShaderObject_PF);
    /*Here there should be vertex shader complication error Cheking */

    iInfoLogLength = 0;
    iShaderCompileStatus = 0;
    szInfoLog = NULL;

    glGetShaderiv(gVertexShaderObject_PF, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        glGetShaderiv(gVertexShaderObject_PF, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gVertexShaderObject_PF, iInfoLogLength, &written, szInfoLog);
                printf("Vertex Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                uninitialize();
                exit(0);
            }
        }
    }

    /* Fragment Shader Object */
    gFragmentShaderObject_PF = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSourceCode_F =
        "#version 450 core\n" \
        "uniform int u_lKey;\n" \
        "uniform vec3 u_la[3];\n" \
        "uniform vec3 u_ld[3];\n" \
        "uniform vec3 u_ls[3];\n" \
        "uniform vec4 u_light_position[3];\n" \
        "uniform vec3 u_ka;\n" \
        "uniform vec3 u_kd;\n" \
        "uniform vec3 u_ks;\n" \
        "uniform float u_shininess;\n" \
        "in vec4 eye_cordinates;\n" \
        "in vec3 transformed_normal;\n"  \
        "out vec4 FragColor;\n" \
        "vec3 fong_ads_light[3];\n" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \
        "for( int i = 0; i < 3; i++){\n"
        "vec3 ambient[3];\n" \
        "vec3 diffuse[3];\n" \
        "vec3 specular[3];\n" \
        "vec3 light_direction[3];\n" \
        "vec3 reflection_vector[3];\n" \
        "light_direction[i] = normalize(vec3(u_light_position[i] - eye_cordinates));\n" \
        "reflection_vector[i] = reflect(-light_direction[i], transformed_normal);\n" \
        "ambient[i] = u_la[i] * u_ka;\n" \
        "diffuse[i] = u_ld[i] * u_kd * max(dot(light_direction[i], transformed_normal), 0.0);\n" \
        "specular[i] = u_ls[i] * u_ks * pow( max(dot(reflection_vector[i], view_vector), 0.0), u_shininess);\n" \
        "fong_ads_light[i] = ambient[i] + diffuse[i] + specular[i];\n" \
        "}\n" \
        "FragColor = vec4(vec3(fong_ads_light[0] + fong_ads_light[1] + fong_ads_light[2]), 1.0f);\n"\
        "}\n" \
        "else{\n" \
        "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"\
        "}\n"
        "}";


    glShaderSource(gFragmentShaderObject_PF, 1, (const GLchar**)&fragmentShaderSourceCode_F, NULL);

    glCompileShader(gFragmentShaderObject_PF);
    /*Here there should be Fragment shader complication error Cheking */

    glGetShaderiv(gFragmentShaderObject_PF, GL_COMPILE_STATUS, &iShaderCompileStatus);
    if (iShaderCompileStatus == GL_FALSE)
    {
        
        glGetShaderiv(gFragmentShaderObject_PF, GL_INFO_LOG_LENGTH, &iInfoLogLength);

        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PF, iInfoLogLength, &written, szInfoLog);
                printf("Fragment Shader Compilation Log : %s \n", szInfoLog);
                free(szInfoLog);
                XDestroyWindow(gpDisplay,gWindow);
            }
        }
    }

    gshaderProgramObject_PF = glCreateProgram();

    glAttachShader(gshaderProgramObject_PF, gVertexShaderObject_PF);
    glAttachShader(gshaderProgramObject_PF, gFragmentShaderObject_PF);

    glBindAttribLocation(gshaderProgramObject_PF, DVM_ATTTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(gshaderProgramObject_PF, DVM_ATTTRIBUTE_NORMAL, "vNormal");
    
	glLinkProgram(gshaderProgramObject_PF);

    iShaderProgramLinkStatus = 0;
    glGetProgramiv(gshaderProgramObject_PF, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(gshaderProgramObject_PF, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength + 1);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(gFragmentShaderObject_PF, iInfoLogLength, &written, szInfoLog);
                printf( "Shader program Link Log : %s \n", szInfoLog);
                free(szInfoLog);
                XDestroyWindow(gpDisplay,gWindow);
            }
        }
    }

    gMMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_model_matrix");
    gVMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_view_matrix");
    gPMatrixUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_p_matrix");

    LKeyPressedUniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_lKey");
    
    LA_Uniform_PF_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_la[0]");
    LD_Uniform_PF_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_ld[0]");
    LS_Uniform_PF_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_ls[0]");
    LP_Uniform_PF_DM[0] = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position[0]");

    LA_Uniform_PF_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_la[1]");
    LD_Uniform_PF_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_ld[1]");
    LS_Uniform_PF_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_ls[1]");
    LP_Uniform_PF_DM[1] = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position[1]");

    LA_Uniform_PF_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_la[2]");
    LD_Uniform_PF_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_ld[2]");
    LS_Uniform_PF_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_ls[2]");
    LP_Uniform_PF_DM[2] = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position[2]");

    KA_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ka");
    KD_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_kd");
    KS_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ks");
    shininess_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_shininess");


    /**/
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    gNumVertices = getNumberOfSphereVertices();
    gNumElements = getNumberOfSphereElements();

    glGenVertexArrays(1, &gVao_Sphere);
    glBindVertexArray(gVao_Sphere);

    /*For Vertex */
    glGenBuffers(1, &gVbo_sphere_position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL314
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    /*For Normals */
    glGenBuffers(1, &gVbo_sphere_normal);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*For elements */
    glGenBuffers(1, &gVbo_sphere_element);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

        if (per_Fragment)
    {
        glUseProgram(gshaderProgramObject_PV);
    }
    else if (per_Vertex)
    {
        glUseProgram(gshaderProgramObject_PF);
    }
    else {
        glUseProgram(gshaderProgramObject_PV);
    }

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        //glUniform1f(shininess_PV_DM, Shininess);
        glUniform1f(shininess_PF_DM, Shininess);

        Lights[0].lPosition[0] = 0.0f;
        Lights[0].lPosition[1] = 1.5f * sinf(L_angle_1);
        Lights[0].lPosition[2] = 2.5f * cosf(L_angle_1);
        Lights[0].lPosition[3] = 1.0f;
 
        glUniform3fv(LA_Uniform_PV_DM[0], 1, Lights[0].lAmbient);
        glUniform3fv(LD_Uniform_PV_DM[0], 1, Lights[0].lDefues);
        glUniform3fv(LS_Uniform_PV_DM[0], 1, Lights[0].lSpecular);
        glUniform4fv(LP_Uniform_PV_DM[0], 1, Lights[0].lPosition);

        glUniform3fv(LA_Uniform_PF_DM[0], 1, Lights[0].lAmbient);
        glUniform3fv(LD_Uniform_PF_DM[0], 1, Lights[0].lDefues);
        glUniform3fv(LS_Uniform_PF_DM[0], 1, Lights[0].lSpecular);
        glUniform4fv(LP_Uniform_PF_DM[0], 1, Lights[0].lPosition);
 
        Lights[1].lPosition[0] = 2.0f * sinf(L_angle_1);
        Lights[1].lPosition[1] = 0.0f;
        Lights[1].lPosition[2] = 2.0f * cosf(L_angle_1);
        Lights[1].lPosition[3] = 1.0f;

        glUniform3fv(LA_Uniform_PV_DM[1], 1, Lights[1].lAmbient);
        glUniform3fv(LD_Uniform_PV_DM[1], 1, Lights[1].lDefues);
        glUniform3fv(LS_Uniform_PV_DM[1], 1, Lights[1].lSpecular);
        glUniform4fv(LP_Uniform_PV_DM[1], 1, Lights[1].lPosition);

        glUniform3fv(LA_Uniform_PF_DM[1], 1, Lights[1].lAmbient);
        glUniform3fv(LD_Uniform_PF_DM[1], 1, Lights[1].lDefues);
        glUniform3fv(LS_Uniform_PF_DM[1], 1, Lights[1].lSpecular);
        glUniform4fv(LP_Uniform_PF_DM[1], 1, Lights[1].lPosition);
 
        Lights[2].lPosition[0] = 2.0f * sinf(L_angle_1);
        Lights[2].lPosition[1] = 2.0f * cosf(L_angle_1);
        Lights[2].lPosition[2] = 0.0f;
        Lights[2].lPosition[3] = 1.0f;

        glUniform3fv(LA_Uniform_PV_DM[2], 1, Lights[2].lAmbient);
        glUniform3fv(LD_Uniform_PV_DM[2], 1, Lights[2].lDefues);
        glUniform3fv(LS_Uniform_PV_DM[2], 1, Lights[2].lSpecular);
        glUniform4fv(LP_Uniform_PV_DM[2], 1, Lights[2].lPosition);

        glUniform3fv(LA_Uniform_PF_DM[2], 1, Lights[2].lAmbient);
        glUniform3fv(LD_Uniform_PF_DM[2], 1, Lights[2].lDefues);
        glUniform3fv(LS_Uniform_PF_DM[2], 1, Lights[2].lSpecular);
        glUniform4fv(LP_Uniform_PF_DM[2], 1, Lights[2].lPosition);
        
        /* material */
        glUniform3fv(KA_Uniform_PV_DM, 1, (GLfloat*)material_Ambient);
        glUniform3fv(KD_Uniform_PV_DM, 1, (GLfloat*)material_Diffuse);
        glUniform3fv(KS_Uniform_PV_DM, 1, (GLfloat*)material_Specular);

        glUniform3fv(KA_Uniform_PF_DM, 1, (GLfloat*)material_Ambient);
        glUniform3fv(KD_Uniform_PF_DM, 1, (GLfloat*)material_Diffuse);
        glUniform3fv(KS_Uniform_PF_DM, 1, (GLfloat*)material_Specular);

        Update();

    }
    else
    {
        glUniform1i(LKeyPressedUniform_DM, 0);
    }

    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 TranslateMatrix;
    mat4 PMatrix;

    ModelMatrix = mat4::identity();
    ViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    PMatrix = mat4::identity();

    TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
    ModelMatrix = TranslateMatrix;

    PMatrix = gPerspectiveProjectMatix;

    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

    glBindVertexArray(gVao_Sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);
    
    glUseProgram(0);

	glXSwapBuffers( gpDisplay, gWindow);
}

void Update()
{
    L_angle_1 += 0.05f;
    if (L_angle_1 > 360.0f)
    {
        L_angle_1 = 0.0f;
    }
    L_angle_2 += 0.05f;
    if (L_angle_2 > 360.0f)
    {
        L_angle_2 = 0.0f;
    }
    L_angle_3 += 0.05f;
    if (L_angle_3 > 360.0f)
    {
        L_angle_3 = 0.0f;
    }
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

	if (gVao_Sphere)
    {
        glDeleteVertexArrays(1, &gVao_Sphere);
        gVao_Sphere = 0;
    }

    if (gVbo_sphere_position)
    {
        glDeleteBuffers(1, &gVbo_sphere_position);
        gVbo_sphere_position = 0;
    }

    if (gVbo_sphere_normal)
    {
        glDeleteBuffers(1, &gVbo_sphere_normal);
        gVbo_sphere_normal = 0;
    }


	if (gshaderProgramObject_PF)
    {
        glUseProgram(gshaderProgramObject_PF);
        GLsizei shaderCount;
        glGetProgramiv(gshaderProgramObject_PF, GL_ATTACHED_SHADERS, &shaderCount);
        GLuint* pShader = NULL;
        pShader = (GLuint*)malloc(sizeof(GLsizei) * shaderCount);
        if (pShader == NULL)
        {
            printf("\nUnable to allocate memory to pShader.\n");
        }
        glGetAttachedShaders(gshaderProgramObject_PF, shaderCount, &shaderCount, pShader);
        for (GLsizei i = 0; i < shaderCount; i++)
        {
            glDetachShader(gshaderProgramObject_PF, pShader[i]);
        }
        free(pShader);

        glDeleteShader(gVertexShaderObject_PF);
        gVertexShaderObject_PF = 0;

        glDeleteShader(gFragmentShaderObject_PF);
        gFragmentShaderObject_PF = 0;

        glDeleteProgram(gshaderProgramObject_PF);
        gshaderProgramObject_PF = 0;

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

