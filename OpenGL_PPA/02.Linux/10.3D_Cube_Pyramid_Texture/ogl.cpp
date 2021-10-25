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

/* Texture Changes  New */
#include<SOIL/SOIL.h>

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

GLfloat P_angle = 0.0f;
GLfloat C_angle = 0.0f;

GLuint gVao_Pyramid;
GLuint gVbo_Pyramid_Position;
GLuint gVbo_Pyramid_Texture;

GLuint gVao_Cube;
GLuint gVbo_Cube_Position;
GLuint gVbo_Cube_Texture;

GLuint Stone_Texture;
GLuint Kundali_Texture;

GLuint texture_Sampler_Uniform;

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
	
	XStoreName ( gpDisplay, gWindow, "Darshan's OpenGL PP : 3D Pyramid Cube Texture.");
		
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
    GLuint LoadBitmapAsTexture(const char*);


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
        "in vec2 vTexCoord;" \
        "out vec2 out_TexCoord;" \
        "uniform mat4 u_mvp_matrix;" \
        "void main(void){\n" \
        "gl_Position = u_mvp_matrix * vPosition;\n"\
        "out_TexCoord = vTexCoord;" \
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
        "in vec2 out_TexCoord;" \
        "uniform sampler2D u_Texture_Sampler;" \
        "out vec4 FragColor;" \
        "void main(void){\n" \
    	"FragColor = texture(u_Texture_Sampler, out_TexCoord);\n" \
        "}";
        // "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" 

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
    glBindAttribLocation(gshaderProgramObject, DVM_ATTTRIBUTE_TEXCORD, "vTexCoord");
    
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

    texture_Sampler_Uniform = glGetUniformLocation(gshaderProgramObject, "u_Texture_Sampler");
    
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
        const GLfloat PyramidColor[] =
    {
        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.5f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        
        0.5f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    /*New for Cube*/
    const GLfloat Cube_Vertices[] =
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

    const GLfloat CubeColor[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f

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
    
    /*Color */
    glGenBuffers(1, &gVbo_Pyramid_Texture);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Pyramid_Texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidColor), PyramidColor, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_TEXCORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_TEXCORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    glBindVertexArray(0);
   
    /*CUBE Begen*/
    glGenVertexArrays(1, &gVao_Cube);
    glBindVertexArray(gVao_Cube);

    /*Position */
    glGenBuffers(1, &gVbo_Cube_Position);
    
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Cube_Position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cube_Vertices), Cube_Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &gVbo_Cube_Texture);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo_Cube_Texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeColor), CubeColor, GL_STATIC_DRAW);
    glVertexAttribPointer(DVM_ATTTRIBUTE_TEXCORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DVM_ATTTRIBUTE_TEXCORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_TEXTURE_2D);
    
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Stone_Texture = LoadBitmapAsTexture("Stone.bmp");
    Kundali_Texture = LoadBitmapAsTexture("Vijay_Kundali.bmp");
    
	gPerspectiveProjectMatix = mat4::identity();

	Resize( giWindowWidth, giWindowHeight);

}


GLuint LoadBitmapAsTexture(const char* path)
{
	/*variable Declaration */
	int Width,Height;
	unsigned char* ImageData = NULL;
	GLuint *Texture_ID;
	
	/*Calling to soil to load Image (Its pixel data)*/
	ImageData =  SOIL_load_image( path, /* path of image */
				&Width, /* i'll return image width */
				&Height,  /* i'll return image Height */
				NULL,  /* i'll return chanels(numbers) */
				SOIL_LOAD_RGB);  /* which type of chanels user want in images data (why RGB only :- cause it is 24 color bitmap( 8*3 
	(R = 8 byte : G = 8 byte : B = 8 byte) = 24)) */

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, Texture_ID);
	glBindTexture(GL_TEXTURE_2D, *Texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	
	/* Image could be in blue format if we give 6th parameter as GL_BGR_EXT 	 cause it is readable to windows.
	here we are passing data to soil and soil understand GL_RGB .
	
	*Note : 1. In real .BMP is BGR.
		2. Soil converts to BGR into BGR to RGB.
	*/
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, ImageData);

	/*calling soil to delete Object which was taken by soil to create image data */
	SOIL_free_image_data(ImageData);
	
	return (*Texture_ID);
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

	mat4 modelViewMatrix = mat4::identity();
    mat4 RotationMatrtix = mat4::identity();
    mat4 TranslateMatrix = mat4::identity();
    mat4 modelViewProjectionMatrix = mat4::identity();
    
    TranslateMatrix = vmath::translate(-1.0f, 0.0f, -4.0f);
    RotationMatrtix = vmath::rotate(P_angle, 0.0f, 1.0f, 0.0f);
    modelViewMatrix = TranslateMatrix * RotationMatrtix;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;
    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Stone_Texture);
    glUniform1i(texture_Sampler_Uniform, 0);


    glBindVertexArray(gVao_Pyramid);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3); 
    glDrawArrays(GL_TRIANGLES, 6, 3);
    glDrawArrays(GL_TRIANGLES, 9, 3);

    glBindVertexArray(0);
	
    modelViewProjectionMatrix = mat4::identity();
    modelViewMatrix = mat4::identity();

    TranslateMatrix = vmath::translate(1.0f, 0.0f, -4.0f);
    RotationMatrtix = vmath::rotate(P_angle, 0.0f, 1.0f, 0.0f);
    modelViewMatrix = TranslateMatrix * RotationMatrtix;
    modelViewProjectionMatrix = gPerspectiveProjectMatix * modelViewMatrix;

    glUniformMatrix4fv(gMVPMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Kundali_Texture);
    glUniform1i(texture_Sampler_Uniform, 0);

    glBindVertexArray(gVao_Cube);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    glBindVertexArray(0);


    glUseProgram(0);

    Update();

	glXSwapBuffers( gpDisplay, gWindow);
}

void Update()
{
    if (P_angle >= 360.0f)
    {
        P_angle = 0.0f;
    }
    P_angle = P_angle + 0.3f;

    if (C_angle >= 360.0f)
    {
        C_angle = 0.0f;
    }
    C_angle = C_angle + 0.3f;
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

    if (Stone_Texture)
    {
        glDeleteTextures(1, &Stone_Texture);
        Stone_Texture = 0;

    }
    if (Kundali_Texture)
    {
        glDeleteTextures(1, &Kundali_Texture);
        Kundali_Texture = 0;
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

    if (gVbo_Pyramid_Texture)
    {
        glDeleteBuffers(1, &gVbo_Pyramid_Texture);
        gVbo_Pyramid_Texture = 0;
    }

	if (gVao_Cube)
    {
        glDeleteVertexArrays(1, &gVao_Cube);
        gVao_Cube = 0;
    }

    if (gVbo_Cube_Position)
    {
        glDeleteBuffers(1, &gVbo_Cube_Position);
        gVbo_Cube_Position = 0;
    }

    if (gVbo_Cube_Texture)
    {
        glDeleteBuffers(1, &gVbo_Cube_Texture);
        gVbo_Cube_Texture = 0;
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
