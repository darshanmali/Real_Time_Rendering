#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h> /* Memset */
#include<math.h>

#include<X11/Xlib.h>  /* similar windows.h */
#include<X11/Xutil.h> /* XVisual info */
#include<X11/XKBlib.h> /* KeyBoard */
#include<X11/keysym.h> /* key Symbols for taking Event */

#include<GL/gl.h>/* for openGL API */
#include<GL/glu.h>/* Ortho view also for glu quadric and many more */
#include<GL/glx.h>/* for GLX API */

/*Library for OpenAL*/
#include<AL/alut.h>
#include<AL/al.h>

using namespace std;

#define PI 3.141592653589793238

#define ALPHA_FIRST_I 1
#define ALPHA_N 2
#define ALPHA_D 3
#define ALPHA_SECOND_I 4
#define ALPHA_A 5
#define COLOR_ORANGE 6
#define COLOR_GREEN 7
#define COLOR_WHITE 8

bool bFullscreen = true;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;
//Sound Stetus CHecking
ALuint source;

int i = 1;
GLfloat x = 0.0f, y = 0.0f;
bool flag = false;
GLfloat Jangle_T = 310.0f;
GLfloat Jangle_B = 50.0f;
bool Direct_T = false;
bool Direct_B = false;

// Flag for Music
bool first_I = false;
GLfloat N_P_3 = 1.21f;
GLfloat N_P_4 = 1.08f;
GLfloat N_P_6 = 1.08f;
GLfloat N_P_7 = 1.21f;

// Most Jet Params...
GLfloat fRedius = 0.0f,
fTrans_x = 0.0f,
fTrans_y = 0.0f,
fTrans_z = 0.0f,
fScale_x = 0.0f,
fScale_y = 0.0f,
fScale_z = 0.0f;

// Alphabets Parameters...
//I_1
GLfloat X_i = -1.7f,
Y_i = 0.0f,
Z_i = -3.0f;

//N
GLfloat X_n = -0.4f,
Y_n = 2.3f,
Z_n = -3.0f;

//D
GLfloat X_d = -0.35f,
Y_d = 0.0f,
Z_d = -3.0f;

//I Second
GLfloat X_i_2 = -0.3f,
Y_i_2 = -1.7f,
Z_i_2 = -3.0f;

//A 
GLfloat X_a = 1.45f,
Y_a = 0.0f,
Z_a = -3.0f;

//Printing Flags
bool flag_for_Jets = false;
bool flag_for_I_1 = false;
bool flag_for_n = false;
bool flag_for_d = false;
bool flag_for_i_2 = false;
bool flag_for_a = false;

//Close Call Flag
bool flag_Exit = false;


//D_Animated Colors
GLfloat Col_x = 0.0f;
GLfloat Col_y = 0.0f;
GLfloat Col_z = 0.0f;

//Green
GLfloat Col_y_G = 0.0f;

//White
GLfloat Col_x_w = 0.0f;
GLfloat Col_y_w = 0.0f;
GLfloat Col_z_w = 0.0f;


//Increment of X 
GLfloat fIncremet_x = 0.0f;
GLfloat fIncremet_y = 0.0f;

GLfloat fIncremet_x_Middle = 0.0f;

GLfloat fIncremet_x_Bottom = 0.0f;
GLfloat fIncremet_y_Bottom = 0.0f;

GLXContext gGLXContext;

int giWindowWidth = 800;
int giWindowHeight = 600;


//Global Function 
void Update(int, int);
void Moving(GLfloat *, GLfloat*, GLfloat*, int);

void Color_fads(GLfloat *,GLfloat *,GLfloat*,int, int);
void Size_Inc(GLfloat *, int);

//Alphabates Printing
void Print_I_1();
void Print_N();
void Print_D();
void Print_I_2();
void Print_A();
void Print_Top();
void Print_Bottom();


// My New Functions
void CircleDrawing(GLfloat *,GLfloat *, GLfloat*, GLfloat*, GLfloat*, GLfloat*,GLfloat*, bool, int);

void uninitialize(void);

