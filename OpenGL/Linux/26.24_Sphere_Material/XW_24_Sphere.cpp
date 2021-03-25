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

GLfloat lAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lDefues[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lPosition[4];

GLfloat L_Model_Ambient[] = { 0.2f,0.2f,0.2f,1.0f };
GLfloat L_Model_Local_Viewer[] = { 0.0f };

GLfloat angle_for_X_Rotation;
GLfloat angle_for_Y_Rotation;
GLfloat angle_for_Z_Rotation;

GLuint Key_Pressed = 0;

GLUquadric* quadric[24];


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
					case XK_X:
					case XK_x:
            					angle_for_X_Rotation = 0.0f;
            					Key_Pressed = 1;
            				break;
					case XK_Y:
					case XK_y:
            					angle_for_Y_Rotation = 0.0f;
            					Key_Pressed = 2;
            				break;
					case XK_Z:
					case XK_z:
            					angle_for_Z_Rotation = 0.0f;
            					Key_Pressed = 3;
				        break;
					case XK_L:
					case XK_l:
            				if (gbLight == true)
				        {
                				glEnable(GL_LIGHTING);
                				gbLight = false;
            				}
            				else
            				{
                				glDisable(GL_LIGHTING);
                				gbLight = true;
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
		
	glEnable(GL_AUTO_NORMAL);
    	glEnable(GL_NORMALIZE);
	
    	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, L_Model_Ambient);
    	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, L_Model_Local_Viewer);
	
    	glLightfv(GL_LIGHT0, GL_AMBIENT, lAmbient);
    	glLightfv(GL_LIGHT0, GL_DIFFUSE, lDefues);
    	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);

    	glEnable(GL_LIGHT0);

    	for (int i = 0; i < 24; i++)
    	{
        	quadric[i] = gluNewQuadric();
    	}
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

	void draw_24_Sphere(void);
    	void update(void);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

    	if (Key_Pressed == 1)
    	{
    	    	glRotatef(angle_for_X_Rotation, 1.0f, 0.0f, 0.0f);
    	    	lPosition[1] = angle_for_X_Rotation;
    	}
    	else if (Key_Pressed == 2)
    	{
    	    	glRotatef(angle_for_Y_Rotation, 0.0f, 1.0f, 0.0f);
    	    	lPosition[2] = angle_for_Y_Rotation;
    	}
    	else if (Key_Pressed == 3)
    	{
    	    	glRotatef(angle_for_Z_Rotation, 0.0f, 0.0f, 1.0f);
    	    	lPosition[0] = angle_for_Z_Rotation;
    	}
    	lPosition[3] = 1.0f;
    	                                                            
    	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
	
    	draw_24_Sphere();
    	
    	update();
	glXSwapBuffers(gpDisplay,gWindow);
}

