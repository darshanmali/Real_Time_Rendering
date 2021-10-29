#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h> /* Memset */
#include<GL/glew.h>
#include<math.h>

#include"vmath.h"

#include<X11/Xlib.h>  /* similar windows.h */
#include<X11/Xutil.h> /* XVisual info */
#include<X11/XKBlib.h> /* KeyBoard */
#include<X11/keysym.h> /* key Symbols for taking Event */

#include<GL/gl.h>/* for openGL API */
#include<GL/glx.h>/* for GLX API */

using namespace std;
using namespace vmath;
#define PI 3.141592653589793238

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

GLfloat angle = 0.0f;

GLuint gVao;
GLuint gVbo_Position;
GLuint gVbo_Color;
GLuint gMVPMatrixUniform;

//Square
GLuint S_gVao;
GLuint S_gVbo_Position;
GLuint S_gVbo_Color;

//Triangle
GLuint T_gVao;
GLuint T_gVbo_Position;
GLuint T_gVbo_Color;

//circle
GLuint C_gVao;
GLuint C_gVbo_Position;
GLuint C_gVbo_Color;

GLfloat vertexArr[56560];
int v = 0;
bool state = false;

const int circle_Points = 1000;
int Color_flag = 0;
GLfloat T_b = 0.0f, T_h = 0.0f,T_area = 0.0f;
int i = 1;