int main(int argc, char **argv)
{
	/* Local Fuction Declaraton */
	void CreateWindow(void);
	void ToggleFullscreen(void);
	
	/* for game loop */
	bool bDone = false;

	/* Local Functions */
	void Initialize(void);
	void Resize(int , int);
	void Render(void);
	
	bool CreateALConection(int , char**);
	bool LoadWavFile(const char*);

	/* Error Cheching */
	bool Err_che;
	ALint status_Sound;

	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;

	//Graphics Context(Software HDC) 
	CreateWindow();
	
	
	Err_che = CreateALConection(argc, argv);
	if(Err_che == false)
	{
		uninitialize();
	}

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
				case MapNotify:
				flag_for_I_1 = true;
				Err_che = LoadWavFile( "New_3.wav");
				if(Err_che == false)
				{
					uninitialize();
				}
				break;
	
				/* KeyBoard Messages */
				case KeyPress:
				keysym = XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
	
				switch(keysym)
				{
					case XK_Escape:
					bDone = true;
					break;

					case XK_I:
					case XK_i:
					//first_I = true;
					
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
		if(source){
	      		alGetSourcei (source, AL_SOURCE_STATE, &status_Sound);
		}
  		if(status_Sound != AL_PLAYING && source)
		{
			bDone = true;
			alutExit();
		}	
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
	
	XStoreName ( gpDisplay, gWindow,"Darshan's : Dynamic India XWindows");
		
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
bool CreateALConection(int argc,char **argv)
{
	

	if (!alutInit(&argc, argv))
    	{
      		ALenum error = alutGetError ();
      		printf ("Error : %s\n", alutGetErrorString (error));
		uninitialize();
      		alutExit();
	}
	return(true);
}

bool LoadWavFile(const char* path)
{
	ALuint buffer;

	ALenum error;


	buffer = alutCreateBufferFromFile(path);
	if(buffer == AL_NONE)
	{
		error = alutGetError();
		printf("Error Loading file : %s\n", alutGetErrorString(error));
		alutExit();
      		uninitialize();
		return(false);
	}

	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	error = alGetError();
	if (error != ALUT_ERROR_NO_ERROR)
    	{
      		printf("Error : %s\n", alGetString (error));
      		alutExit ();
      		uninitialize();
    	}
	
	return (true);
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

	//variable Declaration
    	GLfloat x_new = 0.0f, y_new = 0.0f;

    	//Function Declaration 
    	void TopJetCall(void);
    	void MiddleJetCall(void);
	void BottomJetCall(void);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (flag_for_I_1)
    	{
        	// Printing I 1
	        Print_I_1();
    	}
    	if (flag_for_n)
    	{
    	    	// Printing N
    	    	Print_N();
    	}
    	if (flag_for_d)
    	{
    	    	Print_D();
    	}
    	if (flag_for_i_2)
    	{
    	    	// Printing I 2
    	    	Print_I_2();
    	}
    	if (flag_for_a)
    	{
    	    	// Printing A
    	    	Print_A();
    	}
    	if (flag_for_Jets == true)
    	{
    	    	//Top jet 1.
    	    	TopJetCall();
	
    	    	//Bottom jet 3.
    	    	BottomJetCall();
		
    	    	// Middle Jet 2.
    	    	MiddleJetCall();
    	}
	
	glXSwapBuffers(gpDisplay,gWindow);
}

void Update( int flag , int direction)
{
    if (flag == 1)
    {
        if (direction == 1)
        {
            if (Jangle_T >= 360.0f)
            {
                Jangle_T = 0.0f;
            }
            if (Jangle_T >= 345.0f)
            {
//I Incresed the angle 
                Jangle_T = Jangle_T + 0.055f;//0.015 , 0.025
            }
            else
            {
                Jangle_T = Jangle_T + 0.055f;//0.01; 0.025
            }
        }
        else if (direction == 2)
        {
            if (Jangle_T >= 335.0f)
            {
                Jangle_T = Jangle_T + 0.055f;//0.015 0.025
            }
            else
            {
                Jangle_T = Jangle_T + 0.07f;//0.02 0.035
            }
        }
    }
    if (flag == 3)
    {
        if (direction == 1)
        {
            if (Jangle_B >= 15.0f)
            {
                Jangle_B = Jangle_B - 0.055f;
            }
            else
            {
                Jangle_B = Jangle_B - 0.055f;
            }
        }
        else if (direction == 2)
        {
            if (Jangle_B >= 15.0f)
            {
                Jangle_B = Jangle_B - 0.055f;
            }
            else
            {
                Jangle_B = Jangle_B - 0.07f;
            }
        }
    }
}

void Moving(GLfloat *x, GLfloat* y, GLfloat* z,int alphabet)
{
    switch (alphabet)
    {
    case ALPHA_FIRST_I:
        if (*x <= -0.45f)
        {
	//first change delete 0 from increment
            *x = *x + 0.005f;
        }
        else
        {
            flag_for_n = true;
        }
    break;
    case ALPHA_N:
        if (*y >= 0.0f)
        {
            *y = *y - 0.005f;
           
        }
        else
        {
            flag_for_a = true;
        }
        break;
    case ALPHA_D:
        if (*z <= -3.0f)
        {
            *z = *z + 0.002f;
        }
        break;
    case ALPHA_SECOND_I:
        if (*y <= 0.0f)
        {
            *y = *y + 0.005f;
        }
        else
        {
            flag_for_d = true;
        }
        break;
    case ALPHA_A:
        if (*x >= -0.2f)
        {
            *x = *x - 0.005f;
        }
        else
        {
            flag_for_i_2 = true;
        }
        break;

    default:
        break;
    }
    
}

void Color_fads(GLfloat* x, GLfloat* y, GLfloat* z, int Color,int Alpha)
{
    if (Alpha == 1)
    {
        if (Color == COLOR_ORANGE)
        {
//Second change is here i deleted 0 from each increment 
            *x += 0.001f;
            *y += 0.0005f;
            *z += 0.0f;
        }
        if (Color == COLOR_GREEN)
        {
            *y += 0.001f;
        }

        if (Color == COLOR_WHITE)
        {
            *x += 0.001f;
            *y += 0.001f;
            *z += 0.001f;
        }
    }
    else if (Alpha == 2)
    {

    }
}

void Size_Inc( GLfloat *increment, int flag)
{
    if (flag == 1)
    {
        if (*increment <= 1.45f && N_P_4 >= 1.21f )
        {
            *increment = *increment + 0.0035f;
        }
    }
    if (flag == 2)
    {
        if (*increment <= 1.59f)
        {
            *increment = *increment + 0.0035f;
        }
    }
    if (flag == 3)
    {
        if (*increment <= 1.46f && N_P_6 >= 1.21f)
        {
            *increment = *increment + 0.0035f;
        }
    }
    if (flag == 4)
    {
        if (*increment <= 1.59f)
        {
            *increment = *increment + 0.0035f;
        }
    }
}

void Print_I_1()
{

    Moving( &X_i, &Y_i, &Z_i, ALPHA_FIRST_I);

    glTranslatef(X_i, Y_i, Z_i);
    glBegin(GL_QUADS);

    //Top of I
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-1.0f, 0.4f, 0.0f);
    glVertex3f(-0.9f, 0.3f, 0.0f);

    glVertex3f(-0.7f, 0.3f, 0.0f);
    glVertex3f(-0.6f, 0.4f, 0.0f);

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
}

void Print_N()
{
    Moving(&X_n, &Y_n, &Z_n, ALPHA_N);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(X_n, Y_n, Z_n);

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
}

void Print_D()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    glTranslatef(X_d, Y_d, Z_d);
    

    //D Began
    glBegin(GL_QUADS);
    Color_fads(&Col_x, &Col_y, &Col_z, COLOR_ORANGE,1);

   // glColor3f(Col_x, Col_y, Col_z);
    if (Col_x <= 1.0f || Col_y < 0.5f)
    {
        glColor3f(Col_x, Col_y, Col_z);
    }
    else
    {
        flag_for_Jets = true;
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    
    //D Top
    glVertex3f(0.44f, 0.4f, 0.0f);
    glVertex3f(0.05f, 0.4f, 0.0f);

    glVertex3f(-0.05f, 0.3f, 0.0f);
    glVertex3f(0.34f, 0.3f, 0.0f);

    //D Middle
    glVertex3f(0.1f, 0.3f, 0.0f);
    glVertex3f(0.2f, 0.3f, 0.0f);

    //White
    Color_fads(&Col_x_w, &Col_y_w, &Col_z_w, COLOR_WHITE, 1);

    if (Col_x_w <= 1.0f && Col_y_w <= 1.0f && Col_z_w <= 1.0f)
    {
        glColor3f(Col_x_w, Col_y_w, Col_z_w);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glVertex3f(0.2f, 0.0f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);

    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.0f);

    Color_fads(&Col_x, &Col_y_G, &Col_z, COLOR_GREEN, 1);

    if (Col_y_G <= 1.0f)
    {
        glColor3f(0.0f, Col_y_G, 0.0f);
    }
    else
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    glVertex3f(0.2f, -0.3f, 0.0f);
    glVertex3f(0.1f, -0.3f, 0.0f);

    //D Right
    if (Col_x <= 1.0f || Col_y < 0.5f)
    {
        glColor3f(Col_x, Col_y, Col_z);
    }
    else
    {
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    glVertex3f(0.45f, 0.4f, 0.0f);
    glVertex3f(0.35f, 0.3f, 0.0f);

    Color_fads(&Col_x_w, &Col_y_w, &Col_z_w, COLOR_WHITE, 1);

    if (Col_x_w <= 1.0f && Col_y_w <= 1.0f && Col_z_w <= 1.0f)
    {
        glColor3f(Col_x_w, Col_y_w, Col_z_w);
    }
    else
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(0.45f, 0.0f, 0.0f);

    glVertex3f(0.45f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);

    Color_fads(&Col_x, &Col_y_G, &Col_z, COLOR_GREEN, 1);

    if (Col_y_G <= 1.0f)
    {
        glColor3f(0.0f, Col_y_G, 0.0f);
    }
    else
    {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    glVertex3f(0.35f, -0.3f, 0.0f);
    glVertex3f(0.45f, -0.4f, 0.0f);

    //D bOTTOM
    glVertex3f(0.44f, -0.4f, 0.0f);
    glVertex3f(0.05f, -0.4f, 0.0f);

    glVertex3f(-0.05f, -0.3f, 0.0f);
    glVertex3f(0.34f, -0.3f, 0.0f);

    glEnd();
}

void Print_I_2()
{
    Moving(&X_i_2, &Y_i_2, &Z_i_2, ALPHA_SECOND_I);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(X_i_2, Y_i_2, Z_i_2);

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
}

void Print_A()
{
   Moving(&X_a, &Y_a, &Z_a, ALPHA_A);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(0.8f, 0.9f, 0.9f);

    glTranslatef(X_a, Y_a, Z_a);
    
    glBegin(GL_QUADS);
    //A Right 1

    glColor3f(1.1f, 0.5f, 0.0f);
    glVertex3f(1.46f, 0.4f, 0.0f);
    glVertex3f(1.36f, 0.3f, 0.0f);

    glColor3f(1.1f, 1.0f, 1.0f);
    glVertex3f(1.45f, 0.05f, 0.0f);
    glVertex3f(1.6f, 0.0f, 0.0f);

    //A right 2

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.75f, -0.4f, 0.0f);
    glVertex3f(1.6f, -0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.48f, -0.04f, 0.0f);
    glVertex3f(1.6f, 0.0f, 0.0f);

    glEnd();

    //Left 1
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.18f, 0.4f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.07f, 0.0f, 0.0f);
    glVertex3f(1.21f, 0.05f, 0.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.28f, 0.3f, 0.0f);

    //Left 2

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.07f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.97f, -0.4f, 0.0f);
    glVertex3f(1.105f, -0.3f, 0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.19f, -0.045f, 0.0f);

    glEnd();
    //Top Line
    Print_Top();
    //Bottom Line
    //Print_Bottom();

}

