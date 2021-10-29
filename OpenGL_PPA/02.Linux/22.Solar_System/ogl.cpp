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
GLfloat year_DM = 0.0;
GLfloat day_DM = 0.0;
GLfloat moon_DM = 0.0;


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

mat4 PushMatrix_DM(mat4);
void PopMatrix_DM(void);
struct Matrix_Stack_DM* CreateMatrix_DM(void);

struct Matrix_Stack_DM
{
    mat4 Matrix;
    struct Matrix_Stack_DM* Next;
}*Start = NULL, *Top = NULL, *Prev = NULL;

GLfloat angle_Sholder_DM = 0.0;
GLfloat angle_Arm_DM = 0.0;

int PushToStack_DM = 0;

GLfloat T_angle = 0.0f;

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

GLuint gMVPMatrixUniform;

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
                    
                    case XK_M:
                    case XK_m:
                    moon_DM += 2.0f;
                    if (moon_DM > 360.0f)
                    {
                        moon_DM = 0.0f;
                    }
                    break;
                    case XK_N:
                    case XK_n:
                    moon_DM -= 2.0f;
                    if (moon_DM > 360.0f)
                    {
                        moon_DM = 0.0f;
                    }
                    break;
 
                     case XK_Up:
                        year_DM += 2.0f;
                        if (year_DM > 360.0f)
                        {
                            year_DM = 0.0f;
                        }
                    break;
                    case XK_Down:
                        year_DM -= 2.0f;
                        if (year_DM > 360.0f)
                        {
                            year_DM = 0.0f;
                        }
                    break;
                    case XK_Left:
                        day_DM += 2.0f;
                        if (day_DM > 360.0f)
                        {
                            day_DM = 0.0f;
                        }
                    break;
                    case XK_Right:
                        day_DM -= 2.0f;
                        if (day_DM > 360.0f)
                        {
                            day_DM = 0.0f;
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
	
	XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP : Solar System ");
		
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

	/* Vertex Shader Object */
    gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSourceCode =
        "#version 450 core\n" \
        "in vec4 vPosition;" \
        "uniform mat4 u_mvp_matrix;" \
        "void main(void){\n" \
        "gl_Position = u_mvp_matrix * vPosition;" \
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
        "out vec4 FragColor;" \
        "void main(void){\n" \
    	"FragColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f);" \
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
    /* Code Began */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gshaderProgramObject);

	mat4 modelViewMatrix;
    mat4 TranslateMatrix;
    mat4 ScaleMatrix;
    mat4 RotationMatrix;

    //Sun
    modelViewMatrix = mat4::identity();
    TranslateMatrix = vmath::translate(0.0f, 0.0f, -8.0f);
    ScaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);
    RotationMatrix = vmath::rotate(year_DM, 0.0f, 1.0f, 0.0f);

    mat4 modelViewProjectionMatrix = mat4::identity();
   
    modelViewMatrix = TranslateMatrix * ScaleMatrix * RotationMatrix;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(gVao_Sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    //Earth
    modelViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    RotationMatrix = mat4::identity();
    ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);

    TranslateMatrix = vmath::translate(1.0f, 0.0f, -0.0f);
    RotationMatrix = vmath::rotate(day_DM, 0.0f, 1.0f, 0.0f);

    modelViewProjectionMatrix = mat4::identity();
    modelViewMatrix = TranslateMatrix * ScaleMatrix * RotationMatrix;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);


    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(gVao_Sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    //Moon
    modelViewMatrix = mat4::identity();
    TranslateMatrix = mat4::identity();
    RotationMatrix = mat4::identity();
    ScaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);

    TranslateMatrix = vmath::translate(1.0f, 0.0f, -0.0f);
    RotationMatrix = vmath::rotate(moon_DM, 0.0f, 1.0f, 0.0f);

    modelViewProjectionMatrix = mat4::identity();
    modelViewMatrix = TranslateMatrix * ScaleMatrix * RotationMatrix;
    modelViewMatrix = PushMatrix_DM(modelViewMatrix);
    PopMatrix_DM();
    PopMatrix_DM();
    PopMatrix_DM();

    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(gVao_Sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);

    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);


	glXSwapBuffers( gpDisplay, gWindow);
}

mat4 PushMatrix_DM(mat4 matrix)
{
    struct Matrix_Stack_DM* Temp = NULL;
    
    Temp = CreateMatrix_DM();
    if (Temp == NULL)
    {
        printf("Node is Null..!\n");
        uninitialize();
    }
    
    Temp->Matrix = matrix;
    Temp->Next = NULL;

    if (PushToStack_DM < 1)
    {
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

struct Matrix_Stack_DM* CreateMatrix_DM(void)
{
    struct Matrix_Stack_DM* Temp = NULL;
    Temp = (struct Matrix_Stack_DM*)malloc(sizeof(struct Matrix_Stack_DM));
    return(Temp);
}

void PopMatrix_DM(void)
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