void Vertexcall(GLfloat, GLfloat, GLfloat, GLfloat);
void calculate_Distance(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat *);


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
	
	XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP :Graph  With All Shapes ");
		
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

	GLfloat x_all = 0.0f, y_all = 0.0f;
    GLfloat s = 0.0f, p = 0.0f, in_r = 0.0f, ab = 0.0f, bc = 0.0f, ca = 0.0f;
   
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

	/* Vertex Shader Object */
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
        "#version 450 core\n" \
        "in vec4 vPosition;" \
        "in vec4 vColor;" \
        "uniform mat4 u_mvp_matrix;" \
        "out vec4 out_Color;" \
        "void main(void){\n" \
        "gl_Position = u_mvp_matrix * vPosition;" \
        "out_Color = vColor;" \
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
        "in vec4 out_Color;" \
        "out vec4 FragColor;" \
        "void main(void){\n" \
    	"FragColor = out_Color;\n" \
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
	glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_COLOR, "vColor");
    
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

    gMVPMatrixUniform = glGetUniformLocation(gshaderProgramObject, "u_mvp_matrix");

   
    const GLfloat GraphVertices[] =
    {
        1.0f, 1.00f, 0.0f,
        -1.0f, 1.00f, 0.0f,
        1.0f, 0.95f, 0.0f,
        -1.0f, 0.95f, 0.0f,
        1.0f, 0.90f, 0.0f,
        -1.0f, 0.90f, 0.0f,
        1.0f, 0.85f, 0.0f,
        -1.0f, 0.85f, 0.0f,
        1.0f, 0.80f, 0.0f,
        -1.0f, 0.80f, 0.0f,
        1.0f, 0.75f, 0.0f,
        -1.0f, 0.75f, 0.0f,
        1.0f, 0.70f, 0.0f,
        -1.0f, 0.70f, 0.0f,
        1.0f, 0.65f, 0.0f,
        -1.0f, 0.65f, 0.0f,
        1.0f, 0.60f, 0.0f,
        -1.0f, 0.60f, 0.0f,
        1.0f, 0.55f, 0.0f,
        -1.0f, 0.55f, 0.0f,
        1.0f, 0.50f, 0.0f,
        -1.0f, 0.50f, 0.0f,
        1.0f, 0.45f, 0.0f,
        -1.0f, 0.45f, 0.0f,
        1.0f, 0.40f, 0.0f,
        -1.0f, 0.40f, 0.0f,
        1.0f, 0.35f, 0.0f,
        -1.0f, 0.35f, 0.0f,
        1.0f, 0.30f, 0.0f,
        -1.0f, 0.30f, 0.0f,
        1.0f, 0.25f, 0.0f,
        -1.0f, 0.25f, 0.0f,
        1.0f, 0.20f, 0.0f,
        -1.0f, 0.20f, 0.0f,
        1.0f, 0.15f, 0.0f,
        -1.0f, 0.15f, 0.0f,
        1.0f, 0.10f, 0.0f,
        -1.0f, 0.10f, 0.0f,
        1.0f, 0.05f, 0.0f,
        -1.0f, 0.05f, 0.0f,
        1.0f, -0.00f, 0.0f,
        -1.0f, -0.00f, 0.0f,
        1.0f, -0.05f, 0.0f,
        -1.0f, -0.05f, 0.0f,
        1.0f, -0.10f, 0.0f,
        -1.0f, -0.10f, 0.0f,
        1.0f, -0.15f, 0.0f,
        -1.0f, -0.15f, 0.0f,
        1.0f, -0.20f, 0.0f,
        -1.0f, -0.20f, 0.0f,
        1.0f, -0.25f, 0.0f,
        -1.0f, -0.25f, 0.0f,
        1.0f, -0.30f, 0.0f,
        -1.0f, -0.30f, 0.0f,
        1.0f, -0.35f, 0.0f,
        -1.0f, -0.35f, 0.0f,
        1.0f, -0.40f, 0.0f,
        -1.0f, -0.40f, 0.0f,
        1.0f, -0.45f, 0.0f,
        -1.0f, -0.45f, 0.0f,
        1.0f, -0.50f, 0.0f,
        -1.0f, -0.50f, 0.0f,
        1.0f, -0.55f, 0.0f,
        -1.0f, -0.55f, 0.0f,
        1.0f, -0.60f, 0.0f,
        -1.0f, -0.60f, 0.0f,
        1.0f, -0.65f, 0.0f,
        -1.0f, -0.65f, 0.0f,
        1.0f, -0.70f, 0.0f,
        -1.0f, -0.70f, 0.0f,
        1.0f, -0.75f, 0.0f,
        -1.0f, -0.75f, 0.0f,
        1.0f, -0.80f, 0.0f,
        -1.0f, -0.80f, 0.0f,
        1.0f, -0.85f, 0.0f,
        -1.0f, -0.85f, 0.0f,
        1.0f, -0.90f, 0.0f,
        -1.0f, -0.90f, 0.0f,
        1.0f, -0.95f, 0.0f,
        -1.0f, -0.95f, 0.0f,
        1.0f, -1.00f, 0.0f,
        -1.0f, -1.00f, 0.0f

    };

    GLfloat Graph_Color[82][3];
    for (int i = 0; i < 82; i++)
    {
        Graph_Color[i][0] = 0.0f;
        Graph_Color[i][1] = 0.0f;
        Graph_Color[i][2] = 1.0f;

    }
    

    glGenVertexArrays(1, &gVao);
    glBindVertexArray(gVao);

    glGenBuffers(1, &gVbo_Position);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GraphVertices), GraphVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &gVbo_Color);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graph_Color), Graph_Color, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*New for Circle */


	//calculating the Distance of lines x1,x2,y1,y2
    //Left side of triangle
    calculate_Distance(-1.0f, 0.0f, -1.0f, 1.0f, &ab);

    //Right side of triangle
    calculate_Distance(0.0f, 1.0f, 1.0f, -1.0f, &bc);

    //Bottom side of triangle
    calculate_Distance(1.0f, -1.0f, -1.0f, -1.0f, &ca);

    //I Fount the Perimter 
    p = ab + bc + ca;

    //I Fount the Semiperimeter
    s = p / 2;
   
    //by heron's Formula for calculating the area of triangle 
    T_area = sqrtf(s * (s - ab) * (s - bc) * (s - ca));

    // I Tried to find the radious of circle 
    in_r = (T_area * 3) / p;

    //finding the center of trangle 
    x_all = ((ab * (-1)) + (ca * (0)) + (ab * 1)) / p;
    y_all = ((ab * (-1)) + (ca * (1)) + (ab * -1)) / p;

	if (i == 1)
    {
        printf("Triangle's Area = %f\nTriangle Center Co-Ordinates(x,Y) :\t (%f , %f)\n", T_area, x_all, y_all);

        printf("\nperimeter = %f\nThree sides of Triangle : ab = %f \t bc = %f \t ca = %f\n", p, ab, bc, ca);

        printf("\nSemi-Perimeter = %f\nInCircle's redios : %f\n", s, in_r);
        
		//fprintf_s(gpFile_DM, "\nHeight = %f",T_h);
        i++;
    }

	if(state == false)
	{
	
	for (GLfloat angle = 0.0f; angle <= 2 * PI; angle = angle + 0.001f)
    {
		
		vertexArr[v] = (GLfloat)cos(angle) * 1.0;
		v++;
		vertexArr[v] = (GLfloat)sin(angle) * 1.0;
		v++;
		vertexArr[v] = 0.0f;
		v++;

    }
		printf("Iteretion are %d\n",v);
		printf("Iteretion are %f \t %f \t %f\n",vertexArr[314], vertexArr[1],vertexArr[214]);
		state = true;
	}
   

    const GLfloat CircleColor[] =
    {
        1.0f, 1.0f, 1.0f,

    };

    glGenVertexArrays(1, &C_gVao);
    glBindVertexArray(C_gVao);

    glGenBuffers(1, &C_gVbo_Position);
    glBindBuffer(GL_ARRAY_BUFFER, C_gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArr), vertexArr, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &C_gVbo_Color);
    glBindBuffer(GL_ARRAY_BUFFER, C_gVbo_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CircleColor), CircleColor, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*New for traingle */
    const GLfloat triangleVertices[] =
    {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    const GLfloat triangleColor[] =
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    glGenVertexArrays(1, &T_gVao);
    glBindVertexArray(T_gVao);

    glGenBuffers(1, &T_gVbo_Position);
    glBindBuffer(GL_ARRAY_BUFFER, T_gVbo_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &T_gVbo_Color);
    glBindBuffer(GL_ARRAY_BUFFER, T_gVbo_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

	/* Depth Enabling Fuctions */
        /*for getting anti_Allies */
	glShadeModel(GL_SMOOTH);
	/* Which are less than 1 will be removed */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

	

	/* Code Began */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject);

	mat4 modelViewMatrix = mat4::identity();
    mat4 TranslateMatrix = vmath::translate( 0.0f, 0.0f, -3.0f);
    mat4 RotateMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
    mat4 modelViewProjectionMatrix = mat4::identity();

    modelViewMatrix = TranslateMatrix ;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
   
    glBindVertexArray(gVao);

    for (int i = 0; i <= 80; i += 2)
    {
        glDrawArrays(GL_LINES, i, 2);
    }

    glBindVertexArray(0);

   modelViewMatrix = mat4::identity();
   TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
   RotateMatrix = vmath::rotate(90.0f, 0.0f, 0.0f, 1.0f);
   modelViewProjectionMatrix = mat4::identity();

    modelViewMatrix = TranslateMatrix * RotateMatrix;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glBindVertexArray(gVao);

    for (int i = 0; i <= 80; i += 2)
    {
        glDrawArrays(GL_LINES, i, 2);
    }

    glBindVertexArray(0);

#pragma region TRIANGLE
    modelViewMatrix = mat4::identity();
    TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    modelViewProjectionMatrix = mat4::identity();

    modelViewMatrix = TranslateMatrix ;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glBindVertexArray(T_gVao);

    glDrawArrays(GL_TRIANGLES, 0, 9);
    
    glBindVertexArray(0);

#pragma endregion TRIANGLE



#pragma region CIRCLE

     
	modelViewMatrix = mat4::identity();
    TranslateMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    modelViewProjectionMatrix = mat4::identity();

    modelViewMatrix = TranslateMatrix ;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glBindVertexArray(C_gVao);

    glDrawArrays(GL_LINE_STRIP, 0, 6280);
    
    glBindVertexArray(0);

#pragma endregion TRIANGLE



    glUseProgram(0);

	glXSwapBuffers( gpDisplay, gWindow);
}