void Print_Top()
{
    // - //
    glBegin(GL_QUADS);
    //L B
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.19f, 0.4f, 0.0f);
    glVertex3f(1.45F, 0.4f, 0.0f);
    glVertex3f(1.35f, 0.3f, 0.0f);
    glVertex3f(1.29f, 0.3f, 0.0f);
   
    glEnd();
}

void Print_Bottom()
{
    
    // Center
    glBegin(GL_POLYGON);

    //L CORNER
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.08f, 0.0f, 0.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.21f, 0.04f, 0.0f);
    
    Size_Inc(&N_P_3, 1);
    glVertex3f(N_P_3, 0.04f, 0.0f);
    
    Size_Inc(&N_P_4, 2);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(N_P_4, 0.0f, 0.0f);
    glEnd();

    ////R CORNER
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.08f, 0.0f, 0.0f);
    
    Size_Inc(&N_P_6, 4);
    glVertex3f(N_P_6, 0.0f, 0.0f);//59

    glColor3f(0.0f, 1.0f, 0.0f);

    Size_Inc(&N_P_7, 3);
    glVertex3f(N_P_7, -0.04f, 0.0f);//08

    glVertex3f(1.2f, -0.04f, 0.0f);
    
    glEnd();
}

void TopJetCall(void)
{
    //Local Variables 
    fRedius = 0.3f;
    fTrans_x = -14.0f;
    fTrans_y = 9.0f;
    fTrans_z = -16.0f;
    fScale_x = 5.0f;
    fScale_y = 0.55f;
    fScale_z = 0.0f;

    if (fIncremet_x <= 28.0f)
    {
        if (fIncremet_x >= 21.0f && fIncremet_x <= 26.0f)
        {
//again third change here i deleted 0 from each increment 0.02 from 0.002
            Update(1,2);
            Direct_T = true;
            fIncremet_x = fIncremet_x + 0.01f;//0.002
            fTrans_x = fTrans_x + fIncremet_x;
            
        }
        else
        {
            fIncremet_x = fIncremet_x + 0.01f;
            fTrans_x = fTrans_x + fIncremet_x;
        }
        if (fIncremet_x >= 20.0f)
        {
            Print_Bottom();
        }
    }
    else
    {
        Print_Bottom();
        fTrans_x = 14.0f;
        flag_Exit = true;
        //SendMessage(ghwnd_DM, WM_LBUTTONDOWN, 0, 0);
    }

    if (fIncremet_y >= 0.0f && Direct_T == true)
    {
        fIncremet_y = fIncremet_y - 0.01f;//0.002
        fTrans_y = fTrans_y - fIncremet_y;
    }
    else if(fIncremet_y <= 9.0f && Direct_T == false)
    {
        fIncremet_y = fIncremet_y + 0.01f;//0.002
        fTrans_y = fTrans_y - fIncremet_y;
        Update(1,1);
    }
    else if(fIncremet_x <= 21.0f && fIncremet_x >= -4.0f)
    {
        fTrans_y = 0.0f;
    }
    else if (fIncremet_x <= 21.0f && fIncremet_x >= 28.0f)
    {
        fTrans_y = 9.0f;
    }
    
    if (fTrans_x <= 28.0f)
        CircleDrawing(&fRedius, &fTrans_x, &fTrans_y, &fTrans_z, &fScale_x, &fScale_y, &fScale_z, true, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (fTrans_x <= 28.0f)
    {
        glTranslatef(fTrans_x, fTrans_y, fTrans_z);
        glRotatef(Jangle_T, 0.0f, 0.0f, 1.0f);
    }
    //top most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, 0.965f, 0.0f);
    glVertex3f(0.25f, 0.965f, 0.0f);
    glVertex3f(0.4f, 1.005f, 0.0f);
    glVertex3f(0.4f, 1.01f, 0.0f);
    glVertex3f(0.25f, 1.05f, 0.0f);
    glVertex3f(-0.15f, 1.05f, 0.0f);

    glEnd();

    //second top
    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, 0.635f, 0.0f);
    glVertex3f(0.25f, 0.635f, 0.0f);
    glVertex3f(0.4f, 0.675f, 0.0f);
    glVertex3f(0.4f, 0.685f, 0.0f);
    glVertex3f(0.25f, 0.725f, 0.0f);
    glVertex3f(-0.1f, 0.725f, 0.0f);

    glEnd();

    //Second bottom  

    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, -0.635f, 0.0f);
    glVertex3f(0.25f, -0.635f, 0.0f);
    glVertex3f(0.4f, -0.675f, 0.0f);
    glVertex3f(0.4f, -0.685f, 0.0f);
    glVertex3f(0.25f, -0.725f, 0.0f);
    glVertex3f(-0.1f, -0.725f, 0.0f);

    glEnd();

    //Bottomf most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, -0.965f, 0.0f);
    glVertex3f(0.25f, -0.965f, 0.0f);
    glVertex3f(0.4f, -1.005f, 0.0f);
    glVertex3f(0.4f, -1.01f, 0.0f);
    glVertex3f(0.25f, -1.05f, 0.0f);
    glVertex3f(-0.15f, -1.05f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(1.09f, 0.01f, 0.0f);
    glVertex3f(1.08f, 0.03f, 0.0f);
    glVertex3f(1.04f, 0.067f, 0.0f);
    glVertex3f(1.02f, 0.07f, 0.0f);
    glVertex3f(1.02f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.08f, 0.0f);

    glVertex3f(0.7f, 0.08f, 0.0f);
    glVertex3f(0.65f, 0.07f, 0.0f);
    glVertex3f(0.64f, 0.06f, 0.0f);
    glVertex3f(0.63f, 0.05f, 0.0f);
    glVertex3f(0.62f, 0.025f, 0.0f);
    glVertex3f(0.61f, 0.01f, 0.0f);
    glVertex3f(0.61f, -0.01f, 0.0f);
    glVertex3f(0.62f, -0.025f, 0.0f);
    glVertex3f(0.63f, -0.05f, 0.0f);
    glVertex3f(0.64f, -0.06f, 0.0f);
    glVertex3f(0.65f, -0.07f, 0.0f);
    glVertex3f(0.7f, -0.08f, 0.0f);

    glVertex3f(1.0f, -0.08f, 0.0f);
    glVertex3f(1.01f, -0.08f, 0.0f);
    glVertex3f(1.02f, -0.07f, 0.0f);
    glVertex3f(1.04f, -0.067f, 0.0f);
    glVertex3f(1.06f, -0.05f, 0.0f);
    glVertex3f(1.08f, -0.03f, 0.0f);
    glVertex3f(1.09f, -0.01f, 0.0f);
    glEnd();

    //Door Lines For Window.
    glColor3f(0.55f, 0.6f, 0.7f);
    glLineWidth(2.0f);


    glBegin(GL_LINES);

    glVertex3f(1.5f, 0.0f, 0.0f);
    glVertex3f(1.7f, 0.0f, 0.0f);

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.75f, 0.08f, 0.0f);
    glVertex3f(0.8f, 0.05f, 0.0f);

    glVertex3f(0.8f, -0.05f, 0.0f);
    glVertex3f(0.75f, -0.08f, 0.0f);

    glEnd();
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.95f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.05f, 0.0f);

    glVertex3f(1.0f, -0.05f, 0.0f);
    glVertex3f(0.95f, -0.08f, 0.0f);

    glEnd();
    glColor3f(0.55f, 0.6f, 0.7f);

    // These are Two main Big Wings of jet.........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, 0.2f, 0.0f);//5
    glVertex3f(-0.15f, 1.05f, 0.0f);//-05
    glVertex3f(-0.4f, 1.05f, 0.0f);//-3
    glVertex3f(-0.175f, 0.2f, 0.0f);//-075

    glEnd();
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, -0.2f, 0.0f);//5
    glVertex3f(-0.15f, -1.05f, 0.0f);//-05
    glVertex3f(-0.4f, -1.05f, 0.0f);//-3
    glVertex3f(-0.175f, -0.2f, 0.0f);//-075


    glEnd();


    // Middle body of the Jet..........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 0.15f, 0.0f);
    glVertex3f(0.4f, 0.3f, 0.0f);
    glVertex3f(-0.7f, 0.3f, 0.0f);

    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(0.4f, -0.3f, 0.0f);
    glVertex3f(0.6f, -0.15f, 0.0f);

    glEnd();

    //These are two wings of the Jet (last Small)..............
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, 0.3f, 0.0f);//45
    glVertex3f(-0.65f, 0.8f, 0.0f);//65
    glVertex3f(-0.85f, 0.7f, 0.0f);//9
    glVertex3f(-0.84f, 0.43f, 0.0f);//89
    glVertex3f(-0.7f, 0.3f, 0.0f);//7

    glEnd();

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, -0.3f, 0.0f);//45
    glVertex3f(-0.65f, -0.8f, 0.0f);//65
    glVertex3f(-0.85f, -0.7f, 0.0f);//9
    glVertex3f(-0.84f, -0.43f, 0.0f);//89
    glVertex3f(-0.7f, -0.3f, 0.0f);//7

    glEnd();


    //These are three Last burnes of the jett.................
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.29f, 0.0f);//25
    glVertex3f(-1.0f, 0.34f, 0.0f);//4
    glVertex3f(-1.0f, 0.09f, 0.0f);//5
    glVertex3f(-0.7f, 0.13f, 0.0f);//4

    glEnd();
   
    
    
    //smoke
    glBegin(GL_QUADS);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-1.0f, 0.05f, 0.0f);//25
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.6f, 0.05f, 0.0f);//4
    glVertex3f(-1.6f, -0.025f, 0.0f);//4
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(-1.0f, -0.025f, 0.0f);//4
    glEnd();


    glColor3f(0.55f, 0.6f, 0.7f);

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, -0.29f, 0.0f);//25
    glVertex3f(-1.0f, -0.34f, 0.0f);//4
    glVertex3f(-1.0f, -0.09f, 0.0f);//5
    glVertex3f(-0.7f, -0.13f, 0.0f);//4

    glEnd();

    //3.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.05f, 0.0f);//25
    glVertex3f(-1.1f, 0.05f, 0.0f);//4
    glVertex3f(-1.2f, 0.025f, 0.0f);//4
    glVertex3f(-1.2f, -0.025f, 0.0f);//4
    glVertex3f(-1.1f, -0.05f, 0.0f);//5
    glVertex3f(-0.7f, -0.05f, 0.0f);//4

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // I
    glVertex3f(-0.35f, 0.08f, 0.0f);
    glVertex3f(-0.25f, 0.08f, 0.0f);

    glVertex3f(-0.35f, -0.08f, 0.0f);
    glVertex3f(-0.25f, -0.08f, 0.0f);

    glVertex3f(-0.3f, 0.08f, 0.0f);
    glVertex3f(-0.3f, -0.08f, 0.0f);

    // A
    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.2f, -0.08f, 0.0f);

    glVertex3f(-0.17f, 0.006f, 0.0f);
    glVertex3f(-0.1f, 0.006f, 0.0f);

    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.08f, -0.08f, 0.0f);

    // F
    glVertex3f(0.0f, -0.08f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glVertex3f(0.0f, 0.09f, 0.0f);
    glVertex3f(0.1f, 0.09f, 0.0f);

    glVertex3f(0.0f, 0.015f, 0.0f);
    glVertex3f(0.1f, 0.015f, 0.0f);


    glEnd();

}

