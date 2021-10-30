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

GLuint LA_Uniform_PF_DM;/*Ambient*/
GLuint LD_Uniform_PF_DM;/*Deffuse*/
GLuint LS_Uniform_PF_DM;/*Specular*/
GLuint LP_Uniform_PF_DM;/*Positon*/

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

struct Light Lights;

GLfloat material_Ambient[3][24] = { 

 0.0215f, 0.1745f, 0.0215f,
0.135f, 0.2225f, 0.1575f,

0.05375f, 0.05f, 0.06625f,
0.25f, 0.20725f, 0.20725f,

0.1745f, 0.01175f, 0.01175f,
1.0f, 0.18725f, 0.1745f,

0.329412f, 0.223529f, 0.027451f,
0.2125f, 0.1275f, 0.054f,

0.25f, 0.25f, 0.25f,
0.19125f, 0.0735f, 0.0225f,

0.24725f, 0.1995f, 0.0745f,
0.19225f, 0.19225f, 0.19225f,

//1st - 3rd
0.0f, 0.0f, 0.0f,
0.0f,0.1f,0.06f,

0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f,

0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f,

//1st - 4rd
0.02f, 0.02f, 0.02f,
0.0f, 0.05f, 0.05f,

0.0f, 0.05f, 0.0f,
0.05f, 0.0f, 0.0f,

0.05f, 0.05f, 0.05f,
0.05f, 0.05f, 0.0f

};

GLfloat material_Diffuse[3][24] = { 
    0.07568f, 0.61424f, 0.07568f,
    0.54f, 0.89f, 0.63f, 
0.18275f, 0.17f, 0.22525f,
1.0f, 0.829f, 0.829f,

0.61424f, 0.04136f, 0.04136f,
0.396f, 0.74151f, 0.69102f, 

0.780392f, 0.568627f, 0.113725f,
0.714f, 0.4284f, 0.18144f,

0.4f, 0.4f, 0.4f,
0.7038f, 0.27048f, 0.0828f,

0.75164f, 0.60648f, 0.22648f,
0.50754f, 0.50754f,0.50754f,

//1st - 3rd
0.01f, 0.01f, 0.01f,
0.0f, 0.50980392f, 0.50980392f,

0.1f, 0.35f, 0.1f,
0.5f, 0.0f, 0.0f,

0.55f, 0.55f, 0.55f,
0.5f, 0.5f, 0.0f,

//1st - 4rd
0.01f, 0.01f, 0.01f,
0.4f, 0.5f, 0.5f,

0.4f, 0.5f, 0.4f,
0.5f, 0.4f, 0.4f,

0.5f, 0.5f, 0.5f,
0.5f, 0.5f, 0.4f,

};

