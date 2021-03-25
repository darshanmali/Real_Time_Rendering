#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h> /* Memset */

#include<X11/Xlib.h>  /* similar windows.h */
#include<X11/Xutil.h> /* XVisual info */
#include<X11/XKBlib.h> /* KeyBoard */
#include<X11/keysym.h> /* key Symbols for taking Event */

#include<GL/gl.h>/* for openGL API */
#include<GL/glu.h>/* Ortho view also for glu quadric and many more */
#include<GL/glx.h>/* for GLX API */


using namespace std;

bool bFullscreen = true;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;

const int circle_Points = 1000;
int Color_flag = 0;


GLXContext gGLXContext;

int giWindowWidth = 800;
int giWindowHeight = 600;


void Vertexcall(GLfloat, GLfloat, GLfloat, GLfloat);

int main(void)
{
	/* Local Fuction Declaraton */
	void CreateWindow(void);
	void ToggleFullscreen(void);
	
	/* for game loop */
	bool bDone = false;

	/* Local variables */
	void Initialize(void);
	void Resize(int , int);
	void Render(void);
	void uninitialize(void);

	
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
	int defaultScreen;
	
	int stylemask;

	/* similar TO Pixel Format Attribute (windows)*/
						/* WHICH TYPE OF COLOR BUFFER WE WANT */
	/*Replaced 1 by 8 */
	static int frameBufferAttributes[] = { GLX_DOUBLEBUFFER, True,
						GLX_RGBA, 
						GLX_RED_SIZE, 8,
						GLX_GREEN_SIZE, 8,
						GLX_BLUE_SIZE, 8,
						GLX_ALPHA_SIZE, 8,
						/* V4L(Video for Linux, it is controled under Device Driver(VA(Video Acceleration ) Api / VDPAU API(Video Driver Presentation Unix API ) Componant)) Recomonds you 24 size*/
						GLX_DEPTH_SIZE, 24,
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

	/* first Bridging API for gettting OpenGL rendering context from system by giving comands in 3 parameter */
	/* this visual has 44 members from that we want only 5 which we are mensioning in 3rd parameter*/
	gpXVisualInfo = glXChooseVisual(gpDisplay, defaultScreen, frameBufferAttributes);

	//XMatchVisualInfo(gpDisplay,defaultScreen,defaultDepth,TrueColor,gpXVisualInfo);
	if(gpXVisualInfo == NULL)
	{
		printf("ERROR : Unable TO Get Visual. \n Exitting now...\n");
		uninitialize();
		exit(1);
	}
	
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
	
	XStoreName ( gpDisplay, gWindow,"Darshan's : Static India XWindows");
		
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
	/* hey Create rendering context which will be compliant to GPDisplay  */
	/* 3rd - do i need to create shared DC (we are saying no) create fresh Context : 4rd - i want real graphic card to render 
											dont use software(AMD - VESA, INTEL - Denue, NVIDIA - Novov) GC*/
	gGLXContext = glXCreateContext( gpDisplay, gpXVisualInfo, NULL, GL_TRUE);
	
	glXMakeCurrent(gpDisplay, gWindow, gGLXContext);

	/* Depth Enabling Fuctions */
        /*for getting anti_Allies */
	glShadeModel(GL_SMOOTH);
	/* Which are less than 1 will be removed */
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
    	glDepthFunc(GL_LEQUAL);
    	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Preventing From Perspective Destortion
		
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Resize( giWindowWidth, giWindowHeight);

}

void Resize(int Width, int Height)
{

	if(Height == 0)
	{
		Height = 1;
	}
	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective( 44.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-0.3f, 0.0f, -3.0f);

    glBegin(GL_QUADS);
    
    //Top of I
    glColor3f( 1.0f, 0.5f, 0.0f);
    glVertex3f( -1.0f, 0.4f, 0.0f);
    glVertex3f( -0.9f, 0.3f, 0.0f);

    glVertex3f( -0.7f, 0.3f, 0.0f);
    glVertex3f( -0.6f, 0.4f, 0.0f);

    //Middle of I part 1.
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-0.85f, 0.3f, 0.0f);
    glVertex3f(-0.75f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.75f, 0.0f, 0.0f);
    glVertex3f(-0.85f, 0.0f, 0.0f);

    //Middle of I part 2.
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.85f, 0.0f, 0.0f);
    glVertex3f(-0.75f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.75f, -0.3f, 0.0f);
    glVertex3f(-0.85f, -0.3f, 0.0f);

    //Bottom of I
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -0.4f, 0.0f);
    glVertex3f(-0.9f, -0.3f, 0.0f);
    
    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(-0.6f, -0.4f, 0.0f);
    
    glEnd();

   
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    //left of N
    glVertex3f(-0.55f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.45f, 0.1f, 0.0f);
    glVertex3f(-0.45f, 0.0f, 0.0f);
    glVertex3f(-0.55f, 0.0f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.55f, 0.0f, 0.0f);
    glVertex3f(-0.45f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.45f, -0.3f, 0.0f);
    glVertex3f(-0.55f, -0.4f, 0.0f);

    //Middle of N
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-0.44f, 0.1f, 0.0f);
    glVertex3f(-0.54f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.28f, 0.0f, 0.0f);
    glVertex3f(-0.38f, 0.0f, 0.0f);

    glVertex3f(-0.38f, 0.0f, 0.0f);
    glVertex3f(-0.28f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.21f, -0.1f, 0.0f);
    glVertex3f(-0.11f, -0.4f, 0.0f);

    //Right of N
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-0.1f, 0.4f, 0.0f);
    glVertex3f(-0.2f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.2f, 0.0f, 0.0f);
    glVertex3f(-0.1f, 0.0f, 0.0f);

    glVertex3f(-0.1f, 0.0f, 0.0f);
    glVertex3f(-0.2f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.2f, -0.1f, 0.0f);
    glVertex3f(-0.1f, -0.4f, 0.0f);
    
    glEnd();

    //D Began
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    
    //D Top
    glVertex3f(0.44f, 0.4f, 0.0f);
    glVertex3f(0.05f, 0.4f, 0.0f);

    glVertex3f(-0.05f, 0.3f, 0.0f);
    glVertex3f(0.34f, 0.3f, 0.0f);
    
    //D Middle
    glVertex3f(0.1f, 0.3f, 0.0f);
    glVertex3f(0.2f, 0.3f, 0.0f);

    //White
    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);

    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.2f, -0.3f, 0.0f);
    glVertex3f(0.1f, -0.3f, 0.0f);

    //D Right
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0.45f, 0.4f, 0.0f);
    glVertex3f(0.35f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(0.45f, 0.0f, 0.0f);

    glVertex3f(0.45f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.35f, -0.3f, 0.0f);
    glVertex3f(0.45f, -0.4f, 0.0f);

    //D bOTTOM
    glVertex3f(0.44f, -0.4f, 0.0f);
    glVertex3f(0.05f, -0.4f, 0.0f);

    glVertex3f(-0.05f, -0.3f, 0.0f);
    glVertex3f(0.34f, -0.3f, 0.0f);

    glEnd();

    glBegin(GL_QUADS);
    //Top of I
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0.9f, 0.4f, 0.0f);
    glVertex3f(0.8f, 0.3f, 0.0f);

    glVertex3f(0.6f, 0.3f, 0.0f);
    glVertex3f(0.5f, 0.4f, 0.0f);

    //Middle of I part 1.
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(0.75f, 0.3f, 0.0f);
    glVertex3f(0.65f, 0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.65f, 0.0f, 0.0f);
    glVertex3f(0.75f, 0.0f, 0.0f);

    //Middle of I part 2.
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.75f, 0.0f, 0.0f);
    glVertex3f(0.65f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.65f, -0.3f, 0.0f);
    glVertex3f(0.75f, -0.3f, 0.0f);

    //Bottom of I
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.9f, -0.4f, 0.0f);
    glVertex3f(0.8f, -0.3f, 0.0f);

    glVertex3f(0.6f, -0.3f, 0.0f);
    glVertex3f(0.5f, -0.4f, 0.0f);

    glEnd();


    //                                              AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

    // - //
    glBegin(GL_QUADS);
    //L B
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.06f, 0.4f, 0.0f);
    glVertex3f(1.3F, 0.4f, 0.0f);
    glVertex3f(1.2f, 0.3f, 0.0f);
    glVertex3f(1.16f, 0.3f, 0.0f);

    glEnd();
    
    //A Right 1
    glBegin(GL_POLYGON);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.31f, 0.4f, 0.0f);
    glVertex3f(1.21f, 0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.3f, 0.05f, 0.0f);
    glVertex3f(1.45f, 0.0f, 0.0f);

    glEnd();
    
    //A right 2
    glBegin(GL_QUADS);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.6f, -0.4f, 0.0f);
    glVertex3f(1.45f, -0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.33f, -0.05f, 0.0f);
    glVertex3f(1.45f, 0.0f, 0.0f);

    glEnd();

    // Left 
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.05f, 0.4f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.97f, 0.0f, 0.0f);
    glVertex3f(1.11f, 0.05f, 0.0f);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.15f, 0.3f, 0.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.97f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.91f, -0.4f, 0.0f);
    glVertex3f(1.05f, -0.3f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.1f, -0.05f, 0.0f);

    glEnd();
  
    //  Center
    glBegin(GL_POLYGON);

    //L CORNER
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.98f, 0.0f, 0.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.11f, 0.04f, 0.0f);
    glVertex3f(1.32f, 0.04f, 0.0f);

    ////R CORNER
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.44f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.32f, -0.04f, 0.0f);
    glVertex3f(1.11f,-0.04f, 0.0f);

    glEnd();

	glXSwapBuffers(gpDisplay,gWindow);
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