void MiddleJetCall(void)
{
    fRedius = 0.3f;
    fTrans_x = -14.0f;
    fTrans_y = 0.0f;
    fTrans_z = -16.0f;
    fScale_x = 5.0f;
    fScale_y = 0.55f;
    fScale_z = 0.0f;

    if (fIncremet_x_Middle <= 28.0f)
    {
//chege occure
        fIncremet_x_Middle = fIncremet_x_Middle + 0.01f;
        fTrans_x = fTrans_x + fIncremet_x_Middle;
    }
    else
    {
        fTrans_x = 14.0f;
    }

    CircleDrawing(&fRedius, &fTrans_x, &fTrans_y, &fTrans_z, &fScale_x, &fScale_y, &fScale_z, false, 2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(fTrans_x, fTrans_y, fTrans_z);

    //top most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, 0.965f, 0.0f);
    glVertex3f(0.25f, 0.965f, 0.0f);
    glVertex3f(0.4f, 1.005f, 0.0f);
    glVertex3f(0.4f, 1.01f, 0.0f);
    glVertex3f(0.25f, 1.05f, 0.0f);
    glVertex3f(-0.15f, 1.05f, 0.0f);

    glEnd();

    //second top
    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, 0.635f, 0.0f);
    glVertex3f(0.25f, 0.635f, 0.0f);
    glVertex3f(0.4f, 0.675f, 0.0f);
    glVertex3f(0.4f, 0.685f, 0.0f);
    glVertex3f(0.25f, 0.725f, 0.0f);
    glVertex3f(-0.1f, 0.725f, 0.0f);

    glEnd();

    //Second bottom  

    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, -0.635f, 0.0f);
    glVertex3f(0.25f, -0.635f, 0.0f);
    glVertex3f(0.4f, -0.675f, 0.0f);
    glVertex3f(0.4f, -0.685f, 0.0f);
    glVertex3f(0.25f, -0.725f, 0.0f);
    glVertex3f(-0.1f, -0.725f, 0.0f);

    glEnd();

    //Bottomf most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, -0.965f, 0.0f);
    glVertex3f(0.25f, -0.965f, 0.0f);
    glVertex3f(0.4f, -1.005f, 0.0f);
    glVertex3f(0.4f, -1.01f, 0.0f);
    glVertex3f(0.25f, -1.05f, 0.0f);
    glVertex3f(-0.15f, -1.05f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(1.09f, 0.01f, 0.0f);
    glVertex3f(1.08f, 0.03f, 0.0f);
    glVertex3f(1.04f, 0.067f, 0.0f);
    glVertex3f(1.02f, 0.07f, 0.0f);
    glVertex3f(1.02f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.08f, 0.0f);

    glVertex3f(0.7f, 0.08f, 0.0f);
    glVertex3f(0.65f, 0.07f, 0.0f);
    glVertex3f(0.64f, 0.06f, 0.0f);
    glVertex3f(0.63f, 0.05f, 0.0f);
    glVertex3f(0.62f, 0.025f, 0.0f);
    glVertex3f(0.61f, 0.01f, 0.0f);
    glVertex3f(0.61f, -0.01f, 0.0f);
    glVertex3f(0.62f, -0.025f, 0.0f);
    glVertex3f(0.63f, -0.05f, 0.0f);
    glVertex3f(0.64f, -0.06f, 0.0f);
    glVertex3f(0.65f, -0.07f, 0.0f);
    glVertex3f(0.7f, -0.08f, 0.0f);

    glVertex3f(1.0f, -0.08f, 0.0f);
    glVertex3f(1.01f, -0.08f, 0.0f);
    glVertex3f(1.02f, -0.07f, 0.0f);
    glVertex3f(1.04f, -0.067f, 0.0f);
    glVertex3f(1.06f, -0.05f, 0.0f);
    glVertex3f(1.08f, -0.03f, 0.0f);
    glVertex3f(1.09f, -0.01f, 0.0f);
    glEnd();

    //Door Lines For Window.
    glColor3f(0.55f, 0.6f, 0.7f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(1.5f, 0.0f, 0.0f);
    glVertex3f(1.7f, 0.0f, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.75f, 0.08f, 0.0f);
    glVertex3f(0.8f, 0.05f, 0.0f);

    glVertex3f(0.8f, -0.05f, 0.0f);
    glVertex3f(0.75f, -0.08f, 0.0f);

    glEnd();
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.95f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.05f, 0.0f);

    glVertex3f(1.0f, -0.05f, 0.0f);
    glVertex3f(0.95f, -0.08f, 0.0f);

    glEnd();
    glColor3f(0.55f, 0.6f, 0.7f);

    // These are Two main Big Wings of jet.........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, 0.2f, 0.0f);//5
    glVertex3f(-0.15f, 1.05f, 0.0f);//-05
    glVertex3f(-0.4f, 1.05f, 0.0f);//-3
    glVertex3f(-0.175f, 0.2f, 0.0f);//-075

    glEnd();
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, -0.2f, 0.0f);//5
    glVertex3f(-0.15f, -1.05f, 0.0f);//-05
    glVertex3f(-0.4f, -1.05f, 0.0f);//-3
    glVertex3f(-0.175f, -0.2f, 0.0f);//-075


    glEnd();


    // Middle body of the Jet..........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 0.15f, 0.0f);
    glVertex3f(0.4f, 0.3f, 0.0f);
    glVertex3f(-0.7f, 0.3f, 0.0f);

    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(0.4f, -0.3f, 0.0f);
    glVertex3f(0.6f, -0.15f, 0.0f);

    glEnd();

    //These are two wings of the Jet (last Small)..............
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, 0.3f, 0.0f);//45
    glVertex3f(-0.65f, 0.8f, 0.0f);//65
    glVertex3f(-0.85f, 0.7f, 0.0f);//9
    glVertex3f(-0.84f, 0.43f, 0.0f);//89
    glVertex3f(-0.7f, 0.3f, 0.0f);//7

    glEnd();

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, -0.3f, 0.0f);//45
    glVertex3f(-0.65f, -0.8f, 0.0f);//65
    glVertex3f(-0.85f, -0.7f, 0.0f);//9
    glVertex3f(-0.84f, -0.43f, 0.0f);//89
    glVertex3f(-0.7f, -0.3f, 0.0f);//7

    glEnd();


    //These are three Last burnes of the jett.................
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.29f, 0.0f);//25
    glVertex3f(-1.0f, 0.34f, 0.0f);//4
    glVertex3f(-1.0f, 0.09f, 0.0f);//5
    glVertex3f(-0.7f, 0.13f, 0.0f);//4

    glEnd();

    if (fIncremet_x >= 9.0f && fIncremet_x <= 22.0f)
    {
        //smoke
        glBegin(GL_QUADS);

        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.0f, 0.125f, 0.0f);//25

        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-1.6f, 0.125f, 0.0f);//4
        glVertex3f(-1.6f, 0.05f, 0.0f);//4

        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-0.8f, 0.05f, 0.0f);//4
        glEnd();
    }

    //smoke
    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 0.05f, 0.0f);//25
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.6f, 0.05f, 0.0f);//4
    glVertex3f(-1.6f, -0.025f, 0.0f);//4
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.8f, -0.025f, 0.0f);//4

    glEnd();
    if (fIncremet_x >= 9.0f && fIncremet_x <= 22.0f)
    {
        //smoke
        glBegin(GL_QUADS);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -0.025f, 0.0f);//25
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(-1.6f, -0.025f, 0.0f);//4
        glVertex3f(-1.6f, -0.095f, 0.0f);//4
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.8f, -0.095f, 0.0f);//4
        
        glEnd();
    }

    glColor3f(0.55f, 0.6f, 0.7f);
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, -0.29f, 0.0f);//25
    glVertex3f(-1.0f, -0.34f, 0.0f);//4
    glVertex3f(-1.0f, -0.09f, 0.0f);//5
    glVertex3f(-0.7f, -0.13f, 0.0f);//4

    glEnd();

    //3.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.05f, 0.0f);//25
    glVertex3f(-1.1f, 0.05f, 0.0f);//4
    glVertex3f(-1.2f, 0.025f, 0.0f);//4
    glVertex3f(-1.2f, -0.025f, 0.0f);//4
    glVertex3f(-1.1f, -0.05f, 0.0f);//5
    glVertex3f(-0.7f, -0.05f, 0.0f);//4

    glEnd();

    

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // I
    glVertex3f(-0.35f, 0.08f, 0.0f);
    glVertex3f(-0.25f, 0.08f, 0.0f);

    glVertex3f(-0.35f, -0.08f, 0.0f);
    glVertex3f(-0.25f, -0.08f, 0.0f);

    glVertex3f(-0.3f, 0.08f, 0.0f);
    glVertex3f(-0.3f, -0.08f, 0.0f);

    // A
    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.2f, -0.08f, 0.0f);

    glVertex3f(-0.17f, 0.006f, 0.0f);
    glVertex3f(-0.1f, 0.006f, 0.0f);

    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.08f, -0.08f, 0.0f);

    // F
    glVertex3f(0.0f, -0.08f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glVertex3f(0.0f, 0.09f, 0.0f);
    glVertex3f(0.1f, 0.09f, 0.0f);

    glVertex3f(0.0f, 0.015f, 0.0f);
    glVertex3f(0.1f, 0.015f, 0.0f);

    glEnd();
}

