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

GLXContext gGLXContext;

int giWindowWidth = 800;
int giWindowHeight = 600;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gshaderProgramObject;

GLuint gVertexShaderObject_PF;
GLuint gFragmentShaderObject_PF;
GLuint gshaderProgramObject_PF;


//Changes for PP
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

glXCreateContextAttribsARBPro"custom"c glXCreateContextAttribsARB = NULL;

GLXFBConfig gGLXFBConfig;

GLfloat S_angle = 0.0f;

bool bAnimate = false;
bool bLights = false;

bool per_Vertex = true;
bool per_Fragment = false;

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

GLuint LA_Uniform_PF_DM;/*Ambient*/
GLuint LD_Uniform_PF_DM;/*Deffuse*/
GLuint LS_Uniform_PF_DM;/*Specular*/
GLuint LPosition_Uniform_PF_DM;/*Positon*/

GLuint KA_Uniform_PF_DM;/*Material Ambient*/
GLuint KD_Uniform_PF_DM;/*Material Deffuse*/
GLuint KS_Uniform_PF_DM;/*Material Specular*/
GLuint shininess_PF_DM;

GLuint LA_Uniform_DM;/*Ambient*/
GLuint LD_Uniform_DM;/*Deffuse*/
GLuint LS_Uniform_DM;/*Specular*/
GLuint LPosition_Uniform_DM;/*Positon*/

GLuint KA_Uniform_DM;/*Material Ambient*/
GLuint KD_Uniform_DM;/*Material Deffuse*/
GLuint KS_Uniform_DM;/*Material Specular*/
GLuint shininess;

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f };
GLfloat lDefues[] = { 1.0f, 1.0f, 1.0f };
GLfloat lSpecular[] = { 1.0f, 1.0f, 1.0f };
GLfloat lPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f };
GLfloat Shininess = 50.0f;

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

                    case XK_V:
					case XK_v:
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
	
	XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP : Light Toggling.");
		
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

/* Vertex Shader  */

/* Vertex Shader Object */
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
        "#version 450 core\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_p_matrix;" \
        "uniform int u_lKey;" \
        "uniform vec3 u_la;" \
        "uniform vec3 u_ld;" \
        "uniform vec3 u_ls;" \
        "uniform vec3 u_ka;" \
        "uniform vec3 u_kd;" \
        "uniform vec3 u_ks;" \
        "uniform vec4 u_light_position;" \
        "uniform float u_shininess;\n" \
        "out vec3 fong_ads_light;\n" \
        "void main(void){\n" \
        "if(u_lKey == 1){\n" \
        "vec4 eye_cordinates = vec4(u_view_matrix * u_model_matrix * vPosition);\n" \
        "vec3 transformed_normal = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);\n" \
        "vec3 light_direction = normalize(vec3(u_light_position - eye_cordinates));\n" \
        "vec3 reflection_vector = reflect(-light_direction , transformed_normal);\n" \
        "vec3 view_vector = normalize(-eye_cordinates.xyz); \n" \
        "vec3 ambient = u_la * u_ka;\n" \
        "vec3 diffuse = u_ld * u_kd * max(dot(light_direction, transformed_normal), 0.0);\n" \
        "vec3 specular = u_ls * u_ks * pow( max(dot(reflection_vector, view_vector), 0.0), u_shininess);\n" \
        "fong_ads_light = ambient + diffuse + specular;\n" \
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

    LA_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_la");
    LD_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_ld");
    LS_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_ls");
    LPosition_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_light_position");

    KA_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_ka");
    KD_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_kd");
    KS_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_ks");
    shininess_PF_DM = glGetUniformLocation(gshaderProgramObject, "u_shininess");


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
    
    LA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_la");
    LD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ld");
    LS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ls");
    LPosition_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position");

    KA_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ka");
    KD_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_kd");
    KS_Uniform_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ks");
    shininess = glGetUniformLocation(gshaderProgramObject_PF, "u_shininess");


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
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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

    
     if (per_Vertex)
    {
        glUseProgram(gshaderProgramObject);
        if (bLights)
        {
            glUniform1i(LKeyPressedUniform_DM, 1);
            glUniform1f(shininess_PF_DM, Shininess);
            glUniform3fv(LA_Uniform_PF_DM, 1, (GLfloat*)lAmbient);
            glUniform3fv(LD_Uniform_PF_DM, 1, (GLfloat*)lDefues);
            glUniform3fv(LS_Uniform_PF_DM, 1, (GLfloat*)lSpecular);
            glUniform3fv(KA_Uniform_PF_DM, 1, (GLfloat*)material_Ambient);
            glUniform3fv(KD_Uniform_PF_DM, 1, (GLfloat*)material_Diffuse);
            glUniform3fv(KS_Uniform_PF_DM, 1, (GLfloat*)material_Specular);
            glUniform4fv(LPosition_Uniform_PF_DM, 1, (GLfloat*)lPosition);
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
        RotationMatrtix = vmath::rotate(S_angle, 0.0f, 1.0f, 0.0f);
        ModelMatrix = TranslateMatrix * RotationMatrtix;

        PMatrix = gPerspectiveProjectMatix;

        glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

        glBindVertexArray(gVao_Sphere);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
        glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);

        glUseProgram(0);
    }

    if (per_Fragment)
    {
        glUseProgram(gshaderProgramObject_PF);

        if (bLights)
        {

            glUniform1i(LKeyPressedUniform_DM, 1);
            glUniform1f(shininess_PF_DM, Shininess);
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

        TranslateMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
        RotationMatrtix = vmath::rotate(S_angle, 0.0f, 1.0f, 0.0f);
        ModelMatrix = TranslateMatrix * RotationMatrtix;

        PMatrix = gPerspectiveProjectMatix;

        glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
        glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

        glBindVertexArray(gVao_Sphere);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
        glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);

        glUseProgram(0);
    }


    
    if (bAnimate)
    {
        Update();
    }

	glXSwapBuffers( gpDisplay, gWindow);
}

void Update()
{
    if (S_angle >= 360.0f)
    {
        S_angle = 0.0f;
    }
    S_angle = S_angle + 0.25f;
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