void draw_24_Sphere(void) 
{

    //Emereable
    GLfloat material_Ambient[4];
    material_Ambient[0] = { 0.0215f };
     material_Ambient[1] = { 0.1745f };
     material_Ambient[2] = { 0.0215f };
     material_Ambient[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient);
        
    GLfloat material_Diffuse[4];
    material_Diffuse[0] = { 0.07568f };
    material_Diffuse[1] = { 0.61424f };
    material_Diffuse[2] = { 0.07568f };
    material_Diffuse[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse);

    GLfloat material_Specular[4];
    material_Specular[0] =  { 0.633f };
    material_Specular[1] = { 0.727811f };
    material_Specular[2] = { 0.633f };
    material_Specular[3] = { 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular);

    GLfloat material_Shininess = 0.6f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 14.0f, 0.0f);

    gluSphere(quadric[0], 1.0f, 30, 30);

    //jade 
    GLfloat material_Ambient_1[4];
    material_Ambient_1[0] = { 0.135f };
    material_Ambient_1[1] = { 0.2225f };
    material_Ambient_1[2] = { 0.1575f };
    material_Ambient_1[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_1);

    GLfloat material_Diffuse_1[4];
     material_Diffuse_1[0] = { 0.54f };
     material_Diffuse_1[1] = { 0.89f };
     material_Diffuse_1[2] = { 0.63f };
     material_Diffuse_1[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_1);

    GLfloat material_Specular_1[4];
     material_Specular_1[0] = { 0.316228f };
     material_Specular_1[1] = { 0.316228f };
     material_Specular_1[2] = { 0.316228f };
     material_Specular_1[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_1);

    GLfloat material_Shininess_1 = 0.1f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 14.0f, 0.0f);

    gluSphere(quadric[1], 1.0f, 30, 30);

    //obsidian 
    GLfloat material_Ambient_2[4];
    material_Ambient_2[0] = { 0.05375f };
    material_Ambient_2[1] = { 0.05f };
    material_Ambient_2[2] = { 0.06625f };
    material_Ambient_2[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_2);
    
    GLfloat material_Diffuse_2[4];
    material_Diffuse_2[0] = { 0.18275f };
    material_Diffuse_2[1] = { 0.17f };
    material_Diffuse_2[2] = { 0.22525f };
    material_Diffuse_2[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_2);

    GLfloat material_Specular_2[4];
    material_Specular_2[0] = { 0.332741f };
     material_Specular_2[1] = { 0.328634f };
     material_Specular_2[2] = { 0.346435f };
     material_Specular_2[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_2);

    GLfloat material_Shininess_2 = 0.3f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 14.0f, 0.0f);

    gluSphere(quadric[2], 1.0f, 30, 30);

    //pearl
    GLfloat material_Ambient_3[4];
    material_Ambient_3[0] = { 0.25f };
     material_Ambient_3[1] = { 0.20725f };
     material_Ambient_3[2] = { 0.20725f };
     material_Ambient_3[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_3);

    GLfloat material_Diffuse_3[4];
    material_Diffuse_3[0] = { 1.0f };
     material_Diffuse_3[1] = { 0.829f };
     material_Diffuse_3[2] = { 0.829f };
     material_Diffuse_3[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_3);

    GLfloat material_Specular_3[4];
    material_Specular_3[0] = {  0.296648f };
     material_Specular_3[1] = { 0.296648f };
     material_Specular_3[2] = { 0.296648f };
     material_Specular_3[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_3);

    GLfloat material_Shininess_3 = 0.88f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_3);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 14.0f, 0.0f);

    gluSphere(quadric[3], 1.0f, 30, 30);

    //ruby
    GLfloat material_Ambient_4[4];
    material_Ambient_4[0] = { 0.1745f };
     material_Ambient_4[1] = { 0.01175f };
     material_Ambient_4[2] = { 0.01175f };
     material_Ambient_4[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_4);

    GLfloat material_Diffuse_4[4];
    material_Diffuse_4[0] = { 0.61424f };
     material_Diffuse_4[1] = { 0.04136f };
     material_Diffuse_4[2] = { 0.04136f };
     material_Diffuse_4[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_4);

    GLfloat material_Specular_4[4];
    material_Specular_4[0] = { 0.727811f };
     material_Specular_4[1] = { 0.626959f };
     material_Specular_4[2] = { 0.626959f };
     material_Specular_4[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_4);

    GLfloat material_Shininess_4 = 0.6f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_4);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 14.0f, 0.0f);

    gluSphere(quadric[4], 1.0f, 30, 30);

    //turquoise
    GLfloat material_Ambient_5[4];
    material_Ambient_5[0] = { 0.1f };
     material_Ambient_5[1] = { 0.18725f };
     material_Ambient_5[2] = { 0.1745f };
     material_Ambient_5[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_5);

    GLfloat material_Diffuse_5[4];
    material_Diffuse_5[0] = { 0.396f };
     material_Diffuse_5[1] = { 0.74151f };
     material_Diffuse_5[2] = { 0.69102f };
     material_Diffuse_5[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_5);

    GLfloat material_Specular_5[4];
    material_Specular_5[0] = { 0.297254f };
     material_Specular_5[1] = { 0.30829f };
     material_Specular_5[2] = { 0.306678f };
     material_Specular_5[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_5);

    GLfloat material_Shininess_5 = 0.1f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_5);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 14.0f, 0.0f);

    gluSphere(quadric[5], 1.0f, 30, 30);


    //brass
    GLfloat material_Ambient_6[4];
    material_Ambient_6[0] = { 0.329412f };
     material_Ambient_6[1] = { 0.223529f };
     material_Ambient_6[2] = { 0.027451f };
     material_Ambient_6[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_6);

    GLfloat material_Diffuse_6[4];
    material_Diffuse_6[0] = { 0.780392f };
     material_Diffuse_6[1] = { 0.568627f };
     material_Diffuse_6[2] = { 0.113725f };
     material_Diffuse_6[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_6);

    GLfloat material_Specular_6[4];
    material_Specular_6[0] = { 0.992157f };
     material_Specular_6[1] = { 0.941176f };
     material_Specular_6[2] = { 0.807843f };
     material_Specular_6[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_6);

    GLfloat material_Shininess_6 = 0.21794872f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_6);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 12.0f, 0.0f);

    gluSphere(quadric[6], 1.0f, 30, 30);

    //bronze
    GLfloat material_Ambient_7[4];
    material_Ambient_7[0] = { 0.2125f };
     material_Ambient_7[1] = { 0.1275f };
     material_Ambient_7[2] = { 0.054f };
     material_Ambient_7[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_7);

    GLfloat material_Diffuse_7[4];
    material_Diffuse_7[0] = { 0.714f };
     material_Diffuse_7[1] = { 0.4284f };
     material_Diffuse_7[2] = { 0.18144f };
     material_Diffuse_7[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_7);

    GLfloat material_Specular_7[4];
    material_Specular_7[0] = { 0.393548f };
     material_Specular_7[1] = { 0.271906f };
     material_Specular_7[2] = { 0.166721f };
     material_Specular_7[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_7);

    GLfloat material_Shininess_7 = 0.2f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_7);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 12.0f, 0.0f);

    gluSphere(quadric[7], 1.0f, 30, 30);


    //chroma
    GLfloat material_Ambient_8[4];
    material_Ambient_8[0] = { 0.25f };
     material_Ambient_8[1] = { 0.25f };
     material_Ambient_8[2] = { 0.25f };
     material_Ambient_8[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_8);

    GLfloat material_Diffuse_8[4];
    material_Diffuse_8[0] = { 0.4f };
     material_Diffuse_8[1] = { 0.4f };
     material_Diffuse_8[2] = { 0.4f };
     material_Diffuse_8[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_8);

    GLfloat material_Specular_8[4];
    material_Specular_8[0] = { 0.774597f };
     material_Specular_8[1] = { 0.774597f };
     material_Specular_8[2] = { 0.774597f };
     material_Specular_8[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_8);

    GLfloat material_Shininess_8 = 0.6f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_8);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 12.0f, 0.0f);

    gluSphere(quadric[8], 1.0f, 30, 30);

    //Copper
    GLfloat material_Ambient_9[4];
    material_Ambient_9[0] = { 0.19125f };
     material_Ambient_9[1] = { 0.0735f };
     material_Ambient_9[2] = { 0.0225f };
     material_Ambient_9[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_9);

    GLfloat material_Diffuse_9[4];
    material_Diffuse_9[0] = { 0.7038f };
     material_Diffuse_9[1] = { 0.27048f };
     material_Diffuse_9[2] = { 0.0828f };
     material_Diffuse_9[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_9);

    GLfloat material_Specular_9[4];
    material_Specular_9[0] = { 0.256777f };
     material_Specular_9[1] = { 0.137622f };
     material_Specular_9[2] = { 0.086014f };
     material_Specular_9[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_9);

    GLfloat material_Shininess_9 = 0.1f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_9);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 12.0f, 0.0f);

    gluSphere(quadric[9], 1.0f, 30, 30);

    //Gold
    GLfloat material_Ambient_10[4];
    material_Ambient_10[0] = { 0.24725f };
     material_Ambient_10[1] = { 0.1995f };
     material_Ambient_10[2] = { 0.0745f };
     material_Ambient_10[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_10);

    GLfloat material_Diffuse_10[4];
    material_Diffuse_10[0] = { 0.75164f };
     material_Diffuse_10[1] = { 0.60648f };
     material_Diffuse_10[2] = { 0.22648f };
     material_Diffuse_10[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_10);

    GLfloat material_Specular_10[4];
    material_Specular_10[0] = { 0.628281f };
     material_Specular_10[1] = { 0.555802f };
     material_Specular_10[2] = { 0.366065f };
     material_Specular_10[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_10);

    GLfloat material_Shininess_10 = 0.4f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 12.0f, 0.0f);

    gluSphere(quadric[10], 1.0f, 30, 30);

    //Silver
     GLfloat material_Ambient_11[4];
     material_Ambient_11[0] = { 0.19225f };
     material_Ambient_11[1] = { 0.19225f };
     material_Ambient_11[2] = { 0.19225f };
     material_Ambient_11[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_11);

    GLfloat material_Diffuse_11[4];
    material_Diffuse_11[0] = { 0.50754f };
     material_Diffuse_11[1] = { 0.50754f };
     material_Diffuse_11[2] = { 0.50754f };
     material_Diffuse_11[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_11);

    GLfloat material_Specular_11[4];
    material_Specular_11[0] = { 0.508273f };
     material_Specular_11[1] = { 0.508273f };
     material_Specular_11[2] = { 0.508273f };
     material_Specular_11[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_11);

    GLfloat material_Shininess_11 = 0.4f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_11);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 12.0f, 0.0f);

    gluSphere(quadric[11], 1.0f, 30, 30);

    //Black
    GLfloat material_Ambient_12[4];
    material_Ambient_12[0] = { 0.0f };
     material_Ambient_12[1] = { 0.0f };
     material_Ambient_12[2] = { 0.0f };
     material_Ambient_12[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_12);

    GLfloat material_Diffuse_12[4];
    material_Diffuse_12[0] = { 0.01f };
     material_Diffuse_12[1] = { 0.01f };
     material_Diffuse_12[2] = { 0.01f };
     material_Diffuse_12[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_12);

    GLfloat material_Specular_12[4];
    material_Specular_12[0] = { 0.50f };
     material_Specular_12[1] = { 0.50f };
     material_Specular_12[2] = { 0.50f };
     material_Specular_12[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_12);

    GLfloat material_Shininess_12 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_12);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 10.0f, 0.0f);

    gluSphere(quadric[12], 1.0f, 30, 30);

    //cyan
    GLfloat material_Ambient_13[4];
     material_Ambient_13[0] = { 0.0f };
     material_Ambient_13[1] = { 0.1f };
     material_Ambient_13[2] = { 0.06f };
     material_Ambient_13[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_13);

    GLfloat material_Diffuse_13[4];
    material_Diffuse_13[0] = { 0.0f };
     material_Diffuse_13[1] = { 0.50980392f };
     material_Diffuse_13[2] = { 0.50980392f };
     material_Diffuse_13[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_13);

    GLfloat material_Specular_13[4];
    material_Specular_13[0] = { 0.50196078f };
     material_Specular_13[1] = { 0.50196078f };
     material_Specular_13[2] = { 0.50196078f };
     material_Specular_13[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_13);

    GLfloat material_Shininess_13 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_13);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 10.0f, 0.0f);

    gluSphere(quadric[13], 1.0f, 30, 30);

    //green
    GLfloat material_Ambient_14[4];
    material_Ambient_14[0] = { 0.0f };
     material_Ambient_14[1] = { 0.0f };
     material_Ambient_14[2] = { 0.0f };
     material_Ambient_14[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_14);

    GLfloat material_Diffuse_14[4];
    material_Diffuse_14[0] = { 0.1f };
     material_Diffuse_14[1] = { 0.35f };
     material_Diffuse_14[2] = { 0.1f };
     material_Diffuse_14[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_14);

    GLfloat material_Specular_14[4];
    material_Specular_14[0] = { 0.45f };
     material_Specular_14[1] = { 0.45f };
     material_Specular_14[2] = { 0.45f };
     material_Specular_14[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_14);

    GLfloat material_Shininess_14 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_14);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 10.0f, 0.0f);

    gluSphere(quadric[14], 1.0f, 30, 30);

    //red
    GLfloat material_Ambient_15[4];
    material_Ambient_15[0] = { 0.0f };
     material_Ambient_15[1] = { 0.0f };
     material_Ambient_15[2] = { 0.0f };
     material_Ambient_15[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_15);

    GLfloat material_Diffuse_15[4];
    material_Diffuse_15[0] = { 0.5f };
     material_Diffuse_15[1] = { 0.0f };
     material_Diffuse_15[2] = { 0.0f };
     material_Diffuse_15[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_15);

    GLfloat material_Specular_15[4] ;
    material_Specular_15[0] = { 0.7f };
     material_Specular_15[1] = { 0.6f };
     material_Specular_15[2] = { 0.6f };
     material_Specular_15[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_15);

    GLfloat material_Shininess_15 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_15);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 10.0f, 0.0f);

    gluSphere(quadric[15], 1.0f, 30, 30);

    //Samle
    GLfloat material_Ambient_16[4];
    material_Ambient_16[0] = { 0.0f };
     material_Ambient_16[1] = { 0.0f };
     material_Ambient_16[2] = { 0.0f };
     material_Ambient_16[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_16);

    GLfloat material_Diffuse_16[4];
    material_Diffuse_16[0] = { 0.55f };
     material_Diffuse_16[1] = { 0.55f };
     material_Diffuse_16[2] = { 0.55f };
     material_Diffuse_16[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_16);

    GLfloat material_Specular_16[4];
     material_Specular_16[0] = { 0.70f };
     material_Specular_16[1] = { 0.70f };
     material_Specular_16[2] = { 0.70f };
     material_Specular_16[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_16);

    GLfloat material_Shininess_16 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_16);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 10.0f, 0.0f);

    gluSphere(quadric[16], 1.0f, 30, 30);

    //yellow plastic
    GLfloat material_Ambient_17[4];
    material_Ambient_17[0] = { 0.0f };
     material_Ambient_17[1] = { 0.0f };
     material_Ambient_17[2] = { 0.0f };
     material_Ambient_17[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_17);

    GLfloat material_Diffuse_17[4];
    material_Diffuse_17[0] = { 0.5f };
     material_Diffuse_17[1] = { 0.5f };
     material_Diffuse_17[2] = { 0.0f };
     material_Diffuse_17[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_17);

    GLfloat material_Specular_17[4];
    material_Specular_17[0] = { 0.60f };
     material_Specular_17[1] = { 0.60f };
     material_Specular_17[2] = { 0.50f };
     material_Specular_17[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_17);

    GLfloat material_Shininess_17 = 0.25f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_17);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 10.0f, 0.0f);

    gluSphere(quadric[17], 1.0f, 30, 30);

    //Samle
    GLfloat material_Ambient_18[4];
    material_Ambient_18[0] = { 0.02f };
     material_Ambient_18[1] = { 0.02f };
     material_Ambient_18[2] = { 0.02f };
     material_Ambient_18[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_18);

    GLfloat material_Diffuse_18[4];
    material_Diffuse_18[0] = { 0.01f };
     material_Diffuse_18[1] = { 0.01f };
     material_Diffuse_18[2] = { 0.01f };
     material_Diffuse_18[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_18);

    GLfloat material_Specular_18[4];
    material_Specular_18[0] = { 0.4f };
     material_Specular_18[1] = { 0.4f };
     material_Specular_18[2] = { 0.4f };
     material_Specular_18[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_18);

    GLfloat material_Shininess_18 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_18);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(1.0f, 8.0f, 0.0f);

    gluSphere(quadric[18], 1.0f, 30, 30);

    //cyan
    GLfloat material_Ambient_19[4];
    material_Ambient_19[0] = { 0.0f };
     material_Ambient_19[1] = { 0.05f };
     material_Ambient_19[2] = { 0.05f };
     material_Ambient_19[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_19);

    GLfloat material_Diffuse_19[4];
    material_Diffuse_19[0] = { 0.4f };
     material_Diffuse_19[1] = { 0.5f };
     material_Diffuse_19[2] = { 0.5f };
     material_Diffuse_19[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_19);

    GLfloat material_Specular_19[4];
    material_Specular_19[0] = { 0.04f };
     material_Specular_19[1] = { 0.7f };
     material_Specular_19[2] = { 0.7f };
     material_Specular_19[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_19);

    GLfloat material_Shininess_19 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_19);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(3.0f, 8.0f, 0.0f);

    gluSphere(quadric[19], 1.0f, 30, 30);

    //green
    GLfloat material_Ambient_20[4];
    material_Ambient_20[0] = { 0.0f };
     material_Ambient_20[1] = { 0.05f };
     material_Ambient_20[2] = { 0.0f };
     material_Ambient_20[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_20);

    GLfloat material_Diffuse_20[4];
    material_Diffuse_20[0] = { 0.4f };
     material_Diffuse_20[1] = { 0.5f };
     material_Diffuse_20[2] = { 0.4f };
     material_Diffuse_20[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_20);

    GLfloat material_Specular_20[4];
    material_Specular_20[0] = { 0.04f };
     material_Specular_20[1] = { 0.7f };
     material_Specular_20[2] = { 0.04f };
     material_Specular_20[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_20);

    GLfloat material_Shininess_20 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(5.0f, 8.0f, 0.0f);

    gluSphere(quadric[20], 1.0f, 30, 30);

    //red
    GLfloat material_Ambient_21[4];
    material_Ambient_21[0] = { 0.05f };
     material_Ambient_21[1] = { 0.0f };
     material_Ambient_21[2] = { 0.0f };
     material_Ambient_21[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_21);

    GLfloat material_Diffuse_21[4];
    material_Diffuse_21[0] = { 0.5f };
     material_Diffuse_21[1] = { 0.4f };
     material_Diffuse_21[2] = { 0.4f };
     material_Diffuse_21[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_21);

    GLfloat material_Specular_21[4];
    material_Specular_21[0] = { 0.7f };
    material_Specular_21[1] = { 0.04f };
     material_Specular_21[2] = { 0.04f };
     material_Specular_21[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_21);

    GLfloat material_Shininess_21 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_21);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(7.0f, 8.0f, 0.0f);

    gluSphere(quadric[21], 1.0f, 30, 30);

    //White
    GLfloat material_Ambient_22[4];
    material_Ambient_22[0] = { 0.05f };
     material_Ambient_22[1] = { 0.05f };
     material_Ambient_22[2] = { 0.05f };
     material_Ambient_22[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_22);

    GLfloat material_Diffuse_22[4];
     material_Diffuse_22[0] = { 0.5f };
     material_Diffuse_22[1] = { 0.5f };
     material_Diffuse_22[2] = { 0.5f };
     material_Diffuse_22[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_22);

    GLfloat material_Specular_22[4];
    material_Specular_22[0] = { 0.7f };
     material_Specular_22[1] = { 0.7f };
     material_Specular_22[2] = { 0.7f };
     material_Specular_22[3] = { 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_22);

    GLfloat material_Shininess_22 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_22);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(9.0f, 8.0f, 0.0f);

    gluSphere(quadric[22], 1.0f, 30, 30);

    //yellow rubber
    GLfloat material_Ambient_23[4];
    material_Ambient_23[0] =   0.0f;
     material_Ambient_23[1] =  0.0f;
     material_Ambient_23[2] =  0.0f;
     material_Ambient_23[3] =  1.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ambient_23);

    GLfloat material_Diffuse_23[4];
    material_Diffuse_23[0] =   0.0f;
     material_Diffuse_23[1] =  0.0f;
     material_Diffuse_23[2] =  0.0f;
     material_Diffuse_23[3] =  1.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_Diffuse_23);

    GLfloat material_Specular_23[4];
    material_Specular_23[0] = 0.0f;
     material_Specular_23[1] = 0.0f;
     material_Specular_23[2] = 0.0f;
     material_Specular_23[3] = 1.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_Specular_23);

    GLfloat material_Shininess_23 = 0.078125f * 128;
    glMaterialf(GL_FRONT, GL_SHININESS, material_Shininess_23);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(11.0f, 8.0f, 0.0f);

    gluSphere(quadric[23], 1.0f, 30, 30);




}

void update(void)
{
    if (Key_Pressed == 1)
    {
        angle_for_X_Rotation += 1.0f;
    }
    else if (Key_Pressed == 2)
    {
        angle_for_Y_Rotation += 1.0f;
    }
    else if (Key_Pressed == 3)
    {
        angle_for_Z_Rotation += 1.0f;
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
	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}

    	for (int i = 0; i < 24; i++)
    	{
        	gluDeleteQuadric(quadric[i]);
        	quadric[i] = NULL;
        
    	}
}