void BottomJetCall(void)
{
    fRedius = 0.3f;
    fTrans_x = -14.0f;
    fTrans_y = -9.0f;
    fTrans_z = -16.0f;
    fScale_x = 5.0f;
    fScale_y = 0.55f;
    fScale_z = 0.0f;

    if (fIncremet_x_Bottom <= 28.0f)
    {
        if (fIncremet_x_Bottom >= 21.0f && fIncremet_x_Bottom <= 28.0f)
        {
//i changed the increment
            Update(3, 2);
            Direct_B = true;
            fIncremet_x_Bottom = fIncremet_x_Bottom + 0.01f;
            fTrans_x = fTrans_x + fIncremet_x_Bottom;
        }
        else
        {
            fIncremet_x_Bottom = fIncremet_x_Bottom + 0.01f;
            fTrans_x = fTrans_x + fIncremet_x_Bottom;
        }
    }
    else
    {
        fTrans_x = 18.0f;
    }

    if (fIncremet_y_Bottom >= 0.0f && Direct_B == true)
    {
        fIncremet_y_Bottom = fIncremet_y_Bottom - 0.01f;
        fTrans_y = fTrans_y + fIncremet_y_Bottom;
    }
    else if (fIncremet_y_Bottom <= 9.0f && Direct_B == false)
    {
        fIncremet_y_Bottom = fIncremet_y_Bottom + 0.01f;
        fTrans_y = fTrans_y + fIncremet_y_Bottom;
        Update(3,1);
    }
    else if (fIncremet_x_Bottom <= 21.0f && fIncremet_x_Bottom >= -4.0f)
    {
        fTrans_y = 0.0f;
    }
    else if (fIncremet_x_Bottom <= 28.0f && fIncremet_x_Bottom >= 21.0f)
    {
        fTrans_y = -9.0f;
    }

    if (fTrans_x <= 14.0f)
        CircleDrawing(&fRedius, &fTrans_x, &fTrans_y, &fTrans_z, &fScale_x, &fScale_y, &fScale_z, true , 3);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (fTrans_x <= 14.0f)
    {
        glTranslatef(fTrans_x, fTrans_y, fTrans_z);
        glRotatef(Jangle_B, 0.0f, 0.0f, 1.0f);
    }

    //top most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, 0.965f, 0.0f);
    glVertex3f(0.25f, 0.965f, 0.0f);
    glVertex3f(0.4f, 1.005f, 0.0f);
    glVertex3f(0.4f, 1.01f, 0.0f);
    glVertex3f(0.25f, 1.05f, 0.0f);
    glVertex3f(-0.15f, 1.05f, 0.0f);

    glEnd();

    //second top
    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, 0.635f, 0.0f);
    glVertex3f(0.25f, 0.635f, 0.0f);
    glVertex3f(0.4f, 0.675f, 0.0f);
    glVertex3f(0.4f, 0.685f, 0.0f);
    glVertex3f(0.25f, 0.725f, 0.0f);
    glVertex3f(-0.1f, 0.725f, 0.0f);

    glEnd();

    //Second bottom  

    glBegin(GL_POLYGON);

    glVertex3f(-0.1f, -0.635f, 0.0f);
    glVertex3f(0.25f, -0.635f, 0.0f);
    glVertex3f(0.4f, -0.675f, 0.0f);
    glVertex3f(0.4f, -0.685f, 0.0f);
    glVertex3f(0.25f, -0.725f, 0.0f);
    glVertex3f(-0.1f, -0.725f, 0.0f);

    glEnd();

    //Bottomf most
    glBegin(GL_POLYGON);

    glVertex3f(-0.15f, -0.965f, 0.0f);
    glVertex3f(0.25f, -0.965f, 0.0f);
    glVertex3f(0.4f, -1.005f, 0.0f);
    glVertex3f(0.4f, -1.01f, 0.0f);
    glVertex3f(0.25f, -1.05f, 0.0f);
    glVertex3f(-0.15f, -1.05f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(1.09f, 0.01f, 0.0f);
    glVertex3f(1.08f, 0.03f, 0.0f);
    glVertex3f(1.04f, 0.067f, 0.0f);
    glVertex3f(1.02f, 0.07f, 0.0f);
    glVertex3f(1.02f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.08f, 0.0f);

    glVertex3f(0.7f, 0.08f, 0.0f);
    glVertex3f(0.65f, 0.07f, 0.0f);
    glVertex3f(0.64f, 0.06f, 0.0f);
    glVertex3f(0.63f, 0.05f, 0.0f);
    glVertex3f(0.62f, 0.025f, 0.0f);
    glVertex3f(0.61f, 0.01f, 0.0f);
    glVertex3f(0.61f, -0.01f, 0.0f);
    glVertex3f(0.62f, -0.025f, 0.0f);
    glVertex3f(0.63f, -0.05f, 0.0f);
    glVertex3f(0.64f, -0.06f, 0.0f);
    glVertex3f(0.65f, -0.07f, 0.0f);
    glVertex3f(0.7f, -0.08f, 0.0f);

    glVertex3f(1.0f, -0.08f, 0.0f);
    glVertex3f(1.01f, -0.08f, 0.0f);
    glVertex3f(1.02f, -0.07f, 0.0f);
    glVertex3f(1.04f, -0.067f, 0.0f);
    glVertex3f(1.06f, -0.05f, 0.0f);
    glVertex3f(1.08f, -0.03f, 0.0f);
    glVertex3f(1.09f, -0.01f, 0.0f);
    glEnd();

    //Door Lines For Window.
    glColor3f(0.55f, 0.6f, 0.7f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(1.5f, 0.0f, 0.0f);
    glVertex3f(1.7f, 0.0f, 0.0f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.75f, 0.08f, 0.0f);
    glVertex3f(0.8f, 0.05f, 0.0f);

    glVertex3f(0.8f, -0.05f, 0.0f);
    glVertex3f(0.75f, -0.08f, 0.0f);

    glEnd();
    glBegin(GL_LINE_STRIP);

    glVertex3f(0.95f, 0.08f, 0.0f);
    glVertex3f(1.0f, 0.05f, 0.0f);

    glVertex3f(1.0f, -0.05f, 0.0f);
    glVertex3f(0.95f, -0.08f, 0.0f);

    glEnd();
    glColor3f(0.55f, 0.6f, 0.7f);

    // These are Two main Big Wings of jet.........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, 0.2f, 0.0f);//5
    glVertex3f(-0.15f, 1.05f, 0.0f);//-05
    glVertex3f(-0.4f, 1.05f, 0.0f);//-3
    glVertex3f(-0.175f, 0.2f, 0.0f);//-075

    glEnd();
    //2.
    glBegin(GL_POLYGON);

    glVertex3f(0.4f, -0.2f, 0.0f);//5
    glVertex3f(-0.15f, -1.05f, 0.0f);//-05
    glVertex3f(-0.4f, -1.05f, 0.0f);//-3
    glVertex3f(-0.175f, -0.2f, 0.0f);//-075


    glEnd();


    // Middle body of the Jet..........
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(0.6f, 0.15f, 0.0f);
    glVertex3f(0.4f, 0.3f, 0.0f);
    glVertex3f(-0.7f, 0.3f, 0.0f);

    glVertex3f(-0.7f, -0.3f, 0.0f);
    glVertex3f(0.4f, -0.3f, 0.0f);
    glVertex3f(0.6f, -0.15f, 0.0f);

    glEnd();

    //These are two wings of the Jet (last Small)..............
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, 0.3f, 0.0f);//45
    glVertex3f(-0.65f, 0.8f, 0.0f);//65
    glVertex3f(-0.85f, 0.7f, 0.0f);//9
    glVertex3f(-0.84f, 0.43f, 0.0f);//89
    glVertex3f(-0.7f, 0.3f, 0.0f);//7

    glEnd();

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.4f, -0.3f, 0.0f);//45
    glVertex3f(-0.65f, -0.8f, 0.0f);//65
    glVertex3f(-0.85f, -0.7f, 0.0f);//9
    glVertex3f(-0.84f, -0.43f, 0.0f);//89
    glVertex3f(-0.7f, -0.3f, 0.0f);//7

    glEnd();


    //These are three Last burnes of the jett.................
    //1.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.29f, 0.0f);//25
    glVertex3f(-1.0f, 0.34f, 0.0f);//4
    glVertex3f(-1.0f, 0.09f, 0.0f);//5
    glVertex3f(-0.7f, 0.13f, 0.0f);//4

    glEnd();

    //smoke
    glBegin(GL_QUADS);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.05f, 0.0f);//25
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.6f, 0.05f, 0.0f);//4
    glVertex3f(-1.6f, -0.025f, 0.0f);//4
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -0.025f, 0.0f);//4
    glEnd();

    glColor3f(0.55f, 0.6f, 0.7f);

    //2.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, -0.29f, 0.0f);//25
    glVertex3f(-1.0f, -0.34f, 0.0f);//4
    glVertex3f(-1.0f, -0.09f, 0.0f);//5
    glVertex3f(-0.7f, -0.13f, 0.0f);//4

    glEnd();

    //3.
    glBegin(GL_POLYGON);

    glVertex3f(-0.7f, 0.05f, 0.0f);//25
    glVertex3f(-1.1f, 0.05f, 0.0f);//4
    glVertex3f(-1.2f, 0.025f, 0.0f);//4
    glVertex3f(-1.2f, -0.025f, 0.0f);//4
    glVertex3f(-1.1f, -0.05f, 0.0f);//5
    glVertex3f(-0.7f, -0.05f, 0.0f);//4

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    // I
    glVertex3f(-0.35f, 0.08f, 0.0f);
    glVertex3f(-0.25f, 0.08f, 0.0f);

    glVertex3f(-0.35f, -0.08f, 0.0f);
    glVertex3f(-0.25f, -0.08f, 0.0f);

    glVertex3f(-0.3f, 0.08f, 0.0f);
    glVertex3f(-0.3f, -0.08f, 0.0f);

    // A
    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.2f, -0.08f, 0.0f);

    glVertex3f(-0.17f, 0.006f, 0.0f);
    glVertex3f(-0.1f, 0.006f, 0.0f);

    glVertex3f(-0.14f, 0.1f, 0.0f);
    glVertex3f(-0.08f, -0.08f, 0.0f);

    // F
    glVertex3f(0.0f, -0.08f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);

    glVertex3f(0.0f, 0.09f, 0.0f);
    glVertex3f(0.1f, 0.09f, 0.0f);

    glVertex3f(0.0f, 0.015f, 0.0f);
    glVertex3f(0.1f, 0.015f, 0.0f);

    glEnd();

}