void calculate_Distance(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat* side)
{
    *side = sqrtf((((x2) - (x1)) * (((x2) - (x1)))) + (((y2) - (y1)) * ((y2) - (y1))));
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
	if (gVao)
    {
        glDeleteVertexArrays(1, &gVao);
        gVao = 0;
    }

    if (gVbo_Position)
    {
        glDeleteBuffers(1, &gVbo_Position);
        gVbo_Position = 0;
    }

	if (gVbo_Color)
    {
        glDeleteBuffers(1, &gVbo_Color);
        gVbo_Color = 0;
    }

	if (S_gVao)
    {
        glDeleteVertexArrays(1, &S_gVao);
        S_gVao = 0;
    }

    if (S_gVbo_Position)
    {
        glDeleteBuffers(1,&S_gVbo_Position);
        S_gVbo_Position = 0;
    }
     if (T_gVao)
    {
        glDeleteVertexArrays(1, &T_gVao);
        T_gVao = 0;
    }

    if (T_gVbo_Position)
    {
        glDeleteBuffers(1, &T_gVbo_Position);
        T_gVbo_Position = 0;
    }

    if (C_gVao)
    {
        glDeleteVertexArrays(1, &C_gVao);
        C_gVao = 0;
    }

    if (C_gVbo_Position)
    {
        glDeleteBuffers(1, &C_gVbo_Position);
        C_gVbo_Position = 0;
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