GLfloat material_Specular[3][24] = {
    
 0.633f, 0.727811f, 0.633f,
 0.316228f, 0.316228f, 0.316228f,

0.332741f, 0.328634f, 0.346435f,
0.296648f, 0.296648f, 0.296648f,

0.727811f, 0.626959f, 0.626959f,
0.297254f, 0.30829f, 0.306678f,

0.992157f, 0.941176f, 0.807843f,
0.393548f, 0.271906f, 0.166721f,

0.774597f, 0.774597f, 0.774597f,
0.256777f, 0.137622f, 0.086014f,

0.628281f, 0.555802f, 0.366065f,
0.508273f, 0.508273f, 0.508273f,

//1st - 3rd
0.50f, 0.50f, 0.50f,
0.50196078f, 0.50196078f, 0.50196078f,

0.45f, 0.55f, 0.45f,
0.7f, 0.6f, 0.6f,

0.70f, 0.70f, 0.70f,
0.60f, 0.60f, 0.50f,

//1st - 4rd
0.4f, 0.4f, 0.4f,
0.04f, 0.7f, 0.7f,

0.04f, 0.7f, 0.04f,
0.7f, 0.04f, 0.04f,

0.7f, 0.7f, 0.7f,
0.7f, 0.7f, 0.04f

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

bool bAnimate = false;
bool bLights = false;

bool per_Vertex = true;
bool per_Fragment = false;

bool X_Rotation = true;
bool Y_Rotation = false;
bool Z_Rotation = false;

GLuint count = 0;

mat4 gPerspectiveProjectMatix;
/* Local variables */
void Initialize(void);
void Resize(int , int);
void Render(void);
void uninitialize(void);

int ig_Width;
int ig_Hight;

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
                    case XK_X:
                    case XK_x:
                        X_Rotation = true;
                        Y_Rotation = false;
                        Z_Rotation = false;
                    break;

                    case XK_Y:
					case XK_y:
                        X_Rotation = false;
                        Y_Rotation = true;
                        Z_Rotation = false;
					break;
                    
                    case XK_Z:
                    case XK_z:
                        X_Rotation = false;
                        Y_Rotation = false;
                        Z_Rotation = true;
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
					ig_Width = winWidth = event.xconfigure.width;
					ig_Hight = winHeight = event.xconfigure.height;
                     
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
	
	XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP : 24 Sphere.");
		
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
    Lights.lAmbient = { 0.0f, 0.0f, 0.0f };
    Lights.lDefues = { 1.0f, 1.0f, 1.0f };
    Lights.lSpecular = { 1.0f, 1.0f, 1.0f };
    Lights.lPosition = { -1.0f, 0.0f, 0.0f, 0.0f};
    
    /* Vertex Shader  */

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

    GLint iInfoLogLength = 0;
    GLint iShaderCompileStatus = 0;
    char* szInfoLog = NULL;

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

    int iShaderProgramLinkStatus = 0;
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
    
    LA_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_la");
    LD_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ld");
    LS_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_ls");
    LP_Uniform_PF_DM = glGetUniformLocation(gshaderProgramObject_PF, "u_light_position");

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

    glUseProgram(gshaderProgramObject_PF);

    mat4 ModelMatrix;
    mat4 ViewMatrix;
    mat4 TranslateMatrix;
    mat4 ScaleMatrix;
    mat4 PMatrix;

    ModelMatrix = mat4::identity();
    ViewMatrix = mat4::identity();
    ScaleMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    PMatrix = mat4::identity();
        GLfloat y = 0.0f,
            x = 0.0f;

    TranslateMatrix = vmath::translate(x, y, -1.0f);
    ModelMatrix = TranslateMatrix;

    PMatrix = gPerspectiveProjectMatix;

    glUniformMatrix4fv(gPMatrixUniform_DM, 1, GL_FALSE, PMatrix);
    glUniformMatrix4fv(gVMatrixUniform_DM, 1, GL_FALSE, ViewMatrix);
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

    if (bLights)
    {
        glUniform1i(LKeyPressedUniform_DM, 1);
        

        if (X_Rotation)
        {
            Lights.lPosition[0] = 0.0f;
            Lights.lPosition[1] = 1.1f * sinf(L_angle_1);
            Lights.lPosition[2] = 1.1f * cosf(L_angle_1);
            Lights.lPosition[3] = 0.0f;
        }
        else if (Y_Rotation)
        {
            Lights.lPosition[0] = 1.25f * sinf(L_angle_1); 
            Lights.lPosition[1] = 0.0f;
            Lights.lPosition[2] = 1.25f * cosf(L_angle_1);
            Lights.lPosition[3] = 1.25f;
        }
        else if (Z_Rotation)
        {
            Lights.lPosition[0] = 1.25f * sinf(L_angle_1); 
            Lights.lPosition[1] = 1.25f * cosf(L_angle_1);
            Lights.lPosition[2] = 0.0f;
            Lights.lPosition[3] = 1.25f;
        }

        glUniform3fv(LA_Uniform_PF_DM, 1, Lights.lAmbient);
        glUniform3fv(LD_Uniform_PF_DM, 1, Lights.lDefues);
        glUniform3fv(LS_Uniform_PF_DM, 1, Lights.lSpecular);
        glUniform4fv(LP_Uniform_PF_DM, 1, Lights.lPosition);
 
        Update();
    }
    else
    {
        glUniform1i(LKeyPressedUniform_DM, 0);
    }

    glViewport(0, 0, (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);
    glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

    ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
    ModelMatrix = TranslateMatrix * ScaleMatrix;
    glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

    glBindVertexArray(gVao_Sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);
    count++;

    //1
   glViewport(0, (GLsizei)ig_Hight / 6, (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);
   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


     //2
   glViewport(0, (GLsizei)ig_Hight / 6 + (GLsizei)ig_Hight / 6, (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);
   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //3
   glViewport(0, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);
   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //4
   glViewport(0, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);
   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //5
   glViewport(0, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //6
   glViewport((GLsizei)ig_Width / 4, 0, (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;
   

   //7
   glViewport((GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //8
   glViewport((GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;
   
   
   //9
   glViewport((GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //10
   glViewport((GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //11
   glViewport((GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //12
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, 0, (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //13
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //14
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;
   

   //15
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //16
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //17
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //18
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, 0, (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);
   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //19
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //20
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //21
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;


   //22
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

   //23
   glViewport((GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4 + (GLsizei)ig_Width / 4, ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6) + ((GLsizei)ig_Hight / 6), (GLsizei)ig_Width / 6, (GLsizei)ig_Hight / 6);

   glUniform1f(shininess_PF_DM, Shininess[0]);

   glUniform3f(KA_Uniform_PF_DM, material_Ambient[0][count], material_Ambient[1][count], material_Ambient[2][count]);
   glUniform3f(KD_Uniform_PF_DM, material_Diffuse[0][count], material_Diffuse[1][count], material_Diffuse[2][count]);
   glUniform3f(KS_Uniform_PF_DM, material_Specular[0][count], material_Specular[1][count], material_Specular[2][count]);

   ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
   TranslateMatrix = vmath::translate(x, y, -1.0f);
   ModelMatrix = TranslateMatrix * ScaleMatrix;
   glUniformMatrix4fv(gMMatrixUniform_DM, 1, GL_FALSE, ModelMatrix);

   glBindVertexArray(gVao_Sphere);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
   glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(0);
   count++;

  
    
    glUseProgram(0);

    
    if (y > 1.0f)
   {
       x -= 0.8f;
       y = -1.0f;
   }
   
	glXSwapBuffers( gpDisplay, gWindow);
    count = 0;

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