void CircleDrawing(GLfloat* redius, GLfloat* fTrans_x, GLfloat* fTrans_y, GLfloat* fTrans_z, GLfloat* fScale_x, GLfloat* fScale_y, GLfloat* fScale_z, bool flag, int position)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

        glTranslatef(*fTrans_x, *fTrans_y, *fTrans_z);
        if (flag == true)
        {
            if(position == 1)
                glRotatef(Jangle_T, 0.0f, 0.0f, 1.0f);
            if (position == 2)
            {
                //glRotatef(Jangle_M, 0.0f, 0.0f, 1.0f);
            }
            if (position == 3)
            {
                glRotatef(Jangle_B, 0.0f, 0.0f, 1.0f);
            }
        }
        glScalef(*fScale_x, *fScale_y, *fScale_z);
    
        glBegin(GL_POLYGON);

    glColor3f(0.55f, 0.6f, 0.7f);
    for (GLfloat angle = 0.0f; angle <= 2 * PI; angle = angle + 0.001f)
    {
        x = (GLfloat)cos(angle) * (*redius);
        y = (GLfloat)sin(angle) * (*redius);

        if (x <= -0.0f)
        {
        }
        else
        {
            glVertex3f(x, y, 0.0f);
        }
    }
    glEnd();
}

void uninitialize(void)
{
	GLXContext currentGLXContext;
	
	currentGLXContext = glXGetCurrentContext();
	
	alutExit();
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


