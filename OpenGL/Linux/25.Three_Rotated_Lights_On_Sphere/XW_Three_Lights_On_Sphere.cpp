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

GLXContext gGLXContext;

int giWindowWidth = 800;
int giWindowHeight = 600;

bool gbLight = true;

/*Red Light Properties */
GLfloat light_Ambient_Zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_Defues_Zero[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_Specular_Zero[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_Position_Zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/*Green Light Properties */
GLfloat light_Ambient_One[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_Defues_One[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat light_Specular_One[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat light_Position_One[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/*Blue Light Properties */
GLfloat light_Ambient_Two[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_Defues_Two[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat light_Specular_Two[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat light_Position_Two[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/*Object Material Properties */
GLfloat material_Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat Light_Angle_0 = 0.0f;
GLfloat Light_Angle_1 = 0.0f;
GLfloat Light_Angle_2 = 0.0f;

GLfloat material_Shininess = 128.0f;

GLUquadric* quadric = NULL;


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
					case XK_L:
					case XK_l:
					if (gbLight == true)
            				{
            				    	glEnable(GL_LIGHTING);
            				    	gbLight = false;
						printf("Lighting is Enabled\n");
            				}
            				else
            				{
                				glDisable(GL_LIGHTING);
                				gbLight = true;
						printf("Lighting is Disablel\n");
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
	
	XStoreName ( gpDisplay, gWindow,"Darshan's : Depth XWindows");
		
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

    	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ambient_Zero);
    	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Defues_Zero);
    	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Specular_Zero);
    	glLightfv(GL_LIGHT0, GL_POSITION, light_Position_Zero);

    	glLightfv(GL_LIGHT1, GL_AMBIENT, light_Ambient_One);
    	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_Defues_One);
    	glLightfv(GL_LIGHT1, GL_SPECULAR, light_Specular_One);
    	glLightfv(GL_LIGHT1, GL_POSITION, light_Position_One);
    
    	glLightfv(GL_LIGHT2, GL_AMBIENT, light_Ambient_Two);
    	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_Defues_Two);
    	glLightfv(GL_LIGHT2, GL_SPECULAR, light_Specular_Two);
    	glLightfv(GL_LIGHT2, GL_POSITION, light_Position_Two);
	
    	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient);
    	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Diffuse);
    	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular);
    	glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess);;

    	glEnable(GL_LIGHT0);
    	glEnable(GL_LIGHT1); 
    	glEnable(GL_LIGHT2);
	
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
    	void Update(void);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

    	glPushMatrix();
    	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    	glPushMatrix();

    	glRotatef(Light_Angle_0, 1.0f, 0.0f, 0.0f);
    	light_Position_Zero[1] = Light_Angle_0;
    	glLightfv(GL_LIGHT0, GL_POSITION, light_Position_Zero);

    	glPopMatrix();


    	glPushMatrix();
    
    	glRotatef(Light_Angle_1, 0.0f, 1.0f, 0.0f);
    	light_Position_One[0] = Light_Angle_1;
    	glLightfv(GL_LIGHT1, GL_POSITION, light_Position_One);
    
    	glPopMatrix();

    	glPushMatrix();

    	glRotatef(Light_Angle_2, 0.0f, 0.0f, 1.0f);
    	light_Position_Two[0] = Light_Angle_2;
    	glLightfv(GL_LIGHT2, GL_POSITION, light_Position_Two);

    	glPopMatrix();
    
    	glTranslatef(0.0f, 0.0f, -1.0f);
    	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    	quadric = gluNewQuadric();
    	gluSphere(quadric, 0.5f, 60, 60);

    	glPopMatrix();

    	Update();

	glXSwapBuffers(gpDisplay,gWindow);
}

void Update()
{
    	Light_Angle_0 += 1.0f;
    	if (Light_Angle_0 >= 360.0f)
    	{
    	    Light_Angle_0 = 0;
    	}
	
    	Light_Angle_1 += 1.0f;
    	if (Light_Angle_1 >= 360.0f)
    	{
    	    Light_Angle_1 = 0;
    	}
	
    	Light_Angle_2 += 1.0f;
    	if (Light_Angle_2 >= 360.0f)
    	{
    	    Light_Angle_2 = 0;
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
    	if (quadric)
    	{
    	    gluDeleteQuadric(quadric);
    	    quadric = NULL;
    	}
	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}

